/*
Despite the memory layout and A,B,C are all different, we stick to the cache blocking strategy.
Other conjugate blocking strategy is going to be further discussed in the future version.
*/
#include "lib.h"
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64
#define UN_INIT -10.0

#ifndef NTHREADS
#define NTHREADS 2
#endif


#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM 1
#endif

#ifndef NDIM
    #define NDIM 1024
#endif

#ifndef KDIM
    #define KDIM 1024
#endif

#ifndef NITER
    #define NITER 10
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j, ld) ( A + ( i ) * ld + ( j ) )
#define B(i, j, ld) ( B + ( i ) * ld + ( j ) )
#define C(i, j, ld) ( C + ( i ) * ld + ( j ) )




// openmp setting
#define OMP_SCHEDULE static
#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

void matmul_simd_cache_friendly_v1(float* A, float* B, float* C, const int M, const int N, const int K) {
    for( int p = 0; p < K; p++)
    {
        PRAGMA_OMP_PARALLEL_FOR
        for (int j = 0; j < N; j+=8)
        {
            __m256 acc = _mm256_loadu_ps(C(0, j, N));
            __m256 lhs = _mm256_set1_ps(*A(0, p, K));
            __m256 rhs = _mm256_loadu_ps(B(p, j, N));
            acc = _mm256_fmadd_ps(lhs, rhs, acc);
            _mm256_storeu_ps(C(0, j, N), acc);
        }

    }
}


void kernel_per_thread(float* A, float* B, float* C, const int M, const int N, const int K, int start_col, int end_col) {

    for( int p = 0; p < K; p++)
    {
        for (int j = start_col; j < end_col; j+=8)
        {
            __m256 acc = _mm256_loadu_ps(C(0, j, N));
            __m256 lhs = _mm256_set1_ps(*A(0, p, K));
            __m256 rhs = _mm256_loadu_ps(B(p, j, N));
            acc = _mm256_fmadd_ps(lhs, rhs, acc);
            _mm256_storeu_ps(C(0, j, N), acc);
        }

    }
}

void matmul_simd_cache_friendly_v2(float* A, float* B, float* C, const int M, const int N, const int K)
{
    int actual_threads = 0;
    #pragma omp parallel num_threads(NTHREADS)
    {
        // Check actual number of threads (only print from thread 0 to avoid multiple prints)
        #pragma omp single
        {
            actual_threads = omp_get_num_threads();
            // printf("Requested threads: %d, Actual threads: %d\n", NTHREADS, actual_threads);
        }
        
        int thread_idx = omp_get_thread_num();
        int labor_per_thread = ((N + actual_threads -  1 )/ actual_threads);
        int start_col =  labor_per_thread * thread_idx;
        int end_col = labor_per_thread * (thread_idx + 1);
        printf("\e[31m[INFO]\e[m start_col:%d end_col:%d\n", start_col, end_col);
        kernel_per_thread(A, B, C, M, N, K, start_col, end_col);
    }
}

void matmul_kernel_transposed(float* A, float* B_untrans, float * B, float * C, const int M, const int N, const int K) {
    // B_untras: (K, N) row-major
    // B: (K, N) column-major
    for (int j = 0; j < N; ++j)
    {
        for (int p = 0; p < K; ++p)
        {
            *(B + j * K + p) = *(B_untrans + p * N + j);
        }
    }

    #pragma omp parallel for num_threads(NTHREADS)
    for( int j = 0; j < N; j++)
    {
        __m256 acc = _mm256_setzero_ps();
        for( int p = 0; p < K; p+=8)
        {
            __m256 lhs = _mm256_loadu_ps(A(0, p, K));
            __m256 rhs = _mm256_loadu_ps( B + j * K + p);
            acc = _mm256_fmadd_ps(lhs, rhs, acc);
        }
        *C(0, j, N) = hsum_float_8(acc);
    }
}

void gemv_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) 
        {
            float acc = 0;
            for (int p = 0; p < K; p++) {
                acc += *A(i, p, K) * *B(p, j, N);
            }
            *C(i, j, N) = acc;
        }
    }
}
\
void matmul_naive_cache_friendly(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int p = 0; p < K; p++)
    {
        for (int j = 0; j < N; j++) 
        {
            *C(0, j, N) += *A(0, p, K) * *B(p, j, N);
        }
    }
}

void matmul_simd(float* A, float* B, float* C, const int M, const int N, const int K) {
    const int BLOCK_SIZE = 64;  // Cache block size
    
    for (int j = 0; j < N; j += 8) {
        __m256 acc = _mm256_loadu_ps(C(0, j, N));
        
        // Process K dimension in blocks
        for (int p = 0; p < K; p += BLOCK_SIZE) {
            int p_end = min(p + BLOCK_SIZE, K);
            
            // Process each block
            for (int p_inner = p; p_inner < p_end; p_inner++) {
                __m256 lhs = _mm256_set1_ps(*A(0, p_inner, K));
                __m256 rhs = _mm256_loadu_ps(B(p_inner, j, N));
                acc = _mm256_fmadd_ps(lhs, rhs, acc);
            }
        }
        _mm256_storeu_ps(C(0, j, N), acc);
    }
}

void float_matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int p = 0; p < K; p++) {
                C[j * M + i] += A[p * M + i] * B[j * K + p];
            }
        }
    }
}


int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    float* A = (float*)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    float* B = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* B_T = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    init_rand(A, M, K);
    init_rand(B, K, N);
    
    // These matrixes are allocated to override cache lines when benchmarking vector-matrix multiplication
    const int M_ = 1000;
    const int K_ = 1000;
    const int N_ = 1000;
    float* A_ = (float*)_mm_malloc(M_ * K_ * sizeof(float), MEM_ALIGN);
    float* B_ = (float*)_mm_malloc(K_ * N_ * sizeof(float), MEM_ALIGN);
    float* C_ = (float*)_mm_malloc(M_ * N_ * sizeof(float), MEM_ALIGN);
    init_rand(A_, M_, K_);
    init_rand(B_, K_, N_);

#ifdef TEST
    gemv_naive(A, B, C_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {
        init_const(C, 0.0, M, N);
        // init_const(C, -1000, M, N);
        float_matmul_naive(A_, B_, C_, M_, N_, K_);
        uint64_t start = timer();
        // matmul_naive_cache_friendly(A, B, C, M, N, K);
        matmul_simd_cache_friendly_v2(A, B, C, M, N, K);
        uint64_t end = timer();
        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;
        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS = \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
#ifdef TEST
        int res = compare_mats(C, C_ref, M * N);
#endif
        printf("\n");
    }

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);
    _mm_free(A_);
    _mm_free(B_);
    _mm_free(C_);

    return 0;
}