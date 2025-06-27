#include <assert.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include "stdbool.h"

#include "lib.h"

#ifndef NTHREADS
#define NTHREADS 1
#endif

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#ifndef MDIM
    #define MDIM 1024
#endif

#ifndef NDIM
    #define NDIM 1024
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
#define blockMinB(i, j, ld) (blockMinB + (j) * (ld) + (i))

 

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{
    for (int i = 0; i < M; i += 4)
    for (int j = 0; j < N; j += 4)
    {
        __m256 sum_packed_fp_00 = _mm256_setzero_ps();
        __m256 sum_packed_fp_01 = _mm256_setzero_ps();
        __m256 sum_packed_fp_02 = _mm256_setzero_ps();
        __m256 sum_packed_fp_03 = _mm256_setzero_ps();
        __m256 sum_packed_fp_10 = _mm256_setzero_ps();
        __m256 sum_packed_fp_11 = _mm256_setzero_ps();
        __m256 sum_packed_fp_12 = _mm256_setzero_ps();
        __m256 sum_packed_fp_13 = _mm256_setzero_ps();
        __m256 sum_packed_fp_20 = _mm256_setzero_ps();
        __m256 sum_packed_fp_21 = _mm256_setzero_ps();
        __m256 sum_packed_fp_22 = _mm256_setzero_ps();
        __m256 sum_packed_fp_23 = _mm256_setzero_ps();
        __m256 sum_packed_fp_30 = _mm256_setzero_ps();
        __m256 sum_packed_fp_31 = _mm256_setzero_ps();
        __m256 sum_packed_fp_32 = _mm256_setzero_ps();
        __m256 sum_packed_fp_33 = _mm256_setzero_ps();
        
        float extra_sum_fp_00 = 0, extra_sum_fp_01 = 0, extra_sum_fp_02 = 0, extra_sum_fp_03 = 0;
        float extra_sum_fp_10 = 0, extra_sum_fp_11 = 0, extra_sum_fp_12 = 0, extra_sum_fp_13 = 0;
        float extra_sum_fp_20 = 0, extra_sum_fp_21 = 0, extra_sum_fp_22 = 0, extra_sum_fp_23 = 0;
        float extra_sum_fp_30 = 0, extra_sum_fp_31 = 0, extra_sum_fp_32 = 0, extra_sum_fp_33 = 0;
        
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            
            // Load scaling factors for A (reused for all j)
            float sA_i0_0 = *SA(i+0, block_idx, K/Q_BLK_SIZE);
            float sA_i0_1 = *SA(i+0, block_idx + 1, K/Q_BLK_SIZE);
            float sA_i1_0 = *SA(i+1, block_idx, K/Q_BLK_SIZE);
            float sA_i1_1 = *SA(i+1, block_idx + 1, K/Q_BLK_SIZE);
            float sA_i2_0 = *SA(i+2, block_idx, K/Q_BLK_SIZE);
            float sA_i2_1 = *SA(i+2, block_idx + 1, K/Q_BLK_SIZE);
            float sA_i3_0 = *SA(i+3, block_idx, K/Q_BLK_SIZE);
            float sA_i3_1 = *SA(i+3, block_idx + 1, K/Q_BLK_SIZE);
            
            // Load scaling factors for B (reused for all i)
            float sB_j0_0 = *SB(block_idx, j+0, K/Q_BLK_SIZE);
            float sB_j0_1 = *SB(block_idx + 1, j+0, K/Q_BLK_SIZE);
            float sB_j1_0 = *SB(block_idx, j+1, K/Q_BLK_SIZE);
            float sB_j1_1 = *SB(block_idx + 1, j+1, K/Q_BLK_SIZE);
            float sB_j2_0 = *SB(block_idx, j+2, K/Q_BLK_SIZE);
            float sB_j2_1 = *SB(block_idx + 1, j+2, K/Q_BLK_SIZE);
            float sB_j3_0 = *SB(block_idx, j+3, K/Q_BLK_SIZE);
            float sB_j3_1 = *SB(block_idx + 1, j+3, K/Q_BLK_SIZE);
            
            // Fused scaling factors
            float fused_s_i0j0_0 = sA_i0_0 * sB_j0_0;
            float fused_s_i0j0_1 = sA_i0_1 * sB_j0_1;
            float fused_s_i0j1_0 = sA_i0_0 * sB_j1_0;
            float fused_s_i0j1_1 = sA_i0_1 * sB_j1_1;
            float fused_s_i0j2_0 = sA_i0_0 * sB_j2_0;
            float fused_s_i0j2_1 = sA_i0_1 * sB_j2_1;
            float fused_s_i0j3_0 = sA_i0_0 * sB_j3_0;
            float fused_s_i0j3_1 = sA_i0_1 * sB_j3_1;
            
            float fused_s_i1j0_0 = sA_i1_0 * sB_j0_0;
            float fused_s_i1j0_1 = sA_i1_1 * sB_j0_1;
            float fused_s_i1j1_0 = sA_i1_0 * sB_j1_0;
            float fused_s_i1j1_1 = sA_i1_1 * sB_j1_1;
            float fused_s_i1j2_0 = sA_i1_0 * sB_j2_0;
            float fused_s_i1j2_1 = sA_i1_1 * sB_j2_1;
            float fused_s_i1j3_0 = sA_i1_0 * sB_j3_0;
            float fused_s_i1j3_1 = sA_i1_1 * sB_j3_1;
            
            float fused_s_i2j0_0 = sA_i2_0 * sB_j0_0;
            float fused_s_i2j0_1 = sA_i2_1 * sB_j0_1;
            float fused_s_i2j1_0 = sA_i2_0 * sB_j1_0;
            float fused_s_i2j1_1 = sA_i2_1 * sB_j1_1;
            float fused_s_i2j2_0 = sA_i2_0 * sB_j2_0;
            float fused_s_i2j2_1 = sA_i2_1 * sB_j2_1;
            float fused_s_i2j3_0 = sA_i2_0 * sB_j3_0;
            float fused_s_i2j3_1 = sA_i2_1 * sB_j3_1;
            
            float fused_s_i3j0_0 = sA_i3_0 * sB_j0_0;
            float fused_s_i3j0_1 = sA_i3_1 * sB_j0_1;
            float fused_s_i3j1_0 = sA_i3_0 * sB_j1_0;
            float fused_s_i3j1_1 = sA_i3_1 * sB_j1_1;
            float fused_s_i3j2_0 = sA_i3_0 * sB_j2_0;
            float fused_s_i3j2_1 = sA_i3_1 * sB_j2_1;
            float fused_s_i3j3_0 = sA_i3_0 * sB_j3_0;
            float fused_s_i3j3_1 = sA_i3_1 * sB_j3_1;
            
            // Load min_b values (reused for all i)
            float min_b_j0_0 = *MinB(block_idx, j+0, K / Q_BLK_SIZE);
            float min_b_j0_1 = *MinB(block_idx + 1, j+0, K / Q_BLK_SIZE);
            float min_b_j1_0 = *MinB(block_idx, j+1, K / Q_BLK_SIZE);
            float min_b_j1_1 = *MinB(block_idx + 1, j+1, K / Q_BLK_SIZE);
            float min_b_j2_0 = *MinB(block_idx, j+2, K / Q_BLK_SIZE);
            float min_b_j2_1 = *MinB(block_idx + 1, j+2, K / Q_BLK_SIZE);
            float min_b_j3_0 = *MinB(block_idx, j+3, K / Q_BLK_SIZE);
            float min_b_j3_1 = *MinB(block_idx + 1, j+3, K / Q_BLK_SIZE);
            
            // Load sum_a values (reused for all j)
            float sum_a_i0_0 = *ScaledSumA(i+0, block_idx, K / Q_BLK_SIZE);
            float sum_a_i0_1 = *ScaledSumA(i+0, block_idx + 1, K / Q_BLK_SIZE);
            float sum_a_i1_0 = *ScaledSumA(i+1, block_idx, K / Q_BLK_SIZE);
            float sum_a_i1_1 = *ScaledSumA(i+1, block_idx + 1, K / Q_BLK_SIZE);
            float sum_a_i2_0 = *ScaledSumA(i+2, block_idx, K / Q_BLK_SIZE);
            float sum_a_i2_1 = *ScaledSumA(i+2, block_idx + 1, K / Q_BLK_SIZE);
            float sum_a_i3_0 = *ScaledSumA(i+3, block_idx, K / Q_BLK_SIZE);
            float sum_a_i3_1 = *ScaledSumA(i+3, block_idx + 1, K / Q_BLK_SIZE);
            
            // Load A values (reused for all j)
            __m256i a_pack_int8_low_i0 = _mm256_loadu_si256((__m256i *) A(i+0, p, K));
            __m256i a_pack_int8_high_i0 = _mm256_loadu_si256((__m256i *) A(i+0, p + Q_BLK_SIZE, K));
            __m256i a_pack_int8_low_i1 = _mm256_loadu_si256((__m256i *) A(i+1, p, K));
            __m256i a_pack_int8_high_i1 = _mm256_loadu_si256((__m256i *) A(i+1, p + Q_BLK_SIZE, K));
            __m256i a_pack_int8_low_i2 = _mm256_loadu_si256((__m256i *) A(i+2, p, K));
            __m256i a_pack_int8_high_i2 = _mm256_loadu_si256((__m256i *) A(i+2, p + Q_BLK_SIZE, K));
            __m256i a_pack_int8_low_i3 = _mm256_loadu_si256((__m256i *) A(i+3, p, K));
            __m256i a_pack_int8_high_i3 = _mm256_loadu_si256((__m256i *) A(i+3, p + Q_BLK_SIZE, K));
            
            // Load B values (reused for all i)
            __m256i b_pack_int8_j0 = _mm256_loadu_si256((__m256i *) B(p/2, j+0, K/2));
            __m256i b_pack_int8_j1 = _mm256_loadu_si256((__m256i *) B(p/2, j+1, K/2));
            __m256i b_pack_int8_j2 = _mm256_loadu_si256((__m256i *) B(p/2, j+2, K/2));
            __m256i b_pack_int8_j3 = _mm256_loadu_si256((__m256i *) B(p/2, j+3, K/2));
            
            __m256i mask = _mm256_set1_epi8(0x0f);
            
            // Process B for j0
            __m256i b_pack_int8_low_j0 = _mm256_and_si256(b_pack_int8_j0, mask);
            __m256i b_pack_int8_high_j0 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j0, 4), mask);
            
            // Process B for j1
            __m256i b_pack_int8_low_j1 = _mm256_and_si256(b_pack_int8_j1, mask);
            __m256i b_pack_int8_high_j1 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j1, 4), mask);
            
            // Process B for j2
            __m256i b_pack_int8_low_j2 = _mm256_and_si256(b_pack_int8_j2, mask);
            __m256i b_pack_int8_high_j2 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j2, 4), mask);
            
            // Process B for j3
            __m256i b_pack_int8_low_j3 = _mm256_and_si256(b_pack_int8_j3, mask);
            __m256i b_pack_int8_high_j3 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j3, 4), mask);
            
            // Compute products and accumulate for all i,j combinations
            
            // i0
            __m256 tmp_i0j0_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j0, a_pack_int8_low_i0);
            __m256 tmp_i0j0_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j0, a_pack_int8_high_i0);
            sum_packed_fp_00 = _mm256_fmadd_ps(tmp_i0j0_0, _mm256_set1_ps(fused_s_i0j0_0), sum_packed_fp_00);
            sum_packed_fp_00 = _mm256_fmadd_ps(tmp_i0j0_1, _mm256_set1_ps(fused_s_i0j0_1), sum_packed_fp_00);
            
            __m256 tmp_i0j1_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j1, a_pack_int8_low_i0);
            __m256 tmp_i0j1_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j1, a_pack_int8_high_i0);
            sum_packed_fp_01 = _mm256_fmadd_ps(tmp_i0j1_0, _mm256_set1_ps(fused_s_i0j1_0), sum_packed_fp_01);
            sum_packed_fp_01 = _mm256_fmadd_ps(tmp_i0j1_1, _mm256_set1_ps(fused_s_i0j1_1), sum_packed_fp_01);
            
            __m256 tmp_i0j2_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j2, a_pack_int8_low_i0);
            __m256 tmp_i0j2_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j2, a_pack_int8_high_i0);
            sum_packed_fp_02 = _mm256_fmadd_ps(tmp_i0j2_0, _mm256_set1_ps(fused_s_i0j2_0), sum_packed_fp_02);
            sum_packed_fp_02 = _mm256_fmadd_ps(tmp_i0j2_1, _mm256_set1_ps(fused_s_i0j2_1), sum_packed_fp_02);
            
            __m256 tmp_i0j3_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j3, a_pack_int8_low_i0);
            __m256 tmp_i0j3_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j3, a_pack_int8_high_i0);
            sum_packed_fp_03 = _mm256_fmadd_ps(tmp_i0j3_0, _mm256_set1_ps(fused_s_i0j3_0), sum_packed_fp_03);
            sum_packed_fp_03 = _mm256_fmadd_ps(tmp_i0j3_1, _mm256_set1_ps(fused_s_i0j3_1), sum_packed_fp_03);
            
            extra_sum_fp_00 += sum_a_i0_0 * min_b_j0_0 + sum_a_i0_1 * min_b_j0_1;
            extra_sum_fp_01 += sum_a_i0_0 * min_b_j1_0 + sum_a_i0_1 * min_b_j1_1;
            extra_sum_fp_02 += sum_a_i0_0 * min_b_j2_0 + sum_a_i0_1 * min_b_j2_1;
            extra_sum_fp_03 += sum_a_i0_0 * min_b_j3_0 + sum_a_i0_1 * min_b_j3_1;
            
            // i1
            __m256 tmp_i1j0_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j0, a_pack_int8_low_i1);
            __m256 tmp_i1j0_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j0, a_pack_int8_high_i1);
            sum_packed_fp_10 = _mm256_fmadd_ps(tmp_i1j0_0, _mm256_set1_ps(fused_s_i1j0_0), sum_packed_fp_10);
            sum_packed_fp_10 = _mm256_fmadd_ps(tmp_i1j0_1, _mm256_set1_ps(fused_s_i1j0_1), sum_packed_fp_10);
            
            __m256 tmp_i1j1_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j1, a_pack_int8_low_i1);
            __m256 tmp_i1j1_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j1, a_pack_int8_high_i1);
            sum_packed_fp_11 = _mm256_fmadd_ps(tmp_i1j1_0, _mm256_set1_ps(fused_s_i1j1_0), sum_packed_fp_11);
            sum_packed_fp_11 = _mm256_fmadd_ps(tmp_i1j1_1, _mm256_set1_ps(fused_s_i1j1_1), sum_packed_fp_11);
            
            __m256 tmp_i1j2_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j2, a_pack_int8_low_i1);
            __m256 tmp_i1j2_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j2, a_pack_int8_high_i1);
            sum_packed_fp_12 = _mm256_fmadd_ps(tmp_i1j2_0, _mm256_set1_ps(fused_s_i1j2_0), sum_packed_fp_12);
            sum_packed_fp_12 = _mm256_fmadd_ps(tmp_i1j2_1, _mm256_set1_ps(fused_s_i1j2_1), sum_packed_fp_12);
            
            __m256 tmp_i1j3_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j3, a_pack_int8_low_i1);
            __m256 tmp_i1j3_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j3, a_pack_int8_high_i1);
            sum_packed_fp_13 = _mm256_fmadd_ps(tmp_i1j3_0, _mm256_set1_ps(fused_s_i1j3_0), sum_packed_fp_13);
            sum_packed_fp_13 = _mm256_fmadd_ps(tmp_i1j3_1, _mm256_set1_ps(fused_s_i1j3_1), sum_packed_fp_13);
            
            extra_sum_fp_10 += sum_a_i1_0 * min_b_j0_0 + sum_a_i1_1 * min_b_j0_1;
            extra_sum_fp_11 += sum_a_i1_0 * min_b_j1_0 + sum_a_i1_1 * min_b_j1_1;
            extra_sum_fp_12 += sum_a_i1_0 * min_b_j2_0 + sum_a_i1_1 * min_b_j2_1;
            extra_sum_fp_13 += sum_a_i1_0 * min_b_j3_0 + sum_a_i1_1 * min_b_j3_1;
            
            // i2
            __m256 tmp_i2j0_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j0, a_pack_int8_low_i2);
            __m256 tmp_i2j0_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j0, a_pack_int8_high_i2);
            sum_packed_fp_20 = _mm256_fmadd_ps(tmp_i2j0_0, _mm256_set1_ps(fused_s_i2j0_0), sum_packed_fp_20);
            sum_packed_fp_20 = _mm256_fmadd_ps(tmp_i2j0_1, _mm256_set1_ps(fused_s_i2j0_1), sum_packed_fp_20);
            
            __m256 tmp_i2j1_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j1, a_pack_int8_low_i2);
            __m256 tmp_i2j1_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j1, a_pack_int8_high_i2);
            sum_packed_fp_21 = _mm256_fmadd_ps(tmp_i2j1_0, _mm256_set1_ps(fused_s_i2j1_0), sum_packed_fp_21);
            sum_packed_fp_21 = _mm256_fmadd_ps(tmp_i2j1_1, _mm256_set1_ps(fused_s_i2j1_1), sum_packed_fp_21);
            
            __m256 tmp_i2j2_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j2, a_pack_int8_low_i2);
            __m256 tmp_i2j2_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j2, a_pack_int8_high_i2);
            sum_packed_fp_22 = _mm256_fmadd_ps(tmp_i2j2_0, _mm256_set1_ps(fused_s_i2j2_0), sum_packed_fp_22);
            sum_packed_fp_22 = _mm256_fmadd_ps(tmp_i2j2_1, _mm256_set1_ps(fused_s_i2j2_1), sum_packed_fp_22);
            
            __m256 tmp_i2j3_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j3, a_pack_int8_low_i2);
            __m256 tmp_i2j3_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j3, a_pack_int8_high_i2);
            sum_packed_fp_23 = _mm256_fmadd_ps(tmp_i2j3_0, _mm256_set1_ps(fused_s_i2j3_0), sum_packed_fp_23);
            sum_packed_fp_23 = _mm256_fmadd_ps(tmp_i2j3_1, _mm256_set1_ps(fused_s_i2j3_1), sum_packed_fp_23);
            
            extra_sum_fp_20 += sum_a_i2_0 * min_b_j0_0 + sum_a_i2_1 * min_b_j0_1;
            extra_sum_fp_21 += sum_a_i2_0 * min_b_j1_0 + sum_a_i2_1 * min_b_j1_1;
            extra_sum_fp_22 += sum_a_i2_0 * min_b_j2_0 + sum_a_i2_1 * min_b_j2_1;
            extra_sum_fp_23 += sum_a_i2_0 * min_b_j3_0 + sum_a_i2_1 * min_b_j3_1;
            
            // i3
            __m256 tmp_i3j0_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j0, a_pack_int8_low_i3);
            __m256 tmp_i3j0_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j0, a_pack_int8_high_i3);
            sum_packed_fp_30 = _mm256_fmadd_ps(tmp_i3j0_0, _mm256_set1_ps(fused_s_i3j0_0), sum_packed_fp_30);
            sum_packed_fp_30 = _mm256_fmadd_ps(tmp_i3j0_1, _mm256_set1_ps(fused_s_i3j0_1), sum_packed_fp_30);
            
            __m256 tmp_i3j1_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j1, a_pack_int8_low_i3);
            __m256 tmp_i3j1_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j1, a_pack_int8_high_i3);
            sum_packed_fp_31 = _mm256_fmadd_ps(tmp_i3j1_0, _mm256_set1_ps(fused_s_i3j1_0), sum_packed_fp_31);
            sum_packed_fp_31 = _mm256_fmadd_ps(tmp_i3j1_1, _mm256_set1_ps(fused_s_i3j1_1), sum_packed_fp_31);
            
            __m256 tmp_i3j2_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j2, a_pack_int8_low_i3);
            __m256 tmp_i3j2_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j2, a_pack_int8_high_i3);
            sum_packed_fp_32 = _mm256_fmadd_ps(tmp_i3j2_0, _mm256_set1_ps(fused_s_i3j2_0), sum_packed_fp_32);
            sum_packed_fp_32 = _mm256_fmadd_ps(tmp_i3j2_1, _mm256_set1_ps(fused_s_i3j2_1), sum_packed_fp_32);
            
            __m256 tmp_i3j3_0 = mul_sum_us8_pairs_float(b_pack_int8_low_j3, a_pack_int8_low_i3);
            __m256 tmp_i3j3_1 = mul_sum_us8_pairs_float(b_pack_int8_high_j3, a_pack_int8_high_i3);
            sum_packed_fp_33 = _mm256_fmadd_ps(tmp_i3j3_0, _mm256_set1_ps(fused_s_i3j3_0), sum_packed_fp_33);
            sum_packed_fp_33 = _mm256_fmadd_ps(tmp_i3j3_1, _mm256_set1_ps(fused_s_i3j3_1), sum_packed_fp_33);
            
            extra_sum_fp_30 += sum_a_i3_0 * min_b_j0_0 + sum_a_i3_1 * min_b_j0_1;
            extra_sum_fp_31 += sum_a_i3_0 * min_b_j1_0 + sum_a_i3_1 * min_b_j1_1;
            extra_sum_fp_32 += sum_a_i3_0 * min_b_j2_0 + sum_a_i3_1 * min_b_j2_1;
            extra_sum_fp_33 += sum_a_i3_0 * min_b_j3_0 + sum_a_i3_1 * min_b_j3_1;
        }
        
        // Store results
        *C(i+0, j+0, NDIM) += hsum_float_8(sum_packed_fp_00) + extra_sum_fp_00;
        *C(i+0, j+1, NDIM) += hsum_float_8(sum_packed_fp_01) + extra_sum_fp_01;
        *C(i+0, j+2, NDIM) += hsum_float_8(sum_packed_fp_02) + extra_sum_fp_02;
        *C(i+0, j+3, NDIM) += hsum_float_8(sum_packed_fp_03) + extra_sum_fp_03;
        
        *C(i+1, j+0, NDIM) += hsum_float_8(sum_packed_fp_10) + extra_sum_fp_10;
        *C(i+1, j+1, NDIM) += hsum_float_8(sum_packed_fp_11) + extra_sum_fp_11;
        *C(i+1, j+2, NDIM) += hsum_float_8(sum_packed_fp_12) + extra_sum_fp_12;
        *C(i+1, j+3, NDIM) += hsum_float_8(sum_packed_fp_13) + extra_sum_fp_13;
        
        *C(i+2, j+0, NDIM) += hsum_float_8(sum_packed_fp_20) + extra_sum_fp_20;
        *C(i+2, j+1, NDIM) += hsum_float_8(sum_packed_fp_21) + extra_sum_fp_21;
        *C(i+2, j+2, NDIM) += hsum_float_8(sum_packed_fp_22) + extra_sum_fp_22;
        *C(i+2, j+3, NDIM) += hsum_float_8(sum_packed_fp_23) + extra_sum_fp_23;
        
        *C(i+3, j+0, NDIM) += hsum_float_8(sum_packed_fp_30) + extra_sum_fp_30;
        *C(i+3, j+1, NDIM) += hsum_float_8(sum_packed_fp_31) + extra_sum_fp_31;
        *C(i+3, j+2, NDIM) += hsum_float_8(sum_packed_fp_32) + extra_sum_fp_32;
        *C(i+3, j+3, NDIM) += hsum_float_8(sum_packed_fp_33) + extra_sum_fp_33;
    }
}






