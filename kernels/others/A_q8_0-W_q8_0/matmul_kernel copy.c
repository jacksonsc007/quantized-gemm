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
void kernel_3x16(
    int8_t * blockA, int8_t * blockB,
    float * blockSA, float * blockSB,
    float* C, const int block_m, const int block_n, const int block_k) {
    // quantized value
    __m256i a0, a1, a2, a3, a4, a5; // hold 6 rows of A
    __m256i b0; // hold 16 int8 values from B
    __m256i c0_int, c1_int, c2_int, c3_int, c4_int, c5_int; // intermediate results
    __m256i c0_int_low, c0_int_high, c1_int_low, c1_int_high;
    __m256i c2_int_low, c2_int_high, c3_int_low, c3_int_high;
    __m256i c4_int_low, c4_int_high, c5_int_low, c5_int_high;

    // scaling factors
    __m256 sa0, sa1, sa2, sa3, sa4, sa5; // scaling factors for 6 rows of A
    __m256 sb0, sb1; // scaling factors for B
    __m256 fused_s0, fused_s1; // fused scaling factors

    __m256i masks[2];
    // __m256 c00, c01;
    // __m256 c10, c11;
    // __m256 c20, c21;
    __m256 c[MR][2];
    if (block_n != NR)
    {
        const unsigned int bit_mask = 65535;
        masks[0] = _mm256_setr_epi32(bit_mask << (block_n + 15),
                                     bit_mask << (block_n + 14),
                                     bit_mask << (block_n + 13),
                                     bit_mask << (block_n + 12),
                                     bit_mask << (block_n + 11),
                                     bit_mask << (block_n + 10),
                                     bit_mask << (block_n + 9),
                                     bit_mask << (block_n + 8));
        masks[1] = _mm256_setr_epi32(bit_mask << (block_n + 7),
                                     bit_mask << (block_n + 6),
                                     bit_mask << (block_n + 5),
                                     bit_mask << (block_n + 4),
                                     bit_mask << (block_n + 3),
                                     bit_mask << (block_n + 2),
                                     bit_mask << (block_n + 1),
                                     bit_mask <<  block_n);
        // for (int i = 0; i < valid_m; i++)
        // {
        //     C_buffer[i][0] = _mm256_maskload_ps(C(i, 0), masks[0]);
        //     C_buffer[i][1] = _mm256_maskload_ps(C(i, 8), masks[1]);
        // }
        // c00 = _mm256_maskload_ps(C(0, 0, NDIM), masks[0]);
        // c01 = _mm256_maskload_ps(C(0, 8, NDIM), masks[1]);
        // c10 = _mm256_maskload_ps(C(1, 0, NDIM), masks[0]);
        // c11 = _mm256_maskload_ps(C(1, 8, NDIM), masks[1]);
        // c20 = _mm256_maskload_ps(C(2, 0, NDIM), masks[0]);
        // c21 = _mm256_maskload_ps(C(2, 8, NDIM), masks[1]);
        
        for (int i = 0; i < block_m; i++)
        {
            c[i][0] = _mm256_maskload_ps(C(i, 0, NDIM), masks[0]);
            c[i][1] = _mm256_maskload_ps(C(i, 8, NDIM), masks[1]);
        }
        
                                     
    } 
    else
    {
        // float32 accumulators for results
        // c00 = _mm256_loadu_ps(C(0, 0, NDIM));
        // c01 = _mm256_loadu_ps(C(0, 8, NDIM));
        // c10 = _mm256_loadu_ps(C(1, 0, NDIM));
        // c11 = _mm256_loadu_ps(C(1, 8, NDIM));
        // c20 = _mm256_loadu_ps(C(2, 0, NDIM));
        // c21 = _mm256_loadu_ps(C(2, 8, NDIM));
        // c30 = _mm256_loadu_ps(C(3, 0, NDIM));
        // c31 = _mm256_loadu_ps(C(3, 8, NDIM));
        // c40 = _mm256_loadu_ps(C(4, 0, NDIM));
        // c41 = _mm256_loadu_ps(C(4, 8, NDIM));
        // c50 = _mm256_loadu_ps(C(5, 0, NDIM));
        // c51 = _mm256_loadu_ps(C(5, 8, NDIM));
        for (int i = 0; i < block_m; i++)
        {
            c[i][0] = _mm256_loadu_ps(C(i, 0, NDIM));
            c[i][1] = _mm256_loadu_ps(C(i, 8, NDIM));
        }
    }

    for (int p = 0; p < block_k; p += Q_BLK_SIZE) 
    {
        for (int q = 0; q < Q_BLK_SIZE; q += 1) 
        {
            sb0 = _mm256_loadu_ps(blockSB(p / Q_BLK_SIZE, 0, NR));
            sb1 = _mm256_loadu_ps(blockSB(p / Q_BLK_SIZE, 8, NR));
            b0 = _mm256_cvtepi8_epi16(_mm_loadu_si128((__m128i *) blockB(p + q, 0, NR)));

            // 1st row of A
            sa0 = _mm256_broadcast_ss(blockSA(0, p / Q_BLK_SIZE, block_m));
            a0 = _mm256_set1_epi16((int16_t) *blockA(0, p + q, block_m));
            c0_int = _mm256_mullo_epi16(a0, b0);
            c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            fused_s0 = _mm256_mul_ps(sa0, sb0);
            fused_s1 = _mm256_mul_ps(sa0, sb1);
            c[0][0] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c[0][0]);
            c[0][1] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c[0][1]);
            // c00 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c00);
            // c01 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c01);
            
            // 2nd row of A
            // sa0 = _mm256_broadcast_ss(blockSA(1, p / Q_BLK_SIZE, block_m));
            // a0 = _mm256_set1_epi16((int16_t) *blockA(1, p + q, block_m));
            // c0_int = _mm256_mullo_epi16(a0, b0);
            // c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            // c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            // fused_s0 = _mm256_mul_ps(sa0, sb0);
            // fused_s1 = _mm256_mul_ps(sa0, sb1);
            // c[1][0] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c[1][0]);
            // c[1][1] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c[1][1]);

            // 3rd row of A
            // sa0 = _mm256_broadcast_ss(blockSA(2, p / Q_BLK_SIZE, block_m));
            // a0 = _mm256_set1_epi16((int16_t) *blockA(2, p + q, block_m));
            // c0_int = _mm256_mullo_epi16(a0, b0);
            // c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            // c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            // fused_s0 = _mm256_mul_ps(sa0, sb0);
            // fused_s1 = _mm256_mul_ps(sa0, sb1);
            // c[2][0] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c[2][0]);
            // c[2][1] = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c[2][1]);

            // 4th row of A
            // sa0 = _mm256_broadcast_ss(blockSA(3, p / Q_BLK_SIZE, block_m));
            // a0 = _mm256_set1_epi16((int16_t) *blockA(3, p + q, block_m));
            // c0_int = _mm256_mullo_epi16(a0, b0);
            // c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            // c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            // fused_s0 = _mm256_mul_ps(sa0, sb0);
            // fused_s1 = _mm256_mul_ps(sa0, sb1);
            // c30 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c30);
            // c31 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c31);

            // 5th row of A
            // sa0 = _mm256_broadcast_ss(blockSA(4, p / Q_BLK_SIZE, block_m));
            // a0 = _mm256_set1_epi16((int16_t) *blockA(4, p + q, block_m));
            // c0_int = _mm256_mullo_epi16(a0, b0);
            // c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            // c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            // fused_s0 = _mm256_mul_ps(sa0, sb0);
            // fused_s1 = _mm256_mul_ps(sa0, sb1);
            // c40 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c40);
            // c41 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c41);

            // 6th row of A
            // sa0 = _mm256_broadcast_ss(blockSA(5, p / Q_BLK_SIZE, block_m));
            // a0 = _mm256_set1_epi16((int16_t) *blockA(5, p + q, block_m));
            // c0_int = _mm256_mullo_epi16(a0, b0);
            // c0_int_low = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 0));
            // c0_int_high = _mm256_cvtepi16_epi32(_mm256_extracti128_si256(c0_int, 1));
            // fused_s0 = _mm256_mul_ps(sa0, sb0);
            // fused_s1 = _mm256_mul_ps(sa0, sb1);
            // c50 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_low), fused_s0, c50);
            // c51 = _mm256_fmadd_ps(_mm256_cvtepi32_ps(c0_int_high), fused_s1, c51);
        }
    }

    if (block_n != NR)
    {
        
        // _mm256_maskstore_ps(C(0, 0, NDIM), masks[0], c00);
        // _mm256_maskstore_ps(C(0, 8, NDIM), masks[1], c01);
        // _mm256_maskstore_ps(C(1, 0, NDIM), masks[0], c10);
        // _mm256_maskstore_ps(C(1, 8, NDIM), masks[1], c11);
        // _mm256_maskstore_ps(C(2, 0, NDIM), masks[0], c20);
        // _mm256_maskstore_ps(C(2, 8, NDIM), masks[1], c21);
        
        for (int i = 0; i < block_m; ++i)
        {
            _mm256_maskstore_ps(C(i, 0, NDIM), masks[0], c[i][0]);
            _mm256_maskstore_ps(C(i, 8, NDIM), masks[1], c[i][1]);
        }
    }
    
    else
    {
        // Store the results back to C
        // _mm256_storeu_ps(C(0, 0, NDIM), c00);
        // _mm256_storeu_ps(C(0, 8, NDIM), c01);
        // _mm256_storeu_ps(C(1, 0, NDIM), c10);
        // _mm256_storeu_ps(C(1, 8, NDIM), c11);
        // _mm256_storeu_ps(C(2, 0, NDIM), c20);
        // _mm256_storeu_ps(C(2, 8, NDIM), c21);
        // _mm256_storeu_ps(C(3, 0, NDIM), c30);
        // _mm256_storeu_ps(C(3, 8, NDIM), c31);
        // _mm256_storeu_ps(C(4, 0, NDIM), c40);
        // _mm256_storeu_ps(C(4, 8, NDIM), c41);
        // _mm256_storeu_ps(C(5, 0, NDIM), c50);
        // _mm256_storeu_ps(C(5, 8, NDIM), c51);
        
        for (int i = 0; i < block_m; ++i)
        {
            _mm256_storeu_ps(C(i, 0, NDIM), c[i][0]);
            _mm256_storeu_ps(C(i, 8, NDIM), c[i][1]);
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

void pack_blockB(int8_t* B, int8_t* blockB, int valid_nr)
{
    for (int j = 0; j < valid_nr; ++j)
    {
        for (int i = 0; i < KDIM; ++i)
        {
            *blockB(i, j, NR) = *B(i, j, KDIM) ;
        }
    }
    for (int j = valid_nr; j < NR; ++j)
    {
        for (int i = 0; i < KDIM; ++i)
        {
            *blockB(i, j, NR) = 0;
        }
    }
} 
/*
B: scaling factor for weight. column-major order. Shape: (KDIM / Q_BLK_SIZE, NDIM)
blockB: packed scaling factor. row-major order. Shape: (KDIM / Q_BLK_SIZE, NR) 
*/
void pack_sB(float* B, float* blockB, int valid_nr)
{
    for (int j = 0; j < valid_nr; ++j)
    for (int i = 0; i < KDIM / Q_BLK_SIZE; ++i)
    {
        *blockB(i, j, NR) = *B(i, j, KDIM / Q_BLK_SIZE);
    }

    for (int j = valid_nr; j < NR; ++j)
    {
        for (int i = 0; i < KDIM / Q_BLK_SIZE; ++i)
        {
            *blockB(i, j, NR) = 0;
        }
    }

} 

void matmul_kernel(
    int8_t* A, int8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) {
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    for (int j = 0; j < N; j += NR) {
        int valid_nr = min(N - j, NR);
        pack_blockB( B(0, j, K), blockB_packed, valid_nr);
        pack_sB(SB(0, j, K / Q_BLK_SIZE), sB_packed, valid_nr);
        for (int i = 0; i < M; i += MR) {
            // kernel_16x6(&A[i], &B[j * K], &C[j * M + i], M, N, K);
            pack_blockA(A(i, 0, K), blockA_packed);
            pack_sA(SA(i, 0, KDIM / Q_BLK_SIZE), sA_packed);
            kernel_3x16(blockA_packed, blockB_packed, sA_packed, sB_packed, C(i, j, N), MR, valid_nr, K);

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
    for (int i = 0; i < M; ++i)
    for (int j = 0; j < N; ++j)
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
