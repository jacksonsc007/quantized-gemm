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


#define MEM_ALIGN 64
#ifndef MDIM
    #define MDIM 1
#endif

#ifndef NDIM
    #define NDIM 10240
#endif

#ifndef KDIM
    #define KDIM 10240
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


 
struct q4_repack_2x8{
    ggml_fp16_t s_low[8];
    ggml_fp16_t s_high[8];
    uint8_t q_coupled[256]; // (64 * 8) / (8 / 4)
};

struct q8_repack_1x2{
    ggml_fp16_t s_low;
    ggml_fp16_t s_high;
    int8_t q_low[32];
    int8_t q_high[32];
};

#define GGML_F32Cx8_LOAD(x)     _mm256_cvtph_ps(_mm_loadu_si128((const __m128i *)(x)))

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
    int nb_m = M ;
        PRAGMA_OMP_PARALLEL_FOR
        for(int j = 0; j < nb_n; j++)
        {
            struct q4_repack_2x8 * B_ptr = B_ptr_start + j * nb_k;
            struct q8_repack_1x2 * A_ptr = A_ptr_start + 0 * nb_k;
            __m256 acc_row = _mm256_setzero_ps();
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
                    low_blk_B_pack = _mm256_sub_epi8(low_blk_B_pack, _mm256_set1_epi8(8));
                    __m256i high_blk_B_pack = _mm256_and_si256(_mm256_srli_epi16(B_coupled_pack, 4), _mm256_set1_epi8(0x0f));
                    high_blk_B_pack = _mm256_sub_epi8(high_blk_B_pack, _mm256_set1_epi8(8));
                    low_blk_B_packs[pack_idx] = low_blk_B_pack;
                    high_blk_B_packs[pack_idx] = high_blk_B_pack;
                }
                __m256i low_blk_A_packs[8];
                __m256i high_blk_A_packs[8];
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    low_blk_A_packs[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr[blk_id].q_low) )[pack_idx]);
                    high_blk_A_packs[pack_idx] = _mm256_set1_epi32(( (int32_t *)(A_ptr[blk_id].q_high) )[pack_idx]);
                }
                __m256i iacc_low = _mm256_setzero_si256();
                __m256i iacc_high = _mm256_setzero_si256();
                for (int pack_idx = 0; pack_idx < 8; pack_idx+=1)
                {
                    iacc_low = mul_sum_i8_pairs_acc_int32x8(iacc_low, low_blk_A_packs[pack_idx], low_blk_B_packs[pack_idx]);
                    iacc_high = mul_sum_i8_pairs_acc_int32x8(iacc_high, high_blk_A_packs[pack_idx], high_blk_B_packs[pack_idx]);
                }
                // __m256i iacc = _mm256_add_epi32(iacc_low, iacc_high);
                
                // load scaleing factors: fp16 -> fp32
                __m256 sB_low = GGML_F32Cx8_LOAD(B_block.s_low);
                __m256 sB_high = GGML_F32Cx8_LOAD(B_block.s_high);
                __m256 sA_low = _mm256_set1_ps(GGML_FP16_TO_FP32( A_ptr[blk_id].s_low ));
                __m256 sA_high = _mm256_set1_ps(GGML_FP16_TO_FP32( A_ptr[blk_id].s_high ));

                __m256 s_low = _mm256_mul_ps(sA_low, sB_low);
                __m256 s_high = _mm256_mul_ps(sA_high, sB_high);
                acc_row = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_low), s_low, acc_row);
                acc_row = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_high), s_high, acc_row);

            }
            _mm256_storeu_ps(C(0, j * 8, N), acc_row);
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

