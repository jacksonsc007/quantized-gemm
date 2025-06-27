// clang-17 -O2 -mno-avx512f -march=native -DTEST -DNITER=1000 matmul_kernel.c -o matmul_kernel.out && ./matmul_kernel.out
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <cblas.h>
#include <string.h>

#define MEM_ALIGN 64

#ifndef NTHREADS
    #define NTHREADS 1
#endif

#ifndef MDIM
    // #define MDIM MR * 62
    #define MDIM 1
#endif

#ifndef NDIM
    // #define NDIM NR * 166
    #define NDIM 4096
#endif

#ifndef KDIM
    #define KDIM 4096
#endif

#ifndef NITER
    #define NITER 100
#endif


// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j) ( A + i * K + j )
#define B(i, j) ( B + j * K + i )
#define C(i, j) ( C + i * N + j )




void matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float acc = 0;
            for (int p = 0; p < K; p++) {
                // C[j * M + i] += A[p * M + i] * B[j * K + p];
                acc += *A(i, p) * *B(p, j);
            }
            *C(i, j) = acc;
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
    {
        // for (int j = 0; j < N; j++) 
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
    if (acc_err > 1e-3) return 0;
    // printf("MATCH!\n");
    return 1;
}
uint64_t timer() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    return (uint64_t)start.tv_sec * 1000000000 + (uint64_t)start.tv_nsec;
}

void float_matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int p = 0; p < K; p++) {
                C[j * M + i] += A[p * M + i] * B[j * K + p];
            }
        }
    }
}

int main() {
    openblas_set_num_threads(NTHREADS);
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    float* A_ref = (float*)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    float* B_ref = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* A = (float*)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    float* B = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    init_rand(A, M, K);
    init_rand(B, K, N);
    memcpy(A_ref, A, M * K * sizeof(float));
    memcpy(B_ref, B, K * N * sizeof(float));

// #define CACHE_FLUSH_SIZE (4 * 1024 * 1024 * 1024) // 100MB
#define CACHE_FLUSH_SIZE (4ULL * 1024 * 1024 * 1024) // 4GB

    char* flush_buffer = (char*)_mm_malloc(CACHE_FLUSH_SIZE, MEM_ALIGN);
    for (size_t j = 0; j < CACHE_FLUSH_SIZE; j += 64) {
        flush_buffer[j] = 0; // write to every cache line (assume 64B)
    }

    // These matrixes are allocated to override cache lines when benchmarking vector-matrix multiplication
    const int M_ = 1000;
    const int K_ = 1000;
    const int N_ = 1000;
    float* A_ = (float*)_mm_malloc(M_ * K_ * sizeof(float), MEM_ALIGN);
    float* B_ = (float*)_mm_malloc(K_ * N_ * sizeof(float), MEM_ALIGN);
    float* C_ = (float*)_mm_malloc(M_ * N_ * sizeof(float), MEM_ALIGN);
    init_rand(A_, M_, K_);
    init_rand(B_, K_, N_);

#ifdef TEST
    matmul_naive(A_ref, B_ref, C_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    volatile int dummy = 0;
    for (int i = 0; i < NITER; i++) {
        // Flush cache
        for (size_t j = 0; j < CACHE_FLUSH_SIZE; j += 64) {
            flush_buffer[j] = i; // write to every cache line (assume 64B)
        }
        // float_matmul_naive(A_, B_, C_, M_, N_, K_);

        uint64_t start = timer();
        // cblas_sgemv()
        // cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0, A, M, B, K, 0.0, C, M);
        cblas_sgemv(
            CblasColMajor, CblasTrans,
            K, N,
            1.0, B, K,
            A, 1,
            0.0, C, 1);
        // matmul_naive(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS=%.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        // if (!res) break;
#endif
        printf("\n");
    }

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);
// _mm_free(flush_buffer);

    return 0;
}
