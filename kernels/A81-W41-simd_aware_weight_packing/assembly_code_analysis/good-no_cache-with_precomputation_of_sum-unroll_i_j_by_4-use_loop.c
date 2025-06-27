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
#define blockMinB(i, j, ld) (blockMinB + (j) * (ld) + (i))

 

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{

    // Process 4 rows and 4 columns at a time
        
        for (int j = 0; j < N; j += 4) 
        for (int i = 0; i < M; i += 4) 
        {
            
            // Accumulators for all combinations of i and j
            __m256 sum_packed_fp[4][4];
            float extra_sum[4][4];
            // float sum_a_fused[4][4];
            // float min_b[4][4];
            for (int ii = 0; ii < 4; ++ii) {
                for (int jj = 0; jj < 4; ++jj) {
                    sum_packed_fp[ii][jj] = _mm256_setzero_ps();
                    extra_sum[ii][jj] = 0;
                    // min_b[ii][jj] = 0.01;
                    // sum_a_fused[ii][jj] = 1115.6;
                }
            }

            for (int p = 0; p < K; p += 2 * Q_BLK_SIZE) 
            {
                int block_idx = p / Q_BLK_SIZE;
                
                // Load scaling factors for all active i and j
                float sA[4][2]; // [i][0/1 for low/high block]
                float sB[4][2]; // [j][0/1 for low/high block]
                
                for (int ii = 0; ii < 4; ++ii) {
                        sA[ii][0] = *SA(i+ii, block_idx, K/Q_BLK_SIZE);
                        sA[ii][1] = *SA(i+ii, block_idx+1, K/Q_BLK_SIZE);
                }
                
                for (int jj = 0; jj < 4; ++jj) {
                        sB[jj][0] = *SB(block_idx, j+jj, K/Q_BLK_SIZE);
                        sB[jj][1] = *SB(block_idx+1, j+jj, K/Q_BLK_SIZE);
                }
                
                // Load A blocks for active i
                __m256i a_pack_int8[4][2]; // [i][0/1 for low/high block]
                for (int ii = 0; ii < 4; ++ii) {
                        a_pack_int8[ii][0] = _mm256_loadu_si256((__m256i *) A(i+ii, p, K));
                        a_pack_int8[ii][1] = _mm256_loadu_si256((__m256i *) A(i+ii, p+Q_BLK_SIZE, K));
                }
                
                // Load B blocks for active j
                __m256i b_pack_int8[4]; // [j]
                __m256i b_pack_int8_low[4], b_pack_int8_high[4];
                for (int jj = 0; jj < 4; ++jj) {
                        b_pack_int8[jj] = _mm256_loadu_si256((__m256i *) B(p/2, j+jj, K/2));
                        
                        __m256i mask = _mm256_set1_epi8(0x0f);
                        b_pack_int8_low[jj] = _mm256_and_si256(b_pack_int8[jj], mask);
                        b_pack_int8_high[jj] = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8[jj], 4), mask);
                }
                
                // Compute all active i/j combinations
                for (int ii = 0; ii < 4; ++ii) {
                    
                    for (int jj = 0; jj < 4; ++jj) {
                        
                        float fused_s[2] = {
                            sA[ii][0] * sB[jj][0],
                            sA[ii][1] * sB[jj][1]
                        };
                        
                        __m256 tmp[2] = {
                            mul_sum_us8_pairs_float(b_pack_int8_low  [ jj ], a_pack_int8 [ ii ] [ 0 ] ),
                            mul_sum_us8_pairs_float(b_pack_int8_high [ jj ], a_pack_int8 [ ii ] [ 1 ] )
                        };
                        
                        sum_packed_fp[ii][jj] = _mm256_fmadd_ps(tmp[0], _mm256_set1_ps(fused_s[0]), sum_packed_fp[ii][jj]);
                        sum_packed_fp[ii][jj] = _mm256_fmadd_ps(tmp[1], _mm256_set1_ps(fused_s[1]), sum_packed_fp[ii][jj]);
                    
                        float sum0 = *ScaledSumA(i+ii, block_idx, K/Q_BLK_SIZE);
                        float sum1 = *ScaledSumA(i+ii, block_idx+1, K/Q_BLK_SIZE);
                        float min_b_0 = *MinB(block_idx, j+jj, K / Q_BLK_SIZE);
                        float min_b_1 = *MinB(block_idx + 1, j+jj, K / Q_BLK_SIZE);
                        extra_sum[ii][jj] += sum0 * min_b_0;
                        extra_sum[ii][jj] += sum1 * min_b_1;
                    }
                }
            }
            
            // Store results for all active i/j combinations
            for (int ii = 0; ii < 4; ++ii) {
                
                for (int jj = 0; jj < 4; ++jj) {
                    
                    *C(i+ii, j+jj, NDIM) += hsum_float_8(sum_packed_fp[ii][jj]) + extra_sum[ii][jj];
                }
            }
        }
}




