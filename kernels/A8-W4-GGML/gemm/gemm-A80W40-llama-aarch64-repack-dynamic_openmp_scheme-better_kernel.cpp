#include <blis/cblas.h>
#include <cstdint>
#include <cstring>
#include <mm_malloc.h>
#include <vector>
#include <assert.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include "stdbool.h"
#include "ggml.h"
#include <stdexcept>
#include "ggml-cpu.h"
#include "ggml-quants.h"
#include "ggml-impl.h"

#include "lib.h"
#include "profile.h"
#include "ggml-cpu-aarch64.h"

#ifndef NTHREADS
#define NTHREADS 1
#endif

#define OMP_SCHEDULE dynamic
#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

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
    #define NITER 5
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

 
// TODO: move to include file?
template <int K> constexpr int QK_0() {
    if constexpr (K == 4) {
        return QK4_0;
    }
    if constexpr (K == 8) {
        return QK8_0;
    }
    return -1;
}

template <int K, int N> struct block {
    ggml_half d[N];                         // deltas for N qK_0 blocks
    int8_t    qs[(QK_0<K>() * N * K) / 8];  // quants for N qK_0 blocks
};

// control size
static_assert(sizeof(block<4, 4>) == 4 * sizeof(ggml_half) + QK8_0 * 2, "wrong block<4,4> size/padding");
static_assert(sizeof(block<4, 8>) == 8 * sizeof(ggml_half) + QK8_0 * 4, "wrong block<4,8> size/padding");
static_assert(sizeof(block<8, 4>) == 4 * sizeof(ggml_half) + QK8_0 * 4, "wrong block<8,4> size/padding");
static_assert(sizeof(block<8, 8>) == 8 * sizeof(ggml_half) + QK8_0 * 8, "wrong block<8,8> size/padding");

using block_q4_0x4 = block<4, 4>;
using block_q4_0x8 = block<4, 8>;
using block_q8_0x4 = block<8, 4>;
using block_q8_0x8 = block<8, 8>;

#define GGML_F32Cx8_LOAD(x)     _mm256_cvtph_ps(_mm_loadu_si128((const __m128i *)(x)))
#define GGML_F32Cx8_REPEAT_LOAD(x, loadMask)     _mm256_cvtph_ps(_mm_shuffle_epi32(_mm_maskload_epi32((int const*)(x), loadMask), 68))
#define GGML_F32Cx8_REARRANGE_LOAD(x, arrangeMask)     _mm256_cvtph_ps(_mm_shuffle_epi8(_mm_loadu_si128((const __m128i *) x), arrangeMask))

