// clang-17 -O2 -mno-avx512f -march=native -DNITER=1000 matmul_naive.c -o matmul_naive.out && ./matmul_naive.out
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

void matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int p = 0; p < K; p++) {
                C[j * M + i] += A[p * M + i] * B[j * K + p];
            }
        }
    }
}
