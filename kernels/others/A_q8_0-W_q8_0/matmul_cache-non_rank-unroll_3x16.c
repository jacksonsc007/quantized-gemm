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
#define MR 3
#define NR 16
#define MC MR * 11
#define NC NR * 25
#define KC 256

#ifndef MDIM
    #define MDIM 1000
    // #define MDIM MR * 2
#endif

#ifndef NDIM
    #define NDIM 1000
    // #define NDIM NR * 2
#endif

#ifndef KDIM
    #define KDIM 1024
#endif

#ifndef NITER
    #define NITER 100
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C, SA
// column-major order: matrix B, SB
// A: M x K B: K x N C: M x N

#define A(i, j, ld) ( A + ( i ) * ( ld ) + ( j ) )
#define B(i, j, ld) ( B + ( j ) * ( ld ) + ( i ) )
#define C(i, j, ld) ( C + ( i ) * ( ld ) + ( j ) )
#define SA(i, j, ld) (SA + (i) * (ld) + (j))
#define SB(i, j, ld) (SB + (j) * (ld) + (i))

// TODO: Design store pattern that is friendly to the access pattern of int8 weight and activation?

// blockA: packed. row-major.      (MR, KC)
// blockB: packed. column-major.   (KC, NR)
// blockSA: packed. row-major.     (MR, KC/Q_BLK_SIZE)
// blockSB: packed. column-major.  (KC/Q_BLK_SIZE, NR)
#define blockA(i, j, ld) ( blockA  + (i) * (ld) + (j) )
#define blockSA(i, j, ld) (blockSA + (i) * (ld) + (j))
#define blockB(i, j, ld) ( blockB  + (j) * (ld) + (i) )
#define blockSB(i, j, ld) (blockSB + (j) * (ld) + (i))


static int8_t blockA_packed[MC * KC] __attribute__((aligned(64)));
static float sA_packed[MC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));
static int8_t blockB_packed[KC * NC] __attribute__((aligned(64)));
static float sB_packed[NC * KC / Q_BLK_SIZE] __attribute__((aligned(64)));

void matmul_kernel_naive(
    int8_t * blockA, int8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(block_k % Q_BLK_SIZE == 0);
    for (int i = 0; i < block_m; ++i) 
    for (int j = 0; j < block_n; ++j)
    {
        float sum = 0;
        for (int p = 0; p < block_k; p += Q_BLK_SIZE)
        {
            // SA: row-major
            // SB: column-major
            float s_a = *blockSA(i, p / Q_BLK_SIZE, block_k / Q_BLK_SIZE);
            float s_b = *blockSB(p / Q_BLK_SIZE, j, block_k / Q_BLK_SIZE);
            for (int q = 0; q < Q_BLK_SIZE; ++q)
            {
                // A: row-major
                // B: column-major
                int8_t a = *blockA(i, p + q, block_k);
                int8_t b = *blockB(p + q, j, block_k);
                if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                // if (s_a == -1.0)
                {
                    // printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                    printf("\e[31m[ERROR]\e[m s_a=%f s_b=%f, a=%hhd, b=%hhd \n", s_a, s_b, a, b);
                    
                }
                sum += (float)a * (float)b * s_a * s_b;
            }
        }    
        *C(i, j, NDIM) += sum;
    }
}
/*
blockA: packed. row-major.      (MR, KC)
blockB: packed. column-major.   (KC, NR)
blockSA: packed. row-major.     (MR, KC/Q_BLK_SIZE)
blockSB: packed. column-major.  (KC/Q_BLK_SIZE, NR)
*/

