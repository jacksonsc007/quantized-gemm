/*
Despite the memory layout and A,B,C are all different, we stick to the cache blocking strategy.
Other conjugate blocking strategy is going to be further discussed in the future version.
*/
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64
#define UN_INIT -10.0

#ifndef NTHREADS
#define NTHREADS 1
#endif

#define MR 1
#define NR 16
// #define MC MR *256
// #define NC NR * 64
// #define MC MR * 64
// #define NC NR * 256
// #define KC 2000


#define MC (6 * (800 / NTHREADS) * NTHREADS)
#define NC (16 * (40 / NTHREADS) * NTHREADS)
#define KC 500

#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM 1
#endif

#ifndef NDIM
    #define NDIM 10240
#endif

#ifndef KDIM
    #define KDIM 10240
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

// column-major shape (block_m, K)
#define blockA(i, j) ( blockA + ( j ) * block_m + ( i ) )
// row-major shape (K, block_n)
#define blockB(i, j) ( blockB + ( i ) * block_n + ( j ) )



static float blockA_packed[MC * KC] __attribute__((aligned(64)));
static float blockB_packed[KC * NC] __attribute__((aligned(64)));

// openmp setting
#define OMP_SCHEDULE dynamic
#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")

/*
blockA should be column-major order
blockB should be row-major order
*/
void kernel(float* blockA, float* blockB, float* C, const int valid_m, const int valid_n, const int block_m, const int block_n, const int block_K, const int N) {
    // For GEMV, we only need one set of accumulators since we have one row
    __m256 C0 = _mm256_setzero_ps();
    __m256 C1 = _mm256_setzero_ps();

    __m256 a_packFloat8;
    __m256 b0_packFloat8;
    __m256 b1_packFloat8;
    __m256i mask0;
    __m256i mask1;

    // Load C from memory to registers
    if (valid_n != NR) {
        static int8_t mask[32] __attribute__((aligned(64))) = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        mask0 = _mm256_cvtepi8_epi32(_mm_loadu_si64(&mask[16 - valid_n]));
        mask1 = _mm256_cvtepi8_epi32(_mm_loadu_si64(&mask[16 - valid_n + 8]));
        C0 = _mm256_maskload_ps(C, mask0);
        C1 = _mm256_maskload_ps(C + 8, mask1);
    } else {
        C0 = _mm256_loadu_ps(C);
        C1 = _mm256_loadu_ps(C + 8);
    }

    // Main computation loop
    for (int p = 0; p < block_K; p++) {
        b0_packFloat8 = _mm256_loadu_ps(blockB(p, 0));
        b1_packFloat8 = _mm256_loadu_ps(blockB(p, 8));
        a_packFloat8 = _mm256_broadcast_ss(blockA + p);
        C0 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C0);
        C1 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C1);
    }

    // Store results back to memory
    if (valid_n != NR) {
        _mm256_maskstore_ps(C, mask0, C0);
        _mm256_maskstore_ps(C + 8, mask1, C1);
    } else {
        _mm256_storeu_ps(C, C0);
        _mm256_storeu_ps(C + 8, C1);
    }
}




/*
Pack A into the memory layout required by the kernel

A: The origianl matrix A
K: The hidden dimension of the original matrix A, we need it to retrieve elements, since A is row-major.
blockA: The packed matrix A. blockA is column-major, with static shape: MC * KC. NOTE some parts may not be valid
valid_m: The valid row of blockA
valid_k: The valid column of blockA
the valid elements are in matrix of size (valid_m * valid_k)
*/
void pack_blockA_panel(float* A, float* blockA, const int kc, const int mc, const int K)
{
    for( int j = 0; j < kc; ++j)
    {
        for ( int i = 0; i < mc; ++i)
        {
            *(blockA) = *A(i, j, K);
            blockA++;
        }
    }
}
void pack_blockA(float* A, float* blockA, const int mc, const int kc, const int K)
{
    PRAGMA_OMP_PARALLEL_FOR
    for (int i = 0; i < mc; i += MR)
    {
        int valid_m = min(mc - i, MR);
        pack_blockA_panel(
            A(i, 0, K),
            blockA + i * kc,
            kc,
            valid_m,
            K
        );
    }
} 

