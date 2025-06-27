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
#include "stdbool.h"

#include "lib.h"

#ifndef NTHREADS
#define NTHREADS 1
#endif

#ifndef MR
#define MR 4
#endif

#ifndef NR
#define NR 4
#endif

#ifndef MC
#define MC MR * 11 * NTHREADS
#endif

#ifndef NC
#define NC NR * 25 *  NTHREADS
#endif

#ifndef KC
#define KC 1024
#endif

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#ifndef MDIM
    #define MDIM 1000
#endif

#ifndef NDIM
    #define NDIM 1000
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
#define ScaledSumA(i, j, ld) (ScaledSumA + (i) * (ld) + (j))
#define SB(i, j, ld) (SB + (j) * (ld) + (i))
#define MinB(i, j, ld) (MinB + (j) * (ld) + (i))

// TODO: Design store pattern that is friendly to the access pattern of int8 weight and activation?

// blockA: packed. row-major.      (MR, KC)
// blockB: packed. column-major.   (KC, NR)
// blockSA: packed. row-major.     (MR, KC/Q_BLK_SIZE)
// blockSB: packed. column-major.  (KC/Q_BLK_SIZE, NR)
#define blockA(i, j, ld) ( blockA  + (i) * (ld) + (j) )
#define blockSA(i, j, ld) (blockSA + (i) * (ld) + (j))
#define blockScaledSumA(i, j, ld) (blockScaledSumA + (i) * (ld) + (j))
#define blockB(i, j, ld) ( blockB  + (j) * (ld) + (i) )
#define blockSB(i, j, ld) (blockSB + (j) * (ld) + (i))
#define blockMinB(i, j, ld) (blockMinB + (j) * (ld) + (i))


static int8_t blockA_packed[MC * KC] __attribute__((aligned(64)));
static float sA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static float scaledSumA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static uint8_t blockB_packed[KC / 2 * NC] __attribute__((aligned(64)));
static float sB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static float minB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));

 

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{
    for (int i = 0; i < M - 1; i += 2)
    for (int j = 0; j < N; j++)
    {
        __m256 sum_packed_fp_0 = _mm256_setzero_ps();
        __m256 sum_packed_fp_1 = _mm256_setzero_ps();
        float extra_sum_fp_0 = 0;
        float extra_sum_fp_1 = 0;
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0_0 = *SA(i, block_idx, K/Q_BLK_SIZE);
            float sA_0_1 = *SA(i, block_idx + 1, K/Q_BLK_SIZE);
            float sA_1_0 = *SA(i + 1, block_idx, K/Q_BLK_SIZE);
            float sA_1_1 = *SA(i + 1, block_idx + 1, K/Q_BLK_SIZE);
            float sB_0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float sB_1 = *SB(block_idx + 1, j, K/Q_BLK_SIZE);
            float fused_s_0_0 = sA_0_0 * sB_0;
            float fused_s_0_1 = sA_0_1 * sB_1;
            float fused_s_1_0 = sA_1_0 * sB_0;
            float fused_s_1_1 = sA_1_1 * sB_1;
            
            float min_b_0 = *MinB(block_idx, j, K / Q_BLK_SIZE);
            float min_b_1 = *MinB(block_idx + 1, j, K / Q_BLK_SIZE);
            float sum_a_0_0_fused = *ScaledSumA(i, block_idx, K / Q_BLK_SIZE);
            float sum_a_0_1_fused = *ScaledSumA(i, block_idx + 1, K / Q_BLK_SIZE);
            float sum_a_1_0_fused = *ScaledSumA(i + 1, block_idx, K / Q_BLK_SIZE);
            float sum_a_1_1_fused = *ScaledSumA(i + 1, block_idx + 1, K / Q_BLK_SIZE);
            
            __m256i a_pack_int8_low_0 = _mm256_loadu_si256( (__m256i *) A(i, p, K));
            __m256i a_pack_int8_high_0 = _mm256_loadu_si256( (__m256i *) A(i, p + Q_BLK_SIZE, K));
            __m256i a_pack_int8_low_1 = _mm256_loadu_si256( (__m256i *) A(i + 1, p, K));
            __m256i a_pack_int8_high_1 = _mm256_loadu_si256( (__m256i *) A(i + 1, p + Q_BLK_SIZE, K));
            __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) B(p/2, j, K/2));

            __m256i mask = _mm256_set1_epi8(0x0f);
            __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
            __m256i b_pack_int8_high = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8, 4), mask);

            __m256 tmp_0_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_0);
            __m256 tmp_0_1 = mul_sum_us8_pairs_float( b_pack_int8_high, a_pack_int8_high_0);
            __m256 tmp_1_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_1);
            __m256 tmp_1_1 = mul_sum_us8_pairs_float( b_pack_int8_high, a_pack_int8_high_1);

            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_0, _mm256_set1_ps(fused_s_0_0), sum_packed_fp_0);
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_1, _mm256_set1_ps(fused_s_0_1), sum_packed_fp_0);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_0, _mm256_set1_ps(fused_s_1_0), sum_packed_fp_1);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_1, _mm256_set1_ps(fused_s_1_1), sum_packed_fp_1);

            extra_sum_fp_0 += sum_a_0_0_fused * min_b_0;
            extra_sum_fp_0 += sum_a_0_1_fused * min_b_1;
            extra_sum_fp_1 += sum_a_1_0_fused * min_b_0;
            extra_sum_fp_1 += sum_a_1_1_fused * min_b_1;
        }
        *C(i, j, NDIM) += hsum_float_8(sum_packed_fp_0) + extra_sum_fp_0;
        *C(i + 1, j, NDIM) += hsum_float_8(sum_packed_fp_1) + extra_sum_fp_1;
    }

}


