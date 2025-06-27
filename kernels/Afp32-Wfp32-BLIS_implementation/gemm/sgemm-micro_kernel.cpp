// clang-17 -O2 -mno-avx512f -march=native -DTEST -DNITER=1000 matmul_kernel.c -o matmul_kernel.out && ./matmul_kernel.out
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"

#define MEM_ALIGN 64

#define MR 6
#define NR 16

#ifndef MDIM
    // #define MDIM MR * 62
    #define MDIM MR * 166
#endif

#ifndef NDIM
    // #define NDIM NR * 166
    #define NDIM NR * 62
#endif

#ifndef KDIM
    #define KDIM 1000
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

// column-major shape (block_m, K)
#define blockA(i, j) ( blockA + j * block_m + i )
// row-major shape (K, block_n)
#define blockB(i, j) ( blockB + i * block_n + j )


static float blockA_packed[MR * KDIM] __attribute__((aligned(64)));
static float blockB_packed[KDIM * NR] __attribute__((aligned(64)));

/*
blockA should be column-major order
blockB should be row-major order
*/
void kernel_16x6(float* blockA, float* blockB, float* C, const int block_m, const int block_n, const int K, const int N) {
    __m256 C_buffer[6][2];
    __m256 a_packFloat8;
    __m256 b0_packFloat8;
    __m256 b1_packFloat8;
    // load C from memory to registers
    // i: row index
    for (int i = 0; i < 6; i++) {
        C_buffer[i][0] = _mm256_loadu_ps(C(i, 0));
        C_buffer[i][1] = _mm256_loadu_ps(C(i, 8));
    }
    for (int p = 0; p < K; p++) {
        b0_packFloat8 = _mm256_loadu_ps(blockB(p, 0));
        b1_packFloat8 = _mm256_loadu_ps(blockB(p, 8));

        a_packFloat8 = _mm256_broadcast_ss(blockA(0, p));
        C_buffer[0][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[0][0]);
        C_buffer[0][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[0][1]);

        a_packFloat8 = _mm256_broadcast_ss(blockA(1, p));
        C_buffer[1][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[1][0]);
        C_buffer[1][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[1][1]);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(2, p));
        C_buffer[2][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[2][0]);
        C_buffer[2][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[2][1]);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(3, p));
        C_buffer[3][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[3][0]);
        C_buffer[3][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[3][1]);

        a_packFloat8 = _mm256_broadcast_ss(blockA(4, p));
        C_buffer[4][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[4][0]);
        C_buffer[4][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[4][1]);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(5, p));
        C_buffer[5][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[5][0]);
        C_buffer[5][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[5][1]);
    }
    // store C from registers to memory
    // for (int j = 0; j < 6; j++) {
    //     _mm256_storeu_ps(&C[j * M], C_buffer[j][0]);
    //     _mm256_storeu_ps(&C[j * M + 8], C_buffer[j][1]);
    // }
    for (int i = 0; i < 6; i++) {
        _mm256_storeu_ps(C(i, 0), C_buffer[i][0]);
        _mm256_storeu_ps(C(i, 8), C_buffer[i][1]);
    }
}

void pack_blockA(float* A, float* blockA, const int block_m, const int K)
{
    // cache friendly for read TODO: try cache friendly for write
        for (int j = 0; j < K; ++j)
    {
    for (int i = 0; i < block_m; ++i)
        {
            *blockA(i, j) = *A(i, j);
        }
    }
} 
void pack_blockB(float* B, float* blockB, const int block_n, const int K)
{
    // NOTE: blockB is row-major order, while B is column-major order
    // We test both cache friendly for read and write
    // And it turns out that prioritize blockB (write) is better
        for (int j = 0; j < block_n; ++j)
    {
    for (int i = 0; i < K; ++i)
        {
            *blockB(i, j) = *B(i, j) ;
        }
    }
} 

void matmul_kernel(float* A, float* B, float* C, const int M, const int N, const int K) {
    assert(M % MR == 0);
    assert(N % NR == 0);
    for (int j = 0; j < N; j += NR) {
        pack_blockB(B(0, j), blockB_packed, NR, K);
        for (int i = 0; i < M; i += MR) {
            // kernel_16x6(&A[i], &B[j * K], &C[j * M + i], M, N, K);
            pack_blockA(A(i, 0), blockA_packed, MR, K);
            kernel_16x6(blockA_packed, blockB_packed, C(i, j), MR, NR, K, N);

        }
    }
}



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

#ifdef TEST
    matmul_naive(A, B, C_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("GFLOPS = \e[32m%.3f \e[m M: %d, N: %d, K: %d\n\n", FLOPS / 1e9, M, N, K);
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

    return 0;
}