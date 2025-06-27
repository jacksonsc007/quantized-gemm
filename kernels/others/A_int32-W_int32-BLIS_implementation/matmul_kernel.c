// clang-17 -O2 -mno-avx512f -march=native -DTEST -DNITER=1000 matmul_kernel.c -o matmul_kernel.out && ./matmul_kernel.out
#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define MEM_ALIGN 64

#define MR 6
#define NR 16

#ifndef MDIM
    // #define MDIM MR * 62
    #define MDIM MR * 166
#endif

#ifndef NDIM
    // #define NDIM NR * 166
    #define NDIM NR * 62
#endif

#ifndef KDIM
    #define KDIM 10
#endif

#ifndef NITER
    #define NITER 100
#endif


// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j) ( A + i * K + j )
#define B(i, j) ( B + j * K + i )
#define C(i, j) ( C + i * N + j )

// column-major shape (block_m, K)
#define blockA(i, j) ( blockA + j * block_m + i )
// row-major shape (K, block_n)
#define blockB(i, j) ( blockB + i * block_n + j )


#define exp_setting "Testing A,C with row-major order and B with column-major order\n"

static int32_t blockA_packed[MR * KDIM] __attribute__((aligned(64)));
static int32_t blockB_packed[KDIM * NR] __attribute__((aligned(64)));

// void kernel_16x6(int32_t* blockA, int32_t* blockB, int32_t* C, const int block_m, const int block_n, const int K, const int N) {
//     for (int i = 0; i < block_m; i++)
//     {
//         for (int j = 0; j < block_n; j++)
//         {
//             int32_t acc = 0;
//             for (int p = 0; p < K; p++)
//             {
//                 acc += *blockA(i, p) * *blockB(p, j);
//             }
//             *C(i, j) += acc;
//         }
//     }
//     // printf("\e[31m[INFO]\e[m test\n ");
// }

void kernel_16x6(int32_t* blockA, int32_t* blockB, int32_t* C, const int block_m, const int block_n, const int K, const int N) {
    __m256i C_buffer[6][2];
    __m256i a_pack;
    __m256i b0_pack;
    __m256i b1_pack;

    // Load C from memory to registers
    for (int i = 0; i < 6; i++) {
        C_buffer[i][0] = _mm256_loadu_si256((__m256i*)&C[i * N]);
        C_buffer[i][1] = _mm256_loadu_si256((__m256i*)&C[i * N + 8]);
    }

    for (int p = 0; p < K; p++) {
        // Load B vectors
        b0_pack = _mm256_loadu_si256((__m256i*)blockB(p, 0));
        b1_pack = _mm256_loadu_si256((__m256i*)blockB(p, 8));

        // Compute and accumulate for each row of A
        a_pack = _mm256_set1_epi32(*blockA(0, p));
        __m256i prod0_0 = _mm256_mullo_epi32(a_pack, b0_pack);
        __m256i prod0_1 = _mm256_mullo_epi32(a_pack, b1_pack);
        C_buffer[0][0] = _mm256_add_epi32(C_buffer[0][0], prod0_0);
        C_buffer[0][1] = _mm256_add_epi32(C_buffer[0][1], prod0_1);

        a_pack = _mm256_set1_epi32(*blockA(1, p));
        __m256i prod1_0 = _mm256_mullo_epi32(a_pack, b0_pack);
        __m256i prod1_1 = _mm256_mullo_epi32(a_pack, b1_pack);
        C_buffer[1][0] = _mm256_add_epi32(C_buffer[1][0], prod1_0);
        C_buffer[1][1] = _mm256_add_epi32(C_buffer[1][1], prod1_1);

        a_pack = _mm256_set1_epi32(*blockA(2, p));
        __m256i prod2_0 = _mm256_mullo_epi32(a_pack, b0_pack);
        __m256i prod2_1 = _mm256_mullo_epi32(a_pack, b1_pack);
        C_buffer[2][0] = _mm256_add_epi32(C_buffer[2][0], prod2_0);
        C_buffer[2][1] = _mm256_add_epi32(C_buffer[2][1], prod2_1);

        a_pack = _mm256_set1_epi32(*blockA(3, p));
        __m256i prod3_0 = _mm256_mullo_epi32(a_pack, b0_pack);
        __m256i prod3_1 = _mm256_mullo_epi32(a_pack, b1_pack);
        C_buffer[3][0] = _mm256_add_epi32(C_buffer[3][0], prod3_0);
        C_buffer[3][1] = _mm256_add_epi32(C_buffer[3][1], prod3_1);

        a_pack = _mm256_set1_epi32(*blockA(4, p));
        __m256i prod4_0 = _mm256_mullo_epi32(a_pack, b0_pack);
        __m256i prod4_1 = _mm256_mullo_epi32(a_pack, b1_pack);
        C_buffer[4][0] = _mm256_add_epi32(C_buffer[4][0], prod4_0);
        C_buffer[4][1] = _mm256_add_epi32(C_buffer[4][1], prod4_1);

        a_pack = _mm256_set1_epi32(*blockA(5, p));
        __m256i prod5_0 = _mm256_mullo_epi32(a_pack, b0_pack);
        __m256i prod5_1 = _mm256_mullo_epi32(a_pack, b1_pack);
        C_buffer[5][0] = _mm256_add_epi32(C_buffer[5][0], prod5_0);
        C_buffer[5][1] = _mm256_add_epi32(C_buffer[5][1], prod5_1);
    }

    // Store C from registers to memory
    for (int i = 0; i < 6; i++) {
        _mm256_storeu_si256((__m256i*)&C[i * N], C_buffer[i][0]);
        _mm256_storeu_si256((__m256i*)&C[i * N + 8], C_buffer[i][1]);
    }
}


