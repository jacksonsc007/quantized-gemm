/*
This kernel is designed for 1xK and KxN matrix multiplication.

Serveral changes are made to the original code:
1. No need to pack A
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "stdbool.h"
// #include <cblas.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#ifndef NTHREADS
#define NTHREADS 1
#endif

#define OMP_SCHEDULE dynamic
#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

#ifndef MR
#define MR 1
#endif

#ifndef NR
#define NR 4
#endif

#ifndef MC
#define MC 1
#endif

#ifndef NC
#define NC NR * 25 *  NTHREADS
#endif

#ifndef KC
#define KC 1024
#endif

#ifndef MDIM
    #define MDIM 1
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM 4096
    // #define NDIM NR * 2
#endif

#ifndef KDIM
    #define KDIM 4096
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

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{
    PRAGMA_OMP_PARALLEL_FOR
    for (int j = 0; j < N; j += 2)
    {
        __m256 sum_packed_fp_0 = _mm256_setzero_ps();
        __m256 sum_packed_fp_1 = _mm256_setzero_ps();
        
        for (int p = 0; p < K; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            
            // Load scale factors for j and j+1
            float sA_0 = *SA(0, block_idx, K/Q_BLK_SIZE);
            float sB_0_j0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float sB_0_j1 = *SB(block_idx, j+1, K/Q_BLK_SIZE);
            
            float sA_1 = *SA(0, block_idx + 1, K/Q_BLK_SIZE);
            float sB_1_j0 = *SB(block_idx + 1, j, K/Q_BLK_SIZE);
            float sB_1_j1 = *SB(block_idx + 1, j+1, K/Q_BLK_SIZE);
            
            float fused_s_0_j0 = sA_0 * sB_0_j0;
            float fused_s_1_j0 = sA_1 * sB_1_j0;
            float fused_s_0_j1 = sA_0 * sB_0_j1;
            float fused_s_1_j1 = sA_1 * sB_1_j1;
            
            // Load A packed int8 (same for both j iterations)
            __m256i a_pack_int8_low = _mm256_loadu_si256((__m256i *) A(0, p, K));
            __m256i a_pack_int8_high = _mm256_loadu_si256((__m256i *) A(0, p + Q_BLK_SIZE, K));
            
            // Load B packed int8 for j and j+1
            __m256i b_pack_int8_j0 = _mm256_loadu_si256((__m256i *) B(p/2, j, K/2));
            __m256i b_pack_int8_j1 = _mm256_loadu_si256((__m256i *) B(p/2, j+1, K/2));
            
            __m256i mask = _mm256_set1_epi8(0x0f);
            
            // Process B for j
            __m256i b_pack_int8_low_j0 = _mm256_and_si256(b_pack_int8_j0, mask);
            __m256i b_pack_int8_high_j0 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j0, 4), mask);
            b_pack_int8_low_j0 = _mm256_sub_epi8(b_pack_int8_low_j0, _mm256_set1_epi8(8));
            b_pack_int8_high_j0 = _mm256_sub_epi8(b_pack_int8_high_j0, _mm256_set1_epi8(8));
            
            // Process B for j+1
            __m256i b_pack_int8_low_j1 = _mm256_and_si256(b_pack_int8_j1, mask);
            __m256i b_pack_int8_high_j1 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_j1, 4), mask);
            b_pack_int8_low_j1 = _mm256_sub_epi8(b_pack_int8_low_j1, _mm256_set1_epi8(8));
            b_pack_int8_high_j1 = _mm256_sub_epi8(b_pack_int8_high_j1, _mm256_set1_epi8(8));
            
            // Compute for j
            __m256 tmp_0_j0 = mul_sum_i8_pairs_float(a_pack_int8_low, b_pack_int8_low_j0);
            __m256 tmp_1_j0 = mul_sum_i8_pairs_float(a_pack_int8_high, b_pack_int8_high_j0);
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_0_j0, _mm256_set1_ps(fused_s_0_j0), sum_packed_fp_0);
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp_1_j0, _mm256_set1_ps(fused_s_1_j0), sum_packed_fp_0);
            
            // Compute for j+1
            __m256 tmp_0_j1 = mul_sum_i8_pairs_float(a_pack_int8_low, b_pack_int8_low_j1);
            __m256 tmp_1_j1 = mul_sum_i8_pairs_float(a_pack_int8_high, b_pack_int8_high_j1);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_0_j1, _mm256_set1_ps(fused_s_0_j1), sum_packed_fp_1);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp_1_j1, _mm256_set1_ps(fused_s_1_j1), sum_packed_fp_1);
        }
        
        // Store results
        *C(0, j, NDIM) += hsum_float_8(sum_packed_fp_0);
            *C(0, j+1, NDIM) += hsum_float_8(sum_packed_fp_1);
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
    float* sA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);


    init_rand_int8(A, M, K);
    // NOTE: K / 2
    init_rand_uint8(B, K / 2, N);
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    init_int_const(blockB_packed, -1, KC /2, NC);
    init_const(sB_packed, -1.0, KC /Q_BLK_SIZE, NC);    

    // These matrixes are allocated to override cache lines when benchmarking vector-matrix multiplication
    const int M_ = 1000;
    const int K_ = 1000;
    const int N_ = 1000;
    float* A_ = (float*)_mm_malloc(M_ * K_ * sizeof(float), MEM_ALIGN);
    float* B_ = (float*)_mm_malloc(K_ * N_ * sizeof(float), MEM_ALIGN);
    float* C_ = (float*)_mm_malloc(M_ * N_ * sizeof(float), MEM_ALIGN);
    init_rand(A_, M_, K_);
    init_rand(B_, K_, N_);

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    printf("\n\e[32m[INFO]\e[m MR=%d, NR=%d, MC=%d, NC=%d, KC=%d, M=%d, K=%d, N=%d\n", MR, NR, MC, NC, KC, M, K, N);
    // warm up
    // matmul_kernel(A, B, C, sA, sB, M, N, K);
    // float_matmul_naive(A_, B_, C_, M_, N_, K_);
    for (int i = 0; i < NITER; i++) {
        float_matmul_naive(A_, B_, C_, M_, N_, K_);
        // cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, M_, N_, K_, 1.0, A_, M_, B_, K_, 0.0, C_, M_);
    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, sB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;
        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
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
    _mm_free(sA);
    _mm_free(sB);
    _mm_free(A_);
    _mm_free(B_);
    _mm_free(C_);
    return 0;
}