void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB, float * blockMinB,
    const int M, const int N, const int K) {
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

    init_rand_min_max(A, M, K, -128, 127);
    init_rand_uint8(B, K / 2, N);
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    init_rand(MinB, K / Q_BLK_SIZE, N);
    compute_scaled_sum(A, sA, scaledSumA, M, K);

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, MinB, M, N, K);
#endif
    // How many operations do we need?
    double FLOP = 2 * (double)M * N * K;
    double frequency = 5.2 * 1e9;
    int computation_unit_size = 64;
    int num_computation_unit = (M * N) * (K / computation_unit_size);
    int fma_per_computation_unit = 2;
    int dpbusd_per_compuatation_unit = 2;
    int total_fma = num_computation_unit * fma_per_computation_unit;
    int total_dpbusd = num_computation_unit * dpbusd_per_compuatation_unit;

    printf("\n\e[32m[INFO]\e[m M=%d, K=%d, N=%d\n", M, K, N);

    for (int i = 0; i < NITER; i++) {


    // init_rand(A_packed, MR, K);
    // init_rand(B_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, scaledSumA,sB, MinB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double total_clocks = exec_time * frequency;
        double FLOPS = FLOP / exec_time;
        double fma_per_clock = total_fma / total_clocks;
        double dpbusd_per_clock = total_dpbusd / total_clocks;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        printf("FMA/clock= \e[32m%.3f\e[m, DPBUSD/clock= \e[32m%.3f\e[m\n", fma_per_clock, dpbusd_per_clock);
        // print_mat(C, 5, 5);
        // print_mat(C_ref, 5, 5);
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
