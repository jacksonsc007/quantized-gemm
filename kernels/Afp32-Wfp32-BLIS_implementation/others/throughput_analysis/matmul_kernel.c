// clang-17 -O2 -mno-avx512f -march=native -DTEST -DNITER=1000 matmul_kernel.c -o matmul_kernel.out && ./matmul_kernel.out
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

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