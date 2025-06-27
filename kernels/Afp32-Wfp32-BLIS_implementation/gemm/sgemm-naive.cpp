// clang-17 -O2 -mno-avx512f -march=native -DNITER=1000 matmul_naive.c -o matmul_naive.out && ./matmul_naive.out
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"

#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM 1024
#endif

#ifndef NDIM
    #define NDIM 1024
#endif

#ifndef KDIM
    #define KDIM 1024
#endif

#ifndef NITER
    #define NITER 100
#endif

void matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
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
    init_rand(A, M, K);
    init_rand(B, K, N);

    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {
        uint64_t start = timer();
        matmul_naive(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;
        printf("GFLOPS = %.3f M: %d, N: %d, K: %d\n\n", FLOPS / 1e9, M, N, K);
    }

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

    return 0;
}