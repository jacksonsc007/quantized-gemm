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


static int8_t blockA_packed[MC * KC] __attribute__((aligned(64)));
static float sA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static uint8_t blockB_packed[KC / 2 * NC] __attribute__((aligned(64)));
static float sB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));


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
    // Process 4 rows and 4 columns at a time
        
    // Accumulators for all combinations of i and j - expanded
    __m256 sum_packed_fp_0_0 = _mm256_setzero_ps();
    __m256 sum_packed_fp_0_1 = _mm256_setzero_ps();
    __m256 sum_packed_fp_0_2 = _mm256_setzero_ps();
    __m256 sum_packed_fp_0_3 = _mm256_setzero_ps();
    __m256 sum_packed_fp_1_0 = _mm256_setzero_ps();
    __m256 sum_packed_fp_1_1 = _mm256_setzero_ps();
    __m256 sum_packed_fp_1_2 = _mm256_setzero_ps();
    __m256 sum_packed_fp_1_3 = _mm256_setzero_ps();
    __m256 sum_packed_fp_2_0 = _mm256_setzero_ps();
    __m256 sum_packed_fp_2_1 = _mm256_setzero_ps();
    __m256 sum_packed_fp_2_2 = _mm256_setzero_ps();
    __m256 sum_packed_fp_2_3 = _mm256_setzero_ps();
    __m256 sum_packed_fp_3_0 = _mm256_setzero_ps();
    __m256 sum_packed_fp_3_1 = _mm256_setzero_ps();
    __m256 sum_packed_fp_3_2 = _mm256_setzero_ps();
    __m256 sum_packed_fp_3_3 = _mm256_setzero_ps();

    for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE) {
        int block_idx = p / Q_BLK_SIZE;
        
        // Load scaling factors for all active i and j - expanded
        float sA_0_0 = *blockSA(0, block_idx, block_k/Q_BLK_SIZE);
        float sA_0_1 = *blockSA(0, block_idx+1, block_k/Q_BLK_SIZE);
        float sA_1_0 = *blockSA(1, block_idx, block_k/Q_BLK_SIZE);
        float sA_1_1 = *blockSA(1, block_idx+1, block_k/Q_BLK_SIZE);
        float sA_2_0 = *blockSA(2, block_idx, block_k/Q_BLK_SIZE);
        float sA_2_1 = *blockSA(2, block_idx+1, block_k/Q_BLK_SIZE);
        float sA_3_0 = *blockSA(3, block_idx, block_k/Q_BLK_SIZE);
        float sA_3_1 = *blockSA(3, block_idx+1, block_k/Q_BLK_SIZE);
        
        float sB_0_0 = *blockSB(block_idx, 0, block_k/Q_BLK_SIZE);
        float sB_0_1 = *blockSB(block_idx+1, 0, block_k/Q_BLK_SIZE);
        float sB_1_0 = *blockSB(block_idx, 1, block_k/Q_BLK_SIZE);
        float sB_1_1 = *blockSB(block_idx+1, 1, block_k/Q_BLK_SIZE);
        float sB_2_0 = *blockSB(block_idx, 2, block_k/Q_BLK_SIZE);
        float sB_2_1 = *blockSB(block_idx+1, 2, block_k/Q_BLK_SIZE);
        float sB_3_0 = *blockSB(block_idx, 3, block_k/Q_BLK_SIZE);
        float sB_3_1 = *blockSB(block_idx+1, 3, block_k/Q_BLK_SIZE);
        
        // Load A blocks for active i - expanded
        __m256i a_pack_int8_0_0 = _mm256_loadu_si256((__m256i *) blockA(0, p, block_k));
        __m256i a_pack_int8_0_1 = _mm256_loadu_si256((__m256i *) blockA(0, p+Q_BLK_SIZE, block_k));
        __m256i a_pack_int8_1_0 = _mm256_loadu_si256((__m256i *) blockA(1, p, block_k));
        __m256i a_pack_int8_1_1 = _mm256_loadu_si256((__m256i *) blockA(1, p+Q_BLK_SIZE, block_k));
        __m256i a_pack_int8_2_0 = _mm256_loadu_si256((__m256i *) blockA(2, p, block_k));
        __m256i a_pack_int8_2_1 = _mm256_loadu_si256((__m256i *) blockA(2, p+Q_BLK_SIZE, block_k));
        __m256i a_pack_int8_3_0 = _mm256_loadu_si256((__m256i *) blockA(3, p, block_k));
        __m256i a_pack_int8_3_1 = _mm256_loadu_si256((__m256i *) blockA(3, p+Q_BLK_SIZE, block_k));
        
        // Load B blocks for active j - expanded
        __m256i b_pack_int8_0 = _mm256_loadu_si256((__m256i *) blockB(p/2, 0, block_k/2));
        __m256i b_pack_int8_1 = _mm256_loadu_si256((__m256i *) blockB(p/2, 1, block_k/2));
        __m256i b_pack_int8_2 = _mm256_loadu_si256((__m256i *) blockB(p/2, 2, block_k/2));
        __m256i b_pack_int8_3 = _mm256_loadu_si256((__m256i *) blockB(p/2, 3, block_k/2));
        
        __m256i mask = _mm256_set1_epi8(0x0f);
        
        // Process B blocks - expanded
        __m256i b_pack_int8_low_0 = _mm256_and_si256(b_pack_int8_0, mask);
        __m256i b_pack_int8_high_0 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_0, 4), mask);
        b_pack_int8_low_0 = _mm256_sub_epi8(b_pack_int8_low_0, _mm256_set1_epi8(8));
        b_pack_int8_high_0 = _mm256_sub_epi8(b_pack_int8_high_0, _mm256_set1_epi8(8));
        
        __m256i b_pack_int8_low_1 = _mm256_and_si256(b_pack_int8_1, mask);
        __m256i b_pack_int8_high_1 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_1, 4), mask);
        b_pack_int8_low_1 = _mm256_sub_epi8(b_pack_int8_low_1, _mm256_set1_epi8(8));
        b_pack_int8_high_1 = _mm256_sub_epi8(b_pack_int8_high_1, _mm256_set1_epi8(8));
        
        __m256i b_pack_int8_low_2 = _mm256_and_si256(b_pack_int8_2, mask);
        __m256i b_pack_int8_high_2 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_2, 4), mask);
        b_pack_int8_low_2 = _mm256_sub_epi8(b_pack_int8_low_2, _mm256_set1_epi8(8));
        b_pack_int8_high_2 = _mm256_sub_epi8(b_pack_int8_high_2, _mm256_set1_epi8(8));
        
        __m256i b_pack_int8_low_3 = _mm256_and_si256(b_pack_int8_3, mask);
        __m256i b_pack_int8_high_3 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_3, 4), mask);
        b_pack_int8_low_3 = _mm256_sub_epi8(b_pack_int8_low_3, _mm256_set1_epi8(8));
        b_pack_int8_high_3 = _mm256_sub_epi8(b_pack_int8_high_3, _mm256_set1_epi8(8));
        
        // Compute all active i/j combinations - expanded
        
        // i = 0
        {
            // j = 0
            float fused_s_0 = sA_0_0 * sB_0_0;
            float fused_s_1 = sA_0_1 * sB_0_1;
            __m256 tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_0_0, b_pack_int8_low_0);
            __m256 tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_0_1, b_pack_int8_high_0);
            sum_packed_fp_0_0 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_0_0);
            sum_packed_fp_0_0 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_0_0);
            
            // j = 1
            fused_s_0 = sA_0_0 * sB_1_0;
            fused_s_1 = sA_0_1 * sB_1_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_0_0, b_pack_int8_low_1);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_0_1, b_pack_int8_high_1);
            sum_packed_fp_0_1 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_0_1);
            sum_packed_fp_0_1 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_0_1);
            
            // j = 2
            fused_s_0 = sA_0_0 * sB_2_0;
            fused_s_1 = sA_0_1 * sB_2_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_0_0, b_pack_int8_low_2);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_0_1, b_pack_int8_high_2);
            sum_packed_fp_0_2 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_0_2);
            sum_packed_fp_0_2 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_0_2);
            
            // j = 3
            fused_s_0 = sA_0_0 * sB_3_0;
            fused_s_1 = sA_0_1 * sB_3_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_0_0, b_pack_int8_low_3);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_0_1, b_pack_int8_high_3);
            sum_packed_fp_0_3 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_0_3);
            sum_packed_fp_0_3 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_0_3);
        }
        
        // i = 1
        {
            // j = 0
            float fused_s_0 = sA_1_0 * sB_0_0;
            float fused_s_1 = sA_1_1 * sB_0_1;
            __m256 tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_1_0, b_pack_int8_low_0);
            __m256 tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_1_1, b_pack_int8_high_0);
            sum_packed_fp_1_0 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_1_0);
            sum_packed_fp_1_0 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_1_0);
            
            // j = 1
            fused_s_0 = sA_1_0 * sB_1_0;
            fused_s_1 = sA_1_1 * sB_1_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_1_0, b_pack_int8_low_1);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_1_1, b_pack_int8_high_1);
            sum_packed_fp_1_1 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_1_1);
            sum_packed_fp_1_1 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_1_1);
            
            // j = 2
            fused_s_0 = sA_1_0 * sB_2_0;
            fused_s_1 = sA_1_1 * sB_2_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_1_0, b_pack_int8_low_2);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_1_1, b_pack_int8_high_2);
            sum_packed_fp_1_2 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_1_2);
            sum_packed_fp_1_2 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_1_2);
            
            // j = 3
            fused_s_0 = sA_1_0 * sB_3_0;
            fused_s_1 = sA_1_1 * sB_3_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_1_0, b_pack_int8_low_3);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_1_1, b_pack_int8_high_3);
            sum_packed_fp_1_3 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_1_3);
            sum_packed_fp_1_3 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_1_3);
        }
        
        // i = 2
        {
            // j = 0
            float fused_s_0 = sA_2_0 * sB_0_0;
            float fused_s_1 = sA_2_1 * sB_0_1;
            __m256 tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_2_0, b_pack_int8_low_0);
            __m256 tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_2_1, b_pack_int8_high_0);
            sum_packed_fp_2_0 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_2_0);
            sum_packed_fp_2_0 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_2_0);
            
            // j = 1
            fused_s_0 = sA_2_0 * sB_1_0;
            fused_s_1 = sA_2_1 * sB_1_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_2_0, b_pack_int8_low_1);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_2_1, b_pack_int8_high_1);
            sum_packed_fp_2_1 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_2_1);
            sum_packed_fp_2_1 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_2_1);
            
            // j = 2
            fused_s_0 = sA_2_0 * sB_2_0;
            fused_s_1 = sA_2_1 * sB_2_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_2_0, b_pack_int8_low_2);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_2_1, b_pack_int8_high_2);
            sum_packed_fp_2_2 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_2_2);
            sum_packed_fp_2_2 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_2_2);
            
            // j = 3
            fused_s_0 = sA_2_0 * sB_3_0;
            fused_s_1 = sA_2_1 * sB_3_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_2_0, b_pack_int8_low_3);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_2_1, b_pack_int8_high_3);
            sum_packed_fp_2_3 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_2_3);
            sum_packed_fp_2_3 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_2_3);
        }
        
        // i = 3
        {
            // j = 0
            float fused_s_0 = sA_3_0 * sB_0_0;
            float fused_s_1 = sA_3_1 * sB_0_1;
            __m256 tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_3_0, b_pack_int8_low_0);
            __m256 tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_3_1, b_pack_int8_high_0);
            sum_packed_fp_3_0 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_3_0);
            sum_packed_fp_3_0 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_3_0);
            
            // j = 1
            fused_s_0 = sA_3_0 * sB_1_0;
            fused_s_1 = sA_3_1 * sB_1_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_3_0, b_pack_int8_low_1);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_3_1, b_pack_int8_high_1);
            sum_packed_fp_3_1 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_3_1);
            sum_packed_fp_3_1 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_3_1);
            
            // j = 2
            fused_s_0 = sA_3_0 * sB_2_0;
            fused_s_1 = sA_3_1 * sB_2_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_3_0, b_pack_int8_low_2);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_3_1, b_pack_int8_high_2);
            sum_packed_fp_3_2 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_3_2);
            sum_packed_fp_3_2 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_3_2);
            
            // j = 3
            fused_s_0 = sA_3_0 * sB_3_0;
            fused_s_1 = sA_3_1 * sB_3_1;
            tmp_0 = mul_sum_i8_pairs_float(a_pack_int8_3_0, b_pack_int8_low_3);
            tmp_1 = mul_sum_i8_pairs_float(a_pack_int8_3_1, b_pack_int8_high_3);
            sum_packed_fp_3_3 = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp_3_3);
            sum_packed_fp_3_3 = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp_3_3);
        }
    }
    
    // Store results for all active i/j combinations - expanded
    *C(0, 0, NDIM) += hsum_float_8(sum_packed_fp_0_0);
    *C(0, 1, NDIM) += hsum_float_8(sum_packed_fp_0_1);
    *C(0, 2, NDIM) += hsum_float_8(sum_packed_fp_0_2);
    *C(0, 3, NDIM) += hsum_float_8(sum_packed_fp_0_3);
    *C(1, 0, NDIM) += hsum_float_8(sum_packed_fp_1_0);
    *C(1, 1, NDIM) += hsum_float_8(sum_packed_fp_1_1);
    *C(1, 2, NDIM) += hsum_float_8(sum_packed_fp_1_2);
    *C(1, 3, NDIM) += hsum_float_8(sum_packed_fp_1_3);
    *C(2, 0, NDIM) += hsum_float_8(sum_packed_fp_2_0);
    *C(2, 1, NDIM) += hsum_float_8(sum_packed_fp_2_1);
    *C(2, 2, NDIM) += hsum_float_8(sum_packed_fp_2_2);
    *C(2, 3, NDIM) += hsum_float_8(sum_packed_fp_2_3);
    *C(3, 0, NDIM) += hsum_float_8(sum_packed_fp_3_0);
    *C(3, 1, NDIM) += hsum_float_8(sum_packed_fp_3_1);
    *C(3, 2, NDIM) += hsum_float_8(sum_packed_fp_3_2);
    *C(3, 3, NDIM) += hsum_float_8(sum_packed_fp_3_3);
}