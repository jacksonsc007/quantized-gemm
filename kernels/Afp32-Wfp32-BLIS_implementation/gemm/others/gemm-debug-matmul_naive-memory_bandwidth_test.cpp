// clang-17 -O2 -mno-avx512f -march=native -DNITER=1000 matmul_naive.c -o matmul_naive.out && ./matmul_naive.out
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM 1000
#endif

#ifndef NDIM
    #define NDIM 1000
#endif

#ifndef KDIM
    #define KDIM 1000
#endif

#ifndef NITER
    #define NITER 100
#endif

void matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int p = 0; p < K; p++) {
                volatile float a = A[p * M + i];
                volatile float b = B[j * K + p];
                C[j * M + i] += 1;
            }
        }
    }
}

void print_mat(const int M, const int N, float* mat) {
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
    init_rand(A, M, K);
    init_rand(B, K, N);
    
    // collect all bytes needed to load A, B, C and write C
    uint64_t total_bytes = 0;
    uint64_t load_A_B_bytes = (uint64_t)M * N * (2 * K) * sizeof(float);
    total_bytes += load_A_B_bytes;
    // C read and write
    total_bytes += (uint64_t)M * N * sizeof(float);
    total_bytes += (uint64_t)M * N * sizeof(float);


    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {
        uint64_t start = timer();
        matmul_naive(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;
        
        // calculate memory bandwidth
        double memory_bandwidth = total_bytes / exec_time / 1e9;
        printf("Memory bandwidth: %.3f GB/s\n", memory_bandwidth);

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS = %.3f\n\n", FLOPS / 1e9);
    }

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

    return 0;
}