/*
blockA should be column-major order
blockB should be row-major order
*/
// void kernel_16x6(int32_t* blockA, int32_t* blockB, int32_t* C, const int block_m, const int block_n, const int K, const int N) {
//     __m256i C_buffer[6][2];
//     __m256i a_packint32_t8;
//     __m256i b0_packint32_t8;
//     __m256i b1_packint32_t8;
//     // load C from memory to registers
//     // i: row index
//     for (int i = 0; i < 6; i++) {
//         C_buffer[i][0] = _mm256_loadu_epi32(C(i, 0));
//         C_buffer[i][1] = _mm256_loadu_epi32(C(i, 8));
//     }
//     for (int p = 0; p < K; p++) {
//         b0_packint32_t8 = _mm256_loadu_epi32(blockB(p, 0));
//         b1_packint32_t8 = _mm256_loadu_epi32(blockB(p, 8));

//         a_packint32_t8 = _mm256_set1_epi32(blockA(0, p));
//         C_buffer[0][0] = _mm256_fmadd_ps(a_packint32_t8, b0_packint32_t8, C_buffer[0][0]);
//         C_buffer[0][1] = _mm256_fmadd_ps(a_packint32_t8, b1_packint32_t8, C_buffer[0][1]);

//         a_packint32_t8 = _mm256_set1_epi32(blockA(1, p));
//         C_buffer[1][0] = _mm256_fmadd_ps(a_packint32_t8, b0_packint32_t8, C_buffer[1][0]);
//         C_buffer[1][1] = _mm256_fmadd_ps(a_packint32_t8, b1_packint32_t8, C_buffer[1][1]);
        
//         a_packint32_t8 = _mm256_set1_epi32(blockA(2, p));
//         C_buffer[2][0] = _mm256_fmadd_ps(a_packint32_t8, b0_packint32_t8, C_buffer[2][0]);
//         C_buffer[2][1] = _mm256_fmadd_ps(a_packint32_t8, b1_packint32_t8, C_buffer[2][1]);
        
//         a_packint32_t8 = _mm256_set1_epi32(blockA(3, p));
//         C_buffer[3][0] = _mm256_fmadd_ps(a_packint32_t8, b0_packint32_t8, C_buffer[3][0]);
//         C_buffer[3][1] = _mm256_fmadd_ps(a_packint32_t8, b1_packint32_t8, C_buffer[3][1]);

//         a_packint32_t8 = _mm256_set1_epi32(blockA(4, p));
//         C_buffer[4][0] = _mm256_fmadd_ps(a_packint32_t8, b0_packint32_t8, C_buffer[4][0]);
//         C_buffer[4][1] = _mm256_fmadd_ps(a_packint32_t8, b1_packint32_t8, C_buffer[4][1]);
        
