#include <iostream>
#include <chrono>
#include <immintrin.h>

int main() {
    const long N = 100000000; // Adjust for benchmarking

    // Initialize accumulators
    __m256 a1 = _mm256_set1_ps(1.0f);
    __m256 a2 = _mm256_set1_ps(1.0f);
    __m256 a3 = _mm256_set1_ps(1.0f);
    __m256 a4 = _mm256_set1_ps(1.0f);
    __m256 a5 = _mm256_set1_ps(1.0f);
    __m256 a6 = _mm256_set1_ps(1.0f);
    __m256 a7 = _mm256_set1_ps(1.0f);
    __m256 a8 = _mm256_set1_ps(1.0f);
    __m256 a9 = _mm256_set1_ps(1.0f);
    __m256 a10 = _mm256_set1_ps(1.0f);
    __m256 a11 = _mm256_set1_ps(1.0f);
    __m256 a12 = _mm256_set1_ps(1.0f);
    __m256 a13 = _mm256_set1_ps(1.0f);
    __m256 a14 = _mm256_set1_ps(1.0f);
    __m256 a15 = _mm256_set1_ps(1.0f);
    __m256 a16 = _mm256_set1_ps(1.0f);

    __m256 b = _mm256_set1_ps(2.0f);
    __m256 c = _mm256_set1_ps(3.0f);

    auto start = std::chrono::high_resolution_clock::now();

    // Perform independent FMAs
    for (long i = 0; i < N; ++i) {
        a1  = _mm256_fmadd_ps(b, c, a1);
        a2  = _mm256_fmadd_ps(b, c, a2);
        a3  = _mm256_fmadd_ps(b, c, a3);
        a4  = _mm256_fmadd_ps(b, c, a4);
        a5  = _mm256_fmadd_ps(b, c, a5);
        a6  = _mm256_fmadd_ps(b, c, a6);
        a7  = _mm256_fmadd_ps(b, c, a7);
        a8  = _mm256_fmadd_ps(b, c, a8);
        a9  = _mm256_fmadd_ps(b, c, a9);
        a10 = _mm256_fmadd_ps(b, c, a10);
        a11 = _mm256_fmadd_ps(b, c, a11);
        a12 = _mm256_fmadd_ps(b, c, a12);
        a13 = _mm256_fmadd_ps(b, c, a13);
        a14 = _mm256_fmadd_ps(b, c, a14);
        a15 = _mm256_fmadd_ps(b, c, a15);
        a16 = _mm256_fmadd_ps(b, c, a16);
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Store results
    float res[16][8];
    _mm256_storeu_ps(res[0], a1);
    _mm256_storeu_ps(res[1], a2);
    _mm256_storeu_ps(res[2], a3);
    _mm256_storeu_ps(res[3], a4);
    _mm256_storeu_ps(res[4], a5);
    _mm256_storeu_ps(res[5], a6);
    _mm256_storeu_ps(res[6], a7);
    _mm256_storeu_ps(res[7], a8);
    _mm256_storeu_ps(res[8], a9);
    _mm256_storeu_ps(res[9], a10);
    _mm256_storeu_ps(res[10], a11);
    _mm256_storeu_ps(res[11], a12);
    _mm256_storeu_ps(res[12], a13);
    _mm256_storeu_ps(res[13], a14);
    _mm256_storeu_ps(res[14], a15);
    _mm256_storeu_ps(res[15], a16);

    // Prevent compiler from optimizing out the loop
    volatile float sink = 0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 8; ++j)
            sink += res[i][j];

    // Calculate FLOPS: 2 ops/FMA * 16 accumulators * 8 floats * N iterations
    double time = std::chrono::duration<double>(end - start).count();
    double flops = 2.0 * 16 * 8 * N / time;

    std::cout << "Peak FLOPS: " << flops << " (" << flops / 1e9 << " GFLOPS)\n";
    return 0;
}