#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#define MR 2
#define NR 16
#define MC MR * 11
#define NC NR * 25
#define KC 1024

#ifndef MDIM
    #define MDIM 1000
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM 1000
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

void matmul_kernel_naive(
    int8_t * blockA, uint8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(block_k % Q_BLK_SIZE == 0);
    for (int i = 0; i < block_m; ++i) 
    for (int j = 0; j < block_n; ++j)
    {
        float sum = 0;
        for (int p = 0; p < block_k; p += Q_BLK_SIZE * 2)
        {
            // SA: row-major
            // SB: column-major
            int block_idx = p / Q_BLK_SIZE;
            float s_a_0 = *blockSA(i, block_idx, block_k / Q_BLK_SIZE);
            float s_b_0 = *blockSB(block_idx, j, block_k / Q_BLK_SIZE);
            float s_a_1 = *blockSA(i, block_idx + 1, block_k / Q_BLK_SIZE);
            float s_b_1 = *blockSB(block_idx + 1, j, block_k / Q_BLK_SIZE);
            for (int q = 0; q < Q_BLK_SIZE; q++)
            {
                int8_t a_low = *blockA(i, p + q, block_k);
                int8_t a_high = *blockA(i, p + q + Q_BLK_SIZE, block_k); 
                uint8_t b = *blockB(p / 2 + q, j, block_k / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f) - 8;
                int8_t b_high = (b >> 4) - 8;
                // if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                // {
                //     printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                // }
                sum += (float)a_low * (float)b_low * s_a_0 * s_b_0;
                sum += (float)a_high * (float)b_high * s_a_1 * s_b_1;
            }
        }    
        *C(i, j, NDIM) += sum;
    }
}


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
    for (int i = 0; i < block_m; ++i)
    for (int j = 0; j < block_n; ++j)
    {
        __m256 sum_packed_fp = _mm256_setzero_ps();
        for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0 = *blockSA(i, block_idx, block_k/Q_BLK_SIZE);
            float sB_0 = *blockSB(block_idx, j, block_k/Q_BLK_SIZE);
            float sA_1 = *blockSA(i, block_idx + 1, block_k/Q_BLK_SIZE);
            float sB_1 = *blockSB(block_idx + 1, j, block_k/Q_BLK_SIZE);
            float fused_s_0 = sA_0 * sB_0;
            float fused_s_1 = sA_1 * sB_1;
            // volatile float fused_tmp_0 = sA_0 * sB_1;
            // volatile float fused_tmp_1 = sA_1 * sB_0;
            


            // Q_BLK_SIZE = 32
            // 32 * 8 = 256
            // so ymm only need to load once
                __m256i a_pack_int8_low = _mm256_loadu_si256( (__m256i *) blockA(i, p, block_k));
                // __m256i a_pack_int8_high = _mm256_loadu_si256( (__m256i *) blockA(i, p + Q_BLK_SIZE, block_k));
                __m256i a_pack_int8_high = _mm256_loadu_si256( (__m256i *) blockA(i, p + Q_BLK_SIZE, block_k));


                // __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) blockB(p, j, block_k));
                __m256i b_pack_int8 = _mm256_loadu_si256( (__m256i *) blockB(p/2, j, block_k/2));
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


// ======================================================================================================================================================
// pack matrix A
// block size: mc * kc
// panel size: MR * kc
// Both panel and blockA are row-major, no need to pack panel-wise
// ======================================================================================================================================================
void pack_blockA(int8_t* A, int8_t* blockA, const int mc, const int kc)
{
    for (int i = 0; i < mc; i++)
    {
        for( int j = 0; j < kc; j++)
        {
            *blockA(i, j, kc) = *A(i, j, KDIM);
        }
    }
} 

// ======================================================================================================================================================
// pack matrix A scaling factors
// block size: mc * kc (kc is normalized by q_blk_size) 
// panel size: MR * kc 
// ======================================================================================================================================================
void pack_blockSA(float* A, float* blockA, int mc, int kc)
{
    for (int i = 0; i < mc; i++)
    for (int j = 0; j < kc; j++)
    {
        *blockA(i, j, kc) = *A(i, j, KDIM/Q_BLK_SIZE);
    }
} 

// ======================================================================================================================================================
// pack matrix B
// block size: kc * nc
// panel size: kc * NR, row_major
// ======================================================================================================================================================
void pack_blockB(uint8_t* B, uint8_t* blockB, int kc, int nc)
{
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM / 2);
    }
} 

