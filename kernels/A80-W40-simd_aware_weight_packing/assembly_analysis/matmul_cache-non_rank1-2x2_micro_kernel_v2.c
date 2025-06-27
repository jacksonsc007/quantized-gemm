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
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "lib.h"

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#define MR 2
#define NR 2
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
    __m256 sum00 = _mm256_setzero_ps();
    __m256 sum01 = _mm256_setzero_ps();
    __m256 sum10 = _mm256_setzero_ps();
    __m256 sum11 = _mm256_setzero_ps();

    for (int p = 0; p < block_k; p += 2 * Q_BLK_SIZE)
    {
        int block_idx = p / Q_BLK_SIZE;

        float sA_00 = *blockSA(0, block_idx, block_k/Q_BLK_SIZE);
        float sA_01 = *blockSA(0, block_idx+1, block_k/Q_BLK_SIZE);
        float sA_10 = *blockSA(1, block_idx, block_k/Q_BLK_SIZE);
        float sA_11 = *blockSA(1, block_idx+1, block_k/Q_BLK_SIZE);

        float sB_00 = *blockSB(block_idx, 0, block_k/Q_BLK_SIZE);
        float sB_01 = *blockSB(block_idx, 1, block_k/Q_BLK_SIZE);
        float sB_10 = *blockSB(block_idx+1, 0, block_k/Q_BLK_SIZE);
        float sB_11 = *blockSB(block_idx+1, 1, block_k/Q_BLK_SIZE);

        __m256i a00 = _mm256_loadu_si256((__m256i *) blockA(0, p, block_k));
        __m256i a01 = _mm256_loadu_si256((__m256i *) blockA(0, p + Q_BLK_SIZE, block_k));
        __m256i a10 = _mm256_loadu_si256((__m256i *) blockA(1, p, block_k));
        __m256i a11 = _mm256_loadu_si256((__m256i *) blockA(1, p + Q_BLK_SIZE, block_k));

        __m256i b_pack_int8_0 = _mm256_loadu_si256((__m256i *) blockB(p/2, 0, block_k/2)); // same for both j=0,1
        __m256i mask = _mm256_set1_epi8(0x0f);
        __m256i b00 = _mm256_and_si256(b_pack_int8_0, mask);
        __m256i b10 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_0, 4), mask);
        b00 = _mm256_sub_epi8(b00, _mm256_set1_epi8(8));
        b10 = _mm256_sub_epi8(b10, _mm256_set1_epi8(8));
        __m256i b_pack_int8_1 = _mm256_loadu_si256((__m256i *) blockB(p/2, 1, block_k/2)); // same for both j=0,1
        __m256i b01 = _mm256_and_si256(b_pack_int8_1, mask);
        __m256i b11 = _mm256_and_si256(_mm256_srli_epi16(b_pack_int8_1, 4), mask);
        b01 = _mm256_sub_epi8(b01, _mm256_set1_epi8(8));
        b11 = _mm256_sub_epi8(b11, _mm256_set1_epi8(8));

        // i = 0, j = 0
        __m256 t00 = mul_sum_i8_pairs_float(a00, b00);
        __m256 t01 = mul_sum_i8_pairs_float(a01, b10);
        sum00 = _mm256_fmadd_ps(t00, _mm256_set1_ps(sA_00 * sB_00), sum00);
        sum00 = _mm256_fmadd_ps(t01, _mm256_set1_ps(sA_01 * sB_10), sum00);

        // i = 0, j = 1
        __m256 t10 = mul_sum_i8_pairs_float(a00, b01);  // same a00
        __m256 t11 = mul_sum_i8_pairs_float(a01, b11);
        sum01 = _mm256_fmadd_ps(t10, _mm256_set1_ps(sA_00 * sB_01), sum01);
        sum01 = _mm256_fmadd_ps(t11, _mm256_set1_ps(sA_01 * sB_11), sum01);

        // i = 1, j = 0
        __m256 t20 = mul_sum_i8_pairs_float(a10, b00);
        __m256 t21 = mul_sum_i8_pairs_float(a11, b10);
        sum10 = _mm256_fmadd_ps(t20, _mm256_set1_ps(sA_10 * sB_00), sum10);
        sum10 = _mm256_fmadd_ps(t21, _mm256_set1_ps(sA_11 * sB_10), sum10);

        // i = 1, j = 1
        __m256 t30 = mul_sum_i8_pairs_float(a10, b01);
        __m256 t31 = mul_sum_i8_pairs_float(a11, b11);
        sum11 = _mm256_fmadd_ps(t30, _mm256_set1_ps(sA_10 * sB_01), sum11);
        sum11 = _mm256_fmadd_ps(t31, _mm256_set1_ps(sA_11 * sB_11), sum11);
    }

    *C(0, 0, NDIM) += hsum_float_8(sum00);
    *C(0, 1, NDIM) += hsum_float_8(sum01);
    *C(1, 0, NDIM) += hsum_float_8(sum10);
    *C(1, 1, NDIM) += hsum_float_8(sum11);
}