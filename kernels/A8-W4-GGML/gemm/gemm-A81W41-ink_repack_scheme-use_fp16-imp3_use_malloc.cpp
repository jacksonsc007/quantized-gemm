
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ggml-cpu-quants.h"
#include "stdbool.h"

#include "ggml.h"
#include "ggml-cpu.h"
#include "ggml-quants.h"
#include "ggml-impl.h"
#include "lib.h"

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

struct q4_repack_2x8{
    ggml_fp16_t s_low[8];
    ggml_fp16_t s_high[8];
    ggml_fp16_t min_low[8];
    ggml_fp16_t min_high[8];
    uint8_t q_coupled[256]; // (64 * 8) / (8 / 4)
};

struct q8_repack_1x2{
    ggml_fp16_t s_low;
    ggml_fp16_t s_high;
    ggml_fp16_t scaled_sum_low;
    ggml_fp16_t scaled_sum_high;
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
                __m256 sB_low = GGML_F32Cx8_LOAD(B_block.s_low);
                __m256 sB_high = GGML_F32Cx8_LOAD(B_block.s_high);
                __m256 minB_low = GGML_F32Cx8_LOAD(B_block.min_low);
                __m256 minB_high = GGML_F32Cx8_LOAD(B_block.min_high);

                __m256 sA_low = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr[blk_id].s_low));
                __m256 sA_high = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr[blk_id].s_high));
                __m256 scaled_sum_low = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr[blk_id].scaled_sum_low));
                __m256 scaled_sum_high = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr[blk_id].scaled_sum_high));

                __m256 sA_low_next = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next[blk_id].s_low));
                __m256 sA_high_next = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next[blk_id].s_high));
                __m256 scaled_sum_low_next = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next[blk_id].scaled_sum_low));
                __m256 scaled_sum_high_next = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next[blk_id].scaled_sum_high));

                __m256 sA_low_next2 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next2[blk_id].s_low));
                __m256 sA_high_next2 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next2[blk_id].s_high));
                __m256 scaled_sum_low_next2 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next2[blk_id].scaled_sum_low));
                __m256 scaled_sum_high_next2 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next2[blk_id].scaled_sum_high));

                __m256 sA_low_next3 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next3[blk_id].s_low));
                __m256 sA_high_next3 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next3[blk_id].s_high));
                __m256 scaled_sum_low_next3 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next3[blk_id].scaled_sum_low));
                __m256 scaled_sum_high_next3 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next3[blk_id].scaled_sum_high));

                __m256 sA_low_next4 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next4[blk_id].s_low));
                __m256 sA_high_next4 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next4[blk_id].s_high));
                __m256 scaled_sum_low_next4 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next4[blk_id].scaled_sum_low));
                __m256 scaled_sum_high_next4 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next4[blk_id].scaled_sum_high));

                __m256 sA_low_next5 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next5[blk_id].s_low));
                __m256 sA_high_next5 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next5[blk_id].s_high));
                __m256 scaled_sum_low_next5 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next5[blk_id].scaled_sum_low));
                __m256 scaled_sum_high_next5 = _mm256_set1_ps(GGML_FP16_TO_FP32(A_ptr_next5[blk_id].scaled_sum_high));

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

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{
    for (int i = 0; i < M; i++)
    for (int j = 0; j < N; j++)
    {
        __m256 sum_packed_fp = _mm256_setzero_ps();
        float extra_sum_fp = 0;
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0 = *SA(i, block_idx, K/Q_BLK_SIZE);
            float sB_0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float sA_1 = *SA(i, block_idx + 1, K/Q_BLK_SIZE);
            float sB_1 = *SB(block_idx + 1, j, K/Q_BLK_SIZE);
            // float fused_s_0 = 0.02;
            // float fused_s_1 = 0.02;
            float fused_s_0 = sA_0 * sB_0;
            float fused_s_1 = sA_1 * sB_1;
            
            float min_b_0 = *MinB(block_idx, j, K / Q_BLK_SIZE);
            float min_b_1 = *MinB(block_idx + 1, j, K / Q_BLK_SIZE);
            float sum_a_0_fused = *ScaledSumA(i, block_idx, K / Q_BLK_SIZE);
            float sum_a_1_fused = *ScaledSumA(i, block_idx + 1, K / Q_BLK_SIZE);
            
                __m256i a_pack_int8_low = _mm256_loadu_si256( (__m256i *) A(i, p, K));
                __m256i a_pack_int8_high = _mm256_loadu_si256( (__m256i *) A(i, p + Q_BLK_SIZE, K));
                __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) B(p/2, j, K/2));
                __m256i mask = _mm256_set1_epi8(0x0f);
                __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
                __m256i b_pack_int8_high = _mm256_and_si256(  _mm256_srli_epi16(b_pack_int8, 4), mask);

                __m256 tmp_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low);
                __m256 tmp_1 = mul_sum_us8_pairs_float(b_pack_int8_high, a_pack_int8_high);
                sum_packed_fp = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp);
                sum_packed_fp = _mm256_fmadd_ps(tmp_1, _mm256_set1_ps(fused_s_1), sum_packed_fp);
                extra_sum_fp += sum_a_0_fused * min_b_0;
                extra_sum_fp += sum_a_1_fused * min_b_1;
        }
        *C(i, j, NDIM) +=  hsum_float_8(sum_packed_fp) + extra_sum_fp;
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


