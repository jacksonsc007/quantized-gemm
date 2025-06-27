#include <assert.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32

#ifndef MDIM
    #define MDIM 1024
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM 1024
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


struct q40_repack_2x8{
    float s_low[8];
    float s_high[8];
    uint8_t q_coupled[256]; // (64 * 8) / (8 / 4)
};

struct q80_repack_1x2{
    float s_low;
    float s_high;
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
    struct q40_repack_2x8 * B_ptr_start = (struct q40_repack_2x8 *) B_repack;
    struct q80_repack_1x2 * A_ptr_start = (struct q80_repack_1x2 *) A_repack;
    // how many q4_repack and q8_repack blocks along the N dimension
    int nb_k = K / (2 * Q_BLK_SIZE);
    // how many q4_repack blocks along the K dimension
    int nb_n = N / 8; 
    // how many q8_repack blocks along the M dimension
    int nb_m = M ;
    const __m256i perms[8] = {
        _mm256_set1_epi32(0),
        _mm256_set1_epi32(1),
        _mm256_set1_epi32(2),
        _mm256_set1_epi32(3),
        _mm256_set1_epi32(4),
        _mm256_set1_epi32(5),
        _mm256_set1_epi32(6),
        _mm256_set1_epi32(7)
    };

    for (int i = 0; i < nb_m; i += 8)
    {
        for(int j = 0; j < nb_n; j++)
        {
            struct q40_repack_2x8 * B_ptr = B_ptr_start + j * nb_k;
            struct q80_repack_1x2 * A_ptr0 = A_ptr_start + i * nb_k;
            struct q80_repack_1x2 * A_ptr1 = A_ptr_start + (i + 1) * nb_k;
            struct q80_repack_1x2 * A_ptr2 = A_ptr_start + (i + 2) * nb_k;
            struct q80_repack_1x2 * A_ptr3 = A_ptr_start + (i + 3) * nb_k;
            struct q80_repack_1x2 * A_ptr4 = A_ptr_start + (i + 4) * nb_k;
            struct q80_repack_1x2 * A_ptr5 = A_ptr_start + (i + 5) * nb_k;
            struct q80_repack_1x2 * A_ptr6 = A_ptr_start + (i + 6) * nb_k;
            struct q80_repack_1x2 * A_ptr7 = A_ptr_start + (i + 7) * nb_k;
            __m256 acc_row0 = _mm256_setzero_ps();
            __m256 acc_row1 = _mm256_setzero_ps();
            __m256 acc_row2 = _mm256_setzero_ps();
            __m256 acc_row3 = _mm256_setzero_ps();
            __m256 acc_row4 = _mm256_setzero_ps();
            __m256 acc_row5 = _mm256_setzero_ps();
            __m256 acc_row6 = _mm256_setzero_ps();
            __m256 acc_row7 = _mm256_setzero_ps();
            for (int blk_id = 0; blk_id < nb_k; blk_id++)
            {
                struct q40_repack_2x8 B_block =  B_ptr[blk_id];
                // each pack contains 4 elements in the quantization block
                __m256i low_blk_B_packs[8];
                __m256i high_blk_B_packs[8];
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    __m256i B_coupled_pack = _mm256_loadu_si256((__m256i *) (B_block.q_coupled + pack_idx * 32));
                    __m256i low_blk_B_pack = _mm256_and_si256(B_coupled_pack, _mm256_set1_epi8(0x0f));
                    low_blk_B_pack = _mm256_sub_epi8(low_blk_B_pack, _mm256_set1_epi8(8));
                    __m256i high_blk_B_pack = _mm256_and_si256(_mm256_srli_epi16(B_coupled_pack, 4), _mm256_set1_epi8(0x0f));
                    high_blk_B_pack = _mm256_sub_epi8(high_blk_B_pack, _mm256_set1_epi8(8));
                    low_blk_B_packs[pack_idx] = low_blk_B_pack;
                    high_blk_B_packs[pack_idx] = high_blk_B_pack;
                }

                // Process first row
                __m256i low_blk_A0_packs[8];
                __m256i high_blk_A0_packs[8];
                __m256i low_blk_A0 = _mm256_loadu_si256((__m256i *) (A_ptr0[blk_id].q_low));
                __m256i high_blk_A0 = _mm256_loadu_si256((__m256i *) (A_ptr0[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A0_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A0, perms[pack_idx]);
                    high_blk_A0_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A0, perms[pack_idx]);
                }
                
                __m256i iacc_low0 = _mm256_setzero_si256();
                __m256i iacc_high0 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low0 = mul_sum_i8_pairs_acc_int32x8(iacc_low0, low_blk_A0_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high0 = mul_sum_i8_pairs_acc_int32x8(iacc_high0, high_blk_A0_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process second row
                __m256i low_blk_A1_packs[8];
                __m256i high_blk_A1_packs[8];
                __m256i low_blk_A1 = _mm256_loadu_si256((__m256i *) (A_ptr1[blk_id].q_low));
                __m256i high_blk_A1 = _mm256_loadu_si256((__m256i *) (A_ptr1[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A1_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A1, perms[pack_idx]);
                    high_blk_A1_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A1, perms[pack_idx]);
                }
                
                __m256i iacc_low1 = _mm256_setzero_si256();
                __m256i iacc_high1 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low1 = mul_sum_i8_pairs_acc_int32x8(iacc_low1, low_blk_A1_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high1 = mul_sum_i8_pairs_acc_int32x8(iacc_high1, high_blk_A1_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process third row
                __m256i low_blk_A2_packs[8];
                __m256i high_blk_A2_packs[8];
                __m256i low_blk_A2 = _mm256_loadu_si256((__m256i *) (A_ptr2[blk_id].q_low));
                __m256i high_blk_A2 = _mm256_loadu_si256((__m256i *) (A_ptr2[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A2_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A2, perms[pack_idx]);
                    high_blk_A2_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A2, perms[pack_idx]);
                }
                
                __m256i iacc_low2 = _mm256_setzero_si256();
                __m256i iacc_high2 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low2 = mul_sum_i8_pairs_acc_int32x8(iacc_low2, low_blk_A2_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high2 = mul_sum_i8_pairs_acc_int32x8(iacc_high2, high_blk_A2_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process fourth row
                __m256i low_blk_A3_packs[8];
                __m256i high_blk_A3_packs[8];
                __m256i low_blk_A3 = _mm256_loadu_si256((__m256i *) (A_ptr3[blk_id].q_low));
                __m256i high_blk_A3 = _mm256_loadu_si256((__m256i *) (A_ptr3[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A3_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A3, perms[pack_idx]);
                    high_blk_A3_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A3, perms[pack_idx]);
                }
                
                __m256i iacc_low3 = _mm256_setzero_si256();
                __m256i iacc_high3 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low3 = mul_sum_i8_pairs_acc_int32x8(iacc_low3, low_blk_A3_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high3 = mul_sum_i8_pairs_acc_int32x8(iacc_high3, high_blk_A3_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process fifth row
                __m256i low_blk_A4_packs[8];
                __m256i high_blk_A4_packs[8];
                __m256i low_blk_A4 = _mm256_loadu_si256((__m256i *) (A_ptr4[blk_id].q_low));
                __m256i high_blk_A4 = _mm256_loadu_si256((__m256i *) (A_ptr4[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A4_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A4, perms[pack_idx]);
                    high_blk_A4_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A4, perms[pack_idx]);
                }
                
                __m256i iacc_low4 = _mm256_setzero_si256();
                __m256i iacc_high4 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low4 = mul_sum_i8_pairs_acc_int32x8(iacc_low4, low_blk_A4_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high4 = mul_sum_i8_pairs_acc_int32x8(iacc_high4, high_blk_A4_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process sixth row
                __m256i low_blk_A5_packs[8];
                __m256i high_blk_A5_packs[8];
                __m256i low_blk_A5 = _mm256_loadu_si256((__m256i *) (A_ptr5[blk_id].q_low));
                __m256i high_blk_A5 = _mm256_loadu_si256((__m256i *) (A_ptr5[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A5_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A5, perms[pack_idx]);
                    high_blk_A5_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A5, perms[pack_idx]);
                }
                
                __m256i iacc_low5 = _mm256_setzero_si256();
                __m256i iacc_high5 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low5 = mul_sum_i8_pairs_acc_int32x8(iacc_low5, low_blk_A5_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high5 = mul_sum_i8_pairs_acc_int32x8(iacc_high5, high_blk_A5_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process seventh row
                __m256i low_blk_A6_packs[8];
                __m256i high_blk_A6_packs[8];
                __m256i low_blk_A6 = _mm256_loadu_si256((__m256i *) (A_ptr6[blk_id].q_low));
                __m256i high_blk_A6 = _mm256_loadu_si256((__m256i *) (A_ptr6[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A6_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A6, perms[pack_idx]);
                    high_blk_A6_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A6, perms[pack_idx]);
                }
                
                __m256i iacc_low6 = _mm256_setzero_si256();
                __m256i iacc_high6 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low6 = mul_sum_i8_pairs_acc_int32x8(iacc_low6, low_blk_A6_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high6 = mul_sum_i8_pairs_acc_int32x8(iacc_high6, high_blk_A6_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }

                // Process eighth row
                __m256i low_blk_A7_packs[8];
                __m256i high_blk_A7_packs[8];
                __m256i low_blk_A7 = _mm256_loadu_si256((__m256i *) (A_ptr7[blk_id].q_low));
                __m256i high_blk_A7 = _mm256_loadu_si256((__m256i *) (A_ptr7[blk_id].q_high));
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A7_packs[pack_idx] = _mm256_permutevar8x32_epi32(low_blk_A7, perms[pack_idx]);
                    high_blk_A7_packs[pack_idx] = _mm256_permutevar8x32_epi32(high_blk_A7, perms[pack_idx]);
                }
                
                __m256i iacc_low7 = _mm256_setzero_si256();
                __m256i iacc_high7 = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low7 = mul_sum_i8_pairs_acc_int32x8(iacc_low7, low_blk_A7_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high7 = mul_sum_i8_pairs_acc_int32x8(iacc_high7, high_blk_A7_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }
                
                // load scaleing factors
                __m256 sB_low = _mm256_loadu_ps(B_block.s_low);
                __m256 sB_high = _mm256_loadu_ps(B_block.s_high);
                
                // Process first row scaling
                __m256 sA_low0 = _mm256_set1_ps(A_ptr0[blk_id].s_low);
                __m256 sA_high0 = _mm256_set1_ps(A_ptr0[blk_id].s_high);
                __m256 s_low0 = _mm256_mul_ps(sA_low0, sB_low);
                __m256 s_high0 = _mm256_mul_ps(sA_high0, sB_high);
                acc_row0 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low0), s_low0, acc_row0);
                acc_row0 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high0), s_high0, acc_row0);

                // Process second row scaling
                __m256 sA_low1 = _mm256_set1_ps(A_ptr1[blk_id].s_low);
                __m256 sA_high1 = _mm256_set1_ps(A_ptr1[blk_id].s_high);
                __m256 s_low1 = _mm256_mul_ps(sA_low1, sB_low);
                __m256 s_high1 = _mm256_mul_ps(sA_high1, sB_high);
                acc_row1 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low1), s_low1, acc_row1);
                acc_row1 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high1), s_high1, acc_row1);

                // Process third row scaling
                __m256 sA_low2 = _mm256_set1_ps(A_ptr2[blk_id].s_low);
                __m256 sA_high2 = _mm256_set1_ps(A_ptr2[blk_id].s_high);
                __m256 s_low2 = _mm256_mul_ps(sA_low2, sB_low);
                __m256 s_high2 = _mm256_mul_ps(sA_high2, sB_high);
                acc_row2 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low2), s_low2, acc_row2);
                acc_row2 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high2), s_high2, acc_row2);

                // Process fourth row scaling
                __m256 sA_low3 = _mm256_set1_ps(A_ptr3[blk_id].s_low);
                __m256 sA_high3 = _mm256_set1_ps(A_ptr3[blk_id].s_high);
                __m256 s_low3 = _mm256_mul_ps(sA_low3, sB_low);
                __m256 s_high3 = _mm256_mul_ps(sA_high3, sB_high);
                acc_row3 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low3), s_low3, acc_row3);
                acc_row3 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high3), s_high3, acc_row3);

                // Process fifth row scaling
                __m256 sA_low4 = _mm256_set1_ps(A_ptr4[blk_id].s_low);
                __m256 sA_high4 = _mm256_set1_ps(A_ptr4[blk_id].s_high);
                __m256 s_low4 = _mm256_mul_ps(sA_low4, sB_low);
                __m256 s_high4 = _mm256_mul_ps(sA_high4, sB_high);
                acc_row4 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low4), s_low4, acc_row4);
                acc_row4 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high4), s_high4, acc_row4);

                // Process sixth row scaling
                __m256 sA_low5 = _mm256_set1_ps(A_ptr5[blk_id].s_low);
                __m256 sA_high5 = _mm256_set1_ps(A_ptr5[blk_id].s_high);
                __m256 s_low5 = _mm256_mul_ps(sA_low5, sB_low);
                __m256 s_high5 = _mm256_mul_ps(sA_high5, sB_high);
                acc_row5 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low5), s_low5, acc_row5);
                acc_row5 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high5), s_high5, acc_row5);

                // Process seventh row scaling
                __m256 sA_low6 = _mm256_set1_ps(A_ptr6[blk_id].s_low);
                __m256 sA_high6 = _mm256_set1_ps(A_ptr6[blk_id].s_high);
                __m256 s_low6 = _mm256_mul_ps(sA_low6, sB_low);
                __m256 s_high6 = _mm256_mul_ps(sA_high6, sB_high);
                acc_row6 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low6), s_low6, acc_row6);
                acc_row6 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high6), s_high6, acc_row6);

                // Process eighth row scaling
                __m256 sA_low7 = _mm256_set1_ps(A_ptr7[blk_id].s_low);
                __m256 sA_high7 = _mm256_set1_ps(A_ptr7[blk_id].s_high);
                __m256 s_low7 = _mm256_mul_ps(sA_low7, sB_low);
                __m256 s_high7 = _mm256_mul_ps(sA_high7, sB_high);
                acc_row7 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low7), s_low7, acc_row7);
                acc_row7 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high7), s_high7, acc_row7);
            }
            _mm256_storeu_ps(C(i, j * 8, N), acc_row0);
            _mm256_storeu_ps(C(i + 1, j * 8, N), acc_row1);
            _mm256_storeu_ps(C(i + 2, j * 8, N), acc_row2);
            _mm256_storeu_ps(C(i + 3, j * 8, N), acc_row3);
            _mm256_storeu_ps(C(i + 4, j * 8, N), acc_row4);
            _mm256_storeu_ps(C(i + 5, j * 8, N), acc_row5);
            _mm256_storeu_ps(C(i + 6, j * 8, N), acc_row6);
            _mm256_storeu_ps(C(i + 7, j * 8, N), acc_row7);
        }
    }
}

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{

    for (int i = 0; i < M; ++i) 
    for (int j = 0; j < N; ++j)
    {
        __m256 sum_packed_fp = _mm256_setzero_ps();
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0 = *SA(i, block_idx, K/Q_BLK_SIZE);
            float sB_0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float sA_1 = *SA(i, block_idx + 1, K/Q_BLK_SIZE);
            float sB_1 = *SB(block_idx + 1, j, K/Q_BLK_SIZE);
            float fused_s_0 = sA_0 * sB_0;
            float fused_s_1 = sA_1 * sB_1;
            // Q_BLK_SIZE = 32
            // 32 * 8 = 256
            // so ymm only need to load once
                __m256i a_pack_int8_low = _mm256_loadu_si256( (__m256i *)  A(i, p, K));
                __m256i a_pack_int8_high = _mm256_loadu_si256( (__m256i *) A(i, p + Q_BLK_SIZE, K));


                // __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) blockB(p, j, K));
                __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) B(p/2, j, K/2));
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