void quantize_row_q8_0_repack(const float * x, void * vy, int64_t k) {
    assert(QK8_0 == 32);
    assert(k % QK8_0 == 0);
    const int nb = k / QK8_0;

    struct q8_repack_1x2 * y = (struct q8_repack_1x2 *) vy;
    for (int i = 0; i < nb/2; i++) {
        // Load elements into 8 AVX vectors (4 for each block)
        __m256 v0_low = _mm256_loadu_ps(x);
        __m256 v1_low = _mm256_loadu_ps(x + 8);
        __m256 v2_low = _mm256_loadu_ps(x + 16);
        __m256 v3_low = _mm256_loadu_ps(x + 24);
        __m256 v0_high = _mm256_loadu_ps(x + 32);
        __m256 v1_high = _mm256_loadu_ps(x + 40);
        __m256 v2_high = _mm256_loadu_ps(x + 48);
        __m256 v3_high = _mm256_loadu_ps(x + 56);
        x += 64;

        // Compute max(abs(e)) for each block
        const __m256 signBit = _mm256_set1_ps(-0.0f);
        
        // For low block
        __m256 maxAbs_low = _mm256_andnot_ps(signBit, v0_low);
        maxAbs_low = _mm256_max_ps(maxAbs_low, _mm256_andnot_ps(signBit, v1_low));
        maxAbs_low = _mm256_max_ps(maxAbs_low, _mm256_andnot_ps(signBit, v2_low));
        maxAbs_low = _mm256_max_ps(maxAbs_low, _mm256_andnot_ps(signBit, v3_low));

        // For high block
        __m256 maxAbs_high = _mm256_andnot_ps(signBit, v0_high);
        maxAbs_high = _mm256_max_ps(maxAbs_high, _mm256_andnot_ps(signBit, v1_high));
        maxAbs_high = _mm256_max_ps(maxAbs_high, _mm256_andnot_ps(signBit, v2_high));
        maxAbs_high = _mm256_max_ps(maxAbs_high, _mm256_andnot_ps(signBit, v3_high));

        // Get max values
        __m128 max4_low = _mm_max_ps(_mm256_extractf128_ps(maxAbs_low, 1), _mm256_castps256_ps128(maxAbs_low));
        max4_low = _mm_max_ps(max4_low, _mm_movehl_ps(max4_low, max4_low));
        max4_low = _mm_max_ss(max4_low, _mm_movehdup_ps(max4_low));
        const float maxScalar_low = _mm_cvtss_f32(max4_low);

        __m128 max4_high = _mm_max_ps(_mm256_extractf128_ps(maxAbs_high, 1), _mm256_castps256_ps128(maxAbs_high));
        max4_high = _mm_max_ps(max4_high, _mm_movehl_ps(max4_high, max4_high));
        max4_high = _mm_max_ss(max4_high, _mm_movehdup_ps(max4_high));
        const float maxScalar_high = _mm_cvtss_f32(max4_high);

        // Quantize these floats
        const float d_low = maxScalar_low / 127.f;
        const float d_high = maxScalar_high / 127.f;
        y[i].s_low = GGML_FP32_TO_FP16(d_low);
        y[i].s_high = GGML_FP32_TO_FP16(d_high);
        
        const float id_low = (maxScalar_low != 0.0f) ? 127.f / maxScalar_low : 0.0f;
        const float id_high = (maxScalar_high != 0.0f) ? 127.f / maxScalar_high : 0.0f;
        const __m256 mul_low = _mm256_set1_ps(id_low);
        const __m256 mul_high = _mm256_set1_ps(id_high);

        // Apply the multipliers
        v0_low = _mm256_mul_ps(v0_low, mul_low);
        v1_low = _mm256_mul_ps(v1_low, mul_low);
        v2_low = _mm256_mul_ps(v2_low, mul_low);
        v3_low = _mm256_mul_ps(v3_low, mul_low);
        
        v0_high = _mm256_mul_ps(v0_high, mul_high);
        v1_high = _mm256_mul_ps(v1_high, mul_high);
        v2_high = _mm256_mul_ps(v2_high, mul_high);
        v3_high = _mm256_mul_ps(v3_high, mul_high);

        // Round to nearest integer
        v0_low = _mm256_round_ps(v0_low, _MM_ROUND_NEAREST);
        v1_low = _mm256_round_ps(v1_low, _MM_ROUND_NEAREST);
        v2_low = _mm256_round_ps(v2_low, _MM_ROUND_NEAREST);
        v3_low = _mm256_round_ps(v3_low, _MM_ROUND_NEAREST);
        
        v0_high = _mm256_round_ps(v0_high, _MM_ROUND_NEAREST);
        v1_high = _mm256_round_ps(v1_high, _MM_ROUND_NEAREST);
        v2_high = _mm256_round_ps(v2_high, _MM_ROUND_NEAREST);
        v3_high = _mm256_round_ps(v3_high, _MM_ROUND_NEAREST);

        // Convert floats to integers
        __m256i i0_low = _mm256_cvtps_epi32(v0_low);
        __m256i i1_low = _mm256_cvtps_epi32(v1_low);
        __m256i i2_low = _mm256_cvtps_epi32(v2_low);
        __m256i i3_low = _mm256_cvtps_epi32(v3_low);
        
        __m256i i0_high = _mm256_cvtps_epi32(v0_high);
        __m256i i1_high = _mm256_cvtps_epi32(v1_high);
        __m256i i2_high = _mm256_cvtps_epi32(v2_high);
        __m256i i3_high = _mm256_cvtps_epi32(v3_high);

        // Convert int32 to int16
        i0_low = _mm256_packs_epi32(i0_low, i1_low);
        i2_low = _mm256_packs_epi32(i2_low, i3_low);
        i0_high = _mm256_packs_epi32(i0_high, i1_high);
        i2_high = _mm256_packs_epi32(i2_high, i3_high);

        // Convert int16 to int8
        i0_low = _mm256_packs_epi16(i0_low, i2_low);
        i0_high = _mm256_packs_epi16(i0_high, i2_high);

        // Fix the order
        const __m256i perm = _mm256_setr_epi32(0, 4, 1, 5, 2, 6, 3, 7);
        i0_low = _mm256_permutevar8x32_epi32(i0_low, perm);
        i0_high = _mm256_permutevar8x32_epi32(i0_high, perm);

        // Store results
        _mm256_storeu_si256((__m256i *)y[i].q_low, i0_low);
        _mm256_storeu_si256((__m256i *)y[i].q_high, i0_high);
    }
}


