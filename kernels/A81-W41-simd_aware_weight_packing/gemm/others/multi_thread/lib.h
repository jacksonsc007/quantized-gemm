#pragma once
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>


#define Q_BLK_SIZE 32
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

/*
A: row-major
sA: row-major
*/
void compute_scaled_sum(const int8_t* A, const float* sA, float * scaledSumA, const int M, const int K)
{
    int32_t num_q_blocks = (M * K) / Q_BLK_SIZE;
    printf("\e[32m[INFO]\e[m number q blocks in A: %d\n", num_q_blocks);
    for (int32_t block_idx = 0; block_idx < num_q_blocks; block_idx++)
    {
        float s = sA[block_idx];
        int32_t sum = 0;
        for (int32_t i = 0; i < Q_BLK_SIZE; i++)
        {
            sum += (int32_t)(*(A + block_idx * Q_BLK_SIZE + i));
        }
        float scaledsum = (float) sum * s;
        *scaledSumA++ = scaledsum;
    }
}

void print_mat(float* mat, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", mat[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void init_rand(float* mat, const int M, const int N) {
    for (int i = 0; i < M * N; i++) {
        *mat++ = rand() / (float)RAND_MAX;
    }
}

void init_rand_uint8(uint8_t* mat, const int M, const int N) {

    for (int i = 0; i < M * N; i++) {
        *mat++ = (uint8_t)(rand() % 256);
    }
}
void init_rand_int8(int8_t* mat, const int M, const int N) {

    for (int i = 0; i < M * N; i++) {
        *mat++ = (int8_t)(rand() % 256 - 128);
    }
}

void init_rand_min_max(int8_t* mat, const int M, const int N, const int min, const int max) {
    for (int i = 0; i < M * N; i++) {
        *mat++ = (int8_t)(rand() % (max - min + 1) + min);
    }
}
void init_uint_const(uint8_t* mat, const uint8_t value, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            *mat++ = value;
        }
    }
}
void init_const(float* mat, const float value, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            *mat++ = value;
        }
    }
}

int compare_mats(float* mat1, float* mat2, const int M, const int N) {
    int num = 1200;
    float acc_err = 0.;
    // for (int i = 0; i < M; i++) 
    for (int i = M -1; i >= 0; i--) 
    {
        // for (int j = 0; j < N; j++) 
        for (int j = N - 1; j >= 0; j--) 
        {
            float err = fabsf(mat1[i * N + j] - mat2[i * N + j]);
            acc_err += err;
            if ( err > 1e-3) 
            {
                // if (num-- > 0)
                    // printf("MISMATCH! Element[%d][%d] %f != %f\n",
                    //        i,
                    //        j,
                    //        mat1[i * N + j],
                    //        mat2[i * N + j]);
            }
        }
    }
    float avg_err = acc_err / (M * N);
    printf("acc error: % f average error: %f\n", acc_err, avg_err);
    if (acc_err > 1e-3) return 0;
    // printf("MATCH!\n");
    return 1;
}
uint64_t timer() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    return (uint64_t)start.tv_sec * 1000000000 + (uint64_t)start.tv_nsec;
}
