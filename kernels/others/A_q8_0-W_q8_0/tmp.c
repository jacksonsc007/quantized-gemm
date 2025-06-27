/*
Both activation and weight are quantized into int8.
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64
#define Q_BLK_SIZE 32
#define MR 2
#define NR 16
#define MC MR * 11
#define NC NR * 25
#define KC 256

#ifndef MDIM
    #define MDIM MC*2
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM NC*2
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

// row-major blocked scaling factor for weight
// column-major blocked scaling factor for activation
#define blockSA(i, j, ld) (blockSA + (j) * (ld) + (i))
#define blockSB(i, j, ld) (blockSB + (i) * (ld) + (j))

// column-major.
#define blockA(i, j, ld) ( blockA + (j) * (ld) + (i) )
// row-major
#define blockB(i, j, ld) ( blockB + (i) * (ld) + (j) )

void kernel_2x16(
    const int8_t * blockA, const int8_t * blockB,
    const float * blockSA, const float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    // quantized value
    __m256i a0,a1;
    __m256i b0; // hold 16 int8 values from B
    __m256i c0_int, c0_int_low, c0_int_high;
    __m256i c1_int, c1_int_low, c1_int_high;

    // scaling factors
    __m256 sa0, sa1;
    __m256 sb0, sb1; // scaling factors for B
    __m256 fused_s00, fused_s01; // fused scaling factors
    __m256 fused_s10, fused_s11; // fused scaling factors
    // Use separate registers instead of an array
    __m256 c0_0, c0_1, c1_0, c1_1;

    // Initialize with values from memory
    c0_0 = _mm256_loadu_ps(C + 0 * NDIM);
    c0_1 = _mm256_loadu_ps(C + 0 * NDIM + 8);
    c1_0 = _mm256_loadu_ps(C + 1 * NDIM);
    c1_1 = _mm256_loadu_ps(C + 1 * NDIM + 8);
    
    // Store back to memory
    for (int p = 0; p < block_k; p += Q_BLK_SIZE) 
    {
        sb0 = _mm256_loadu_ps(blockSB(p / Q_BLK_SIZE, 0, NR));
        sb1 = _mm256_loadu_ps(blockSB(p / Q_BLK_SIZE, 8, NR));
        sa0 = _mm256_broadcast_ss(blockSA(0, p / Q_BLK_SIZE, MR));
        fused_s00 = _mm256_mul_ps(sa0, sb0);
        fused_s01 = _mm256_mul_ps(sa0, sb1);
        sa1 = _mm256_broadcast_ss(blockSA(1, p / Q_BLK_SIZE, MR));
        fused_s10 = _mm256_mul_ps(sa1, sb0);
        fused_s11 = _mm256_mul_ps(sa1, sb1);
        for (int pq = 0; pq < Q_BLK_SIZE; pq++)
        {
            b0 = _mm256_cvtepi8_epi16(_mm_loadu_si128((__m128i *) blockB(p+pq, 0, NR)));
            // // 1st row of A
            a0 = _mm256_set1_epi16((int16_t) *blockA(0, p+pq, MR));
            c0_int = _mm256_mullo_epi16(a0, b0);
            // c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            c0_int_low = _mm256_cvtepi16_epi32(_mm256_castsi256_si128(c0_int));
            c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            c0_0 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s00, c0_0);
            c0_1 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s01, c0_1);
            // c00 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c00);
            // c01 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c01);

            // 2nd row of A
            a1 = _mm256_set1_epi16((int16_t) *blockA(1, p+pq, MR));
            c1_int = _mm256_mullo_epi16(a1, b0);
            c1_int_low = _mm256_cvtepi16_epi32(_mm256_castsi256_si128(c1_int));
            c1_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c1_int, 1));
            c1_0 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c1_int_low), fused_s10, c1_0);
            c1_1 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c1_int_high), fused_s11, c1_1);
        }
    }

    _mm256_storeu_ps(C + 0 * NDIM, c0_0);
    _mm256_storeu_ps(C + 0 * NDIM + 8, c0_1);
    _mm256_storeu_ps(C + 1 * NDIM, c1_0);
    _mm256_storeu_ps(C + 1 * NDIM + 8, c1_1);
}