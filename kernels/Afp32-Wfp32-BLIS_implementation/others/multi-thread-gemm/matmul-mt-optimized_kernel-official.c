/*
Built upon matmul_optimized_kernel.c, we now apply multi-threading.
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "kernel.h"

#define MEM_ALIGN 64
#define UN_INIT -10.0

// ================== 16x6 kernel hyperparameters================
#ifndef NTHREADS
#define NTHREADS 2
#endif

#define OMP_SCHEDULE dynamic
#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

#define MR 6
#define NR 16
// #define MC MR *256
// #define NC NR * 64
// #define MC 200
// #define NC 400
// #define MC MR *  11
// #define NC NR *  25
// #define MC MR * NTHREADS * 11
// #define NC NR * NTHREADS * 25
// #define KC 500


#define MC (16 * (40 / NTHREADS) * NTHREADS)
#define NC (6 * (800 / NTHREADS) * NTHREADS)
#define KC 500

// ================== 16x6 kernel hyperparameters================

#ifdef DIM
    #define MDIM DIM
    #define NDIM DIM
    #define KDIM DIM
#else


#ifndef MDIM
    #define MDIM 1000
#endif

#ifndef NDIM
    #define NDIM 1000
#endif

#ifndef KDIM
    #define KDIM 1000
#endif

#endif

#ifndef NITER
    #define NITER 10
#endif



#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

#define min(x, y) ((x) < (y) ? (x) : (y))

static float blockA_packed[MC * KC] __attribute__((aligned(64)));
static float blockB_packed[NC * KC] __attribute__((aligned(64)));

void pack_panelB(float* B, float* blockB_packed, int nr, int kc, int K) {
    for (int p = 0; p < kc; p++) {
        for (int j = 0; j < nr; j++) {
            *blockB_packed++ = B[j * K + p];
        }
        for (int j = nr; j < 6; j++) {
            *blockB_packed++ = 0;
        }
    }
}

void pack_blockB(float* B, float* blockB_packed, int nc, int kc, int K) {
    PRAGMA_OMP_PARALLEL_FOR
    for (int j = 0; j < nc; j += 6) {
        int nr = min(6, nc - j);
        pack_panelB(&B[j * K], &blockB_packed[j * kc], nr, kc, K);
    }
}

void pack_panelA(float* A, float* blockA_packed, int mr, int kc, int M) {
    for (int p = 0; p < kc; p++) {
        for (int i = 0; i < mr; i++) {
            *blockA_packed++ = A[p * M + i];
        }
        for (int i = mr; i < 16; i++) {
            *blockA_packed++ = 0;
        }
    }
}

void pack_blockA(float* A, float* blockA_packed, int mc, int kc, int M) {
    PRAGMA_OMP_PARALLEL_FOR
    for (int i = 0; i < mc; i += 16) {
        int mr = min(16, mc - i);
        pack_panelA(&A[i], &blockA_packed[i * kc], mr, kc, M);
    }
}

void matmul_kernel(float* A, float* B, float* C, int M, int N, int K) {

    // The function computes C[M x N] = A[M x K] @ B[K x N]
    // All operands are stored in column-major format, with lda=M, ldb=K, ldc=M

    for (int j = 0; j < N; j += NC) {
        int nc = min(NC, N - j);
        int kc = min(KC, K);
        pack_blockB(&B[j * K], blockB_packed, nc, kc, K);
        for (int i = 0; i < M; i += MC) {
            int mc = min(MC, M - i);
            pack_blockA(&A[i], blockA_packed, mc, kc, M);
            PRAGMA_OMP_PARALLEL_FOR
            for (int jr = 0; jr < nc; jr += 6) {
                int nr = min(6, nc - jr);
                for (int ir = 0; ir < mc; ir += 16) {
                    int mr = min(16, mc - ir);
                    kernel_16x6_zero_init_accum(&blockA_packed[ir * kc],
                                                &blockB_packed[jr * kc],
                                                &C[(j + jr) * M + (i + ir)],
                                                mr,
                                                nr,
                                                kc,
                                                M);
                }
            }
        }
        for (int p = kc; p < K; p += KC) {
            int kc = min(KC, K - p);
            pack_blockB(&B[j * K + p], blockB_packed, nc, kc, K);
            for (int i = 0; i < M; i += MC) {
                int mc = min(MC, M - i);
                pack_blockA(&A[p * M + i], blockA_packed, mc, kc, M);
                PRAGMA_OMP_PARALLEL_FOR
                for (int jr = 0; jr < nc; jr += 6) {
                    int nr = min(6, nc - jr);
                    for (int ir = 0; ir < mc; ir += 16) {
                        int mr = min(16, mc - ir);
                        kernel_16x6_load_accum(&blockA_packed[ir * kc],
                                               &blockB_packed[jr * kc],
                                               &C[(j + jr) * M + (i + ir)],
                                               mr,
                                               nr,
                                               kc,
                                               M);
                    }
                }
            }
        }
    }
}

void print_mat(float* mat, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", mat[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void init_rand(float* mat, const int M, const int N) {
    for (int i = 0; i < M * N; i++) {
        *mat++ = rand() / (float)RAND_MAX;
    }
}

void init_const(float* mat, const float value, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            *mat++ = value;
        }
    }
}

int compare_mats(float* mat1, float* mat2, const int M, const int N) {
    int num = 1200;
    float acc_err = 0.;
    // for (int i = 0; i < M; i++) 
    for (int i = M -1; i >= 0; i--) 
    { // for (int j = 0; j < N; j++) 
        for (int j = N - 1; j >= 0; j--) 
        {
            float err = fabsf(mat1[i * N + j] - mat2[i * N + j]);
            acc_err += err;
            if ( err > 1e-3) 
            {
                // if (num-- > 0)
                    // printf("MISMATCH! Element[%d][%d] %f != %f\n",
                    //        i,
                    //        j,
                    //        mat1[i * N + j],
                    //        mat2[i * N + j]);
            }
        }
    }
    float avg_err = acc_err / (M * N);
    printf("acc error: % f average error: %f\n", acc_err, avg_err);
    if (avg_err > 1e-3) return 0;
    // printf("MATCH!\n");
    return 1;
}

void matmul_naive(float* A, float* B, float* C, int m, int n, int k) {
#pragma omp parallel for collapse(2) num_threads(NTHREADS)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            float accumulator = 0;
            for (int p = 0; p < k; p++) {
                accumulator += A[p * m + i] * B[j * k + p];
            }
            C[j * m + i] = accumulator;
        }
    }
}

uint64_t timer() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    return (uint64_t)start.tv_sec * 1000000000 + (uint64_t)start.tv_nsec;
}

int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    float* A = (float*)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    float* B = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    init_rand(A, M, K);
    init_rand(B, K, N);
    

    printf("\e[31m[INFO]\e[m KC: %d, MC: %d, NC: %d, MR: %d, NR: %d\n", KC, MC, NC, MR, NR);

#ifdef TEST
    matmul_naive(A, B, C_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
    // init blockB_packed and blockA_packed
    // init_const(blockA_packed, UN_INIT, MC, KC);
    // init_const(blockB_packed, UN_INIT, KC, NC);
        
    
        init_const(C, 0.0, M, N);
        // init_const(C, -1000, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("-----------------------------------------------------\n");
        printf("MC=%d NC=%d kC=%d Nthreads=%d\n", MC, NC, KC, NTHREADS);
        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS = %.3f for (%d, %d)x(%d, %d)\n", FLOPS / 1e9, M, K, K, N);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        if (!res)
        {
            if (0)
            {
                printf("\n\n");
                print_mat(C, M, N);
                printf("\n\n");
                print_mat(C_ref, M, N);
                printf("Matrix C start address: %p end address: %p\n", C, C + M * N);
            }
            break;
        }
#endif
        printf("\n");
    }

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);

    return 0;
}