void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
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
                int8_t b_low = (b & 0x0f) - 8; // first block
                int8_t b_high = (b >> 4) - 8;  // second block
                // int8_t b_high = (b & 0x0f) - 8;
                // int8_t b_low = (b >> 4) - 8;
                // if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                // {
                //     printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                // }
                sum += (float)a_low * (float)b_low * s_a_0 * s_b_0;
                sum += (float)a_high * (float)b_high * s_a_1 * s_b_1;
            }
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
    float* SA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* SB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    init_rand_int8(A, M, K);
    init_rand_uint8(B, K / 2, N);
    init_rand(SA, M, K / Q_BLK_SIZE);
    init_rand(SB, K / Q_BLK_SIZE, N);
    // cons init for debugging
    // init_int8_const(A, 2, M, K);
    // init_uint8_const(B, 154, K / 2, N);
    // init_const(SA, 0.1f, M, K / Q_BLK_SIZE);
    // init_const(SB, 0.2f, K / Q_BLK_SIZE, N);

    /*
    Repack A and B.
    Rather than calculate the precise volume needed to store quantized activation and weights,
    we use float size to ensure sufficient memory.
    */
    void * A_repack  = _mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    void * B_repack  = _mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    // repack A in row-major order. Pack size (1 x 64)
    int num_repack_blk_A_along_K = K / (2 * Q_BLK_SIZE);
    struct q80_repack_1x2 * A_repack_start = (struct q80_repack_1x2 *) A_repack;
    for (int i = 0; i < M; i++) 
    {
        struct q80_repack_1x2 * A_ptr = A_repack_start + i * num_repack_blk_A_along_K;
        for (int blk_id = 0; blk_id < num_repack_blk_A_along_K; blk_id++) 
        {
            float s_low =  *SA(i, blk_id * 2, K / Q_BLK_SIZE);
            float s_high = *SA(i, blk_id * 2 + 1, K / Q_BLK_SIZE);
            A_ptr[blk_id].s_low = s_low;
            A_ptr[blk_id].s_high = s_high;
            for (int q = 0; q < Q_BLK_SIZE; q++) 
            {
                A_ptr[blk_id].q_low[q] =  *A(i, blk_id * 2 * Q_BLK_SIZE + q, K);
                A_ptr[blk_id].q_high[q] = *A(i, blk_id * 2 * Q_BLK_SIZE + q + Q_BLK_SIZE, K);
            }
        }
    }
    // validate
    for (int i = 0; i < M; i++) 
    {
        for (int j = 0; j < num_repack_blk_A_along_K; j++) 
        {
            struct q80_repack_1x2 * A_ptr = A_repack_start + i * num_repack_blk_A_along_K + j;
            // printf("A[%d][%d] s_low: %f, s_high: %f\n", i, j, A_ptr->s_low, A_ptr->s_high);
            // for (int q = 0; q < Q_BLK_SIZE; q++) 
            // {
            //     printf("A[%d][%d] q_low[%d]: %d, q_high[%d]: %d\n", i, j, q, A_ptr->q_low[q], q, A_ptr->q_high[q]);
            // }

            // comparea with original value
            for (int q = 0; q < Q_BLK_SIZE; q++) 
            {
                int8_t a_low = A_ptr->q_low[q];
                int8_t a_high = A_ptr->q_high[q];
                int8_t a_low_orig = *A(i, j * 2 * Q_BLK_SIZE + q, K);
                int8_t a_high_orig = *A(i, j * 2 * Q_BLK_SIZE + q + Q_BLK_SIZE, K);
                if (a_low != a_low_orig || a_high != a_high_orig) 
                {
                    printf("\e[31m[ERROR]\e[m Repack A[%d][%d] mismatch: %d vs %d, %d vs %d\n", i, j, a_low, a_low_orig, a_high, a_high_orig);
                    // stop
                    return 1;
                }
            }
            // compare scaling factors
            float s_low = A_ptr->s_low;
            float s_high = A_ptr->s_high;
            float s_low_orig = *SA(i, j * 2, K / Q_BLK_SIZE);
            float s_high_orig = *SA(i, j * 2 + 1, K / Q_BLK_SIZE);  
            if (s_low != s_low_orig || s_high != s_high_orig) 
            {
                printf("\e[31m[ERROR]\e[m Repack A[%d][%d] scaling factors mismatch: %f vs %f, %f vs %f\n", i, j, s_low, s_low_orig, s_high, s_high_orig);
                    return 1;
            }
        }
    }

    // repack B. q40_repack_2x8 blocks are in column-major order, while elements within
    // it are in row-major order. 
    // Pack size (64 x 8)
    int num_repack_blk_B_along_K = K / (2 * Q_BLK_SIZE);
    int num_repack_blk_B_along_N = N / 8;
    struct q40_repack_2x8 * B_start =  (struct q40_repack_2x8 *) B_repack;
    for (int j = 0; j < num_repack_blk_B_along_N; j++) 
    {
        for (int i = 0; i < num_repack_blk_B_along_K; i++)
        {
            struct q40_repack_2x8 * B_ptr =  B_start + j * num_repack_blk_B_along_K + i;

            // pack scaling factors
            for (int jj = 0; jj < 8; jj++)
            {
                float s_low  = *SB ( i * 2    , j * 8 + jj, K / Q_BLK_SIZE ); 
                float s_high = *SB ( i * 2 + 1, j * 8 + jj, K / Q_BLK_SIZE ); 
                B_ptr->s_low[jj] = s_low;
                B_ptr->s_high[jj] = s_high;
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
    // validate B
    for (int j = 0; j < num_repack_blk_B_along_N; j++) 
    {
        for (int i = 0; i < num_repack_blk_B_along_K; i++)
        {
            struct q40_repack_2x8 * B_ptr =  B_start + j * num_repack_blk_B_along_K + i;
            // printf("B[%d][%d] s_low: %f, s_high: %f\n", i, j, B_ptr->s_low[0], B_ptr->s_high[0]);
            // for (int q = 0; q < 32; q++) 
            // {
            //     printf("B[%d][%d] q_coupled[%d]: %d\n", i, j, q, B_ptr->q_coupled[q]);
            // }
            // compare scaling factors
            for (int jj = 0; jj < 8; jj++)
            {
                float s_low  = B_ptr->s_low[jj];
                float s_high = B_ptr->s_high[jj];
                float s_low_orig  = *SB(i * 2    , j * 8 + jj, K / Q_BLK_SIZE);
                float s_high_orig = *SB(i * 2 + 1, j * 8 + jj, K / Q_BLK_SIZE);
                if (s_low != s_low_orig || s_high != s_high_orig) 
                {
                    printf("\e[31m[ERROR]\e[m Repack B[%d][%d] scaling factors mismatch: %f vs %f, %f vs %f\n", i, j, s_low, s_low_orig, s_high, s_high_orig);
                    return 1;
                }
            }
            // compare weights
            uint8_t * B_ptr_start =  B_ptr->q_coupled;
            int num_packs = 8;
            for (int pack_idx = 0; pack_idx < num_packs; pack_idx++)
            {
                uint8_t * pack_start_ptr = B_ptr_start + pack_idx * 32;
                for (int jj = 0; jj < 8; jj++)
                {
                    for (int ii = 0; ii < 4; ii++)
                    {
                        uint8_t weight = pack_start_ptr[ii + jj * 4];
                        uint8_t orig_weight = *B(i * Q_BLK_SIZE + pack_idx * 4 + ii, j * 8 + jj, K / 2);
                        if (weight != orig_weight) 
                        {
                            printf("\e[31m[ERROR]\e[m Repack B[%d][%d] weight mismatch: %d vs %d\n", i, j, weight, orig_weight);
                            return 1;
                        }
                    }
                }
            }

        }
    }


#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, SA, SB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        // matmul_kernel(A, B, C, SA, SB, M, N, K);
        matmul_repack(A_repack, B_repack, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("\e[31mGFLOPS\e[m= %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        // print_mat(C, 5 * 5);
        // printf("\n\n");
        // print_mat(C_ref, 5 * 5);
#ifdef TEST
        int res = compare_mats(C, C_ref, M * N);
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