static inline int hsum_i32_8(const __m256i a) {
    const __m128i sum128 = _mm_add_epi32(_mm256_castsi256_si128(a), _mm256_extractf128_si256(a, 1));
    const __m128i hi64 = _mm_unpackhi_epi64(sum128, sum128);
    const __m128i sum64 = _mm_add_epi32(hi64, sum128);
    const __m128i hi32  = _mm_shuffle_epi32(sum64, _MM_SHUFFLE(2, 3, 0, 1));
    return _mm_cvtsi128_si32(_mm_add_epi32(sum64, hi32));
}

void quantize_row_q8_1_repack(const float * x, void * vy, int64_t k) {
    assert(k % (2 * QK8_1) == 0);  // Must be multiple of 64 (2 blocks of 32)
    const int nb = k / (2 * QK8_1);  // Number of q80_repack_1x2 blocks

    struct q8_repack_1x2 * GGML_RESTRICT y = (struct q8_repack_1x2 *) vy;
    for (int i = 0; i < nb; i++) {
        // Load elements into 8 AVX vectors (64 elements total)
        __m256 v0 = _mm256_loadu_ps(x);
        __m256 v1 = _mm256_loadu_ps(x + 8);
        __m256 v2 = _mm256_loadu_ps(x + 16);
        __m256 v3 = _mm256_loadu_ps(x + 24);
        __m256 v4 = _mm256_loadu_ps(x + 32);
        __m256 v5 = _mm256_loadu_ps(x + 40);
        __m256 v6 = _mm256_loadu_ps(x + 48);
        __m256 v7 = _mm256_loadu_ps(x + 56);
        x += 64;

        // Compute max(abs(e)) for first block (first 32 elements)
        const __m256 signBit = _mm256_set1_ps(-0.0f);
        __m256 maxAbs1 = _mm256_andnot_ps(signBit, v0);
        maxAbs1 = _mm256_max_ps(maxAbs1, _mm256_andnot_ps(signBit, v1));
        maxAbs1 = _mm256_max_ps(maxAbs1, _mm256_andnot_ps(signBit, v2));
        maxAbs1 = _mm256_max_ps(maxAbs1, _mm256_andnot_ps(signBit, v3));

        // Compute max(abs(e)) for second block (next 32 elements)
        __m256 maxAbs2 = _mm256_andnot_ps(signBit, v4);
        maxAbs2 = _mm256_max_ps(maxAbs2, _mm256_andnot_ps(signBit, v5));
        maxAbs2 = _mm256_max_ps(maxAbs2, _mm256_andnot_ps(signBit, v6));
        maxAbs2 = _mm256_max_ps(maxAbs2, _mm256_andnot_ps(signBit, v7));

        // Get max values for both blocks
        __m128 max4_1 = _mm_max_ps(_mm256_extractf128_ps(maxAbs1, 1), _mm256_castps256_ps128(maxAbs1));
        max4_1 = _mm_max_ps(max4_1, _mm_movehl_ps(max4_1, max4_1));
        max4_1 = _mm_max_ss(max4_1, _mm_movehdup_ps(max4_1));
        const float max_scalar1 = _mm_cvtss_f32(max4_1);

        __m128 max4_2 = _mm_max_ps(_mm256_extractf128_ps(maxAbs2, 1), _mm256_castps256_ps128(maxAbs2));
        max4_2 = _mm_max_ps(max4_2, _mm_movehl_ps(max4_2, max4_2));
        max4_2 = _mm_max_ss(max4_2, _mm_movehdup_ps(max4_2));
        const float max_scalar2 = _mm_cvtss_f32(max4_2);

        // Quantize first block
        const float d1 = max_scalar1 / 127.f;
        y[i].s_low = GGML_FP32_TO_FP16(d1);
        const float id1 = (max_scalar1 != 0.0f) ? 127.f / max_scalar1 : 0.0f;
        const __m256 mul1 = _mm256_set1_ps(id1);

        // Quantize second block
        const float d2 = max_scalar2 / 127.f;
        y[i].s_high = GGML_FP32_TO_FP16(d2);
        const float id2 = (max_scalar2 != 0.0f) ? 127.f / max_scalar2 : 0.0f;
        const __m256 mul2 = _mm256_set1_ps(id2);

        // Apply multipliers
        v0 = _mm256_mul_ps(v0, mul1);
        v1 = _mm256_mul_ps(v1, mul1);
        v2 = _mm256_mul_ps(v2, mul1);
        v3 = _mm256_mul_ps(v3, mul1);
        v4 = _mm256_mul_ps(v4, mul2);
        v5 = _mm256_mul_ps(v5, mul2);
        v6 = _mm256_mul_ps(v6, mul2);
        v7 = _mm256_mul_ps(v7, mul2);

        // Round to nearest integer
        v0 = _mm256_round_ps(v0, _MM_ROUND_NEAREST);
        v1 = _mm256_round_ps(v1, _MM_ROUND_NEAREST);
        v2 = _mm256_round_ps(v2, _MM_ROUND_NEAREST);
        v3 = _mm256_round_ps(v3, _MM_ROUND_NEAREST);
        v4 = _mm256_round_ps(v4, _MM_ROUND_NEAREST);
        v5 = _mm256_round_ps(v5, _MM_ROUND_NEAREST);
        v6 = _mm256_round_ps(v6, _MM_ROUND_NEAREST);
        v7 = _mm256_round_ps(v7, _MM_ROUND_NEAREST);

        // Convert floats to integers
        __m256i i0 = _mm256_cvtps_epi32(v0);
        __m256i i1 = _mm256_cvtps_epi32(v1);
        __m256i i2 = _mm256_cvtps_epi32(v2);
        __m256i i3 = _mm256_cvtps_epi32(v3);
        __m256i i4 = _mm256_cvtps_epi32(v4);
        __m256i i5 = _mm256_cvtps_epi32(v5);
        __m256i i6 = _mm256_cvtps_epi32(v6);
        __m256i i7 = _mm256_cvtps_epi32(v7);

        // Compute scaled sums for both blocks
        y[i].scaled_sum_low = GGML_FP32_TO_FP16( d1 * hsum_i32_8(_mm256_add_epi32(_mm256_add_epi32(i0, i1), _mm256_add_epi32(i2, i3))) );
        y[i].scaled_sum_high = GGML_FP32_TO_FP16( d2 * hsum_i32_8(_mm256_add_epi32(_mm256_add_epi32(i4, i5), _mm256_add_epi32(i6, i7))) );

        // Convert int32 to int16 for first block
        i0 = _mm256_packs_epi32(i0, i1);
        i2 = _mm256_packs_epi32(i2, i3);
        i0 = _mm256_packs_epi16(i0, i2);

        // Convert int32 to int16 for second block
        i4 = _mm256_packs_epi32(i4, i5);
        i6 = _mm256_packs_epi32(i6, i7);
        i4 = _mm256_packs_epi16(i4, i6);

        // Fix order for first block
        const __m256i perm = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
        i0 = _mm256_permutevar8x32_epi32(i0, perm);
        i4 = _mm256_permutevar8x32_epi32(i4, perm);

        // Store results
        _mm256_storeu_si256((__m256i *)y[i].q_low, i0);
        _mm256_storeu_si256((__m256i *)y[i].q_high, i4);
    }
}



