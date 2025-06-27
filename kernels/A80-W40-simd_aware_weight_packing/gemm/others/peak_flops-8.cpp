#include <iostream>
#include <chrono>
#include <immintrin.h>

int main() {
    const long N = 100000000; // Adjust as needed

    // Initialize vectors
    __m256 a1 = _mm256_set1_ps(1.0f);
    __m256 a2 = _mm256_set1_ps(1.0f);
    __m256 a3 = _mm256_set1_ps(1.0f);
    __m256 a4 = _mm256_set1_ps(1.0f);
    __m256 a5 = _mm256_set1_ps(1.0f);
    __m256 a6 = _mm256_set1_ps(1.0f);
    __m256 a7 = _mm256_set1_ps(1.0f);
    __m256 a8 = _mm256_set1_ps(1.0f);

    __m256 b = _mm256_set1_ps(2.0f);
    __m256 c = _mm256_set1_ps(3.0f);

    auto start = std::chrono::high_resolution_clock::now();

    // Independent FMAs
    for (long i = 0; i < N; ++i) {
        a1 = _mm256_fmadd_ps(b, c, a1);
        a2 = _mm256_fmadd_ps(b, c, a2);
        a3 = _mm256_fmadd_ps(b, c, a3);
        a4 = _mm256_fmadd_ps(b, c, a4);
        a5 = _mm256_fmadd_ps(b, c, a5);
        a6 = _mm256_fmadd_ps(b, c, a6);
        a7 = _mm256_fmadd_ps(b, c, a7);
        a8 = _mm256_fmadd_ps(b, c, a8);
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Store results
    float res1[8], res2[8], res3[8], res4[8], res5[8], res6[8], res7[8], res8[8];
    _mm256_storeu_ps(res1, a1);
    _mm256_storeu_ps(res2, a2);
    _mm256_storeu_ps(res3, a3);
    _mm256_storeu_ps(res4, a4);
    _mm256_storeu_ps(res5, a5);
    _mm256_storeu_ps(res6, a6);
    _mm256_storeu_ps(res7, a7);
    _mm256_storeu_ps(res8, a8);

    // Prevent optimization
    volatile float sink = 0;
    for (int i = 0; i < 8; ++i) {
        sink += res1[i] + res2[i] + res3[i] + res4[i] +
                res5[i] + res6[i] + res7[i] + res8[i];
    }

    // Calculate FLOPS: 2 ops/FMA × 8 accumulators × 8 floats per register × N iterations
    double time = std::chrono::duration<double>(end - start).count();
    double flops = 2.0 * 8 * 8 * N / time;

    std::cout << "Peak FLOPS: " << flops << " (" << flops / 1e9 << " GFLOPS)\n";
    return 0;
}