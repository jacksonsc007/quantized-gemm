/*
This kernel is designed for 1xK and KxN matrix multiplication.

Serveral changes are made to the original code:
1. No need to pack A
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "stdbool.h"
// #include <cblas.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#ifndef NTHREADS
#define NTHREADS 1
#endif

#ifndef MR
#define MR 1
#endif

#ifndef NR
#define NR 4
#endif

#ifndef MC
#define MC 1
#endif

#ifndef NC
#define NC NR * 25 *  NTHREADS
#endif

#ifndef KC
#define KC 1024
#endif

#ifndef MDIM
    #define MDIM 1
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM 4096
    // #define NDIM NR * 2
#endif

#ifndef KDIM
    #define KDIM 4096
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


void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{
    for (int j = 0; j < N; j += 2)
    {
        __m256 sum_packed_fp_0 = _mm256_setzero_ps();
        __m256 sum_packed_fp_1 = _mm256_setzero_ps();
        
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            
            // Load scale factors for j and j+1
            float sA_0 = *SA(0, block_idx, K/Q_BLK_SIZE);
            float sB_0_j0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float sB_0_j1 = *SB(block_idx, j+1, K/Q_BLK_SIZE);
            
            float sA_1 = *SA(0, block_idx + 1, K/Q_BLK_SIZE);
            float sB_1_j0 = *SB(block_idx + 1, j, K/Q_BLK_SIZE);
            float sB_1_j1 = *SB(block_idx + 1, j+1, K/Q_BLK_SIZE);
            
            float fused_s_0_j0 = sA_0 * sB_0_j0;
            float fused_s_1_j0 = sA_1 * sB_1_j0;
            float fused_s_0_j1 = sA_0 * sB_0_j1;
            float fused_s_1_j1 = sA_1 * sB_1_j1;
            
            // Load A packed int8 (same for both j iterations)
            __m256i a_pack_int8_low = _mm256_loadu_si256((__m256i *) A(0, p, K));
            __m256i a_pack_int8_high = _mm256_loadu_si256((__m256i *) A(0, p + Q_BLK_SIZE, K));
            
            // Load B packed int8 for j and j+1
            __m256i b_pack_int8_j0 = _mm256_loadu_si256((__m256i *) B(p/2, j, K/2));
            __m256i b_pack_int8_j1 = _mm256_loadu_si256((__m256i *) B(p/2, j+1, K/2));
            
            __m256i mask = _mm256_set1_epi8(0x0f);
            
            // Process B for j
            __m256i b_pack_int8_low_j0 = _mm256_and_si256(b_pack_int8_j0, mask);
            __m256i b_pack_int8_high_j0 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j0, 4), mask);
            b_pack_int8_low_j0 = _mm256_sub_epi8(b_pack_int8_low_j0, _mm256_set1_epi8(8));
            b_pack_int8_high_j0 = _mm256_sub_epi8(b_pack_int8_high_j0, _mm256_set1_epi8(8));
            
            // Process B for j+1
            __m256i b_pack_int8_low_j1 = _mm256_and_si256(b_pack_int8_j1, mask);
            __m256i b_pack_int8_high_j1 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j1, 4), mask);
            b_pack_int8_low_j1 = _mm256_sub_epi8(b_pack_int8_low_j1, _mm256_set1_epi8(8));
            b_pack_int8_high_j1 = _mm256_sub_epi8(b_pack_int8_high_j1, _mm256_set1_epi8(8));
            
            // Compute for j
            __m256 tmp_0_j0 = mul_sum_i8_pairs_float(a_pack_int8_low, b_pack_int8_low_j0);
            __m256 tmp_1_j0 = mul_sum_i8_pairs_float(a_pack_int8_high, b_pack_int8_high_j0);
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_j0, _mm256_set1_ps(fused_s_0_j0), sum_packed_fp_0);
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_1_j0, _mm256_set1_ps(fused_s_1_j0), sum_packed_fp_0);
            
            // Compute for j+1
            __m256 tmp_0_j1 = mul_sum_i8_pairs_float(a_pack_int8_low, b_pack_int8_low_j1);
            __m256 tmp_1_j1 = mul_sum_i8_pairs_float(a_pack_int8_high, b_pack_int8_high_j1);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_0_j1, _mm256_set1_ps(fused_s_0_j1), sum_packed_fp_1);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_j1, _mm256_set1_ps(fused_s_1_j1), sum_packed_fp_1);
        }
        
        // Store results
        *C(0, j, NDIM) += hsum_float_8(sum_packed_fp_0);
            *C(0, j+1, NDIM) += hsum_float_8(sum_packed_fp_1);
    }
}
