#include <iostream>
#include <chrono>
#include <immintrin.h>

int main() {
    const long N = 100000000; // Adjust based on your CPU's speed

    // Initialize vectors with constants (avoids memory bottlenecks)
    __m256 a1 = _mm256_set1_ps(1.0f);
    __m256 a2 = _mm256_set1_ps(1.0f);
    __m256 a3 = _mm256_set1_ps(1.0f);
    __m256 a4 = _mm256_set1_ps(1.0f);
    __m256 b = _mm256_set1_ps(2.0f);
    __m256 c = _mm256_set1_ps(3.0f);

    auto start = std::chrono::high_resolution_clock::now();

    // Independent FMAs to maximize instruction-level parallelism
    for (long i = 0; i < N; ++i) {
        a1 = _mm256_fmadd_ps(b, c, a1);
        a2 = _mm256_fmadd_ps(b, c, a2);
        a3 = _mm256_fmadd_ps(b, c, a3);
        a4 = _mm256_fmadd_ps(b, c, a4);
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Use results to prevent optimization
    float res1[8], res2[8], res3[8], res4[8];
    _mm256_storeu_ps(res1, a1);
    _mm256_storeu_ps(res2, a2);
    _mm256_storeu_ps(res3, a3);
    _mm256_storeu_ps(res4, a4);
    // volatile float sink = res1[0] + res2[0]; // Prevent compiler from optimizing away
    volatile float sink = res1[0] + res2[0] + res1[1] + res2[1] + res1[2] + res2[2] + res1[3] + res2[3] + res1[4] + res2[4] + res1[5] + res2[5] + res1[6] + res2[6] + res1[7] + res2[7] +
                        res3[0] + res4[0] + res3[1] + res4[1] + res3[2] + res4[2] + res3[3] + res4[3] + res3[4] + res4[4] + res3[5] + res4[5] + res3[6] + res4[6] + res3[7] + res4[7];

    // Calculate FLOPS
    double time = std::chrono::duration<double>(end - start).count();
    double flops = 2 * 4 * 8 * N / time; // 2 ops/FMA, 2 registers, 8 elements/register

    std::cout << "Peak FLOPS: " << flops << " (" << flops / 1e9 << " GFLOPS)\n";
    return 0;
}