void kernel(
    int8_t * blockA, int8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k)
{
    for (int j = 0; j < block_n; ++j)
    {
        __m256 sum_packed_fp_0 = _mm256_setzero_ps();
        __m256 sum_packed_fp_1 = _mm256_setzero_ps();
        __m256 sum_packed_fp_2 = _mm256_setzero_ps();
        
        for (int p = 0; p < block_k; p += Q_BLK_SIZE)
        {
            float sA0 = *blockSA(0, p / Q_BLK_SIZE, block_k/Q_BLK_SIZE);
            float sA1 = *blockSA(1, p / Q_BLK_SIZE, block_k/Q_BLK_SIZE);
            float sA2 = *blockSA(2, p / Q_BLK_SIZE, block_k/Q_BLK_SIZE);
            float sB = *blockSB(p / Q_BLK_SIZE, j, block_k/Q_BLK_SIZE);
            __m256 fused_s0_v = _mm256_set1_ps(sA0 * sB);
            __m256 fused_s1_v = _mm256_set1_ps(sA1 * sB);
            __m256 fused_s2_v = _mm256_set1_ps(sA2 * sB);
            
            __m256i a_pack_int8_0 = _mm256_loadu_si256((__m256i *) blockA(0, p, block_k));
            __m256i a_pack_int8_1 = _mm256_loadu_si256((__m256i *) blockA(1, p, block_k));
            __m256i a_pack_int8_2 = _mm256_loadu_si256((__m256i *) blockA(2, p, block_k));
            __m256i b_pack_int8 = _mm256_loadu_si256((__m256i *) blockB(p, j, block_k));
            
            __m256i a_pack_int8_abs_0 = _mm256_sign_epi8(a_pack_int8_0, a_pack_int8_0);
            __m256i a_pack_int8_abs_1 = _mm256_sign_epi8(a_pack_int8_1, a_pack_int8_1);
            __m256i a_pack_int8_abs_2 = _mm256_sign_epi8(a_pack_int8_2, a_pack_int8_2);
            
            __m256i b_pack_int8_0 = _mm256_sign_epi8(b_pack_int8, a_pack_int8_0);
            __m256i b_pack_int8_1 = _mm256_sign_epi8(b_pack_int8, a_pack_int8_1);
            __m256i b_pack_int8_2 = _mm256_sign_epi8(b_pack_int8, a_pack_int8_2);
            
            __m256i sum_packed_0 = _mm256_maddubs_epi16(a_pack_int8_abs_0, b_pack_int8_0);
            __m256i sum_packed_1 = _mm256_maddubs_epi16(a_pack_int8_abs_1, b_pack_int8_1);
            __m256i sum_packed_2 = _mm256_maddubs_epi16(a_pack_int8_abs_2, b_pack_int8_2);
            
            sum_packed_0 = _mm256_madd_epi16(sum_packed_0, _mm256_set1_epi16(1));
            sum_packed_1 = _mm256_madd_epi16(sum_packed_1, _mm256_set1_epi16(1));
            sum_packed_2 = _mm256_madd_epi16(sum_packed_2, _mm256_set1_epi16(1));
            
            __m256 tmp0 = _mm256_cvtepi32_ps(sum_packed_0);
            __m256 tmp1 = _mm256_cvtepi32_ps(sum_packed_1);
            __m256 tmp2 = _mm256_cvtepi32_ps(sum_packed_2);
            
            sum_packed_fp_0 = _mm256_fmadd_ps(tmp0, fused_s0_v, sum_packed_fp_0);
            sum_packed_fp_1 = _mm256_fmadd_ps(tmp1, fused_s1_v, sum_packed_fp_1);
            sum_packed_fp_2 = _mm256_fmadd_ps(tmp2, fused_s2_v, sum_packed_fp_2);
        }
        
        __m128 tmp0, tmp1, tmp2;
        
        switch(block_m)
        {
            case 3:
                tmp0 = _mm_add_ps(_mm256_extractf128_ps(sum_packed_fp_0, 1), _mm256_extractf128_ps(sum_packed_fp_0, 0));
                tmp0 = _mm_add_ps(tmp0, _mm_movehl_ps(tmp0, tmp0));
                tmp0 = _mm_add_ss(tmp0, _mm_movehdup_ps(tmp0));
                *C(0, j, NDIM) += _mm_cvtss_f32(tmp0);
                
                tmp1 = _mm_add_ps(_mm256_extractf128_ps(sum_packed_fp_1, 1), _mm256_extractf128_ps(sum_packed_fp_1, 0));
                tmp1 = _mm_add_ps(tmp1, _mm_movehl_ps(tmp1, tmp1));
                tmp1 = _mm_add_ss(tmp1, _mm_movehdup_ps(tmp1));
                *C(1, j, NDIM) += _mm_cvtss_f32(tmp1);
                
                tmp2 = _mm_add_ps(_mm256_extractf128_ps(sum_packed_fp_2, 1), _mm256_extractf128_ps(sum_packed_fp_2, 0));
                tmp2 = _mm_add_ps(tmp2, _mm_movehl_ps(tmp2, tmp2));
                tmp2 = _mm_add_ss(tmp2, _mm_movehdup_ps(tmp2));
                *C(2, j, NDIM) += _mm_cvtss_f32(tmp2);
                break;
            case 2:
                tmp0 = _mm_add_ps(_mm256_extractf128_ps(sum_packed_fp_0, 1), _mm256_extractf128_ps(sum_packed_fp_0, 0));
                tmp0 = _mm_add_ps(tmp0, _mm_movehl_ps(tmp0, tmp0));
                tmp0 = _mm_add_ss(tmp0, _mm_movehdup_ps(tmp0));
                *C(0, j, NDIM) += _mm_cvtss_f32(tmp0);
                
                tmp1 = _mm_add_ps(_mm256_extractf128_ps(sum_packed_fp_1, 1), _mm256_extractf128_ps(sum_packed_fp_1, 0));
                tmp1 = _mm_add_ps(tmp1, _mm_movehl_ps(tmp1, tmp1));
                tmp1 = _mm_add_ss(tmp1, _mm_movehdup_ps(tmp1));
                *C(1, j, NDIM) += _mm_cvtss_f32(tmp1);
                break;
            case 1:
                tmp0 = _mm_add_ps(_mm256_extractf128_ps(sum_packed_fp_0, 1), _mm256_extractf128_ps(sum_packed_fp_0, 0));
                tmp0 = _mm_add_ps(tmp0, _mm_movehl_ps(tmp0, tmp0));
                tmp0 = _mm_add_ss(tmp0, _mm_movehdup_ps(tmp0));
                *C(0, j, NDIM) += _mm_cvtss_f32(tmp0);
                break;
        }
    }
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
                    // raise error
                    exit(1);
                }
                sum += (float)a * (float)b * s_a * s_b;
            }
        }    
        *C(i, j, NDIM) = sum;
    }
}


