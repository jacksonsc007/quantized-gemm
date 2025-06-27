#pragma once
#include <immintrin.h>
#include <stdint.h>

#define MM256_SET_M128I(b, a) _mm256_insertf128_si256(_mm256_castsi128_si256(b), (a), 1)
static inline __m256i bytes_from_nibbles_32(const uint8_t * rsi)
{
    const __m128i tmp = _mm_loadu_si128((const __m128i *)rsi);
    const __m256i bytes = MM256_SET_M128I(_mm_srli_epi16(tmp, 4), tmp);
    const __m256i lowMask = _mm256_set1_epi8( 0xF );
    return _mm256_and_si256(lowMask, bytes);
}

// add int16_t pairwise and return as float vector
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

static inline float hsum_float_8(const __m256 x) {
    __m128 res = _mm256_extractf128_ps(x, 1);
    res = _mm_add_ps(res, _mm256_castps256_ps128(x));
    res = _mm_add_ps(res, _mm_movehl_ps(res, res));
    res = _mm_add_ss(res, _mm_movehdup_ps(res));
    return _mm_cvtss_f32(res);
}
static inline float sum_uint8_32(const __m256i x) {
    // Create two 128-bit lanes for the lower and upper halves
    __m128i low = _mm256_castsi256_si128(x);
    __m128i high = _mm256_extracti128_si256(x, 1);
    
    // Sum the lanes horizontally
    // First step: sum 8-bit values to 16-bit values
    __m128i sum_low = _mm_sad_epu8(low, _mm_setzero_si128());
    __m128i sum_high = _mm_sad_epu8(high, _mm_setzero_si128());
    
    // Combine the two 128-bit lanes
    __m128i sum = _mm_add_epi32(sum_low, sum_high);
    
    // Horizontal sum of the remaining 32-bit values
    sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(1, 0, 3, 2)));
    sum = _mm_add_epi32(sum, _mm_shuffle_epi32(sum, _MM_SHUFFLE(0, 1, 2, 3)));
    
    // Extract the final sum as a 32-bit integer
    int32_t total = _mm_extract_epi32(sum, 0);
    
    // Convert to float and return
    return (float)total;
}
