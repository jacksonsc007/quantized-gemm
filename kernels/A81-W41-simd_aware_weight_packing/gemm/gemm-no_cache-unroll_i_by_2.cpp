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


static int8_t blockA_packed[MC * KC] __attribute__((aligned(64)));
static float sA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static float scaledSumA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static uint8_t blockB_packed[KC / 2 * NC] __attribute__((aligned(64)));
static float sB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static float minB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));

 

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{
    for (int i = 0; i < M - 1; i += 2)
    for (int j = 0; j < N; j++)
    {
        __m256 sum_packed_fp_0 = _mm256_setzero_ps();
        __m256 sum_packed_fp_1 = _mm256_setzero_ps();
        float extra_sum_fp_0 = 0;
        float extra_sum_fp_1 = 0;
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0_0 = *SA(i, block_idx, K/Q_BLK_SIZE);
            float sA_0_1 = *SA(i, block_idx + 1, K/Q_BLK_SIZE);
            float sA_1_0 = *SA(i + 1, block_idx, K/Q_BLK_SIZE);
            float sA_1_1 = *SA(i + 1, block_idx + 1, K/Q_BLK_SIZE);
            float sB_0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float sB_1 = *SB(block_idx + 1, j, K/Q_BLK_SIZE);
            float fused_s_0_0 = sA_0_0 * sB_0;
            float fused_s_0_1 = sA_0_1 * sB_1;
            float fused_s_1_0 = sA_1_0 * sB_0;
            float fused_s_1_1 = sA_1_1 * sB_1;
            
            float min_b_0 = *MinB(block_idx, j, K / Q_BLK_SIZE);
            float min_b_1 = *MinB(block_idx + 1, j, K / Q_BLK_SIZE);
            float sum_a_0_0_fused = *ScaledSumA(i, block_idx, K / Q_BLK_SIZE);
            float sum_a_0_1_fused = *ScaledSumA(i, block_idx + 1, K / Q_BLK_SIZE);
            float sum_a_1_0_fused = *ScaledSumA(i + 1, block_idx, K / Q_BLK_SIZE);
            float sum_a_1_1_fused = *ScaledSumA(i + 1, block_idx + 1, K / Q_BLK_SIZE);
            
            __m256i a_pack_int8_low_0 = _mm256_loadu_si256( (__m256i *) A(i, p, K));
            __m256i a_pack_int8_high_0 = _mm256_loadu_si256( (__m256i *) A(i, p + Q_BLK_SIZE, K));
            __m256i a_pack_int8_low_1 = _mm256_loadu_si256( (__m256i *) A(i + 1, p, K));
            __m256i a_pack_int8_high_1 = _mm256_loadu_si256( (__m256i *) A(i + 1, p + Q_BLK_SIZE, K));
            __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) B(p/2, j, K/2));

            __m256i mask = _mm256_set1_epi8(0x0f);
            __m256i b_pack_int8_low = _mm256_and_si256(b_pack_int8, mask);
            __m256i b_pack_int8_high = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8, 4), mask);

            __m256 tmp_0_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_0);
            __m256 tmp_0_1 = mul_sum_us8_pairs_float( b_pack_int8_high, a_pack_int8_high_0);
            __m256 tmp_1_0 = mul_sum_us8_pairs_float(b_pack_int8_low, a_pack_int8_low_1);
            __m256 tmp_1_1 = mul_sum_us8_pairs_float( b_pack_int8_high, a_pack_int8_high_1);

            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_0, _mm256_set1_ps(fused_s_0_0), sum_packed_fp_0);
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_1, _mm256_set1_ps(fused_s_0_1), sum_packed_fp_0);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_0, _mm256_set1_ps(fused_s_1_0), sum_packed_fp_1);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_1, _mm256_set1_ps(fused_s_1_1), sum_packed_fp_1);

            extra_sum_fp_0 += sum_a_0_0_fused * min_b_0;
            extra_sum_fp_0 += sum_a_0_1_fused * min_b_1;
            extra_sum_fp_1 += sum_a_1_0_fused * min_b_0;
            extra_sum_fp_1 += sum_a_1_1_fused * min_b_1;
        }
        *C(i, j, NDIM) += hsum_float_8(sum_packed_fp_0) + extra_sum_fp_0;
        *C(i + 1, j, NDIM) += hsum_float_8(sum_packed_fp_1) + extra_sum_fp_1;
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

    // init_rand(A, M, K);
    // init_rand(B, K, N);
    //
    // init_rand_int8(A, M, K);
    init_rand_min_max(A, M, K, -128, 127);
    // init_uint_const(A, 100, M, K);
    // NOTE: K / 2
    init_rand_uint8(B, K / 2, N);
    // init_uint_const(B, 1, K / 2, N);

    

    
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(scaledSumA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    init_rand(MinB, K / Q_BLK_SIZE, N);
    // init_const(sA, 10.1, M, K / Q_BLK_SIZE);
    // init_const(sB, 50.5, K / Q_BLK_SIZE, N);
    
    // init packed to debug
    // init_int_const(blockA_packed, -1, MC, KC);
    // init_int_const(blockB_packed, -1, KC /2, NC);
    // init_const(sA_packed, -1.0, MC, KC / Q_BLK_SIZE);
    // init_const(sB_packed, -1.0, KC /Q_BLK_SIZE, NC);    

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, MinB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    printf("\n\e[32m[INFO]\e[m MR=%d, NR=%d, MC=%d, NC=%d, KC=%d, M=%d, K=%d, N=%d\n", MR, NR, MC, NC, KC, M, K, N);
    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, scaledSumA,sB, MinB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
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