int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A  = (int8_t*)malloc(M * K * sizeof(int8_t)); // malloc may not guarantee 64-byte alignment
    uint8_t* B = (uint8_t*)malloc(K / 2 * N * sizeof(uint8_t)); // malloc may not guarantee 64-byte alignment
    float* SA  = (float*)malloc(M * K / Q_BLK_SIZE * sizeof(float)); // malloc may not guarantee 64-byte alignment
    float* ScaledSumA  = (float*)malloc(M * K / Q_BLK_SIZE * sizeof(float)); // malloc may not guarantee 64-byte alignment
    float* SB  = (float*)malloc(K * N / Q_BLK_SIZE * sizeof(float)); // malloc may not guarantee 64-byte alignment
    float* MinB  = (float*)malloc(K * N / Q_BLK_SIZE * sizeof(float)); // malloc may not guarantee 64-byte alignment
    float* C = (float*)malloc(M * N * sizeof(float)); // malloc may not guarantee 64-byte alignment

    
    // ggml initialization
    const ggml_type weight_q_type = GGML_TYPE_Q4_1_ink;
    const ggml_type activation_q_type = GGML_TYPE_Q8_1_ink;

    size_t ctx_size = 0;
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * N); // fp32 weight
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * M); // fp32 activation
    ctx_size += ggml_row_size(GGML_TYPE_F32, M * N); // fp32 output
    ctx_size += ggml_row_size(weight_q_type, K * N); // fp32 activation
    ctx_size += 1024*1024*16;
    printf("Allocating Memory of size %zi bytes, %zi MB\n",ctx_size, (ctx_size/1024/1024));

    struct ggml_init_params params = {
        /*.mem_size   =*/ ctx_size,
        /*.mem_buffer =*/ NULL,
        /* no_alloc   =*/ 0
    };

    struct ggml_context * ctx;
    ctx = ggml_init(params);
    if (!ctx) {
        fprintf(stderr, "%s: ggml_init() failed\n", __func__);
        return 1;
    }
    struct ggml_tensor * weight = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, K, N);
    init_rand((float *)weight->data, K, N);

    struct ggml_tensor * activation = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, K, M);
    init_rand((float *)activation->data, K, M);

    
    int32_t nelements = K * N;
    // quantize the weight tensor to int4
    printf("\e[32m[INFO]\e[m quantize weight\n");
    struct ggml_tensor * q4_weight = ggml_new_tensor_2d(ctx, weight_q_type, K, N);
    ggml_quantize_chunk(
        weight_q_type, (const float *) weight->data, q4_weight->data, 0, nelements/q4_weight->ne[0], q4_weight->ne[0], nullptr
    );
    // quantize the activation tensor to int8
    printf("\e[32m[INFO]\e[m quantize activation\n");
    struct ggml_tensor * q8_activation = ggml_new_tensor_2d(ctx, activation_q_type, K, M);
    // print_q8_0_data((block_q8_0*)q8_activation->data, 2);
    nelements = K * M;
    ggml_quantize_chunk(
        activation_q_type, (const float *) activation->data, q8_activation->data, 0, nelements/q8_activation->ne[0], q8_activation->ne[0], nullptr
    );
    
    
    // print_q8_0_data((block_q8_0*)q8_activation->data, 2);
    printf("\e[32m[INFO]\e[m Copy quantized tensor for evaluation\n");
    block_q8_1_ink * q8_blocks = (block_q8_1_ink *)q8_activation->data;
    int32_t num_q8_blocks = q8_activation->ne[0] * q8_activation->ne[1] / QK8_1_ink;
    for (int block_idx = 0; block_idx < num_q8_blocks; ++block_idx) {
        block_q8_1_ink blk = q8_blocks[block_idx];
        float scale_1 = GGML_FP16_TO_FP32(blk.d1);
        float scaled_sum_1 = GGML_FP16_TO_FP32(blk.s1);
        float scale_2 = GGML_FP16_TO_FP32(blk.d2);
        float scaled_sum_2 = GGML_FP16_TO_FP32(blk.s2);

        SA[2 * block_idx] = scale_1;
        SA[2 * block_idx + 1] = scale_2;
        ScaledSumA[2 * block_idx] = scaled_sum_1;
        ScaledSumA[2 * block_idx + 1] = scaled_sum_2;
        memcpy(A + block_idx * QK8_1_ink, blk.qs1, QK8_1_ink / 2 * sizeof(int8_t));
        memcpy(A + block_idx * QK8_1_ink + QK8_1_ink / 2, blk.qs2, QK8_1_ink / 2 * sizeof(int8_t));
    }
    block_q4_1_ink * q4_blocks = (block_q4_1_ink *)q4_weight->data;
    int32_t num_q4_blocks = q4_weight->ne[0] * q4_weight->ne[1] / QK4_1_ink;
    for (int block_idx = 0; block_idx < num_q4_blocks; ++block_idx) {
        block_q4_1_ink blk = q4_blocks[block_idx];
        float scale_1 = GGML_FP16_TO_FP32(blk.d1);
        float min_value_1 = GGML_FP16_TO_FP32(blk.m1);
        float scale_2 = GGML_FP16_TO_FP32(blk.d2);
        float min_value_2 = GGML_FP16_TO_FP32(blk.m2);

        SB[2 * block_idx] = scale_1;
        SB[2 * block_idx + 1] = scale_2;
        MinB[2 * block_idx] = min_value_1;
        MinB[2 * block_idx + 1] = min_value_2;
        memcpy(B + block_idx * QK4_1_ink / 2, blk.qs, QK4_1_ink / 2 * sizeof(uint8_t));
    }
    


    
    /*
    Repack A and B.
    Rather than calculate the precise volume needed to store quantized activation and weights,
    we use float size to ensure sufficient memory.
    */
    void * A_repack  = malloc(M * K * sizeof(float)); // malloc may not guarantee 64-byte alignment
    void * B_repack  = malloc(K * N * sizeof(float)); // malloc may not guarantee 64-byte alignment

    // quantize activatio into desired repack format
    int num_repack_blk_A_along_K = K / (2 * Q_BLK_SIZE);
    struct q8_repack_1x2 * A_repack_start = (struct q8_repack_1x2 *) A_repack;
    bool repack_during_quantization = true;
    if (repack_during_quantization)
    {
        quantize_row_q8_1_repack((const float *) activation->data, A_repack, M * K);
    }
    else {
        // repack A in row-major order. Pack size (1 x 64)
        for (int i = 0; i < M; i++) 
        {
            struct q8_repack_1x2 * A_ptr = A_repack_start + i * num_repack_blk_A_along_K;
            for (int blk_id = 0; blk_id < num_repack_blk_A_along_K; blk_id++) 
            {
                float s_low =  *SA(i, blk_id * 2, K / Q_BLK_SIZE);
                float s_high = *SA(i, blk_id * 2 + 1, K / Q_BLK_SIZE);
                float scaled_sum_low = *ScaledSumA(i, blk_id * 2, K / Q_BLK_SIZE);
                float scaled_sum_high = *ScaledSumA(i, blk_id * 2 + 1, K / Q_BLK_SIZE);
                A_ptr[blk_id].s_low = s_low;
                A_ptr[blk_id].s_high = s_high;
                A_ptr[blk_id].scaled_sum_low = scaled_sum_low;
                A_ptr[blk_id].scaled_sum_high = scaled_sum_high;
                for (int q = 0; q < Q_BLK_SIZE; q++) 
                {
                    A_ptr[blk_id].q_low[q] =  *A(i, blk_id * 2 * Q_BLK_SIZE + q, K);
                    A_ptr[blk_id].q_high[q] = *A(i, blk_id * 2 * Q_BLK_SIZE + q + Q_BLK_SIZE, K);
                }
            }
        }
    }

    // repack B. q40_repack_2x8 blocks are in column-major order, while elements within
    // it are in row-major order. 
    // Pack size (64 x 8)
    int num_repack_blk_B_along_K = K / (2 * Q_BLK_SIZE);
    int num_repack_blk_B_along_N = N / 8;
    struct q4_repack_2x8 * B_start =  (struct q4_repack_2x8 *) B_repack;
    for (int j = 0; j < num_repack_blk_B_along_N; j++) 
    {
        for (int i = 0; i < num_repack_blk_B_along_K; i++)
        {
            struct q4_repack_2x8 * B_ptr =  B_start + j * num_repack_blk_B_along_K + i;

            // pack scaling factors
            for (int jj = 0; jj < 8; jj++)
            {
                ggml_fp16_t s_low    = GGML_FP32_TO_FP16(*SB   ( i * 2    , j * 8 + jj, K / Q_BLK_SIZE )); 
                ggml_fp16_t s_high   = GGML_FP32_TO_FP16(*SB   ( i * 2 + 1, j * 8 + jj, K / Q_BLK_SIZE )); 
                ggml_fp16_t min_low  = GGML_FP32_TO_FP16(*MinB ( i * 2    , j * 8 + jj, K / Q_BLK_SIZE )); 
                ggml_fp16_t min_high = GGML_FP32_TO_FP16(*MinB ( i * 2 + 1, j * 8 + jj, K / Q_BLK_SIZE )); 
                B_ptr->s_low[jj] = s_low;
                B_ptr->s_high[jj] = s_high;
                B_ptr->min_low[jj] = min_low;
                B_ptr->min_high[jj] = min_high;
            }
            // pack quantized int4 weights in an interleaved manner
            // The block has size of 64 x 8 elements logically, but physically it is laid out as 32 x 8,
            // since each element has 4 bits and the block has type int8, wiht each element only taking half of the byte.
            // 32 x 8 is covered by stacking 8 packs, each measuring 4 x 8 elements.
            // memory layout inside packs is in column-major order
            uint8_t * B_ptr_start =  B_ptr->q_coupled;
            int num_packs = 8;
            for (int pack_idx = 0; pack_idx < num_packs; pack_idx++)
            {
                uint8_t * pack_start_ptr = B_ptr_start + pack_idx * 32;
                for (int jj = 0; jj < 8; jj++)
                {
                    for (int ii = 0; ii < 4; ii++)
                    {
                        pack_start_ptr[ii + jj * 4] = *B(i * Q_BLK_SIZE + pack_idx * 4 + ii, j * 8 + jj, K / 2);
                    }
                }
            }
        }
    } 

    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    // matmul_naive_no_packing(A, B, C_ref, SA, SB, MinB, M, N, K);
    
    // obtain gt result 
    struct ggml_tensor * ggml_result1 = ggml_mul_mat(ctx, weight, activation);
    struct ggml_cgraph * graph1 = ggml_new_graph(ctx);
    ggml_build_forward_expand(graph1, ggml_result1);
    ggml_graph_compute_with_ctx(ctx, graph1, 1);
    float * result_gt = (float *)ggml_result1->data;

    struct ggml_tensor * ggml_result2 = ggml_mul_mat(ctx, q4_weight, q8_activation);
    struct ggml_cgraph * graph = ggml_new_graph(ctx);
    ggml_build_forward_expand(graph, ggml_result2);
    ggml_graph_compute_with_ctx(ctx, graph, 1);
    float * result_q_llama = (float *)ggml_result2->data;



    double FLOP = 2 * (double)M * N * K;
    printf("\n\e[32m[INFO]\e[m MR=%d, NR=%d, MC=%d, NC=%d, KC=%d, M=%d, K=%d, N=%d\n", MR, NR, MC, NC, KC, M, K, N);
    for (int i = 0; i < NITER; i++) {
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        // matmul_kernel(A, B, C, SA, ScaledSumA,SB, MinB, M, N, K);
        matmul_repack(A_repack, B_repack, C, M, N, K);
        // matmul_naive_no_packing(A, B, C, SA, SB, MinB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        printf("\nresult of custom kernel:\n");
        print_mat(C, 5 );
        printf("\nresult of gt:\n");
        print_mat(result_gt, 5 );
        printf("\nresult of ggml kernel:\n");
        print_mat(result_q_llama, 5 );
        printf("\nCompare results:\n");
        compare_mats(C, result_gt, M * N);
        compare_mats(C, result_q_llama, M * N);
        // break;
    }
    free(A);
    free(B);
    free(C);
    free(A_repack);
    free(B_repack);
    return 0;
}
