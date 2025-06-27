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
#include <stdint.h>
#include <stdio.h>
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
#define ScaledSumA(i, j, ld) (ScaledSumA + (i) * (ld) + (j))
#define SB(i, j, ld) (SB + (j) * (ld) + (i))
#define MinB(i, j, ld) (MinB + (j) * (ld) + (i))

 

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
        for (int p = 0; p < K; p += Q_BLK_SIZE)
        {
            int block_idx = p / Q_BLK_SIZE;
            float sA_0 = *SA(i, block_idx, K/Q_BLK_SIZE);
            float sB_0 = *SB(block_idx, j, K/Q_BLK_SIZE);
            float fused_s_0 = sA_0 * sB_0;
            
            float min_b_0 = *MinB(block_idx, j, K / Q_BLK_SIZE);
            float sum_a_0_fused = *ScaledSumA(i, block_idx, K / Q_BLK_SIZE);
            
                __m256i a_pack_int8_low = _mm256_loadu_si256( (__m256i *) A(i, p, K));
                __m256i b_pack_int8 = bytes_from_nibbles_32(B(p / 2, j, K / 2));
                __m256 tmp_0 = mul_sum_us8_pairs_float(b_pack_int8, a_pack_int8_low);
                sum_packed_fp = _mm256_fmadd_ps(tmp_0, _mm256_set1_ps(fused_s_0), sum_packed_fp);
                extra_sum_fp += sum_a_0_fused * min_b_0;
        }
        *C(i, j, NDIM) +=  hsum_float_8(sum_packed_fp) + extra_sum_fp;
    }
}
