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

void matmul_kernel_naive(
    int8_t * blockA, uint8_t * blockB,
    float * blockSA, float * blockSB, float * blockMinB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(block_k % Q_BLK_SIZE == 0);
    
    for (int i = 0; i < block_m; ++i) 
    for (int j = 0; j < block_n; ++j)
    {
        float sum = 0;
        for (int p = 0; p < block_k; p += Q_BLK_SIZE * 2)
        {
            // SA: row-major
            // SB: column-major
            int block_idx = p / Q_BLK_SIZE;
            float s_a_0 = *blockSA(i, block_idx, block_k / Q_BLK_SIZE);
            float s_b_0 = *blockSB(block_idx, j, block_k / Q_BLK_SIZE);
            float s_a_1 = *blockSA(i, block_idx + 1, block_k / Q_BLK_SIZE);
            float s_b_1 = *blockSB(block_idx + 1, j, block_k / Q_BLK_SIZE);

            float min_b_0 = *blockMinB(block_idx, j, block_k / Q_BLK_SIZE);
            float min_b_1 = *blockMinB(block_idx + 1, j, block_k / Q_BLK_SIZE);
            float sum_a_0 = 0;
            float sum_a_1 = 0;
            
            for (int q = 0; q < Q_BLK_SIZE; q++)
            {
                int8_t a_low = *blockA(i, p + q, block_k);
                int8_t a_high = *blockA(i, p + q + Q_BLK_SIZE, block_k); 
                uint8_t b = *blockB(p / 2 + q, j, block_k / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f);
                int8_t b_high = (b >> 4);
                // if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                // {
                //     printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                // }
                sum_a_0 += (float) a_low;
                sum_a_1 += (float) a_high;
                sum += (float)a_low * (float)b_low * s_a_0 * s_b_0;
                sum += (float)a_high * (float)b_high * s_a_1 * s_b_1;
            }
            sum += sum_a_0 * s_a_0 * min_b_0;
            sum += sum_a_1 * s_a_1 * min_b_1;
        }    
        *C(i, j, NDIM) += sum;
    }
}


