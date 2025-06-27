#include <assert.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#define MR 2
#define NR 16
#define MC MR * 11
#define NC NR * 25
#define KC 1024

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


static int8_t blockA_packed[MC * KC] __attribute__((aligned(64)));
static float sA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static uint8_t blockB_packed[KC / 2 * NC] __attribute__((aligned(64)));
static float sB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{

    // Process 4 rows and 4 columns at a time
    PRAGMA_OMP_PARALLEL_FOR
    for (int i = 0; i < M; i+=4) 
    for (int j = 0; j < N; j+=4)
    {
        // Accumulators for all combinations of i and j
        __m256 sum_packed_fp[4][4];
        // float sum_a_fused[4][4];
        // float min_b[4][4];
        for (int ii = 0; ii < 4; ++ii) {
            for (int jj = 0; jj < 4; ++jj) {
                sum_packed_fp[ii][jj] = _mm256_setzero_ps();
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
                // b_pack_int8_low[jj] = _mm256_and_si256(b_pack_int8[jj], mask) - 8;
                // b_pack_int8_high[jj] = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8[jj], 4), mask) - 8;

                // b_pack_int8_low[jj] = _mm256_and_si256(b_pack_int8[jj], mask);
                // b_pack_int8_high[jj] = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8[jj], 4), mask);
                // b_pack_int8_low[jj] = _mm256_sub_epi8(b_pack_int8_low[jj], _mm256_set1_epi8(8));
                // b_pack_int8_high[jj] = _mm256_sub_epi8(b_pack_int8_high[jj], _mm256_set1_epi8(8));
                
                b_pack_int8_low[jj] = _mm256_sub_epi8(
                    _mm256_and_si256(b_pack_int8[jj], mask),
                    _mm256_set1_epi8(8)
                );
                b_pack_int8_high[jj] = _mm256_sub_epi8(
                    _mm256_and_si256(_mm256_srli_epi16(b_pack_int8[jj], 4), mask),
                    _mm256_set1_epi8(8)
                );

            }

            // Compute all active i/j combinations
            for (int ii = 0; ii < 4; ++ii) {

                for (int jj = 0; jj < 4; ++jj) {

                    float fused_s[2] = {
                        sA[ii][0] * sB[jj][0],
                        sA[ii][1] * sB[jj][1]
                    };

                    const __m256i ax_0 = _mm256_sign_epi8(a_pack_int8 [ ii ] [ 0 ], a_pack_int8 [ ii ] [ 0 ]);
                    const __m256i ax_1 = _mm256_sign_epi8(a_pack_int8 [ ii ] [ 1 ], a_pack_int8 [ ii ] [ 1 ]);
                    const __m256i sy_0 = _mm256_sign_epi8(b_pack_int8_low  [ jj ] ,a_pack_int8 [ ii ] [ 0 ]);
                    const __m256i sy_1 = _mm256_sign_epi8(b_pack_int8_high  [ jj ],a_pack_int8 [ ii ] [ 1 ]);
                    __m256 tmp[2] = {
                        mul_sum_us8_pairs_float(ax_0, sy_0),
                        mul_sum_us8_pairs_float(ax_1, sy_1)
                    };

                    sum_packed_fp[ii][jj] = _mm256_fmadd_ps(tmp[0], _mm256_set1_ps(fused_s[0]), sum_packed_fp[ii][jj]);
                    sum_packed_fp[ii][jj] = _mm256_fmadd_ps(tmp[1], _mm256_set1_ps(fused_s[1]), sum_packed_fp[ii][jj]);
                }
            }
        }

        // Store results for all active i/j combinations
        for (int ii = 0; ii < 4; ++ii) {

            for (int jj = 0; jj < 4; ++jj) {

                *C(i+ii, j+jj, NDIM) += hsum_float_8(sum_packed_fp[ii][jj]);
            }
        }

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
    float* sA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    init_rand_int8(A, M, K);
    init_rand_uint8(B, K / 2, N);
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    
    // init packed to debug
    // init_int_const(blockA_packed, -1, MC, KC);
    // init_int_const(blockB_packed, -1, KC, NC /2);
    // init_const(sA_packed, -1.0, MC,  KC / Q_BLK_SIZE);
    // init_const(sB_packed, -1.0, KC / Q_BLK_SIZE, NC);

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, sB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("\e[31mGFLOPS\e[m= %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        // print_mat(C, 5, 5);
        // printf("\n\n");
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
