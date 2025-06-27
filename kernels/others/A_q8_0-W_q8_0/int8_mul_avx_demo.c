/*
Both activation and weight are quantized into int8.
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j, ld) ( A + ( i ) * ( ld ) + ( j ) )
#define B(i, j, ld) ( B + ( j ) * ( ld ) + ( i ) )
#define C(i, j, ld) ( C + ( i ) * ( ld ) + ( j ) )

// row-major shape (block_m, block_k)
#define blockA(i, j, ld) ( blockA + (i) * (ld) + (j) )
// column-major shape (block_k, block_n)
#define blockB(i, j, ld) ( blockB + (j) * (ld) + (i) )

/*
blockA should be column-major order
blockB should be row-major order
*/
void kernel_xx32(int8_t * blockA, int8_t * blockB, int8_t * C, const int block_m, const int block_n, const int block_k) {
    __m256i a0; // 0-th block of 32 int8
    __m256i a0_l; // 0-th block of 32 int8
    __m256i a0_h; // 0-th block of 32 int8
    __m256i res_l; // 0-th block of 32 int8
    __m256i res_h; // 0-th block of 32 int8
    __m256i res; // 0-th block of 32 int8
    __m256i b0; // 0-th row
    __m256i b0_l; // 0-th block of 32 int8
    __m256i b0_h; // 0-th block of 32 int8
    __m256i c0; // 0-th row

    for (int p = 0; p < block_k; ++p)
    {
        a0 = _mm256_loadu_si256( (__m256i *) blockA(p, 0, block_n));
        b0 = _mm256_set1_epi8(* blockB(0, p, block_m));
        // convert to int16 since no int8 multiplication is supported
        a0_l = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(a0));
        b0_l = _mm256_cvtepi8_epi16(_mm256_castsi256_si128(b0));
        a0_h = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(a0, 1));
        b0_h = _mm256_cvtepi8_epi16(_mm256_extracti128_si256(b0, 1));
        
        res_l = _mm256_mullo_epi16(a0_l, b0_l);
        res_h = _mm256_mullo_epi16(a0_h, b0_h);
        
        res = _mm256_packs_epi16(res_l, res_h);
    }
    _mm256_storeu_si256((__m256i *) C(0, 0, block_n), c0);
}