/*
blockA: packed. row-major.      (MR, KC)
blockB: packed. column-major.   (KC, NR)
blockSA: packed. row-major.     (MR, KC/Q_BLK_SIZE)
blockSB: packed. column-major.  (KC/Q_BLK_SIZE, NR)
*/
void kernel(
    int8_t * blockA, uint8_t * blockB,
    float * blockSA, float * blockScaledSumA, float * blockSB, float * blockMinB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    for (int i = 0; i < block_m; i += 4) {
        for (int j = 0; j < block_n; j += 2) {
            // Initialize accumulators for i+0, i+1, i+2, i+3 and j+0, j+1
            __m256 sum_packed_fp_00 = _mm256_setzero_ps();
            __m256 sum_packed_fp_01 = _mm256_setzero_ps();
            __m256 sum_packed_fp_10 = _mm256_setzero_ps();
            __m256 sum_packed_fp_11 = _mm256_setzero_ps();
            __m256 sum_packed_fp_20 = _mm256_setzero_ps();
            __m256 sum_packed_fp_21 = _mm256_setzero_ps();
            __m256 sum_packed_fp_30 = _mm256_setzero_ps();
            __m256 sum_packed_fp_31 = _mm256_setzero_ps();
            
            float extra_sum_fp_00 = 0;
            float extra_sum_fp_01 = 0;
            float extra_sum_fp_10 = 0;
            float extra_sum_fp_11 = 0;
            float extra_sum_fp_20 = 0;
            float extra_sum_fp_21 = 0;
            float extra_sum_fp_30 = 0;
            float extra_sum_fp_31 = 0;

            for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE) {
                int block_idx = p / Q_BLK_SIZE;
                
                // Load scale factors for all i and j combinations
                float sA_0_0 = *blockSA(i+0, block_idx, block_k/Q_BLK_SIZE);
                float sA_0_1 = *blockSA(i+1, block_idx, block_k/Q_BLK_SIZE);
                float sA_0_2 = *blockSA(i+2, block_idx, block_k/Q_BLK_SIZE);
                float sA_0_3 = *blockSA(i+3, block_idx, block_k/Q_BLK_SIZE);
                
                float sB_0_0 = *blockSB(block_idx, j+0, block_k/Q_BLK_SIZE);
                float sB_0_1 = *blockSB(block_idx, j+1, block_k/Q_BLK_SIZE);
                
                float sA_1_0 = *blockSA(i+0, block_idx + 1, block_k/Q_BLK_SIZE);
                float sA_1_1 = *blockSA(i+1, block_idx + 1, block_k/Q_BLK_SIZE);
                float sA_1_2 = *blockSA(i+2, block_idx + 1, block_k/Q_BLK_SIZE);
                float sA_1_3 = *blockSA(i+3, block_idx + 1, block_k/Q_BLK_SIZE);
                
                float sB_1_0 = *blockSB(block_idx + 1, j+0, block_k/Q_BLK_SIZE);
                float sB_1_1 = *blockSB(block_idx + 1, j+1, block_k/Q_BLK_SIZE);
                
                // Compute fused scales
                float fused_s_0_00 = sA_0_0 * sB_0_0;
                float fused_s_0_01 = sA_0_0 * sB_0_1;
                float fused_s_0_10 = sA_0_1 * sB_0_0;
                float fused_s_0_11 = sA_0_1 * sB_0_1;
                float fused_s_0_20 = sA_0_2 * sB_0_0;
                float fused_s_0_21 = sA_0_2 * sB_0_1;
                float fused_s_0_30 = sA_0_3 * sB_0_0;
                float fused_s_0_31 = sA_0_3 * sB_0_1;
                
                float fused_s_1_00 = sA_1_0 * sB_1_0;
                float fused_s_1_01 = sA_1_0 * sB_1_1;
                float fused_s_1_10 = sA_1_1 * sB_1_0;
                float fused_s_1_11 = sA_1_1 * sB_1_1;
                float fused_s_1_20 = sA_1_2 * sB_1_0;
                float fused_s_1_21 = sA_1_2 * sB_1_1;
                float fused_s_1_30 = sA_1_3 * sB_1_0;
                float fused_s_1_31 = sA_1_3 * sB_1_1;

                // Load min and sum values
                float min_b_0_0 = *blockMinB(block_idx, j+0, block_k / Q_BLK_SIZE);
                float min_b_0_1 = *blockMinB(block_idx, j+1, block_k / Q_BLK_SIZE);
                float min_b_1_0 = *blockMinB(block_idx + 1, j+0, block_k / Q_BLK_SIZE);
                float min_b_1_1 = *blockMinB(block_idx + 1, j+1, block_k / Q_BLK_SIZE);
                
                float sum_a_0_fused_0 = *blockScaledSumA(i+0, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_0_fused_1 = *blockScaledSumA(i+1, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_0_fused_2 = *blockScaledSumA(i+2, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_0_fused_3 = *blockScaledSumA(i+3, block_idx, block_k / Q_BLK_SIZE);
                
                float sum_a_1_fused_0 = *blockScaledSumA(i+0, block_idx + 1, block_k / Q_BLK_SIZE);
                float sum_a_1_fused_1 = *blockScaledSumA(i+1, block_idx + 1, block_k / Q_BLK_SIZE);
                float sum_a_1_fused_2 = *blockScaledSumA(i+2, block_idx + 1, block_k / Q_BLK_SIZE);
                float sum_a_1_fused_3 = *blockScaledSumA(i+3, block_idx + 1, block_k / Q_BLK_SIZE);

                // Load A blocks for all i
                __m256i a_pack_int8_low_0 = _mm256_loadu_si256((__m256i *) blockA(i+0, p, block_k));
                __m256i a_pack_int8_high_0 = _mm256_loadu_si256((__m256i *) blockA(i+0, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_low_1 = _mm256_loadu_si256((__m256i *) blockA(i+1, p, block_k));
                __m256i a_pack_int8_high_1 = _mm256_loadu_si256((__m256i *) blockA(i+1, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_low_2 = _mm256_loadu_si256((__m256i *) blockA(i+2, p, block_k));
                __m256i a_pack_int8_high_2 = _mm256_loadu_si256((__m256i *) blockA(i+2, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_low_3 = _mm256_loadu_si256((__m256i *) blockA(i+3, p, block_k));
                __m256i a_pack_int8_high_3 = _mm256_loadu_si256((__m256i *) blockA(i+3, p + Q_BLK_SIZE, block_k));

                // Load B blocks (shared for all i)
                __m256i b_pack_int8 = _mm256_loadu_si256((__m256i *) blockB(p/2, j/2, block_k/2));
                __m256i mask = _mm256_set1_epi8(0x0f);
                __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
                __m256i b_pack_int8_high = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8, 4), mask);

                // Compute products for all i and j combinations
                // j+0
                __m256 tmp_0_00 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_0);
                __m256 tmp_1_00 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_0);
                sum_packed_fp_00 = _mm256_fmadd_ps(tmp_0_00, _mm256_set1_ps(fused_s_0_00), sum_packed_fp_00);
                sum_packed_fp_00 = _mm256_fmadd_ps(tmp_1_00, _mm256_set1_ps(fused_s_1_00), sum_packed_fp_00);
                
                __m256 tmp_0_10 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_1);
                __m256 tmp_1_10 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_1);
                sum_packed_fp_10 = _mm256_fmadd_ps(tmp_0_10, _mm256_set1_ps(fused_s_0_10), sum_packed_fp_10);
                sum_packed_fp_10 = _mm256_fmadd_ps(tmp_1_10, _mm256_set1_ps(fused_s_1_10), sum_packed_fp_10);
                
                __m256 tmp_0_20 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_2);
                __m256 tmp_1_20 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_2);
                sum_packed_fp_20 = _mm256_fmadd_ps(tmp_0_20, _mm256_set1_ps(fused_s_0_20), sum_packed_fp_20);
                sum_packed_fp_20 = _mm256_fmadd_ps(tmp_1_20, _mm256_set1_ps(fused_s_1_20), sum_packed_fp_20);
                
                __m256 tmp_0_30 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_3);
                __m256 tmp_1_30 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_3);
                sum_packed_fp_30 = _mm256_fmadd_ps(tmp_0_30, _mm256_set1_ps(fused_s_0_30), sum_packed_fp_30);
                sum_packed_fp_30 = _mm256_fmadd_ps(tmp_1_30, _mm256_set1_ps(fused_s_1_30), sum_packed_fp_30);
                
                // j+1
                __m256 tmp_0_01 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_0);
                __m256 tmp_1_01 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_0);
                sum_packed_fp_01 = _mm256_fmadd_ps(tmp_0_01, _mm256_set1_ps(fused_s_0_01), sum_packed_fp_01);
                sum_packed_fp_01 = _mm256_fmadd_ps(tmp_1_01, _mm256_set1_ps(fused_s_1_01), sum_packed_fp_01);
                
                __m256 tmp_0_11 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_1);
                __m256 tmp_1_11 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_1);
                sum_packed_fp_11 = _mm256_fmadd_ps(tmp_0_11, _mm256_set1_ps(fused_s_0_11), sum_packed_fp_11);
                sum_packed_fp_11 = _mm256_fmadd_ps(tmp_1_11, _mm256_set1_ps(fused_s_1_11), sum_packed_fp_11);
                
                __m256 tmp_0_21 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_2);
                __m256 tmp_1_21 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_2);
                sum_packed_fp_21 = _mm256_fmadd_ps(tmp_0_21, _mm256_set1_ps(fused_s_0_21), sum_packed_fp_21);
                sum_packed_fp_21 = _mm256_fmadd_ps(tmp_1_21, _mm256_set1_ps(fused_s_1_21), sum_packed_fp_21);
                
                __m256 tmp_0_31 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_3);
                __m256 tmp_1_31 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_3);
                sum_packed_fp_31 = _mm256_fmadd_ps(tmp_0_31, _mm256_set1_ps(fused_s_0_31), sum_packed_fp_31);
                sum_packed_fp_31 = _mm256_fmadd_ps(tmp_1_31, _mm256_set1_ps(fused_s_1_31), sum_packed_fp_31);

                // Update extra sums
                extra_sum_fp_00 += sum_a_0_fused_0 * min_b_0_0 + sum_a_1_fused_0 * min_b_1_0;
                extra_sum_fp_01 += sum_a_0_fused_0 * min_b_0_1 + sum_a_1_fused_0 * min_b_1_1;
                extra_sum_fp_10 += sum_a_0_fused_1 * min_b_0_0 + sum_a_1_fused_1 * min_b_1_0;
                extra_sum_fp_11 += sum_a_0_fused_1 * min_b_0_1 + sum_a_1_fused_1 * min_b_1_1;
                extra_sum_fp_20 += sum_a_0_fused_2 * min_b_0_0 + sum_a_1_fused_2 * min_b_1_0;
                extra_sum_fp_21 += sum_a_0_fused_2 * min_b_0_1 + sum_a_1_fused_2 * min_b_1_1;
                extra_sum_fp_30 += sum_a_0_fused_3 * min_b_0_0 + sum_a_1_fused_3 * min_b_1_0;
                extra_sum_fp_31 += sum_a_0_fused_3 * min_b_0_1 + sum_a_1_fused_3 * min_b_1_1;
            }
            
            // Store results for all unrolled i and j combinations
            *C(i + 0, j + 0, NDIM) += (hsum_float_8(sum_packed_fp_00) + extra_sum_fp_00);
            *C(i + 0, j + 1, NDIM) += (hsum_float_8(sum_packed_fp_01) + extra_sum_fp_01);
            *C(i + 1, j + 0, NDIM) += (hsum_float_8(sum_packed_fp_10) + extra_sum_fp_10);
            *C(i + 1, j + 1, NDIM) += (hsum_float_8(sum_packed_fp_11) + extra_sum_fp_11);
            *C(i + 2, j + 0, NDIM) += (hsum_float_8(sum_packed_fp_20) + extra_sum_fp_20);
            *C(i + 2, j + 1, NDIM) += (hsum_float_8(sum_packed_fp_21) + extra_sum_fp_21);
            *C(i + 3, j + 0, NDIM) += (hsum_float_8(sum_packed_fp_30) + extra_sum_fp_30);
            *C(i + 3, j + 1, NDIM) += (hsum_float_8(sum_packed_fp_31) + extra_sum_fp_31);
        }
        
        // Handle remaining j iterations if block_n not divisible by 2
        for (int j = (block_n / 2) * 2; j < block_n; ++j) {
            __m256 sum_packed_fp_0 = _mm256_setzero_ps();
            __m256 sum_packed_fp_1 = _mm256_setzero_ps();
            __m256 sum_packed_fp_2 = _mm256_setzero_ps();
            __m256 sum_packed_fp_3 = _mm256_setzero_ps();
            
            float extra_sum_fp_0 = 0;
            float extra_sum_fp_1 = 0;
            float extra_sum_fp_2 = 0;
            float extra_sum_fp_3 = 0;

            for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE) {
                int block_idx = p / Q_BLK_SIZE;
                float sA_0 = *blockSA(i+0, block_idx, block_k/Q_BLK_SIZE);
                float sA_1 = *blockSA(i+1, block_idx, block_k/Q_BLK_SIZE);
                float sA_2 = *blockSA(i+2, block_idx, block_k/Q_BLK_SIZE);
                float sA_3 = *blockSA(i+3, block_idx, block_k/Q_BLK_SIZE);
                float sB_0 = *blockSB(block_idx, j, block_k/Q_BLK_SIZE);
                float sA_0_1 = *blockSA(i+0, block_idx + 1, block_k/Q_BLK_SIZE);
                float sA_1_1 = *blockSA(i+1, block_idx + 1, block_k/Q_BLK_SIZE);
                float sA_2_1 = *blockSA(i+2, block_idx + 1, block_k/Q_BLK_SIZE);
                float sA_3_1 = *blockSA(i+3, block_idx + 1, block_k/Q_BLK_SIZE);
                float sB_1 = *blockSB(block_idx + 1, j, block_k/Q_BLK_SIZE);
                
                float fused_s_0_0 = sA_0 * sB_0;
                float fused_s_0_1 = sA_1 * sB_0;
                float fused_s_0_2 = sA_2 * sB_0;
                float fused_s_0_3 = sA_3 * sB_0;
                
                float fused_s_1_0 = sA_0_1 * sB_1;
                float fused_s_1_1 = sA_1_1 * sB_1;
                float fused_s_1_2 = sA_2_1 * sB_1;
                float fused_s_1_3 = sA_3_1 * sB_1;

                float min_b_0 = *blockMinB(block_idx, j, block_k / Q_BLK_SIZE);
                float min_b_1 = *blockMinB(block_idx + 1, j, block_k / Q_BLK_SIZE);
                
                float sum_a_0_fused_0 = *blockScaledSumA(i+0, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_0_fused_1 = *blockScaledSumA(i+1, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_0_fused_2 = *blockScaledSumA(i+2, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_0_fused_3 = *blockScaledSumA(i+3, block_idx, block_k / Q_BLK_SIZE);
                
                float sum_a_1_fused_0 = *blockScaledSumA(i+0, block_idx + 1, block_k / Q_BLK_SIZE);
                float sum_a_1_fused_1 = *blockScaledSumA(i+1, block_idx + 1, block_k / Q_BLK_SIZE);
                float sum_a_1_fused_2 = *blockScaledSumA(i+2, block_idx + 1, block_k / Q_BLK_SIZE);
                float sum_a_1_fused_3 = *blockScaledSumA(i+3, block_idx + 1, block_k / Q_BLK_SIZE);

                __m256i a_pack_int8_low_0 = _mm256_loadu_si256((__m256i *) blockA(i+0, p, block_k));
                __m256i a_pack_int8_high_0 = _mm256_loadu_si256((__m256i *) blockA(i+0, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_low_1 = _mm256_loadu_si256((__m256i *) blockA(i+1, p, block_k));
                __m256i a_pack_int8_high_1 = _mm256_loadu_si256((__m256i *) blockA(i+1, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_low_2 = _mm256_loadu_si256((__m256i *) blockA(i+2, p, block_k));
                __m256i a_pack_int8_high_2 = _mm256_loadu_si256((__m256i *) blockA(i+2, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_low_3 = _mm256_loadu_si256((__m256i *) blockA(i+3, p, block_k));
                __m256i a_pack_int8_high_3 = _mm256_loadu_si256((__m256i *) blockA(i+3, p + Q_BLK_SIZE, block_k));

                __m256i b_pack_int8 = _mm256_loadu_si256((__m256i *) blockB(p/2, j/2, block_k/2));
                __m256i mask = _mm256_set1_epi8(0x0f);
                __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
                __m256i b_pack_int8_high = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8, 4), mask);

                __m256 tmp_0_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_0);
                __m256 tmp_1_0 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_0);
                sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_0, _mm256_set1_ps(fused_s_0_0), sum_packed_fp_0);
                sum_packed_fp_0 = _mm256_fmadd_ps(tmp_1_0, _mm256_set1_ps(fused_s_1_0), sum_packed_fp_0);
                
                __m256 tmp_0_1 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_1);
                __m256 tmp_1_1 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_1);
                sum_packed_fp_1 = _mm256_fmadd_ps(tmp_0_1, _mm256_set1_ps(fused_s_0_1), sum_packed_fp_1);
                sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_1, _mm256_set1_ps(fused_s_1_1), sum_packed_fp_1);
                
                __m256 tmp_0_2 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_2);
                __m256 tmp_1_2 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_2);
                sum_packed_fp_2 = _mm256_fmadd_ps(tmp_0_2, _mm256_set1_ps(fused_s_0_2), sum_packed_fp_2);
                sum_packed_fp_2 = _mm256_fmadd_ps(tmp_1_2, _mm256_set1_ps(fused_s_1_2), sum_packed_fp_2);
                
                __m256 tmp_0_3 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_3);
                __m256 tmp_1_3 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high_3);
                sum_packed_fp_3 = _mm256_fmadd_ps(tmp_0_3, _mm256_set1_ps(fused_s_0_3), sum_packed_fp_3);
                sum_packed_fp_3 = _mm256_fmadd_ps(tmp_1_3, _mm256_set1_ps(fused_s_1_3), sum_packed_fp_3);

                extra_sum_fp_0 += sum_a_0_fused_0 * min_b_0 + sum_a_1_fused_0 * min_b_1;
                extra_sum_fp_1 += sum_a_0_fused_1 * min_b_0 + sum_a_1_fused_1 * min_b_1;
                extra_sum_fp_2 += sum_a_0_fused_2 * min_b_0 + sum_a_1_fused_2 * min_b_1;
                extra_sum_fp_3 += sum_a_0_fused_3 * min_b_0 + sum_a_1_fused_3 * min_b_1;
            }
            
            *C(i+0, j, NDIM) += (hsum_float_8(sum_packed_fp_0) + extra_sum_fp_0);
            *C(i+1, j, NDIM) += (hsum_float_8(sum_packed_fp_1) + extra_sum_fp_1);
            *C(i+2, j, NDIM) += (hsum_float_8(sum_packed_fp_2) + extra_sum_fp_2);
            *C(i+3, j, NDIM) += (hsum_float_8(sum_packed_fp_3) + extra_sum_fp_3);
        }
    }
    
    // Handle remaining i iterations if block_m not divisible by 4
    for (int i = (block_m / 4) * 4; i < block_m; ++i) {
        for (int j = 0; j < block_n; ++j) {
            __m256 sum_packed_fp = _mm256_setzero_ps();
            float extra_sum_fp = 0;
            for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE) {
                int block_idx = p / Q_BLK_SIZE;
                float sA_0 = *blockSA(i, block_idx, block_k/Q_BLK_SIZE);
                float sB_0 = *blockSB(block_idx, j, block_k/Q_BLK_SIZE);
                float sA_1 = *blockSA(i, block_idx + 1, block_k/Q_BLK_SIZE);
                float sB_1 = *blockSB(block_idx + 1, j, block_k/Q_BLK_SIZE);
                float fused_s_0 = sA_0 * sB_0;
                float fused_s_1 = sA_1 * sB_1;

                float min_b_0 = *blockMinB(block_idx, j, block_k / Q_BLK_SIZE);
                float min_b_1 = *blockMinB(block_idx + 1, j, block_k / Q_BLK_SIZE);
                float sum_a_0_fused = *blockScaledSumA(i, block_idx, block_k / Q_BLK_SIZE);
                float sum_a_1_fused = *blockScaledSumA(i, block_idx + 1, block_k / Q_BLK_SIZE);

                __m256i a_pack_int8_low = _mm256_loadu_si256((__m256i *) blockA(i, p, block_k));
                __m256i a_pack_int8_high = _mm256_loadu_si256((__m256i *) blockA(i, p + Q_BLK_SIZE, block_k));

                __m256i b_pack_int8 = _mm256_loadu_si256((__m256i *) blockB(p/2, j/2, block_k/2));
                __m256i mask = _mm256_set1_epi8(0x0f);
                __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
                __m256i b_pack_int8_high = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8, 4), mask);

                __m256 tmp_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low);
                __m256 tmp_1 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high);
                sum_packed_fp = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp);
                sum_packed_fp = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp);

                extra_sum_fp += sum_a_0_fused * min_b_0 + sum_a_1_fused * min_b_1;
            }
            *C(i, j, NDIM) += (hsum_float_8(sum_packed_fp) + extra_sum_fp);
        }
    }
}


