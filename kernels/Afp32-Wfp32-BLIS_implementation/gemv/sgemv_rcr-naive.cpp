#include "lib.h"
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64
#define UN_INIT -10.0

#ifndef NTHREADS
#define NTHREADS 1
#endif

#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM 1
#endif

#ifndef NDIM
    #define NDIM 10240
#endif

#ifndef KDIM
    #define KDIM 10240
#endif

#ifndef NITER
    #define NITER 10
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j, ld) ( A + ( i ) * ld + ( j ) )
#define B(i, j, ld) ( B + ( j ) * ld + ( i ) )
#define C(i, j, ld) ( C + ( i ) * ld + ( j ) )

// column-major shape (block_m, K)
#define blockA(i, j) ( blockA + ( j ) * block_m + ( i ) )
// row-major shape (K, block_n)
#define blockB(i, j) ( blockB + ( i ) * block_n + ( j ) )



void gemv_naive(float* A, float* B, float* C, const int N, const int K) {
    PRAGMA_OMP_PARALLEL_FOR
    for (int j = 0; j < N; j++) {
        float acc = 0;
        for (int p = 0; p < K; p++) {
            acc += *A(0, p, K) * *B(p, j, K);
        }
        *C(0, j, N) = acc;
    }
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
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    float* A = (float*)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    float* B = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    init_rand(A, M, K);
    init_rand(B, K, N);
    
    const int M_ = 1000;
    const int K_ = 1000;
    const int N_ = 1000;
    float* A_ = (float*)_mm_malloc(M_ * K_ * sizeof(float), MEM_ALIGN);
    float* B_ = (float*)_mm_malloc(K_ * N_ * sizeof(float), MEM_ALIGN);
    float* C_ = (float*)_mm_malloc(M_ * N_ * sizeof(float), MEM_ALIGN);
    init_rand(A_, M_, K_);
    init_rand(B_, K_, N_);


    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {
        init_const(C, 0.0, M, N);
        float_matmul_naive(A_, B_, C_, M_, N_, K_);
        uint64_t start = timer();
        gemv_naive(A, B, C, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;
        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS = \e[32m%.3f\e[m] for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);
    _mm_free(A_);
    _mm_free(B_);
    _mm_free(C_);
    return 0;
}