// ======================================================================================================================================================
// pack matrix A
// block size: mc * kc
// panel size: MR * kc
// Both panel and blockA are row-major, no need to pack panel-wise
// ======================================================================================================================================================
void pack_blockA(int8_t* A, int8_t* blockA, const int mc, const int kc)
{
    for (int i = 0; i < mc; i++)
    {
        for( int j = 0; j < kc; j++)
        {
            *blockA(i, j, kc) = *A(i, j, KDIM);
        }
    }
} 

// ======================================================================================================================================================
// pack matrix A scaling factors
// block size: mc * kc (kc is normalized by q_blk_size) 
// panel size: MR * kc 
// ======================================================================================================================================================
void pack_blockSA(float* A, float* blockA, int mc, int kc)
{
    for (int i = 0; i < mc; i++)
    for (int j = 0; j < kc; j++)
    {
        *blockA(i, j, kc) = *A(i, j, KDIM/Q_BLK_SIZE);
    }
} 

// ======================================================================================================================================================
// pack matrix B
// block size: kc * nc
// panel size: kc * NR, row_major
// ======================================================================================================================================================
void pack_blockB(int8_t* B, int8_t* blockB, int kc, int nc)
{
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM);
    }
} 

// ======================================================================================================================================================
// pack matrix B scaling factor
// block size: kc * nc (kc here is already normalized by q_blk_size)
// panel size: kc * NR, row_major
// ======================================================================================================================================================

