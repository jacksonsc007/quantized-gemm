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
#define MR 1
#define NR 16

#ifndef MDIM
    #define MDIM MR * 166
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM NR * 62
    // #define NDIM NR * 2
#endif

#ifndef KDIM
    #define KDIM 1024
#endif

#ifndef NITER
    #define NITER 100
#endif


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


#define exp_setting "Testing A,C with row-major order and B with column-major order\n"

static int8_t blockA_packed[MR * KDIM] __attribute__((aligned(64)));
static float sA_packed[MR * KDIM / Q_BLK_SIZE] __attribute__((aligned(64)));
static int8_t blockB_packed[KDIM * NR] __attribute__((aligned(64)));
static float sB_packed[KDIM * NR / Q_BLK_SIZE] __attribute__((aligned(64)));

/*
blockA should be column-major order
blockB should be row-major order
*/
void kernel_1x16(
    int8_t * blockA, int8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k) {
    // quantized value
    __m256i a0; // 
    __m256i b0; // hold 16 int16
    // __m256i b1; // 
    __m256i c0_int; //
    __m256i c0_int_low; //
    __m256i c0_int_high; //
    // scaling factors
    __m256 sa0; // 
    __m256 sb0; // 
    __m256 sb1; // 
    __m256 fused_s0; // 
    __m256 fused_s1; // 
    // float32 acumulator for results
    __m256 c00 = _mm256_loadu_ps( C(0, 0, NDIM));
    __m256 c01 = _mm256_loadu_ps( C(0, 8, NDIM));

    for (int p = 0; p < block_k; p+= Q_BLK_SIZE)
    {
        for (int q = 0; q < Q_BLK_SIZE; q+= 1)
        {
            // sa0 = _mm256_set1_ps( *sA(0, p, block_m));
            sb0 = _mm256_loadu_ps( blockSB(p / Q_BLK_SIZE, 0, block_n));
            sb1 = _mm256_loadu_ps( blockSB(p / Q_BLK_SIZE, 8, block_n));


            sa0 = _mm256_broadcast_ss( blockSA(0, p / Q_BLK_SIZE, block_m));
            a0 = _mm256_set1_epi16(  (int16_t) *blockA(0, p+q, block_m));
            fused_s0 = _mm256_mul_ps(sa0, sb0);
            fused_s1 = _mm256_mul_ps(sa0, sb1);
            // load 16 int8 and sign extend them into 16 int16
            b0 = _mm256_cvtepi8_epi16(_mm_loadu_si128( (__m128i *) blockB(p+q, 0, block_n)));
            c0_int = _mm256_mullo_epi16(a0, b0);
            c0_int_low = _mm256_cvtepi16_epi32( _mm256_extracti128_si256(c0_int, 0) );
            c0_int_high = _mm256_cvtepi16_epi32( _mm256_extracti128_si256(c0_int, 1) );
            // convert 16 int16 into 16 int32, thus we need two ymm registers
            c00 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c00);
            c01 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c01);

        }

    }
    _mm256_storeu_ps( C(0, 0, NDIM), c00);
    _mm256_storeu_ps( C(0, 8, NDIM), c01);
} 
void int8_kernel_naive(
    int8_t * blockA, int8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k) {
    for (int i = 0; i < block_m; ++i)
    for (int j = 0; j < block_n; ++j)
    {
        float sum = 0;
        for (int p = 0; p < block_k; p += Q_BLK_SIZE)
        {
            float s_a = *blockSA(i, p / Q_BLK_SIZE, block_m);
            float s_b = *blockSB(p / Q_BLK_SIZE, j, block_n);
            for (int q = 0; q < Q_BLK_SIZE; ++q)
            {
                int8_t a = *blockA(i, p + q, block_m);
                int8_t b = *blockB(p + q, j, block_n);
                if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                {
                    printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                }
                sum += (float)a * (float)b * s_a * s_b;
            }
        }    
        *C(i, j, NDIM) = sum;
    }
}
void int8_kernel_naive_no_packing(
    int8_t * A, int8_t * B,
    float * SA, float * SB,
    float* C, const int block_m, const int block_n, const int block_k) {
    for (int i = 0; i < block_m; ++i)
    for (int j = 0; j < block_n; ++j)
    {
        float sum = 0;
        for (int p = 0; p < block_k; p += Q_BLK_SIZE)
        {
            // SA: row-major
            // SB: column-major
            float s_a = *SA(i, p / Q_BLK_SIZE, KDIM / Q_BLK_SIZE);
            float s_b = *SB(p / Q_BLK_SIZE, j, KDIM / Q_BLK_SIZE);
            for (int q = 0; q < Q_BLK_SIZE; ++q)
            {
                // A: row-major
                // B: column-major
                int8_t a = *A(i, p + q, KDIM);
                int8_t b = *B(p + q, j, KDIM);
                if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                {
                    printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                }
                sum += (float)a * (float)b * s_a * s_b;
            }
        }    
        *C(i, j, NDIM) = sum;
    }
}