// ======================================================================================================================================================
// pack matrix A
// block size: mc * kc
// panel size: MR * kc
// Both panel and blockA are row-major, no need to pack panel-wise
// ======================================================================================================================================================
void pack_blockA(int8_t* A, int8_t* blockA, const int mc, const int kc)
{
    // #pragma omp parallel for num_threads(NTHREADS)
    for (int i = 0; i < mc; i++)
    {
        for( int j = 0; j < kc; j++)
        {
            *blockA(i, j, kc) = *A(i, j, KDIM);
        }
    }
} 

// ======================================================================================================================================================
// pack matrix A scaling factors
// block size: mc * kc (kc is normalized by q_blk_size) 
// panel size: MR * kc 
// ======================================================================================================================================================
void pack_blockScaledSumA(float* A, float* blockA, int mc, int kc)
{
    // #pragma omp parallel for num_threads(NTHREADS)
    for (int i = 0; i < mc; i++)
    for (int j = 0; j < kc; j++)
    {
        *blockA(i, j, kc) = *A(i, j, KDIM/Q_BLK_SIZE);
    }
} 

void pack_blockSA(float* A, float* blockA, int mc, int kc)
{
    // #pragma omp parallel for num_threads(NTHREADS)
    for (int i = 0; i < mc; i++)
    for (int j = 0; j < kc; j++)
    {
        *blockA(i, j, kc) = *A(i, j, KDIM/Q_BLK_SIZE);
    }
} 