// void matmul_kernel( int K, float * C, int N, char * Bq, char * Aq, int M)
void matmul_kernel(int n, float * GGML_RESTRICT s, size_t bs, const void * GGML_RESTRICT vx, const void * GGML_RESTRICT vy, int nr, int nc)
{


    int K = n;
    int M = nr;
    int N = bs;
    size_t flops = (size_t) M * N * K * 2;
    const std::string profile_name = "matmul kernel: ";
    std::ostringstream oss;
    oss << "[" << profile_name << M << " x " << N << " x " << K << "]";
    std::string formatted_profile_name = oss.str();
    PROFILE_START_FLOPS(formatted_profile_name, flops);
    const int qk = QK8_0;
    const int nb = n / qk;
    const int ncols_interleaved = 8;
    const int blocklen = 8;

    assert (n % qk == 0);
    assert (nr % 4 == 0);
    assert (nc % ncols_interleaved == 0);

    {
        const block_q4_0x8 * b_ptr_start = (const block_q4_0x8 *)vx;
        const block_q8_0x4 * a_ptr_start = (const block_q8_0x4 *)vy;
        int64_t b_nb = n / QK4_0;
        // int64_t y = 0;
        // Mask to mask out nibbles from packed bytes
        const __m256i m4b = _mm256_set1_epi8(0x0F);
        const __m128i loadMask = _mm_blend_epi32(_mm_setzero_si128(), _mm_set1_epi32(0xFFFFFFFF), 3);
        // Lookup table to convert signed nibbles to signed bytes
        __m256i signextendlut = _mm256_castsi128_si256(_mm_set_epi8(-1, -2, -3, -4, -5, -6, -7, -8, 7, 6, 5, 4, 3, 2, 1, 0));
        signextendlut = _mm256_permute2f128_si256(signextendlut, signextendlut, 0);
        // Permute mask used for easier vector processing at later stages
        __m256i requiredOrder = _mm256_set_epi32(3, 2, 1, 0, 7, 6, 5, 4);
        int64_t xstart = 0;
        int anr = nr - nr%16; // Used to align nr with boundary of 16

        // Take group of four block_q8_0x4 structures at each pass of the loop and perform dot product operation

        PRAGMA_OMP_PARALLEL_FOR
        for (int64_t y = 0; y < anr / 4; y += 4) {
            const block_q8_0x4 * a_ptrs[4];

            a_ptrs[0] = a_ptr_start + (y * nb);
            for (int i = 0; i < 3; ++i) {
                a_ptrs[i + 1] = a_ptrs[i] + nb;
            }

            // Take group of eight block_q4_0x8 structures at each pass of the loop and perform dot product operation
            for (int64_t x = xstart; x < nc / 8; x++) {

                const block_q4_0x8 * b_ptr = b_ptr_start + (x * b_nb);

                // Master FP accumulators
                __m256 acc_rows[16];
                for (int i = 0; i < 16; i++) {
                    acc_rows[i] = _mm256_setzero_ps();
                }

                for (int64_t b = 0; b < nb; b++) {
                    // Load the eight block_q4_0 quantized values interleaved with each other in chunks of eight - B0,B1 ....B6,B7
                    const __m256i rhs_raw_mat_0123_0 = _mm256_loadu_si256((const __m256i *)(b_ptr[b].qs));
                    const __m256i rhs_raw_mat_4567_0 = _mm256_loadu_si256((const __m256i *)(b_ptr[b].qs + 32));
                    const __m256i rhs_raw_mat_0123_1 = _mm256_loadu_si256((const __m256i *)(b_ptr[b].qs + 64));
                    const __m256i rhs_raw_mat_4567_1 = _mm256_loadu_si256((const __m256i *)(b_ptr[b].qs + 96));

                    // Save the values in the following vectors in the formats B0B1B4B5, B2B3B6B7 for further processing and storing of values
                    const __m256i rhs_raw_mat_0145_0 = _mm256_blend_epi32(rhs_raw_mat_0123_0, _mm256_permutevar8x32_epi32(rhs_raw_mat_4567_0, requiredOrder), 240);
                    const __m256i rhs_raw_mat_2367_0 = _mm256_blend_epi32(_mm256_permutevar8x32_epi32(rhs_raw_mat_0123_0, requiredOrder), rhs_raw_mat_4567_0, 240);
                    const __m256i rhs_raw_mat_0145_1 = _mm256_blend_epi32(rhs_raw_mat_0123_1, _mm256_permutevar8x32_epi32(rhs_raw_mat_4567_1, requiredOrder), 240);
                    const __m256i rhs_raw_mat_2367_1 = _mm256_blend_epi32(_mm256_permutevar8x32_epi32(rhs_raw_mat_0123_1, requiredOrder), rhs_raw_mat_4567_1, 240);

                    // 4-bit -> 8-bit - Sign is maintained
                    const __m256i rhs_mat_0145_0 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(rhs_raw_mat_0145_0, m4b)); //B0(0-7) B1(0-7) B4(0-7) B5(0-7)
                    const __m256i rhs_mat_2367_0 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(rhs_raw_mat_2367_0, m4b)); //B2(0-7) B3(0-7) B6(0-7) B7(0-7)

                    const __m256i rhs_mat_0145_1 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(rhs_raw_mat_0145_1, m4b)); //B0(8-15) B1(8-15) B4(8-15) B5(8-15)
                    const __m256i rhs_mat_2367_1 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(rhs_raw_mat_2367_1, m4b)); //B2(8-15) B3(8-15) B6(8-15) B7(8-15)

                    const __m256i rhs_mat_0145_2 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(_mm256_srli_epi16(rhs_raw_mat_0145_0, 4), m4b)); //B0(16-23) B1(16-23) B4(16-23) B5(16-23)
                    const __m256i rhs_mat_2367_2 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(_mm256_srli_epi16(rhs_raw_mat_2367_0, 4), m4b)); //B2(16-23) B3(16-23) B6(16-23) B7(16-23)

                    const __m256i rhs_mat_0145_3 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(_mm256_srli_epi16(rhs_raw_mat_0145_1, 4), m4b)); //B0(24-31) B1(24-31) B4(24-31) B5(24-31)
                    const __m256i rhs_mat_2367_3 = _mm256_shuffle_epi8(signextendlut, _mm256_and_si256(_mm256_srli_epi16(rhs_raw_mat_2367_1, 4), m4b)); //B2(24-31) B3(24-31) B6(24-31) B7(24-31)

                    // Shuffle pattern one - right side input
                    const __m256i rhs_mat_0145_0_sp1 = _mm256_shuffle_epi32(rhs_mat_0145_0, 136);  //B0(0-3) B1(0-3) B0(0-3) B1(0-3) B4(0-3) B5(0-3) B4(0-3) B5(0-3)
                    const __m256i rhs_mat_2367_0_sp1 = _mm256_shuffle_epi32(rhs_mat_2367_0, 136);  //B2(0-3) B3(0-3) B2(0-3) B3(0-3) B6(0-3) B7(0-3) B6(0-3) B7(0-3)

                    const __m256i rhs_mat_0145_1_sp1 = _mm256_shuffle_epi32(rhs_mat_0145_1, 136);  //B0(8-11) B1(8-11) B0(8-11) B1(8-11) B4(8-11) B5(8-11) B4(8-11) B5(8-11)
                    const __m256i rhs_mat_2367_1_sp1 = _mm256_shuffle_epi32(rhs_mat_2367_1, 136);  //B2(8-11) B3(8-11) B2(8-11) B3(8-11) B6(8-11) B7(8-11) B6(8-11) B7(8-11)

                    const __m256i rhs_mat_0145_2_sp1 = _mm256_shuffle_epi32(rhs_mat_0145_2, 136);  //B0(16-19) B1(16-19) B0(16-19) B1(16-19) B4(16-19) B5(16-19) B4(16-19) B5(16-19)
                    const __m256i rhs_mat_2367_2_sp1 = _mm256_shuffle_epi32(rhs_mat_2367_2, 136);  //B2(16-19) B3(16-19) B2(16-19) B3(16-19) B6(16-19) B7(16-19) B6(16-19) B7(16-19)

                    const __m256i rhs_mat_0145_3_sp1 = _mm256_shuffle_epi32(rhs_mat_0145_3, 136);  //B0(24-27) B1(24-27) B0(24-27) B1(24-27) B4(24-27) B5(24-27) B4(24-27) B5(24-27)
                    const __m256i rhs_mat_2367_3_sp1 = _mm256_shuffle_epi32(rhs_mat_2367_3, 136);  //B2(24-27) B3(24-27) B2(24-27) B3(24-27) B6(24-27) B7(24-27) B6(24-27) B7(24-27)

                    // Shuffle pattern two - right side input

                    const __m256i rhs_mat_0145_0_sp2 = _mm256_shuffle_epi32(rhs_mat_0145_0, 221);  //B0(4-7) B1(4-7) B0(4-7) B1(4-7) B4(4-7) B5(4-7) B4(4-7) B5(4-7)
                    const __m256i rhs_mat_2367_0_sp2 = _mm256_shuffle_epi32(rhs_mat_2367_0, 221);  //B2(4-7) B3(4-7) B2(4-7) B3(4-7) B6(4-7) B7(4-7) B6(4-7) B7(4-7)

                    const __m256i rhs_mat_0145_1_sp2 = _mm256_shuffle_epi32(rhs_mat_0145_1, 221);  //B0(12-15) B1(12-15) B0(12-15) B1(12-15) B4(12-15) B5(12-15) B4(12-15) B5(12-15)
                    const __m256i rhs_mat_2367_1_sp2 = _mm256_shuffle_epi32(rhs_mat_2367_1, 221);  //B2(12-15) B3(12-15) B2(12-15) B3(12-15) B6(12-15) B7(12-15) B6(12-15) B7(12-15)

                    const __m256i rhs_mat_0145_2_sp2 = _mm256_shuffle_epi32(rhs_mat_0145_2, 221);  //B0(20-23) B1(20-23) B0(20-23) B1(20-23) B4(20-23) B5(20-23) B4(20-23) B5(20-23)
                    const __m256i rhs_mat_2367_2_sp2 = _mm256_shuffle_epi32(rhs_mat_2367_2, 221);  //B2(20-23) B3(20-23) B2(20-23) B3(20-23) B6(20-23) B7(20-23) B6(20-23) B7(20-23)

                    const __m256i rhs_mat_0145_3_sp2 = _mm256_shuffle_epi32(rhs_mat_0145_3, 221);  //B0(28-31) B1(28-31) B0(28-31) B1(28-31) B4(28-31) B5(28-31) B4(28-31) B5(28-31)
                    const __m256i rhs_mat_2367_3_sp2 = _mm256_shuffle_epi32(rhs_mat_2367_3, 221);  //B2(28-31) B3(28-31) B2(28-31) B3(28-31) B6(28-31) B7(28-31) B6(28-31) B7(28-31)

                    // Scale values - Load the wight scale values of block_q4_0x8
                    const __m256 col_scale_f32 = GGML_F32Cx8_LOAD(b_ptr[b].d);

                    // Process LHS in groups of four
                    for (int rp = 0; rp < 4; rp++) {
                        // Load the four block_q4_0 quantized values interleaved with each other in chunks of eight - A0,A1,A2,A3
                        // Loaded as set of 128 bit vectors and repeated into a 256 bit vector
                        __m256i lhs_mat_0123_0 = _mm256_loadu_si256((const __m256i *)((a_ptrs[rp][b].qs)));
                        __m256i lhs_mat_01_0 = _mm256_permute2f128_si256(lhs_mat_0123_0, lhs_mat_0123_0, 0);
                        __m256i lhs_mat_23_0 = _mm256_permute2f128_si256(lhs_mat_0123_0, lhs_mat_0123_0, 17);
                        __m256i lhs_mat_0123_1 = _mm256_loadu_si256((const __m256i *)((a_ptrs[rp][b].qs + 32)));
                        __m256i lhs_mat_01_1 = _mm256_permute2f128_si256(lhs_mat_0123_1, lhs_mat_0123_1, 0);
                        __m256i lhs_mat_23_1 = _mm256_permute2f128_si256(lhs_mat_0123_1, lhs_mat_0123_1, 17);
                        __m256i lhs_mat_0123_2 = _mm256_loadu_si256((const __m256i *)((a_ptrs[rp][b].qs + 64)));
                        __m256i lhs_mat_01_2 = _mm256_permute2f128_si256(lhs_mat_0123_2, lhs_mat_0123_2, 0);
                        __m256i lhs_mat_23_2 = _mm256_permute2f128_si256(lhs_mat_0123_2, lhs_mat_0123_2, 17);
                        __m256i lhs_mat_0123_3 = _mm256_loadu_si256((const __m256i *)((a_ptrs[rp][b].qs + 96)));
                        __m256i lhs_mat_01_3 = _mm256_permute2f128_si256(lhs_mat_0123_3, lhs_mat_0123_3, 0);
                        __m256i lhs_mat_23_3 = _mm256_permute2f128_si256(lhs_mat_0123_3, lhs_mat_0123_3, 17);

                        // Shuffle pattern one - left side input
                        const __m256i lhs_mat_01_0_sp1 = _mm256_shuffle_epi32(lhs_mat_01_0, 160);  //A0(0-3) A0(0-3) A1(0-3) A1(0-3) A0(0-3) A0(0-3) A1(0-3) A1(0-3)
                        const __m256i lhs_mat_23_0_sp1 = _mm256_shuffle_epi32(lhs_mat_23_0, 160);  //A2(0-3) A2(0-3) A3(0-3) A3(0-3) A2(0-3) A2(0-3) A3(0-3) A3(0-3)

                        const __m256i lhs_mat_01_1_sp1 = _mm256_shuffle_epi32(lhs_mat_01_1, 160);  //A0(8-11) A0(8-11) A1(8-11) A1(8-11) A0(8-11) A0(8-11) A1(8-11) A1(8-11)
                        const __m256i lhs_mat_23_1_sp1 = _mm256_shuffle_epi32(lhs_mat_23_1, 160);  //A2(8-11) A2(8-11) A3(8-11) A3(8-11) A2(8-11) A2(8-11) A3(8-11) A3(8-11)

                        const __m256i lhs_mat_01_2_sp1 = _mm256_shuffle_epi32(lhs_mat_01_2, 160);  //A0(16-19) A0(16-19) A1(16-19) A1(16-19) A0(16-19) A0(16-19) A1(16-19) A1(16-19)
                        const __m256i lhs_mat_23_2_sp1 = _mm256_shuffle_epi32(lhs_mat_23_2, 160);  //A2(16-19) A2(16-19) A3(16-19) A3(16-19) A2(16-19) A2(16-19) A3(16-19) A3(16-19)

                        const __m256i lhs_mat_01_3_sp1 = _mm256_shuffle_epi32(lhs_mat_01_3, 160);  //A0(24-27) A0(24-27) A1(24-27) A1(24-27) A0(24-27) A0(24-27) A1(24-27) A1(24-27)
                        const __m256i lhs_mat_23_3_sp1 = _mm256_shuffle_epi32(lhs_mat_23_3, 160);  //A2(24-27) A2(24-27) A3(24-27) A3(24-27) A2(24-27) A2(24-27) A3(24-27) A3(24-27)

                        // Shuffle pattern two - left side input
                        const __m256i lhs_mat_01_0_sp2 = _mm256_shuffle_epi32(lhs_mat_01_0, 245);  //A0(4-7) A0(4-7) A1(4-7) A1(4-7) A0(4-7) A0(4-7) A1(4-7) A1(4-7)
                        const __m256i lhs_mat_23_0_sp2 = _mm256_shuffle_epi32(lhs_mat_23_0, 245);  //A2(4-7) A2(4-7) A3(4-7) A3(4-7) A2(4-7) A2(4-7) A3(4-7) A3(4-7)

                        const __m256i lhs_mat_01_1_sp2 = _mm256_shuffle_epi32(lhs_mat_01_1, 245);  //A0(12-15) A0(12-15) A1(12-15) A1(12-15) A0(12-15) A0(12-15) A1(12-15) A1(12-15)
                        const __m256i lhs_mat_23_1_sp2 = _mm256_shuffle_epi32(lhs_mat_23_1, 245);  //A2(12-15) A2(12-15) A3(12-15) A3(12-15) A2(12-15) A2(12-15) A3(12-15) A3(12-15)

                        const __m256i lhs_mat_01_2_sp2 = _mm256_shuffle_epi32(lhs_mat_01_2, 245);  //A0(20-23) A0(20-23) A1(20-23) A1(20-23) A0(20-23) A0(20-23) A1(20-23) A1(20-23)
                        const __m256i lhs_mat_23_2_sp2 = _mm256_shuffle_epi32(lhs_mat_23_2, 245);  //A2(20-23) A2(20-23) A3(20-23) A3(20-23) A2(20-23) A2(20-23) A3(20-23) A3(20-23)

                        const __m256i lhs_mat_01_3_sp2 = _mm256_shuffle_epi32(lhs_mat_01_3, 245);  //A0(28-31) A0(28-31) A1(28-31) A1(28-31) A0(28-31) A0(28-31) A1(28-31) A1(28-31)
                        const __m256i lhs_mat_23_3_sp2 = _mm256_shuffle_epi32(lhs_mat_23_3, 245);  //A2(28-31) A2(28-31) A3(28-31) A3(28-31) A2(28-31) A2(28-31) A3(28-31) A3(28-31)

                        // The values arranged in shuffle patterns are operated with dot product operation within 32 bit lane i.e corresponding bytes and multiplied and added into 32 bit integers within 32 bit lane
                        // Resembles MMLAs into 2x2 matrices in ARM Version
                        const __m256i zero = _mm256_setzero_si256();
                        __m256i iacc_mat_00_sp1 = mul_sum_i8_pairs_acc_int32x8(
                            mul_sum_i8_pairs_acc_int32x8(
                                mul_sum_i8_pairs_acc_int32x8(
                                    mul_sum_i8_pairs_acc_int32x8(
                                        zero, 
                                        lhs_mat_01_3_sp1,
                                        rhs_mat_0145_3_sp1
                                    ), 
                                    lhs_mat_01_2_sp1,
                                    rhs_mat_0145_2_sp1
                                ), 
                                lhs_mat_01_1_sp1,
                                rhs_mat_0145_1_sp1
                            ), 
                            lhs_mat_01_0_sp1,
                            rhs_mat_0145_0_sp1
                        );
                        __m256i iacc_mat_01_sp1 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_01_3_sp1, rhs_mat_2367_3_sp1), lhs_mat_01_2_sp1, rhs_mat_2367_2_sp1), lhs_mat_01_1_sp1, rhs_mat_2367_1_sp1), lhs_mat_01_0_sp1, rhs_mat_2367_0_sp1);
                        __m256i iacc_mat_10_sp1 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_23_3_sp1, rhs_mat_0145_3_sp1), lhs_mat_23_2_sp1, rhs_mat_0145_2_sp1), lhs_mat_23_1_sp1, rhs_mat_0145_1_sp1), lhs_mat_23_0_sp1, rhs_mat_0145_0_sp1);
                        __m256i iacc_mat_11_sp1 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_23_3_sp1, rhs_mat_2367_3_sp1), lhs_mat_23_2_sp1, rhs_mat_2367_2_sp1), lhs_mat_23_1_sp1, rhs_mat_2367_1_sp1), lhs_mat_23_0_sp1, rhs_mat_2367_0_sp1);
                        __m256i iacc_mat_00_sp2 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_01_3_sp2, rhs_mat_0145_3_sp2), lhs_mat_01_2_sp2, rhs_mat_0145_2_sp2), lhs_mat_01_1_sp2, rhs_mat_0145_1_sp2), lhs_mat_01_0_sp2, rhs_mat_0145_0_sp2);
                        __m256i iacc_mat_01_sp2 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_01_3_sp2, rhs_mat_2367_3_sp2), lhs_mat_01_2_sp2, rhs_mat_2367_2_sp2), lhs_mat_01_1_sp2, rhs_mat_2367_1_sp2), lhs_mat_01_0_sp2, rhs_mat_2367_0_sp2);
                        __m256i iacc_mat_10_sp2 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_23_3_sp2, rhs_mat_0145_3_sp2), lhs_mat_23_2_sp2, rhs_mat_0145_2_sp2), lhs_mat_23_1_sp2, rhs_mat_0145_1_sp2), lhs_mat_23_0_sp2, rhs_mat_0145_0_sp2);
                        __m256i iacc_mat_11_sp2 = mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(mul_sum_i8_pairs_acc_int32x8(zero, lhs_mat_23_3_sp2, rhs_mat_2367_3_sp2), lhs_mat_23_2_sp2, rhs_mat_2367_2_sp2), lhs_mat_23_1_sp2, rhs_mat_2367_1_sp2), lhs_mat_23_0_sp2, rhs_mat_2367_0_sp2);

                        // Output of both shuffle patterns are added in order to sum dot product outputs of all 32 values in block
                        __m256i iacc_mat_00 = _mm256_add_epi32(iacc_mat_00_sp1, iacc_mat_00_sp2);
                        __m256i iacc_mat_01 = _mm256_add_epi32(iacc_mat_01_sp1, iacc_mat_01_sp2);
                        __m256i iacc_mat_10 = _mm256_add_epi32(iacc_mat_10_sp1, iacc_mat_10_sp2);
                        __m256i iacc_mat_11 = _mm256_add_epi32(iacc_mat_11_sp1, iacc_mat_11_sp2);

                        // Straighten out to make 4 row vectors
                        __m256i iacc_row_0 = _mm256_blend_epi32(iacc_mat_00, _mm256_shuffle_epi32(iacc_mat_01, 78), 204);
                        __m256i iacc_row_1 = _mm256_blend_epi32(_mm256_shuffle_epi32(iacc_mat_00, 78), iacc_mat_01, 204);
                        __m256i iacc_row_2 = _mm256_blend_epi32(iacc_mat_10, _mm256_shuffle_epi32(iacc_mat_11, 78), 204);
                        __m256i iacc_row_3 = _mm256_blend_epi32(_mm256_shuffle_epi32(iacc_mat_10, 78), iacc_mat_11, 204);

                        // Load the scale(d) values for all the 4 Q8_0 blocks and repeat it across lanes
                        const __m256 row_scale_f32 = GGML_F32Cx8_REPEAT_LOAD(a_ptrs[rp][b].d, loadMask);

                        // Multiply with appropiate scales and accumulate
                        acc_rows[rp * 4    ] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_row_0), _mm256_mul_ps(col_scale_f32, _mm256_shuffle_ps(row_scale_f32, row_scale_f32, 0)), acc_rows[rp * 4]);
                        acc_rows[rp * 4 + 1] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_row_1), _mm256_mul_ps(col_scale_f32, _mm256_shuffle_ps(row_scale_f32, row_scale_f32, 85)), acc_rows[rp * 4 + 1]);
                        acc_rows[rp * 4 + 2] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_row_2), _mm256_mul_ps(col_scale_f32, _mm256_shuffle_ps(row_scale_f32, row_scale_f32, 170)), acc_rows[rp * 4 + 2]);
                        acc_rows[rp * 4 + 3] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(iacc_row_3), _mm256_mul_ps(col_scale_f32, _mm256_shuffle_ps(row_scale_f32, row_scale_f32,  255)), acc_rows[rp * 4 + 3]);
                    }
                }

                // Store the accumulated values
                for (int i = 0; i < 16; i++) {
                    _mm256_storeu_ps((float *)(s + ((y * 4 + i) * bs + x * 8)), acc_rows[i]);
                }
            }
        }

        PROFILE_END(formatted_profile_name);
        return;
    }
}