void pack_blockA(int8_t* A, int8_t* blockA)
{
    for (int j = 0; j < KDIM; ++j)
    {
        for (int i = 0; i < MR; ++i)
        {
            *blockA(i, j, MR) = *A(i, j, KDIM);
        }
    }
} 
void pack_sA(float* A, float* blockA)
{
    for (int j = 0; j < KDIM / Q_BLK_SIZE; ++j)
    {
        for (int i = 0; i < MR; ++i)
        {
            *blockA(i, j, MR) = *A(i, j, KDIM / Q_BLK_SIZE);
        }
    }
} 

void pack_blockB(int8_t* B, int8_t* blockB)
{
    for (int j = 0; j < NR; ++j)
    {
        for (int i = 0; i < KDIM; ++i)
        {
            *blockB(i, j, NR) = *B(i, j, KDIM) ;
        }
    }
} 
/*
B: scaling factor for weight. column-major order. Shape: (KDIM / Q_BLK_SIZE, NDIM)
blockB: packed scaling factor. row-major order. Shape: (KDIM / Q_BLK_SIZE, NR) 
*/
void pack_sB(float* B, float* blockB)
{
    for (int j = 0; j < NR; ++j)
    for (int i = 0; i < KDIM / Q_BLK_SIZE; ++i)
    {
        *blockB(i, j, NR) = *B(i, j, KDIM / Q_BLK_SIZE);
    }

} 

void matmul_kernel(
    int8_t* A, int8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) {
    assert(M % MR == 0);
    assert(N % NR == 0);
    for (int j = 0; j < N; j += NR) {
        pack_blockB( B(0, j, K), blockB_packed);
        pack_sB(SB(0, j, K / Q_BLK_SIZE), sB_packed);
        for (int i = 0; i < M; i += MR) {
            // kernel_16x6(&A[i], &B[j * K], &C[j * M + i], M, N, K);
            pack_blockA(A(i, 0, K), blockA_packed);
            pack_sA(SA(i, 0, KDIM / Q_BLK_SIZE), sA_packed);
            kernel_1x16(blockA_packed, blockB_packed, sA_packed, sB_packed, C(i, j, N), MR, NR, K);

        }
    }
}



void matmul_naive(
    int8_t* A, int8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) {
    assert(M % MR == 0);
    assert(N % NR == 0);
    for (int j = 0; j < N; j += NR) {
        pack_blockB( B(0, j, K), blockB_packed);
        pack_sB(SB(0, j, K / Q_BLK_SIZE), sB_packed);
        for (int i = 0; i < M; i += MR) {
            // kernel_16x6(&A[i], &B[j * K], &C[j * M + i], M, N, K);
            pack_blockA(A(i, 0, K), blockA_packed);
            pack_sA(SA(i, 0, KDIM / Q_BLK_SIZE), sA_packed);
            int8_kernel_naive(blockA_packed, blockB_packed, sA_packed, sB_packed, C(i, j, N), MR, NR, K);

        }
    }
}


void matmul_naive_no_packing(
    int8_t* A, int8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) {
    assert(M % MR == 0);
    assert(N % NR == 0);
    for (int j = 0; j < N; j += NR) {
        for (int i = 0; i < M; i += MR) {
            int8_kernel_naive_no_packing(A(i, 0, KDIM), B(0, j, KDIM), SA(i, 0, KDIM/Q_BLK_SIZE), SB(0, j, KDIM/Q_BLK_SIZE), C(i, j, N), MR, NR, K);

        }
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


void init_int_const(int8_t* mat, const int8_t value, const int M, const int N) {
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

int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A = (int8_t*)_mm_malloc(M * K * sizeof(int8_t), MEM_ALIGN);
    int8_t* B = (int8_t*)_mm_malloc(K * N * sizeof(int8_t), MEM_ALIGN);
    float* sA = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    

    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    // init_rand(A, M, K);
    // init_rand(B, K, N);
    init_int_const(A, 3, M, K);
    init_int_const(B, 4, K, N);
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    // init_const(sA, 10.1, M, K / Q_BLK_SIZE);
    // init_const(sB, 50.5, K / Q_BLK_SIZE, N);
    
    // init packed to debug
    init_int_const(blockA_packed, -1, MR, KDIM);
    init_int_const(blockB_packed, -1, KDIM, NR);
    init_const(sA_packed, -1.0, MR, KDIM / Q_BLK_SIZE);
    init_const(sB_packed, -1.0, KDIM / Q_BLK_SIZE, NR);

#ifdef TEST
    matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    // matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, sA, sB, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("\e[31mGFLOPS\e[m= %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        print_mat(C, 5, 5);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        if (!res) break;
#endif
        printf("\n");
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);

    return 0;
}