void pack_blockSB(float* B, float* blockB, int kc, int nc)
{
    for (int j = 0; j < nc; j++)
    for (int i = 0; i < kc; i++)
    {
        *blockB(i, j, kc) = *B(i, j, KDIM/Q_BLK_SIZE);
    }
} 

void matmul_kernel(
    int8_t* A, int8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{
    assert (KDIM % Q_BLK_SIZE == 0);
    assert (KC % Q_BLK_SIZE == 0);
    for (int jc = 0; jc < N; jc += NC)
    {
        int valid_NC = min(N - jc, NC);
        for (int pc = 0; pc < K; pc += KC)
        {
            int valid_KC = min(K - pc, KC);
            pack_blockB(
                B(pc, jc, KDIM), blockB_packed, valid_KC, valid_NC
            );
            pack_blockSB(
                SB(pc/Q_BLK_SIZE, jc, KDIM / Q_BLK_SIZE), sB_packed, valid_KC/Q_BLK_SIZE, valid_NC
            );
            for (int ic = 0; ic < M; ic+= MC)
            {
                int valid_MC = min(M - ic, MC);
                pack_blockA(
                    A(ic, pc, KDIM), blockA_packed, valid_MC, valid_KC
                );
                pack_blockSA(
                    SA(ic, pc/Q_BLK_SIZE, KDIM / Q_BLK_SIZE), sA_packed, valid_MC, valid_KC/Q_BLK_SIZE
                );
                for(int jr = 0; jr < valid_NC; jr += NR)
                {
                    int valid_nr = min(valid_NC - jr, NR);
                    for(int ir = 0; ir < valid_MC; ir += MR)
                    {
                        int valid_mr = min(valid_MC - ir, MR);
                        #ifdef naive
                        matmul_kernel_naive(
                            (blockA_packed + ir * valid_KC),
                            (blockB_packed + jr * valid_KC),
                            (sA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (sB_packed + jr * valid_KC / Q_BLK_SIZE),
                            C(ic + ir, jc + jr, N),
                            valid_mr,
                            valid_nr,
                            valid_KC
                        );
                        #else
                        kernel(
                            (blockA_packed + ir * valid_KC),
                            (blockB_packed + jr * valid_KC),
                            (sA_packed + ir * valid_KC / Q_BLK_SIZE),
                            (sB_packed + jr * valid_KC / Q_BLK_SIZE),
                            C(ic + ir, jc + jr, N),
                            valid_mr,
                            valid_nr,
                            valid_KC
                        );
                        #endif
                    }
                }
            }
        }
    }

}



// void matmul_naive(
//     int8_t* A, int8_t* B, float* C,
//     float* SA, float* SB,
//     const int M, const int N, const int K) {
//     // assert(M % MR == 0);
//     // assert(N % NR == 0);
//     for (int j = 0; j < N; j += NR) {
//         pack_blockB( B(0, j, K), blockB_packed);
//         pack_sB(SB(0, j, K / Q_BLK_SIZE), sB_packed);
//         for (int i = 0; i < M; i += MR) {
//             // kernel_16x6(&A[i], &B[j * K], &C[j * M + i], M, N, K);
//             pack_blockA(A(i, 0, K), blockA_packed);
//             pack_sA(SA(i, 0, KDIM / Q_BLK_SIZE), sA_packed);
//             int8_kernel_naive(blockA_packed, blockB_packed, sA_packed, sB_packed, C(i, j, N), MR, NR, K);

//         }
//     }
// }


void matmul_naive_no_packing(
    int8_t* A, int8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) {
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(K % Q_BLK_SIZE == 0);
    for (int i = 0; i < M; ++i) for (int j = 0; j < N; ++j)
    {
        float sum = 0;
        for (int p = 0; p < K; p += Q_BLK_SIZE)
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
    init_int_const(A, -3, M, K);
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
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
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