void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{

    assert(K % Q_BLK_SIZE == 0);
    for (int i = 0; i < M; ++i) 
    for (int j = 0; j < N; ++j)
    {
        float sum = 0;
        // load 1 quantization blocks at once
        for (int p = 0; p < K; p += Q_BLK_SIZE )
        {
            // SA: row-major
            // SB: column-major
            int block_idx = p / Q_BLK_SIZE;
            float s_a = *SA(i, block_idx, KDIM / Q_BLK_SIZE);
            float s_b = *SB(block_idx, j, KDIM / Q_BLK_SIZE);
            // 1 elements in B contains two int4 weights, matching 2 int8 activations 
            for (int q = 0; q < Q_BLK_SIZE / 2; q += 1)
            {
                int8_t a_low = *A(i, p + q, KDIM); // get (p + q)-th element
                int8_t a_high = *A(i, p + q + Q_BLK_SIZE / 2, KDIM); // get (p + q + 16)
                uint8_t b = *B(p / 2 + q , j, KDIM / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f) - 8; 
                int8_t b_high = (b >> 4) - 8; 
                float a_low_f = (float)a_low * s_a;
                float a_high_f = (float)a_high * s_a;
                float b_low_f = (float)b_low * s_b;
                float b_high_f = (float)b_high * s_b;
                sum += a_low_f * b_low_f + a_high_f * b_high_f;

            }
        }    
        *C(i, j, NDIM) = sum;
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

void print_q8_1_data(block_q8_0 * GGML_RESTRICT blocks, size_t n_blocks) {

    for (size_t b = 0; b < n_blocks; ++b) {
         block_q8_0 blk = blocks[b];
        float scale = GGML_FP16_TO_FP32 (blk.d);
        printf("Block %zu (scale = %.6f):\n", b, scale);
        for (int i = 0; i < QK8_0; ++i) {
            int32_t q = (int32_t) blk.qs[i];
            float val =  scale * (float) q;
            printf(" %6.2f", val);
            if ((i + 1) % 8 == 0) printf(" |");
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n\n");
    }
}

using buft_list_t = std::vector<std::pair<ggml_backend_dev_t, ggml_backend_buffer_type_t>>;
static buft_list_t make_cpu_buft_list(const std::vector<ggml_backend_dev_t> & devices) {
    buft_list_t buft_list;

    // add ACCEL buffer types
    for (size_t i = 0; i < ggml_backend_dev_count(); ++i) {
        ggml_backend_dev_t dev = ggml_backend_dev_get(i);
        if (ggml_backend_dev_type(dev) == GGML_BACKEND_DEVICE_TYPE_ACCEL) {
            auto * buft = ggml_backend_dev_buffer_type(dev);
            // skip
            if (buft != ggml_backend_cpu_buffer_type()) {
                buft_list.emplace_back(dev, buft);
            }
        }
    }

    // add a host buffer type
    // storing the tensors in a host buffer is useful when the processing of large batches
    // is offloaded to a GPU device, since it reduces the time spent on data transfers
    // generally, this will be done using the first device in the list
    // a better approach would be to handle this on a weight-by-weight basis using the offload_op
    // function of the device to determine if it would benefit from being stored in a host buffer
    for (auto * dev : devices) {
        ggml_backend_buffer_type_t buft = ggml_backend_dev_host_buffer_type(dev);
        if (buft) {
            buft_list.emplace_back(dev, buft);
            break;
        }
    }

    // add extra buffer types, only if no GPU device is present
    // ref: https://github.com/ggml-org/llama.cpp/issues/12481#issuecomment-2743136094
    auto * cpu_dev = ggml_backend_dev_by_type(GGML_BACKEND_DEVICE_TYPE_CPU);
    auto * cpu_reg = ggml_backend_dev_backend_reg(cpu_dev);
    auto ggml_backend_dev_get_extra_bufts_fn = (ggml_backend_dev_get_extra_bufts_t)
        ggml_backend_reg_get_proc_address(cpu_reg, "ggml_backend_dev_get_extra_bufts");
    if (ggml_backend_dev_get_extra_bufts_fn) {
        ggml_backend_buffer_type_t * extra_bufts = ggml_backend_dev_get_extra_bufts_fn(cpu_dev);
        while (extra_bufts && *extra_bufts) {
            buft_list.emplace_back(cpu_dev, *extra_bufts);
            ++extra_bufts;
        }
    }

    // add the CPU buffer type
    for (size_t i = 0; i < ggml_backend_dev_count(); ++i) {
        ggml_backend_dev_t dev = ggml_backend_dev_get(i);
        if (ggml_backend_dev_type(dev) == GGML_BACKEND_DEVICE_TYPE_CPU) {
            buft_list.emplace_back(dev, ggml_backend_dev_buffer_type(dev));
        }
    }

    return buft_list;
}

template <int64_t INTER_SIZE, ggml_type PARAM_TYPE>
void ggml_quantize_mat_t(const float * GGML_RESTRICT x, void * GGML_RESTRICT vy, int64_t nrow, int64_t n_per_row);

int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A  = (int8_t*)_mm_malloc(M * K * sizeof(int8_t), MEM_ALIGN);
    uint8_t* B = (uint8_t*)_mm_malloc(K / 2 * N * sizeof(uint8_t), MEM_ALIGN);
    float* sA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    // float* scaledSumA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    // float* MinB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C_target = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    
    // Buffer for Quantized Activation 
    // We do not calulate precise volume and use float size to ensure sufficient memory.
    void* A_q = _mm_malloc(M * K * sizeof(float), MEM_ALIGN); // buffer to hold quantized A
    void* W_q = _mm_malloc(N * K * sizeof(float), MEM_ALIGN); // buffer to hold quantized A

    /*
    Stage 1: Use GGML api to perform quantizatoin on fp32 matrix
    */
    const ggml_type weight_q_type = GGML_TYPE_Q4_0;
    const ggml_type activation_q_type = GGML_TYPE_Q8_0;

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
    
    // repack activation based on aarch64 backend
    // repack activation: q8_0 into q8_0x4
    assert (M % 4 == 0);
    const size_t bytes_per_row  = ggml_row_size(GGML_TYPE_Q8_0, K);
    for (int64_t i = 0; i < M; i+=4)
    {
        ggml_quantize_mat_t<8, GGML_TYPE_Q8_0>(
            (float *) ((float *)activation->data + i * K), (void *) (static_cast<char *>(A_q) + i * bytes_per_row), 4, K
        );
    }
    
    
    // print_q8_0_data((block_q8_0*)q8_activation->data, 2);
    printf("\e[32m[INFO]\e[m Copy quantized tensor to buffers for comparison between gemm results from various implementations\n");
    block_q8_0 * q8_blocks = (block_q8_0 *)q8_activation->data;
    int32_t num_q8_blocks = q8_activation->ne[0] * q8_activation->ne[1] / QK8_0;
    for (int block_idx = 0; block_idx < num_q8_blocks; ++block_idx) {
        block_q8_0 blk = q8_blocks[block_idx];
        float scale = GGML_FP16_TO_FP32(blk.d);
        sA[block_idx] = scale;
        memcpy(A + block_idx * QK8_0, blk.qs, QK8_0 * sizeof(int8_t));
    }
    block_q4_0 * q4_blocks = (block_q4_0 *)q4_weight->data;
    int32_t num_q4_blocks = q4_weight->ne[0] * q4_weight->ne[1] / QK4_0;
    for (int block_idx = 0; block_idx < num_q4_blocks; ++block_idx) {
        block_q4_0 blk = q4_blocks[block_idx];
        float scale = GGML_FP16_TO_FP32(blk.d);
        sB[block_idx] = scale;
        memcpy(B + block_idx * QK4_0 / 2, blk.qs, QK4_0 / 2 * sizeof(uint8_t));
    }
    


    /*
    S2: Backend Computation.
    */
    printf("\e[32m[INFO]\e[m Allocate backend\n");
    std::vector<ggml_backend_dev_t> devices;
    buft_list_t buft_list = make_cpu_buft_list(devices);
    ggml_backend_buffer_type_t aarch64_buft = buft_list[0].second;

    // 1. Initialize backend
    ggml_backend_t backend = NULL;

    // if there aren't GPU Backends fallback to CPU backend
    if (!backend) {
        backend = ggml_backend_cpu_init();
    }

    // Calculate the size needed to allocate
    size_t ctx_size_backend = 0;
    ctx_size_backend += 2 * ggml_tensor_overhead(); // tensors
    // no need to allocate anything else!

    // 2. Allocate `ggml_context` to store tensor data
    struct ggml_init_params params_backend = {
        /*.mem_size   =*/ ctx_size_backend,
        /*.mem_buffer =*/ NULL,
        /*.no_alloc   =*/ true, // the tensors will be allocated later by ggml_backend_alloc_ctx_tensors()
    };
    struct ggml_context * ctx_backend = ggml_init(params_backend);

    // Create tensors metadata (only there shapes and data type)
    struct ggml_tensor * weight_int4 = ggml_new_tensor_2d(ctx_backend, weight_q_type, K, N);

    // 4. Allocate a `ggml_backend_buffer` to store all tensors
    // ggml_backend_buffer_t buffer = ggml_backend_alloc_ctx_tensors(ctx_backend, backend);
    ggml_backend_buffer_t buf = ggml_backend_alloc_ctx_tensors_from_buft(ctx_backend, aarch64_buft);
    if (buf == nullptr) {
        throw std::runtime_error(std::string("unable to allocate ") + ggml_backend_buft_name(aarch64_buft) + " buffer");
    }

    // 5. Copy tensor data from main memory (RAM) to backend buffer
    // NOTE: Weights are repacked in this instruction
    ggml_backend_tensor_set(weight_int4, q4_weight->data, 0, ggml_nbytes(weight_int4));
    // ggml_backend_tensor_set(activation_fp32, activation->data, 0, ggml_nbytes(activation_fp32));
    memcpy(W_q, weight_int4->data, ggml_nbytes(weight_int4));

    // 6. Create a `ggml_cgraph` for mul_mat operation
    struct ggml_cgraph * gf = NULL;
    struct ggml_context * ctx_cgraph = NULL;
    {
        // create a temporally context to build the graph
        struct ggml_init_params params0 = {
            /*.mem_size   =*/ ggml_tensor_overhead()*GGML_DEFAULT_GRAPH_SIZE + ggml_graph_overhead(),
            /*.mem_buffer =*/ NULL,
            /*.no_alloc   =*/ true, // the tensors will be allocated later by ggml_gallocr_alloc_graph()
        };
        ctx_cgraph = ggml_init(params0);
        gf = ggml_new_graph(ctx_cgraph);

        // NOTE: 
        // 1. weight_int4 is to be repacked
        // 2. weight_int4 and activation are in different backends.
        struct ggml_tensor * result0 = ggml_mul_mat(ctx_cgraph, weight_int4, activation);

        // Add "result" tensor and all of its dependencies to the cgraph
        ggml_build_forward_expand(gf, result0);
    }
    // 7. Create a `ggml_gallocr` for cgraph computation
    ggml_gallocr_t allocr = ggml_gallocr_new(ggml_backend_get_default_buffer_type(backend));
    ggml_gallocr_alloc_graph(allocr, gf);

    // (we skip step 8. Optionally: schedule the cgraph using `ggml_backend_sched`)

    // 9. Run the computation
    int n_threads = NTHREADS; // Optional: number of threads to perform some operations with multi-threading
    if (ggml_backend_is_cpu(backend)) {
        ggml_backend_cpu_set_n_threads(backend, n_threads);
    }

#ifdef TEST
    matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    // warm up
    printf("\n\e[32m[INFO]\e[m M=%d, K=%d, N=%d nthreasds=%d\n",  M, K, N, NTHREADS);
    printf("\e[32m[INFO]\e[m re-implementation on repacked A80W40 \n");
    
    ggml_backend_graph_compute(backend, gf);
    struct ggml_tensor * result_node = ggml_graph_node(gf, -1);
    // 10. Retrieve results (output tensors)
    // in this example, output tensor is always the last tensor in the graph
    float * result_using_ggml_backend = (float *) malloc(ggml_nbytes(result_node));
    // because the tensor data is stored in device buffer, we need to copy it back to RAM
    ggml_backend_tensor_get(result_node, result_using_ggml_backend, 0, ggml_nbytes(result_node));
    
    for (int i = 0; i < NITER; i++) {
        init_const(C_target, 0, M, N);
        uint64_t start = timer();
        matmul_kernel(
            K,
            C_target,
            N,
            (const char *)W_q,
            (const char *)A_q,
            M,
            N
        );
        // ggml_backend_graph_compute(backend, gf);
        uint64_t end = timer();
        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        print_mat(result_using_ggml_backend, 5);
        print_mat(C_ref, 5);
        print_mat(C_target, 5);
#ifdef TEST
        int res = compare_mats(C_target, C_ref, (long)M * (long)N);
        // if (!res) break;
#endif
        printf("\n");
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C_target);
    _mm_free(C_ref);
    Profiler::getInstance().report_internal();
    return 0;
}