void float_matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int p = 0; p < K; p++) {
                C[j * M + i] += A[p * M + i] * B[j * K + p];
            }
        }
    }
}

int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A  = (int8_t*)_mm_malloc(M * K * sizeof(int8_t), MEM_ALIGN);
    uint8_t* B = (uint8_t*)_mm_malloc(K / 2 * N * sizeof(uint8_t), MEM_ALIGN);
    ggml_fp16_t* SA  = (ggml_fp16_t*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(ggml_fp16_t), MEM_ALIGN);
    ggml_fp16_t* ScaledSumA  = (ggml_fp16_t*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(ggml_fp16_t), MEM_ALIGN);
    ggml_fp16_t* SB  = (ggml_fp16_t*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(ggml_fp16_t), MEM_ALIGN);
    ggml_fp16_t* MinB  = (ggml_fp16_t*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(ggml_fp16_t), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    
    // ggml initialization
    const ggml_type weight_q_type = GGML_TYPE_Q4_0_ink;
    const ggml_type activation_q_type = GGML_TYPE_Q8_0_ink;

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
    block_q8_0_ink * q8_blocks = (block_q8_0_ink *)q8_activation->data;
    int32_t num_q8_blocks = q8_activation->ne[0] * q8_activation->ne[1] / QK8_0_ink;
    for (int block_idx = 0; block_idx < num_q8_blocks; ++block_idx) {
        block_q8_0_ink blk = q8_blocks[block_idx];
        ggml_fp16_t scale_1 = (blk.d1);
        ggml_fp16_t scale_2 = (blk.d2);

        SA[2 * block_idx] = scale_1;
        SA[2 * block_idx + 1] = scale_2;
        memcpy(A + block_idx * QK8_0_ink, blk.qs1, QK8_0_ink / 2 * sizeof(int8_t));
        memcpy(A + block_idx * QK8_0_ink + QK8_0_ink / 2, blk.qs2, QK8_0_ink / 2 * sizeof(int8_t));
    }
    block_q4_0_ink * q4_blocks = (block_q4_0_ink *)q4_weight->data;
    int32_t num_q4_blocks = q4_weight->ne[0] * q4_weight->ne[1] / QK4_0_ink;
    for (int block_idx = 0; block_idx < num_q4_blocks; ++block_idx) {
        block_q4_0_ink blk = q4_blocks[block_idx];
        ggml_fp16_t scale_1 = (blk.d1);
        ggml_fp16_t scale_2 = (blk.d2);

        SB[2 * block_idx] = scale_1;
        SB[2 * block_idx + 1] = scale_2;
        memcpy(B + block_idx * QK4_0_ink / 2, blk.qs, QK4_0_ink / 2 * sizeof(uint8_t));
    }
    


    
    void * A_repack  = _mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    void * B_repack  = _mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    // repack A in row-major order. Pack size (1 x 64)
    bool repack_during_quantization = true;
    int num_repack_blk_A_along_K = K / (2 * Q_BLK_SIZE);
    struct q8_repack_1x2 * A_repack_start = (struct q8_repack_1x2 *) A_repack;
    if (repack_during_quantization)
    {
        quantize_row_q8_0_repack((const float *) activation->data, A_repack, M * K);
    }
    else {
        for (int i = 0; i < M; i++) 
        {
            struct q8_repack_1x2 * A_ptr = A_repack_start + i * num_repack_blk_A_along_K;
            for (int blk_id = 0; blk_id < num_repack_blk_A_along_K; blk_id++) 
            {
                ggml_fp16_t s_low =  *SA(i, blk_id * 2, K / Q_BLK_SIZE);
                ggml_fp16_t s_high = *SA(i, blk_id * 2 + 1, K / Q_BLK_SIZE);
                A_ptr[blk_id].s_low = s_low;
                A_ptr[blk_id].s_high = s_high;
                for (int q = 0; q < Q_BLK_SIZE; q++) 
                {
                    A_ptr[blk_id].q_low[q] =  *A(i, blk_id * 2 * Q_BLK_SIZE + q, K);
                    A_ptr[blk_id].q_high[q] = *A(i, blk_id * 2 * Q_BLK_SIZE + q + Q_BLK_SIZE, K);
                }
            }
        }
    }

    // repack B. q4_repack_2x8 blocks are in column-major order, while elements within
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
                ggml_fp16_t s_low  = *SB ( i * 2    , j * 8 + jj, K / Q_BLK_SIZE ); 
                ggml_fp16_t s_high = *SB ( i * 2 + 1, j * 8 + jj, K / Q_BLK_SIZE ); 
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


    // These matrixes are allocated to override cache lines when benchmarking vector-matrix multiplication
    const int M_ = 1000;
    const int K_ = 1000;
    const int N_ = 1000;
    float* A_ = (float*)_mm_malloc(M_ * K_ * sizeof(float), MEM_ALIGN);
    float* B_ = (float*)_mm_malloc(K_ * N_ * sizeof(float), MEM_ALIGN);
    float* C_ = (float*)_mm_malloc(M_ * N_ * sizeof(float), MEM_ALIGN);
    init_rand(A_, M_, K_);
    init_rand(B_, K_, N_);

    double FLOP = 2 * (double)M * N * K;
    printf("\n\e[32m[INFO]\e[m M=%d, K=%d, N=%d\n", M, K, N);
    for (int i = 0; i < NITER; i++) {
        init_const(C, 0.0, M, N);
        float_matmul_naive(A_, B_, C_, M_, N_, K_);
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
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(A_repack);
    _mm_free(B_repack);
    _mm_free(A_);
    _mm_free(B_);
    _mm_free(C_);
    return 0;
}
