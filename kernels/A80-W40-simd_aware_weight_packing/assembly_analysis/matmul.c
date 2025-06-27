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
        
        for (int j = 0; j < block_n; j += 4) {
            
            // Accumulators for all combinations of i and j
            __m256 sum_packed_fp[4][4];
            for (int ii = 0; ii < 4; ++ii) {
                for (int jj = 0; jj < 4; ++jj) {
                    sum_packed_fp[ii][jj] = _mm256_setzero_ps();
                }
            }

            for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE) {
                int block_idx = p / Q_BLK_SIZE;
                
                // Load scaling factors for all active i and j
                float sA[4][2]; // [i][0/1 for low/high block]
                float sB[4][2]; // [j][0/1 for low/high block]
                
                for (int ii = 0; ii < 4; ++ii) {
                        sA[ii][0] = *blockSA(0+ii, block_idx, block_k/Q_BLK_SIZE);
                        sA[ii][1] = *blockSA(0+ii, block_idx+1, block_k/Q_BLK_SIZE);
                }
                
                for (int jj = 0; jj < 4; ++jj) {
                        sB[jj][0] = *blockSB(block_idx, j+jj, block_k/Q_BLK_SIZE);
                        sB[jj][1] = *blockSB(block_idx+1, j+jj, block_k/Q_BLK_SIZE);
                }
                
                // Load A blocks for active i
                __m256i a_pack_int8[4][2]; // [i][0/1 for low/high block]
                for (int ii = 0; ii < 4; ++ii) {
                        a_pack_int8[ii][0] = _mm256_loadu_si256((__m256i *) blockA(0+ii, p, block_k));
                        a_pack_int8[ii][1] = _mm256_loadu_si256((__m256i *) blockA(0+ii, p+Q_BLK_SIZE, block_k));
                }
                
                // Load B blocks for active j
                __m256i b_pack_int8[4]; // [j]
                __m256i b_pack_int8_low[4], b_pack_int8_high[4];
                for (int jj = 0; jj < 4; ++jj) {
                        b_pack_int8[jj] = _mm256_loadu_si256((__m256i *) blockB(p/2, j+jj, block_k/2));
                        
                        __m256i mask = _mm256_set1_epi8(0x0f);
                        b_pack_int8_low[jj] = _mm256_and_si256(b_pack_int8[jj], mask);
                        b_pack_int8_high[jj] = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8[jj], 4), mask);
                        b_pack_int8_low[jj] = _mm256_sub_epi8(b_pack_int8_low[jj], _mm256_set1_epi8(8));
                        b_pack_int8_high[jj] = _mm256_sub_epi8(b_pack_int8_high[jj], _mm256_set1_epi8(8));
                }
                
                // Compute all active i/j combinations
                for (int ii = 0; ii < 4; ++ii) {
                    
                    for (int jj = 0; jj < 4; ++jj) {
                        
                        float fused_s[2] = {
                            sA[ii][0] * sB[jj][0],
                            sA[ii][1] * sB[jj][1]
                        };
                        
                        __m256 tmp[2] = {
                            mul_sum_i8_pairs_float(a_pack_int8[ii][0], b_pack_int8_low[jj]),
                            mul_sum_i8_pairs_float(a_pack_int8[ii][1], b_pack_int8_high[jj])
                        };
                        
                        sum_packed_fp[ii][jj] = _mm256_fmadd_ps(tmp[0], _mm256_set1_ps(fused_s[0]), sum_packed_fp[ii][jj]);
                        sum_packed_fp[ii][jj] = _mm256_fmadd_ps(tmp[1], _mm256_set1_ps(fused_s[1]), sum_packed_fp[ii][jj]);
                    }
                }
            }
            
            // Store results for all active i/j combinations
            for (int ii = 0; ii < block_m; ++ii) {
                
                for (int jj = 0; jj < 4; ++jj) {
                    
                    *C(0+ii, j+jj, NDIM) += hsum_float_8(sum_packed_fp[ii][jj]);
                }
            }
        }
}