// ======================================================================================================================================================
// pack matrix B
// block size: kc * nc
// panel size: kc * NR, row_major
// ======================================================================================================================================================
void pack_blockB(uint8_t* B, uint8_t* blockB, int kc, int nc)
{
    // #pragma omp parallel for num_threads(NTHREADS)
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM / 2);
    }
} 

// ======================================================================================================================================================
// pack matrix B scaling factor
// block size: kc * nc (kc here is already normalized by q_blk_size)
// panel size: kc * NR, row_major
// ======================================================================================================================================================

void pack_blockMinB(float* B, float* blockB, int kc, int nc)
{
    // #pragma omp parallel for num_threads(NTHREADS)
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM/Q_BLK_SIZE);
    }
} 
void pack_blockSB(float* B, float* blockB, int kc, int nc)
{
    // #pragma omp parallel for num_threads(NTHREADS)
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM/Q_BLK_SIZE);
    }
} 

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{
    assert (KDIM % Q_BLK_SIZE == 0);
    assert (KC % Q_BLK_SIZE == 0);
    for (int jc = 0; jc < N; jc += NC)
    {
        int valid_NC = min(N - jc, NC);
        for (int pc = 0; pc < K; pc += KC)
        {
            int valid_KC = min(K - pc, KC);
            pack_blockB(
                B(pc/2, jc, KDIM / 2), blockB_packed, valid_KC/2, valid_NC
            );
            pack_blockSB(
                SB(pc/Q_BLK_SIZE, jc, KDIM / Q_BLK_SIZE), sB_packed, valid_KC/Q_BLK_SIZE, valid_NC
            );
            pack_blockMinB(
                MinB(pc/Q_BLK_SIZE, jc, KDIM / Q_BLK_SIZE), minB_packed, valid_KC/Q_BLK_SIZE, valid_NC
            );
            for (int ic = 0; ic < M; ic+= MC)
            {
                int valid_MC = min(M - ic, MC);
                pack_blockA(
                    A(ic, pc, KDIM), blockA_packed, valid_MC, valid_KC
                );
                pack_blockSA(
                    SA(ic, pc/Q_BLK_SIZE, KDIM / Q_BLK_SIZE), sA_packed, valid_MC, valid_KC/Q_BLK_SIZE
                );
                pack_blockScaledSumA(
                    ScaledSumA(ic, pc/Q_BLK_SIZE, KDIM / Q_BLK_SIZE), scaledSumA_packed, valid_MC, valid_KC/Q_BLK_SIZE
                );
                #pragma omp parallel for collapse(2) num_threads(NTHREADS)
                for(int jr = 0; jr < valid_NC; jr += NR)
                {
                    for(int ir = 0; ir < valid_MC; ir += MR)
                    {
                        int valid_nr = min(valid_NC - jr, NR);
                        int valid_mr = min(valid_MC - ir, MR);
                        #ifdef naive
                        matmul_kernel_naive(
                            (blockA_packed + ir * valid_KC),
                            (blockB_packed + jr * valid_KC / 2),
                            (sA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (scaledSumA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (sB_packed + jr * valid_KC / Q_BLK_SIZE),
                            (minB_packed + jr * valid_KC / Q_BLK_SIZE),
                            C(ic + ir, jc + jr, N),
                            valid_mr,
                            valid_nr,
                            valid_KC
                        );
                        #else
                        kernel(
                            (blockA_packed + ir * valid_KC),
                            (blockB_packed + jr * valid_KC / 2),
                            (sA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (scaledSumA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (sB_packed + jr * valid_KC / Q_BLK_SIZE),
                            (minB_packed + jr * valid_KC / Q_BLK_SIZE),
                            C(ic + ir, jc + jr, N),
                            valid_mr,
                            valid_nr,
                            valid_KC
                        );
                        #endif
                    }
                }
            }
        }
    }

}




void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB, float * blockMinB,
    const int M, const int N, const int K) {
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(K % Q_BLK_SIZE == 0);
    for (int i = 0; i < M; ++i) 
    for (int j = 0; j < N; ++j)
    {
        float sum = 0;
        // load 2 quantization blocks at once
        for (int p = 0; p < K; p += Q_BLK_SIZE * 2)
        {
            // SA: row-major
            // SB: column-major
            int block_idx = p / Q_BLK_SIZE;
            float s_a_0 = *SA(i, block_idx, KDIM / Q_BLK_SIZE);
            float s_a_1 = *SA(i, block_idx + 1, KDIM / Q_BLK_SIZE);
            float s_b_0 = *SB(block_idx, j, KDIM / Q_BLK_SIZE);
            float s_b_1 = *SB(block_idx + 1, j, KDIM / Q_BLK_SIZE);
                
                
            float min_b_0 = *blockMinB(block_idx, j, KDIM / Q_BLK_SIZE);
            float min_b_1 = *blockMinB(block_idx + 1, j, KDIM / Q_BLK_SIZE);
            float sum_a_0 = 0;
            float sum_a_1 = 0;
            for (int q = 0; q < Q_BLK_SIZE; q++)
            {
                // due to q4_0 memory strategy, the first byte consists of w0 and w16, so we need to get a0 and a16
                // origin order: (w0,w1), (w2,w3), (w4,w5), (w6,w7), (w8, w9), ... (w15,w31)
                // QM_ARM order: (w0,w16),(w1,w17),(w2,w18),(w3,w19),(w4, w20),... (w15,w31)
                //               |--|
                //               4 bits
                //               |------|
                //               8 bits (byte)
                //            low|----------------------------------------------------------|high
                //               0                         128 bit
                int8_t a_low = *A(i, p + q, KDIM);
                int8_t a_high = *A(i, p + q + Q_BLK_SIZE, KDIM); 
                uint8_t b = *B(p / 2 + q, j, KDIM / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f); // first block
                int8_t b_high = (b >> 4);  // second block
                // int8_t b_high = (b & 0x0f) - 8;
                // int8_t b_low = (b >> 4) - 8;
                // if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                // {
                //     printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                // }
                sum += (float)a_low * (float)b_low * s_a_0 * s_b_0;
                sum += (float)a_high * (float)b_high * s_a_1 * s_b_1;
                sum_a_0 += (float) a_low;
                sum_a_1 += (float) a_high;
            }
            sum += sum_a_0 * s_a_0 * min_b_0;
            sum += sum_a_1 * s_a_1 * min_b_1;
        }    
        *C(i, j, NDIM) = sum;
    }
}



void print_mat(float* mat, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", mat[i * N + j]);
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

void init_rand_uint8(uint8_t* mat, const int M, const int N) {

    for (int i = 0; i < M * N; i++) {
        *mat++ = (uint8_t)(rand() % 256);
    }
}
void init_rand_int8(int8_t* mat, const int M, const int N) {

    for (int i = 0; i < M * N; i++) {
        *mat++ = (int8_t)(rand() % 256 - 128);
    }
}

void init_rand_min_max(int8_t* mat, const int M, const int N, const int min, const int max) {
    for (int i = 0; i < M * N; i++) {
        *mat++ = (int8_t)(rand() % (max - min + 1) + min);
    }
}
void init_uint_const(uint8_t* mat, const uint8_t value, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            *mat++ = value;
        }
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

int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A  = (int8_t*)_mm_malloc(M * K * sizeof(int8_t), MEM_ALIGN);
    uint8_t* B = (uint8_t*)_mm_malloc(K / 2 * N * sizeof(uint8_t), MEM_ALIGN);
    float* sA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* scaledSumA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* MinB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    // init_rand(A, M, K);
    // init_rand(B, K, N);
    //
    // init_rand_int8(A, M, K);
    init_rand_min_max(A, M, K, -128, 127);
    // init_uint_const(A, 100, M, K);
    // NOTE: K / 2
    init_rand_uint8(B, K / 2, N);
    // init_uint_const(B, 1, K / 2, N);

    

    
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(scaledSumA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    init_rand(MinB, K / Q_BLK_SIZE, N);
    // init_const(sA, 10.1, M, K / Q_BLK_SIZE);
    // init_const(sB, 50.5, K / Q_BLK_SIZE, N);
    
    // init packed to debug
    // init_int_const(blockA_packed, -1, MC, KC);
    // init_int_const(blockB_packed, -1, KC /2, NC);
    // init_const(sA_packed, -1.0, MC, KC / Q_BLK_SIZE);
    // init_const(sB_packed, -1.0, KC /Q_BLK_SIZE, NC);    

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, MinB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    printf("\n\e[32m[INFO]\e[m MR=%d, NR=%d, MC=%d, NC=%d, KC=%d, M=%d, K=%d, N=%d\n", MR, NR, MC, NC, KC, M, K, N);
    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, scaledSumA,sB, MinB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        // print_mat(C, 5, 5);
        printf("\n\n");
        // print_mat(C_ref, 5, 5);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        if (!res) break;
#endif
        printf("\n");
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);

    return 0;
}