//         a_packint32_t8 = _mm256_set1_epi32(blockA(5, p));
//         C_buffer[5][0] = _mm256_fmadd_ps(a_packint32_t8, b0_packint32_t8, C_buffer[5][0]);
//         C_buffer[5][1] = _mm256_fmadd_ps(a_packint32_t8, b1_packint32_t8, C_buffer[5][1]);
//     }
//     // store C from registers to memory
//     // for (int j = 0; j < 6; j++) {
//     //     _mm256_storeu_ps(&C[j * M], C_buffer[j][0]);
//     //     _mm256_storeu_ps(&C[j * M + 8], C_buffer[j][1]);
//     // }
//     for (int i = 0; i < 6; i++) {
//         _mm256_storeu_ps(C(i, 0), C_buffer[i][0]);
//         _mm256_storeu_ps(C(i, 8), C_buffer[i][1]);
//     }
// }

void pack_blockA(int32_t* A, int32_t* blockA, const int block_m, const int K)
{
    // cache friendly for read TODO: try cache friendly for write
        for (int j = 0; j < K; ++j)
    {
    for (int i = 0; i < block_m; ++i)
        {
            *blockA(i, j) = *A(i, j);
        }
    }
} 
void pack_blockB(int32_t* B, int32_t* blockB, const int block_n, const int K)
{
    // NOTE: blockB is row-major order, while B is column-major order
    // We test both cache friendly for read and write
    // And it turns out that prioritize blockB (write) is better
        for (int j = 0; j < block_n; ++j)
    {
    for (int i = 0; i < K; ++i)
        {
            *blockB(i, j) = *B(i, j) ;
        }
    }
} 

void matmul_kernel(int32_t* A, int32_t* B, int32_t* C, const int M, const int N, const int K) {
    assert(M % MR == 0);
    assert(N % NR == 0);
    for (int j = 0; j < N; j += NR) {
        pack_blockB(B(0, j), blockB_packed, NR, K);
        for (int i = 0; i < M; i += MR) {
            // kernel_16x6(&A[i], &B[j * K], &C[j * M + i], M, N, K);
            pack_blockA(A(i, 0), blockA_packed, MR, K);
            kernel_16x6(blockA_packed, blockB_packed, C(i, j), MR, NR, K, N);

        }
    }
}



void matmul_naive(int32_t* A, int32_t* B, int32_t* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int32_t acc = 0;
            for (int p = 0; p < K; p++) {
                // C[j * M + i] += A[p * M + i] * B[j * K + p];
                acc += *A(i, p) * *B(p, j);
            }
            *C(i, j) = acc;
        }
    }
}

void print_mat(int32_t* mat, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", mat[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void init_rand(int32_t* mat, const int M, const int N) {
    for (int i = 0; i < M * N; i++) {
        *mat++ = rand() ;
    }
}

void init_const(int32_t* mat, const int32_t value, const int M, const int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            *mat++ = value;
        }
    }
}

int compare_mats(int32_t* mat1, int32_t* mat2, const int M, const int N) {
    int num = 1200;
    float acc_err = 0.;
    // for (int i = 0; i < M; i++) 
    for (int i = M -1; i >= 0; i--) 
    {
        // for (int j = 0; j < N; j++) 
        for (int j = N - 1; j >= 0; j--) 
        {
            float err = fabsf( (float)mat1[i * N + j] - (float)mat2[i * N + j]);
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
    int32_t* A = (int32_t*)_mm_malloc(M * K * sizeof(int32_t), MEM_ALIGN);
    int32_t* B = (int32_t*)_mm_malloc(K * N * sizeof(int32_t), MEM_ALIGN);
    int32_t* C = (int32_t*)_mm_malloc(M * N * sizeof(int32_t), MEM_ALIGN);
    int32_t* C_ref = (int32_t*)_mm_malloc(M * N * sizeof(int32_t), MEM_ALIGN);
    // init_rand(A, M, K);
    // init_rand(B, K, N);
    init_const(A, M, K, 1);
    init_const(B, K, N, 1);
    printf(exp_setting);

#ifdef TEST
    matmul_naive(A, B, C_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        // init_const(C, 0.0, M, N);
        init_const(C, 0, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS = %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
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