void pack_blockB_panel(float *B, float *blockB, const int nc, const int kc, const int N)
{
    for (int i = 0; i < kc; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            *(blockB++) = *B(i, j, N);
        }
    }
}

void pack_blockB(float* B, float* blockB, const int nc, const int kc, const int N)
{   
    PRAGMA_OMP_PARALLEL_FOR
    for (int jc = 0; jc < nc; jc += NR)
    {
        int valid_NR = min(nc - jc, NR);
        pack_blockB_panel(
            B(0, jc, N),
            blockB + jc * kc, 
            valid_NR,
            kc,
            N
        );
    }
} 




/*
j: column index
i: row index
p: hidden index
c: Cache level
r: register level
*/
void matmul_kernel(float* A, float* B, float* C, const int M, const int N, const int K) {
    // For GEMV (M=1), we don't need MC/MR blocking
    for (int pc = 0; pc < K; pc += KC)
    {
        int valid_KC = min(K - pc, KC);
        // For GEMV, we just need to copy the row of A
        for (int k = 0; k < valid_KC; k++) {
            blockA_packed[k] = A[pc + k];
        }
        
        for (int jc = 0; jc < N; jc += NC)
        {
            int valid_NC = min(N - jc, NC);
            pack_blockB(
                B(pc, jc, N),
                blockB_packed,
                valid_NC,
                valid_KC,
                N
            );
            
            PRAGMA_OMP_PARALLEL_FOR
            for (int jr = 0; jr < valid_NC; jr += NR)
            {
                int valid_NR = min(valid_NC - jr, NR);
                float *blockA_panel = blockA_packed;
                float *blockB_panel = blockB_packed + jr * valid_KC;
                kernel(blockA_panel, blockB_panel, C(0, jc + jr, N), 1, valid_NR, 1, valid_NR, valid_KC, N);
            }
        }
    }
}



void gemv_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float acc = 0;
            for (int p = 0; p < K; p++) {
                acc += *A(i, p, K) * *B(p, j, N);
            }
            *C(i, j, N) = acc;
        }
    }
}

void print_mat(float* mat, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", mat[i * N + j]);
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

void init_const(float* mat, const float value, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            *mat++ = value;
        }
    }
}

int compare_mats(float* mat1, float* mat2, const int M, const int N) {
    float acc_err = 0.;
    for (int i = M -1; i >= 0; i--) 
    {
        for (int j = N - 1; j >= 0; j--) 
        {
            float err = fabsf(mat1[i * N + j] - mat2[i * N + j]);
            acc_err += err;
            if ( err > 1e-3) 
            {
            }
        }
    }
    float avg_err = acc_err / (M * N);
    printf("acc error: % f average error: %f\n", acc_err, avg_err);
    if (avg_err > 1e-3) return 0;
    return 1;
}

uint64_t timer() {
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    return (uint64_t)start.tv_sec * 1000000000 + (uint64_t)start.tv_nsec;
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
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    init_rand(A, M, K);
    init_rand(B, K, N);
    

    printf("\e[31m[INFO]\e[m KC: %d, MC: %d, NC: %d, MR: %d, NR: %d\n", KC, MC, NC, MR, NR);


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
        float_matmul_naive(A_, B_, C_, M_, N_, K_);
        // init_const(C, -1000, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;
        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS = \e[32m%.3f\e[m] for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        if (!res)
        {
            if (0)
            {
                printf("\n\n");
                print_mat(C, M, N);
                printf("\n\n");
                print_mat(C_ref, M, N);
                printf("Matrix C start address: %p end address: %p\n", C, C + M * N);
            }
            // break;
        }
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