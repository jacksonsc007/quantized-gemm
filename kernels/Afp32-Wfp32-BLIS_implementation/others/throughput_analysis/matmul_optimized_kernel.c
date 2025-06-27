/*
Despite the memory layout and A,B,C are all different, we stick to the cache blocking strategy.
Other conjugate blocking strategy is going to be further discussed in the future version.
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64
#define UN_INIT -10.0

#define MR 6
#define NR 16
// #define MC MR *256
// #define NC NR * 64
#define MC MR * 11
#define NC NR * 25
// #define MC 200
// #define NC 400

#define KC 500

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

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j) ( A + ( i ) * K + ( j ) )
#define B(i, j) ( B + ( j ) * K + ( i ) )
#define C(i, j) ( C + ( i ) * N + ( j ) )

// column-major shape (block_m, K)
#define blockA(i, j) ( blockA + ( j ) * block_m + ( i ) )
// row-major shape (K, block_n)
#define blockB(i, j) ( blockB + ( i ) * block_n + ( j ) )



static float blockA_packed[MC * KC] __attribute__((aligned(64)));
static float blockB_packed[KC * NC] __attribute__((aligned(64)));

/*
blockA should be column-major order
blockB should be row-major order
*/
void kernel_16x6(float* blockA, float* blockB, float* C, const int valid_m, const int valid_n, const int block_m, const int block_n, const int block_K, const int N) {
    // Explicitly unroll the array
    // __m256 C_buffer[6][2];
    __m256 C00 = _mm256_setzero_ps();
    __m256 C01 = _mm256_setzero_ps();
    __m256 C10 = _mm256_setzero_ps();
    __m256 C11 = _mm256_setzero_ps();
    __m256 C20 = _mm256_setzero_ps();
    __m256 C21 = _mm256_setzero_ps();
    __m256 C30 = _mm256_setzero_ps();
    __m256 C31 = _mm256_setzero_ps();
    __m256 C40 = _mm256_setzero_ps();
    __m256 C41 = _mm256_setzero_ps();
    __m256 C50 = _mm256_setzero_ps();
    __m256 C51 = _mm256_setzero_ps();

    __m256 a_packFloat8;
    __m256 b0_packFloat8;
    __m256 b1_packFloat8;
    // __m256i masks[2];
    // Explicitly unroll the array
    __m256i mask0;
    __m256i mask1;
    // load C from memory to registers
    // i: row index
    if (valid_n != NR)
    {
        // const unsigned int bit_mask = 65535;
        // mask0 = _mm256_setr_epi32(bit_mask << (valid_n + 15),
        //                              bit_mask << (valid_n + 14),
        //                              bit_mask << (valid_n + 13),
        //                              bit_mask << (valid_n + 12),
        //                              bit_mask << (valid_n + 11),
        //                              bit_mask << (valid_n + 10),
        //                              bit_mask << (valid_n + 9),
        //                              bit_mask << (valid_n + 8));
        // mask1 = _mm256_setr_epi32(bit_mask << (valid_n + 7),
        //                              bit_mask << (valid_n + 6),
        //                              bit_mask << (valid_n + 5),
        //                              bit_mask << (valid_n + 4),
        //                              bit_mask << (valid_n + 3),
        //                              bit_mask << (valid_n + 2),
        //                              bit_mask << (valid_n + 1),
        //                              bit_mask <<  valid_n);

static int8_t mask[32]
    __attribute__((aligned(64))) = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
mask0 = _mm256_cvtepi8_epi32(_mm_loadu_si64(&mask[16 - valid_n]));
mask1 = _mm256_cvtepi8_epi32(_mm_loadu_si64(&mask[16 - valid_n + 8]));
        // Since we unroll the array, we need to manually load the data
        // for (int i =  0; i < valid_m; i++)
        // {
        //     C_buffer[i][0] = _mm256_maskload_ps(C(i, 0), mask0);
        //     C_buffer[i][1] = _mm256_maskload_ps(C(i, 8), mask1);
        // }
        switch (valid_m)
        {
            case 6:
                C00 = _mm256_maskload_ps(C(0, 0), mask0);
                C01 = _mm256_maskload_ps(C(0, 8), mask1);
                C10 = _mm256_maskload_ps(C(1, 0), mask0);
                C11 = _mm256_maskload_ps(C(1, 8), mask1);
                C20 = _mm256_maskload_ps(C(2, 0), mask0);
                C21 = _mm256_maskload_ps(C(2, 8), mask1);
                C30 = _mm256_maskload_ps(C(3, 0), mask0);
                C31 = _mm256_maskload_ps(C(3, 8), mask1);
                C40 = _mm256_maskload_ps(C(4, 0), mask0);
                C41 = _mm256_maskload_ps(C(4, 8), mask1);
                C50 = _mm256_maskload_ps(C(5, 0), mask0);
                C51 = _mm256_maskload_ps(C(5, 8), mask1);
                break;
            case 5:
                C00 = _mm256_maskload_ps(C(0, 0), mask0);
                C01 = _mm256_maskload_ps(C(0, 8), mask1);
                C10 = _mm256_maskload_ps(C(1, 0), mask0);
                C11 = _mm256_maskload_ps(C(1, 8), mask1);
                C20 = _mm256_maskload_ps(C(2, 0), mask0);
                C21 = _mm256_maskload_ps(C(2, 8), mask1);
                C30 = _mm256_maskload_ps(C(3, 0), mask0);
                C31 = _mm256_maskload_ps(C(3, 8), mask1);
                C40 = _mm256_maskload_ps(C(4, 0), mask0);
                C41 = _mm256_maskload_ps(C(4, 8), mask1);
                break;
            case 4:
                C00 = _mm256_maskload_ps(C(0, 0), mask0);
                C01 = _mm256_maskload_ps(C(0, 8), mask1);
                C10 = _mm256_maskload_ps(C(1, 0), mask0);
                C11 = _mm256_maskload_ps(C(1, 8), mask1);
                C20 = _mm256_maskload_ps(C(2, 0), mask0);
                C21 = _mm256_maskload_ps(C(2, 8), mask1);
                C30 = _mm256_maskload_ps(C(3, 0), mask0);
                C31 = _mm256_maskload_ps(C(3, 8), mask1);
                break;
            case 3:
                C00 = _mm256_maskload_ps(C(0, 0), mask0);
                C01 = _mm256_maskload_ps(C(0, 8), mask1);
                C10 = _mm256_maskload_ps(C(1, 0), mask0);
                C11 = _mm256_maskload_ps(C(1, 8), mask1);
                C20 = _mm256_maskload_ps(C(2, 0), mask0);
                C21 = _mm256_maskload_ps(C(2, 8), mask1);
                break;
            case 2:
                C00 = _mm256_maskload_ps(C(0, 0), mask0);
                C01 = _mm256_maskload_ps(C(0, 8), mask1);
                C10 = _mm256_maskload_ps(C(1, 0), mask0);
                C11 = _mm256_maskload_ps(C(1, 8), mask1);
                break;
            case 1:
                C00 = _mm256_maskload_ps(C(0, 0), mask0);
                C01 = _mm256_maskload_ps(C(0, 8), mask1);
                break;
        }
                                     
    } 
    else
    {
        // for (int i = 0; i < valid_m; i++) {
        //     C_buffer[i][0] = _mm256_loadu_ps(C(i, 0));
        //     C_buffer[i][1] = _mm256_loadu_ps(C(i, 8));
        // }
        switch (valid_m)
        {
            case 6:
                C00 = _mm256_loadu_ps(C(0, 0));
                C01 = _mm256_loadu_ps(C(0, 8));
                C10 = _mm256_loadu_ps(C(1, 0));
                C11 = _mm256_loadu_ps(C(1, 8));
                C20 = _mm256_loadu_ps(C(2, 0));
                C21 = _mm256_loadu_ps(C(2, 8));
                C30 = _mm256_loadu_ps(C(3, 0));
                C31 = _mm256_loadu_ps(C(3, 8));
                C40 = _mm256_loadu_ps(C(4, 0));
                C41 = _mm256_loadu_ps(C(4, 8));
                C50 = _mm256_loadu_ps(C(5, 0));
                C51 = _mm256_loadu_ps(C(5, 8));
                break;
            case 5:
                C00 = _mm256_loadu_ps(C(0, 0));
                C01 = _mm256_loadu_ps(C(0, 8));
                C10 = _mm256_loadu_ps(C(1, 0));
                C11 = _mm256_loadu_ps(C(1, 8));
                C20 = _mm256_loadu_ps(C(2, 0));
                C21 = _mm256_loadu_ps(C(2, 8));
                C30 = _mm256_loadu_ps(C(3, 0));
                C31 = _mm256_loadu_ps(C(3, 8));
                C40 = _mm256_loadu_ps(C(4, 0));
                C41 = _mm256_loadu_ps(C(4, 8));
                break;
            case 4:
                C00 = _mm256_loadu_ps(C(0, 0));
                C01 = _mm256_loadu_ps(C(0, 8));
                C10 = _mm256_loadu_ps(C(1, 0));
                C11 = _mm256_loadu_ps(C(1, 8));
                C20 = _mm256_loadu_ps(C(2, 0));
                C21 = _mm256_loadu_ps(C(2, 8));
                C30 = _mm256_loadu_ps(C(3, 0));
                C31 = _mm256_loadu_ps(C(3, 8));
                break;
            case 3:
                C00 = _mm256_loadu_ps(C(0, 0));
                C01 = _mm256_loadu_ps(C(0, 8));
                C10 = _mm256_loadu_ps(C(1, 0));
                C11 = _mm256_loadu_ps(C(1, 8));
                C20 = _mm256_loadu_ps(C(2, 0));
                C21 = _mm256_loadu_ps(C(2, 8));
                break;
            case 2:
                C00 = _mm256_loadu_ps(C(0, 0));
                C01 = _mm256_loadu_ps(C(0, 8));
                C10 = _mm256_loadu_ps(C(1, 0));
                C11 = _mm256_loadu_ps(C(1, 8));
                break;
            case 1:
                C00 = _mm256_loadu_ps(C(0, 0));
                C01 = _mm256_loadu_ps(C(0, 8));
                break;
        }
    }

    for (int p = 0; p < block_K; p++) {
        b0_packFloat8 = _mm256_loadu_ps(blockB(p, 0));
        b1_packFloat8 = _mm256_loadu_ps(blockB(p, 8));

        a_packFloat8 = _mm256_broadcast_ss(blockA(0, p));
        C00 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C00);
        C01 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C01);

        a_packFloat8 = _mm256_broadcast_ss(blockA(1, p));
        C10 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C10);
        C11 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C11);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(2, p));
        C20 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C20);
        C21 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C21);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(3, p));
        C30 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C30);
        C31 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C31);

        a_packFloat8 = _mm256_broadcast_ss(blockA(4, p));
        C40 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C40);
        C41 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C41);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(5, p));
        C50 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C50);
        C51 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C51);
    }
    if (valid_n != NR)
    {
        // for (int i = 0; i < valid_m; i++) {
        //     _mm256_maskstore_ps(C(i, 0), mask0, C_buffer[i][0]);
        //     _mm256_maskstore_ps(C(i, 8), mask1, C_buffer[i][1]);

        // }
        switch(valid_m)
        {
            case 6:
                _mm256_maskstore_ps(C(0, 0), mask0, C00);
                _mm256_maskstore_ps(C(0, 8), mask1, C01);
                _mm256_maskstore_ps(C(1, 0), mask0, C10);
                _mm256_maskstore_ps(C(1, 8), mask1, C11);
                _mm256_maskstore_ps(C(2, 0), mask0, C20);
                _mm256_maskstore_ps(C(2, 8), mask1, C21);
                _mm256_maskstore_ps(C(3, 0), mask0, C30);
                _mm256_maskstore_ps(C(3, 8), mask1, C31);
                _mm256_maskstore_ps(C(4, 0), mask0, C40);
                _mm256_maskstore_ps(C(4, 8), mask1, C41);
                _mm256_maskstore_ps(C(5, 0), mask0, C50);
                _mm256_maskstore_ps(C(5, 8), mask1, C51);
                break;
            case 5:
                _mm256_maskstore_ps(C(0, 0), mask0, C00);
                _mm256_maskstore_ps(C(0, 8), mask1, C01);
                _mm256_maskstore_ps(C(1, 0), mask0, C10);
                _mm256_maskstore_ps(C(1, 8), mask1, C11);
                _mm256_maskstore_ps(C(2, 0), mask0, C20);
                _mm256_maskstore_ps(C(2, 8), mask1, C21);
                _mm256_maskstore_ps(C(3, 0), mask0, C30);
                _mm256_maskstore_ps(C(3, 8), mask1, C31);
                _mm256_maskstore_ps(C(4, 0), mask0, C40);
                _mm256_maskstore_ps(C(4, 8), mask1, C41);
                break;
            case 4:
                _mm256_maskstore_ps(C(0, 0), mask0, C00);
                _mm256_maskstore_ps(C(0, 8), mask1, C01);
                _mm256_maskstore_ps(C(1, 0), mask0, C10);
                _mm256_maskstore_ps(C(1, 8), mask1, C11);
                _mm256_maskstore_ps(C(2, 0), mask0, C20);
                _mm256_maskstore_ps(C(2, 8), mask1, C21);
                _mm256_maskstore_ps(C(3, 0), mask0, C30);
                _mm256_maskstore_ps(C(3, 8), mask1, C31);
                break;
            case 3:
                _mm256_maskstore_ps(C(0, 0), mask0, C00);
                _mm256_maskstore_ps(C(0, 8), mask1, C01);
                _mm256_maskstore_ps(C(1, 0), mask0, C10);
                _mm256_maskstore_ps(C(1, 8), mask1, C11);
                _mm256_maskstore_ps(C(2, 0), mask0, C20);
                _mm256_maskstore_ps(C(2, 8), mask1, C21);
                break;
            case 2:
                _mm256_maskstore_ps(C(0, 0), mask0, C00);
                _mm256_maskstore_ps(C(0, 8), mask1, C01);
                _mm256_maskstore_ps(C(1, 0), mask0, C10);
                _mm256_maskstore_ps(C(1, 8), mask1, C11);
                break;
            case 1:
                _mm256_maskstore_ps(C(0, 0), mask0, C00);
                _mm256_maskstore_ps(C(0, 8), mask1, C01);
                break;
        }
    }
    else
    {
        // for (int i = 0; i < valid_m; i++) {
        //     _mm256_storeu_ps(C(i, 0), C_buffer[i][0]);
        //     _mm256_storeu_ps(C(i, 8), C_buffer[i][1]);
        // }
        switch(valid_m)
        {
            case 6:
                _mm256_storeu_ps(C(0, 0), C00);
                _mm256_storeu_ps(C(0, 8), C01);
                _mm256_storeu_ps(C(1, 0), C10);
                _mm256_storeu_ps(C(1, 8), C11);
                _mm256_storeu_ps(C(2, 0), C20);
                _mm256_storeu_ps(C(2, 8), C21);
                _mm256_storeu_ps(C(3, 0), C30);
                _mm256_storeu_ps(C(3, 8), C31);
                _mm256_storeu_ps(C(4, 0), C40);
                _mm256_storeu_ps(C(4, 8), C41);
                _mm256_storeu_ps(C(5, 0), C50);
                _mm256_storeu_ps(C(5, 8), C51);
                break;
            case 5:
                _mm256_storeu_ps(C(0, 0), C00);
                _mm256_storeu_ps(C(0, 8), C01);
                _mm256_storeu_ps(C(1, 0), C10);
                _mm256_storeu_ps(C(1, 8), C11);
                _mm256_storeu_ps(C(2, 0), C20);
                _mm256_storeu_ps(C(2, 8), C21);
                _mm256_storeu_ps(C(3, 0), C30);
                _mm256_storeu_ps(C(3, 8), C31);
                _mm256_storeu_ps(C(4, 0), C40);
                _mm256_storeu_ps(C(4, 8), C41);
                break;
            case 4:
                _mm256_storeu_ps(C(0, 0), C00);
                _mm256_storeu_ps(C(0, 8), C01);
                _mm256_storeu_ps(C(1, 0), C10);
                _mm256_storeu_ps(C(1, 8), C11);
                _mm256_storeu_ps(C(2, 0), C20);
                _mm256_storeu_ps(C(2, 8), C21);
                _mm256_storeu_ps(C(3, 0), C30);
                _mm256_storeu_ps(C(3, 8), C31);
                break;
            case 3:
                _mm256_storeu_ps(C(0, 0), C00);
                _mm256_storeu_ps(C(0, 8), C01);
                _mm256_storeu_ps(C(1, 0), C10);
                _mm256_storeu_ps(C(1, 8), C11);
                _mm256_storeu_ps(C(2, 0), C20);
                _mm256_storeu_ps(C(2, 8), C21);
                break;
            case 2:
                _mm256_storeu_ps(C(0, 0), C00);
                _mm256_storeu_ps(C(0, 8), C01);
                _mm256_storeu_ps(C(1, 0), C10);
                _mm256_storeu_ps(C(1, 8), C11);
                break;
            case 1:
                _mm256_storeu_ps(C(0, 0), C00);
                _mm256_storeu_ps(C(0, 8), C01);
                break;
        }
    }
}
