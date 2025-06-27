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
#include <cstdint>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "stdbool.h"

#include "../lib.h"

#ifndef NTHREADS
#define NTHREADS 1
#endif

#define OMP_SCHEDULE dynamic
#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

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
 
#define GGML_F32Cx8_LOAD(x)     _mm256_cvtph_ps(_mm_loadu_si128((const __m128i *)(x)))
#define BROADCAST_FP16_FP32(x)  _mm256_cvtph_ps(_mm_set1_epi16(x))

typedef uint16_t ggml_fp16_t;
struct q4_repack_2x8{
    ggml_fp16_t s_low_fp32[8];
    ggml_fp16_t s_high_fp32[8];
    ggml_fp16_t min_low_fp32[8];
    ggml_fp16_t min_high_fp32[8];
    uint8_t q_coupled[256]; // (64 * 8) / (8 / 4)
};

struct q8_repack_1x2{
    ggml_fp16_t s_low_fp32;
    ggml_fp16_t s_high_fp32;
    ggml_fp16_t scaled_sum_low_fp32;
    ggml_fp16_t scaled_sum_high_fp32;
    int8_t q_low[32];
    int8_t q_high[32];
};

void matmul_repack(
    void * A_repack,
    void * B_repack,
    float* C,
    const int M, const int N, const int K
)
{
    struct q4_repack_2x8 * B_ptr_start = (struct q4_repack_2x8 *) B_repack;
    struct q8_repack_1x2 * A_ptr_start = (struct q8_repack_1x2 *) A_repack;
    // how many q4_repack and q8_repack blocks along the N dimension
    int nb_k = K / (2 * Q_BLK_SIZE);
    // how many q4_repack blocks along the K dimension
    int nb_n = N / 8; 
    // how many q8_repack blocks along the M dimension
    int nb_m = M; // M is a multiple of 6 for this implementation
    PRAGMA_OMP_PARALLEL_FOR
    for (int i = 0; i < nb_m; i += 6)
    {
        int valid_rows = min(6, nb_m - i);
        struct q8_repack_1x2 * A_ptr = A_ptr_start + i * nb_k;
        struct q8_repack_1x2 * A_ptr_next = A_ptr_start + (i + 1) * nb_k;
        struct q8_repack_1x2 * A_ptr_next2 = A_ptr_start + (i + 2) * nb_k;
        struct q8_repack_1x2 * A_ptr_next3 = A_ptr_start + (i + 3) * nb_k;
        struct q8_repack_1x2 * A_ptr_next4 = A_ptr_start + (i + 4) * nb_k;
        struct q8_repack_1x2 * A_ptr_next5 = A_ptr_start + (i + 5) * nb_k;
        for(int j = 0; j < nb_n; j++)
        {
            struct q4_repack_2x8 * B_ptr = B_ptr_start + j * nb_k;
            
            __m256 acc_row0 = _mm256_setzero_ps();
            __m256 acc_row1 = _mm256_setzero_ps();
            __m256 acc_row2 = _mm256_setzero_ps();
            __m256 acc_row3 = _mm256_setzero_ps();
            __m256 acc_row4 = _mm256_setzero_ps();
            __m256 acc_row5 = _mm256_setzero_ps();
            
            for (int blk_id = 0; blk_id < nb_k; blk_id++)
            {
                struct q4_repack_2x8 B_block =  B_ptr[blk_id];
                // each pack contains 4 elements in the quantization block
                __m256i low_blk_B_packs[8];
                __m256i high_blk_B_packs[8];
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    __m256i B_coupled_pack = _mm256_loadu_si256((__m256i *) (B_block.q_coupled + pack_idx * 32));
                    __m256i low_blk_B_pack = _mm256_and_si256(B_coupled_pack, _mm256_set1_epi8(0x0f));
                    __m256i high_blk_B_pack = _mm256_and_si256(_mm256_srli_epi16(B_coupled_pack, 4), _mm256_set1_epi8(0x0f));
                    low_blk_B_packs[pack_idx] = low_blk_B_pack;
                    high_blk_B_packs[pack_idx] = high_blk_B_pack;
                }
                
                // Process six rows of A simultaneously
                __m256i low_blk_A_packs[8];
                __m256i high_blk_A_packs[8];
                __m256i low_blk_A_packs_next[8];
                __m256i high_blk_A_packs_next[8];
                __m256i low_blk_A_packs_next2[8];
                __m256i high_blk_A_packs_next2[8];
                __m256i low_blk_A_packs_next3[8];
                __m256i high_blk_A_packs_next3[8];
                __m256i low_blk_A_packs_next4[8];
                __m256i high_blk_A_packs_next4[8];
                __m256i low_blk_A_packs_next5[8];
                __m256i high_blk_A_packs_next5[8];
                
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A_packs[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr[blk_id].q_high) )[pack_idx]);
                    low_blk_A_packs_next[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs_next[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next[blk_id].q_high) )[pack_idx]);
                    low_blk_A_packs_next2[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next2[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs_next2[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next2[blk_id].q_high) )[pack_idx]);
                    low_blk_A_packs_next3[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next3[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs_next3[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next3[blk_id].q_high) )[pack_idx]);
                    low_blk_A_packs_next4[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next4[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs_next4[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next4[blk_id].q_high) )[pack_idx]);
                    low_blk_A_packs_next5[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next5[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs_next5[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr_next5[blk_id].q_high) )[pack_idx]);
                }
                
                __m256i iacc_low0 = _mm256_setzero_si256();
                __m256i iacc_high0 = _mm256_setzero_si256();
                __m256i iacc_low1 = _mm256_setzero_si256();
                __m256i iacc_high1 = _mm256_setzero_si256();
                __m256i iacc_low2 = _mm256_setzero_si256();
                __m256i iacc_high2 = _mm256_setzero_si256();
                __m256i iacc_low3 = _mm256_setzero_si256();
                __m256i iacc_high3 = _mm256_setzero_si256();
                __m256i iacc_low4 = _mm256_setzero_si256();
                __m256i iacc_high4 = _mm256_setzero_si256();
                __m256i iacc_low5 = _mm256_setzero_si256();
                __m256i iacc_high5 = _mm256_setzero_si256();
                
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low0  = mul_sum_us8_pairs_acc_int32x8 ( iacc_low0 , low_blk_B_packs  [ pack_idx ] , low_blk_A_packs  [ pack_idx ]); 
                    iacc_high0 = mul_sum_us8_pairs_acc_int32x8 ( iacc_high0, high_blk_B_packs [ pack_idx ] , high_blk_A_packs [ pack_idx ]); 
                    iacc_low1  = mul_sum_us8_pairs_acc_int32x8 ( iacc_low1 , low_blk_B_packs  [ pack_idx ] , low_blk_A_packs_next  [ pack_idx ]); 
                    iacc_high1 = mul_sum_us8_pairs_acc_int32x8 ( iacc_high1, high_blk_B_packs [ pack_idx ] , high_blk_A_packs_next [ pack_idx ]); 
                    iacc_low2  = mul_sum_us8_pairs_acc_int32x8 ( iacc_low2 , low_blk_B_packs  [ pack_idx ] , low_blk_A_packs_next2  [ pack_idx ]); 
                    iacc_high2 = mul_sum_us8_pairs_acc_int32x8 ( iacc_high2, high_blk_B_packs [ pack_idx ] , high_blk_A_packs_next2 [ pack_idx ]); 
                    iacc_low3  = mul_sum_us8_pairs_acc_int32x8 ( iacc_low3 , low_blk_B_packs  [ pack_idx ] , low_blk_A_packs_next3  [ pack_idx ]); 
                    iacc_high3 = mul_sum_us8_pairs_acc_int32x8 ( iacc_high3, high_blk_B_packs [ pack_idx ] , high_blk_A_packs_next3 [ pack_idx ]); 
                    iacc_low4  = mul_sum_us8_pairs_acc_int32x8 ( iacc_low4 , low_blk_B_packs  [ pack_idx ] , low_blk_A_packs_next4  [ pack_idx ]); 
                    iacc_high4 = mul_sum_us8_pairs_acc_int32x8 ( iacc_high4, high_blk_B_packs [ pack_idx ] , high_blk_A_packs_next4 [ pack_idx ]); 
                    iacc_low5  = mul_sum_us8_pairs_acc_int32x8 ( iacc_low5 , low_blk_B_packs  [ pack_idx ] , low_blk_A_packs_next5  [ pack_idx ]); 
                    iacc_high5 = mul_sum_us8_pairs_acc_int32x8 ( iacc_high5, high_blk_B_packs [ pack_idx ] , high_blk_A_packs_next5 [ pack_idx ]); 
                }
                
                // load scaleing factors
                __m256 sB_low = GGML_F32Cx8_LOAD(B_block.s_low_fp32);
                __m256 sB_high = GGML_F32Cx8_LOAD(B_block.s_high_fp32);
                __m256 minB_low = GGML_F32Cx8_LOAD(B_block.min_low_fp32);
                __m256 minB_high = GGML_F32Cx8_LOAD(B_block.min_high_fp32);

                __m256 sA_low = BROADCAST_FP16_FP32(A_ptr[blk_id].s_low_fp32);
                __m256 sA_high = BROADCAST_FP16_FP32(A_ptr[blk_id].s_high_fp32);
                __m256 scaled_sum_low = BROADCAST_FP16_FP32(A_ptr[blk_id].scaled_sum_low_fp32);
                __m256 scaled_sum_high = BROADCAST_FP16_FP32(A_ptr[blk_id].scaled_sum_high_fp32);

                __m256 sA_low_next = BROADCAST_FP16_FP32(A_ptr_next[blk_id].s_low_fp32);
                __m256 sA_high_next = BROADCAST_FP16_FP32(A_ptr_next[blk_id].s_high_fp32);
                __m256 scaled_sum_low_next = BROADCAST_FP16_FP32(A_ptr_next[blk_id].scaled_sum_low_fp32);
                __m256 scaled_sum_high_next = BROADCAST_FP16_FP32(A_ptr_next[blk_id].scaled_sum_high_fp32);

                __m256 sA_low_next2 = BROADCAST_FP16_FP32(A_ptr_next2[blk_id].s_low_fp32);
                __m256 sA_high_next2 = BROADCAST_FP16_FP32(A_ptr_next2[blk_id].s_high_fp32);
                __m256 scaled_sum_low_next2 = BROADCAST_FP16_FP32(A_ptr_next2[blk_id].scaled_sum_low_fp32);
                __m256 scaled_sum_high_next2 = BROADCAST_FP16_FP32(A_ptr_next2[blk_id].scaled_sum_high_fp32);

                __m256 sA_low_next3 = BROADCAST_FP16_FP32(A_ptr_next3[blk_id].s_low_fp32);
                __m256 sA_high_next3 = BROADCAST_FP16_FP32(A_ptr_next3[blk_id].s_high_fp32);
                __m256 scaled_sum_low_next3 = BROADCAST_FP16_FP32(A_ptr_next3[blk_id].scaled_sum_low_fp32);
                __m256 scaled_sum_high_next3 = BROADCAST_FP16_FP32(A_ptr_next3[blk_id].scaled_sum_high_fp32);

                __m256 sA_low_next4 = BROADCAST_FP16_FP32(A_ptr_next4[blk_id].s_low_fp32);
                __m256 sA_high_next4 = BROADCAST_FP16_FP32(A_ptr_next4[blk_id].s_high_fp32);
                __m256 scaled_sum_low_next4 = BROADCAST_FP16_FP32(A_ptr_next4[blk_id].scaled_sum_low_fp32);
                __m256 scaled_sum_high_next4 = BROADCAST_FP16_FP32(A_ptr_next4[blk_id].scaled_sum_high_fp32);

                __m256 sA_low_next5 = BROADCAST_FP16_FP32(A_ptr_next5[blk_id].s_low_fp32);
                __m256 sA_high_next5 = BROADCAST_FP16_FP32(A_ptr_next5[blk_id].s_high_fp32);
                __m256 scaled_sum_low_next5 = BROADCAST_FP16_FP32(A_ptr_next5[blk_id].scaled_sum_low_fp32);
                __m256 scaled_sum_high_next5 = BROADCAST_FP16_FP32(A_ptr_next5[blk_id].scaled_sum_high_fp32);

                __m256 s_low = _mm256_mul_ps(sA_low, sB_low);
                __m256 s_high = _mm256_mul_ps(sA_high, sB_high);
                __m256 s_low_next = _mm256_mul_ps(sA_low_next, sB_low);
                __m256 s_high_next = _mm256_mul_ps(sA_high_next, sB_high);
                __m256 s_low_next2 = _mm256_mul_ps(sA_low_next2, sB_low);
                __m256 s_high_next2 = _mm256_mul_ps(sA_high_next2, sB_high);
                __m256 s_low_next3 = _mm256_mul_ps(sA_low_next3, sB_low);
                __m256 s_high_next3 = _mm256_mul_ps(sA_high_next3, sB_high);
                __m256 s_low_next4 = _mm256_mul_ps(sA_low_next4, sB_low);
                __m256 s_high_next4 = _mm256_mul_ps(sA_high_next4, sB_high);
                __m256 s_low_next5 = _mm256_mul_ps(sA_low_next5, sB_low);
                __m256 s_high_next5 = _mm256_mul_ps(sA_high_next5, sB_high);

                // First row
                acc_row0 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low0), s_low, acc_row0);
                acc_row0 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high0), s_high, acc_row0);
                acc_row0 = _mm256_fmadd_ps(minB_low, scaled_sum_low, acc_row0);
                acc_row0 = _mm256_fmadd_ps(minB_high, scaled_sum_high, acc_row0);

                // Second row
                acc_row1 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low1), s_low_next, acc_row1);
                acc_row1 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high1), s_high_next, acc_row1);
                acc_row1 = _mm256_fmadd_ps(minB_low, scaled_sum_low_next, acc_row1);
                acc_row1 = _mm256_fmadd_ps(minB_high, scaled_sum_high_next, acc_row1);

                // Third row
                acc_row2 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low2), s_low_next2, acc_row2);
                acc_row2 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high2), s_high_next2, acc_row2);
                acc_row2 = _mm256_fmadd_ps(minB_low, scaled_sum_low_next2, acc_row2);
                acc_row2 = _mm256_fmadd_ps(minB_high, scaled_sum_high_next2, acc_row2);

                // Fourth row
                acc_row3 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low3), s_low_next3, acc_row3);
                acc_row3 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high3), s_high_next3, acc_row3);
                acc_row3 = _mm256_fmadd_ps(minB_low, scaled_sum_low_next3, acc_row3);
                acc_row3 = _mm256_fmadd_ps(minB_high, scaled_sum_high_next3, acc_row3);

                // Fifth row
                acc_row4 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low4), s_low_next4, acc_row4);
                acc_row4 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high4), s_high_next4, acc_row4);
                acc_row4 = _mm256_fmadd_ps(minB_low, scaled_sum_low_next4, acc_row4);
                acc_row4 = _mm256_fmadd_ps(minB_high, scaled_sum_high_next4, acc_row4);

                // Sixth row
                acc_row5 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low5), s_low_next5, acc_row5);
                acc_row5 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high5), s_high_next5, acc_row5);
                acc_row5 = _mm256_fmadd_ps(minB_low, scaled_sum_low_next5, acc_row5);
                acc_row5 = _mm256_fmadd_ps(minB_high, scaled_sum_high_next5, acc_row5);
            }
        switch (valid_rows) {
            case 6:
                _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
                _mm256_storeu_ps(C(i + 1, j * 8, N), acc_row1);
                _mm256_storeu_ps(C(i + 2, j * 8, N), acc_row2);
                _mm256_storeu_ps(C(i + 3, j * 8, N), acc_row3);
                _mm256_storeu_ps(C(i + 4, j * 8, N), acc_row4);
                _mm256_storeu_ps(C(i + 5, j * 8, N), acc_row5);
                break;
            case 5:
                _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
                _mm256_storeu_ps(C(i + 1, j * 8, N), acc_row1);
                _mm256_storeu_ps(C(i + 2, j * 8, N), acc_row2);
                _mm256_storeu_ps(C(i + 3, j * 8, N), acc_row3);
                _mm256_storeu_ps(C(i + 4, j * 8, N), acc_row4);
                break;
            case 4:
                _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
                _mm256_storeu_ps(C(i + 1, j * 8, N), acc_row1);
                _mm256_storeu_ps(C(i + 2, j * 8, N), acc_row2);
                _mm256_storeu_ps(C(i + 3, j * 8, N), acc_row3);
                break;
            case 3:
                _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
                _mm256_storeu_ps(C(i + 1, j * 8, N), acc_row1);
                _mm256_storeu_ps(C(i + 2, j * 8, N), acc_row2);
                break;
            case 2:
                _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
                _mm256_storeu_ps(C(i + 1, j * 8, N), acc_row1);
                break;
            case 1:
                _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
        }
        }
    }

}