// ======================================================================================================================================================
// pack matrix B scaling factor
// block size: kc * nc (kc here is already normalized by q_blk_size)
// panel size: kc * NR, row_major
// ======================================================================================================================================================

void pack_blockSB(float* B, float* blockB, int kc, int nc)
{
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM/Q_BLK_SIZE);
    }
} 

void matmul_kernel(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{
    assert (KDIM % Q_BLK_SIZE == 0);
    assert (KC % Q_BLK_SIZE == 0);
    for (int jc = 0; jc < N; jc += NC)
    {
        int valid_NC = min(N - jc, NC);
        for (int pc = 0; pc < K; pc += KC)
        {
            int valid_KC = min(K - pc, KC);
            pack_blockB(
                B(pc/2, jc, KDIM / 2), blockB_packed, valid_KC/2, valid_NC
            );
            pack_blockSB(
                SB(pc/Q_BLK_SIZE, jc, KDIM / Q_BLK_SIZE), sB_packed, valid_KC/Q_BLK_SIZE, valid_NC
            );
            for (int ic = 0; ic < M; ic+= MC)
            {
                int valid_MC = min(M - ic, MC);
                pack_blockA(
                    A(ic, pc, KDIM), blockA_packed, valid_MC, valid_KC
                );
                pack_blockSA(
                    SA(ic, pc/Q_BLK_SIZE, KDIM / Q_BLK_SIZE), sA_packed, valid_MC, valid_KC/Q_BLK_SIZE
                );
                for(int jr = 0; jr < valid_NC; jr += NR)
                {
                    int valid_nr = min(valid_NC - jr, NR);
                    for(int ir = 0; ir < valid_MC; ir += MR)
                    {
                        int valid_mr = min(valid_MC - ir, MR);
                        #ifdef naive
                        matmul_kernel_naive(
                            (blockA_packed + ir * valid_KC),
                            (blockB_packed + jr * valid_KC / 2),
                            (sA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (sB_packed + jr * valid_KC / Q_BLK_SIZE),
                            C(ic + ir, jc + jr, N),
                            valid_mr,
                            valid_nr,
                            valid_KC
                        );
                        #else
                        kernel(
                            (blockA_packed + ir * valid_KC),
                            (blockB_packed + jr * valid_KC / 2),
                            (sA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (sB_packed + jr * valid_KC / Q_BLK_SIZE),
                            C(ic + ir, jc + jr, N),
                            valid_mr,
                            valid_nr,
                            valid_KC
                        );
                        #endif
                    }
                }
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

    // init_rand(A, M, K);
    // init_rand(B, K, N);
    //
    init_rand_int8(A, M, K);
    // NOTE: K / 2
    init_rand_uint8(B, K / 2, N);
    

    
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    // init_const(sA, 10.1, M, K / Q_BLK_SIZE);
    // init_const(sB, 50.5, K / Q_BLK_SIZE, N);
    
    // init packed to debug
    init_int_const(blockA_packed, -1, MC, KC);
    init_int_const(blockB_packed, -1, KC, NC /2);
    init_const(sA_packed, -1.0, MC,  KC / Q_BLK_SIZE);
    init_const(sB_packed, -1.0, KC / Q_BLK_SIZE, NC);

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, sB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("\e[31mGFLOPS\e[m= %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        print_mat(C, 5, 5);
        printf("\n\n");
        print_mat(C_ref, 5, 5);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        if (!res) break;
#endif
        printf("\n");
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);

    return 0;
}
