/*
Activation: int8 quantization
weight: int4 quantization

# matrix A
- each ele is int8.
- element size: (M, K)
- storage memory type: int8
- storage size: (M, K)

# matrix B
- each ele is int4.
- element size: (K, M)
- storage memory type: uint8
- storage size: (K/2, M)

## scaling factor of B
- each ele is float32.
- element size: (K / 32, M)
- storage memory type: float32
- storage size: (K / 32, M)



quantization block size: 32
To get 32 ele from matrix A, we need 32 storage unit (int8).
To get 32 ele from matrix B, we need 16 storage unit (uint8).
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#define MR 2
#define NR 16
#define MC MR * 11
#define NC NR * 25
#define KC 1024

#ifndef MDIM
    #define MDIM 1000
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM 1000
    // #define NDIM NR * 2
#endif

#ifndef KDIM
    #define KDIM 1024
#endif

#ifndef NITER
    #define NITER 100
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C, SA
// column-major order: matrix B, SB
// A: M x K B: K x N C: M x N

#define A(i, j, ld) ( A + ( i ) * ( ld ) + ( j ) )
#define B(i, j, ld) ( B + ( j ) * ( ld ) + ( i ) )
#define C(i, j, ld) ( C + ( i ) * ( ld ) + ( j ) )
#define SA(i, j, ld) (SA + (i) * (ld) + (j))
#define SB(i, j, ld) (SB + (j) * (ld) + (i))

// TODO: Design store pattern that is friendly to the access pattern of int8 weight and activation?

// blockA: packed. row-major.      (MR, KC)
// blockB: packed. column-major.   (KC, NR)
// blockSA: packed. row-major.     (MR, KC/Q_BLK_SIZE)
// blockSB: packed. column-major.  (KC/Q_BLK_SIZE, NR)
#define blockA(i, j, ld) ( blockA  + (i) * (ld) + (j) )
#define blockSA(i, j, ld) (blockSA + (i) * (ld) + (j))
#define blockB(i, j, ld) ( blockB  + (j) * (ld) + (i) )
#define blockSB(i, j, ld) (blockSB + (j) * (ld) + (i))




/*
blockA: packed. row-major.      (MR, KC)
blockB: packed. column-major.   (KC, NR)
blockSA: packed. row-major.     (MR, KC/Q_BLK_SIZE)
blockSB: packed. column-major.  (KC/Q_BLK_SIZE, NR)
*/
void kernel(
    int8_t * blockA, uint8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    for (int i = 0; i < block_m; ++i)
    for (int j = 0; j < block_n; ++j)
    {
        __m256 sum_packed_fp = _mm256_setzero_ps();
        for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0 = *blockSA(i, block_idx, block_k/Q_BLK_SIZE);
            float sB_0 = *blockSB(block_idx, j, block_k/Q_BLK_SIZE);
            float sA_1 = *blockSA(i, block_idx + 1, block_k/Q_BLK_SIZE);
            float sB_1 = *blockSB(block_idx + 1, j, block_k/Q_BLK_SIZE);
            float fused_s_0 = sA_0 * sB_0;
            float fused_s_1 = sA_1 * sB_1;
            // Q_BLK_SIZE = 32
            // 32 * 8 = 256
            // so ymm only need to load once
                __m256i a_pack_int8_low = _mm256_loadu_si256( (__m256i *) blockA(i, p, block_k));
                __m256i a_pack_int8_high = _mm256_loadu_si256( (__m256i *) blockA(i, p + Q_BLK_SIZE, block_k));


                // __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) blockB(p, j, block_k));
                __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) blockB(p/2, j, block_k/2));
                __m256i mask = _mm256_set1_epi8(0x0f);
                __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
                __m256i b_pack_int8_high = _mm256_and_si256(  _mm256_srli_epi16(b_pack_int8, 4), mask);
                b_pack_int8_low = _mm256_sub_epi8(b_pack_int8_low, _mm256_set1_epi8(8));
                b_pack_int8_high = _mm256_sub_epi8(b_pack_int8_high, _mm256_set1_epi8(8));

                __m256 tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_low, b_pack_int8_low);
                __m256 tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_high, b_pack_int8_high);
                sum_packed_fp = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp);
                sum_packed_fp = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp);
                // sum_packed_fp += tmp_0 * _mm256_set1_ps(fused_s_0);
                // sum_packed_fp += tmp_1 * _mm256_set1_ps(fused_s_1);
        }
        // float* ptr = (float *) &sum_packed_fp;
        // *C(i, j, NDIM) +=  ptr[0] + ptr[1] + ptr[2] + ptr[3] + ptr[4] + ptr[5] + ptr[6] + ptr[7];
        *C(i, j, NDIM) +=  hsum_float_8(sum_packed_fp);

    }
    
}
