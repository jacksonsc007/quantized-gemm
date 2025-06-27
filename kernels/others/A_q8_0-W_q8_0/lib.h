#pragma once
#include <immintrin.h>

// multiply int8_t, add results pairwise twice and return as float vector
static inline __m256 sum_i16_pairs_float(const __m256i x) {
    const __m256i ones = _mm256_set1_epi16(1);
    const __m256i summed_pairs = _mm256_madd_epi16(ones, x);
    return _mm256_cvtepi32_ps(summed_pairs);
}

static inline __m256 mul_sum_us8_pairs_float(const __m256i ax, const __m256i sy) {
#if __AVXVNNI__
    const __m256i zero = _mm256_setzero_si256();
    const __m256i summed_pairs = _mm256_dpbusd_epi32(zero, ax, sy);
    return _mm256_cvtepi32_ps(summed_pairs);
#else
    // Perform multiplication and create 16-bit values
    const __m256i dot = _mm256_maddubs_epi16(ax, sy);
    return sum_i16_pairs_float(dot);
#endif
}

static inline __m256 mul_sum_i8_pairs_float(const __m256i x, const __m256i y) {
#if __AVXVNNIINT8__
    const __m256i zero = _mm256_setzero_si256();
    const __m256i summed_pairs = _mm256_dpbssd_epi32(zero, x, y);
    return _mm256_cvtepi32_ps(summed_pairs);
#else
    // Get absolute values of x vectors
    const __m256i ax = _mm256_sign_epi8(x, x);
    // Sign the values of the y vectors
    const __m256i sy = _mm256_sign_epi8(y, x);
    return mul_sum_us8_pairs_float(ax, sy);
#endif
}

