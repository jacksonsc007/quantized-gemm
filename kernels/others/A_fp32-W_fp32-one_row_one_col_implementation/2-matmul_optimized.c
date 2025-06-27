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

#define MR 6
#define NR 16
// #define MC MR *256
// #define NC NR * 64
#define MC MR * 64
#define NC NR * 256
// #define MC 200
// #define NC 400

#define KC 2000

#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM MC * 1
#endif

#ifndef NDIM
    #define NDIM NC * 1
#endif

#ifndef KDIM
    #define KDIM 1000
#endif

#ifndef NITER
    #define NITER 100
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j, ld) ( A + ( i ) * ( ld ) + ( j ) )
#define B(i, j, ld) ( B + ( j ) * ( ld ) + ( i ) )
#define C(i, j, ld) ( C + ( i ) * ( ld ) + ( j ) )

// row-major shape (block_m, block_k)
#define blockA(i, j, ld) ( blockA + (i) * (ld) + (j) )
// column-major shape (block_k, block_n)
#define blockB(i, j, ld) ( blockB + (j) * (ld) + (i) )



static float blockA_packed[MC * KC] __attribute__((aligned(64)));
static float blockB_packed[KC * NC] __attribute__((aligned(64)));

void kernel_6x16(float* blockA, float* blockB, float* C, const int mc, const int nc, const int kc) 
{
    __m256 a0, a1, a2, a3, a4, a5, b0;
    __m256 acc0, acc1, acc2, acc3, acc4, acc5;
    __m256 tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
    __m128 sub0, sub1, sub;

    for (int i = 0; i < MR; i += 6)
    {
        for (int j = 0; j < NR; j++)
        {
            acc0 = _mm256_setzero_ps();
            acc1 = _mm256_setzero_ps();
            acc2 = _mm256_setzero_ps();
            acc3 = _mm256_setzero_ps();
            acc4 = _mm256_setzero_ps();
            acc5 = _mm256_setzero_ps();

            for (int p = 0; p < kc; )
            {
                a0 = _mm256_loadu_ps(blockA(i, p, kc));
                a1 = _mm256_loadu_ps(blockA(i + 1, p, kc));
                a2 = _mm256_loadu_ps(blockA(i + 2, p, kc));
                a3 = _mm256_loadu_ps(blockA(i + 3, p, kc));
                a4 = _mm256_loadu_ps(blockA(i + 4, p, kc));
                a5 = _mm256_loadu_ps(blockA(i + 5, p, kc));
                b0 = _mm256_loadu_ps(blockB(p, j, kc));

                acc0 = _mm256_fmadd_ps(a0, b0, acc0);
                acc1 = _mm256_fmadd_ps(a1, b0, acc1);
                acc2 = _mm256_fmadd_ps(a2, b0, acc2);
                acc3 = _mm256_fmadd_ps(a3, b0, acc3);
                acc4 = _mm256_fmadd_ps(a4, b0, acc4);
                acc5 = _mm256_fmadd_ps(a5, b0, acc5);

                p += 8;
            }

            // Reduce the accumulators to a single value
            tmp0 = _mm256_hadd_ps(acc0, acc0);
            tmp0 = _mm256_hadd_ps(tmp0, tmp0);
            sub0 = _mm256_extractf128_ps(tmp0, 0);
            sub1 = _mm256_extractf128_ps(tmp0, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i, j, NDIM) = _mm_cvtss_f32(sub);

            tmp1 = _mm256_hadd_ps(acc1, acc1);
            tmp1 = _mm256_hadd_ps(tmp1, tmp1);
            sub0 = _mm256_extractf128_ps(tmp1, 0);
            sub1 = _mm256_extractf128_ps(tmp1, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i + 1, j, NDIM) = _mm_cvtss_f32(sub);

            tmp2 = _mm256_hadd_ps(acc2, acc2);
            tmp2 = _mm256_hadd_ps(tmp2, tmp2);
            sub0 = _mm256_extractf128_ps(tmp2, 0);
            sub1 = _mm256_extractf128_ps(tmp2, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i + 2, j, NDIM) = _mm_cvtss_f32(sub);

            tmp3 = _mm256_hadd_ps(acc3, acc3);
            tmp3 = _mm256_hadd_ps(tmp3, tmp3);
            sub0 = _mm256_extractf128_ps(tmp3, 0);
            sub1 = _mm256_extractf128_ps(tmp3, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i + 3, j, NDIM) = _mm_cvtss_f32(sub);

            tmp4 = _mm256_hadd_ps(acc4, acc4);
            tmp4 = _mm256_hadd_ps(tmp4, tmp4);
            sub0 = _mm256_extractf128_ps(tmp4, 0);
            sub1 = _mm256_extractf128_ps(tmp4, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i + 4, j, NDIM) = _mm_cvtss_f32(sub);

            tmp5 = _mm256_hadd_ps(acc5, acc5);
            tmp5 = _mm256_hadd_ps(tmp5, tmp5);
            sub0 = _mm256_extractf128_ps(tmp5, 0);
            sub1 = _mm256_extractf128_ps(tmp5, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i + 5, j, NDIM) = _mm_cvtss_f32(sub);
        }
    }
}
/*
blockA should be column-major order
blockB should be row-major order
*/
/* void kernel_6x16(float* blockA, float* blockB, float* C, const int mc, const int nc, const int kc) 
{

    // make sure block_k is divisible by 
    __m256 a0, b0;
    __m256 acc;
    __m256 tmp;
    __m128 sub1, sub0, sub;
    for (int i = 0; i < MR; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            acc = _mm256_setzero_ps();
            for (int p = 0; p < kc; )
            {
                a0 = _mm256_loadu_ps(blockA(i, p, kc));
                b0 = _mm256_loadu_ps(blockB(p, j, kc));
                acc = _mm256_fmadd_ps(a0, b0, acc);
                // each ymm could hold 8 float numbers
                p += 8;            
            }
            // add vertically the 8 floats in acc
            tmp = _mm256_hadd_ps(acc, acc);
            tmp = _mm256_hadd_ps(tmp, tmp);
            sub0 = _mm256_extractf128_ps(tmp, 0);
            sub1 = _mm256_extractf128_ps(tmp, 1);
            sub = _mm_add_ps(sub0, sub1);
            *C(i, j, NDIM) = _mm_cvtss_f32(sub);
        }
    }
} */

// void kernel_16x6(float* blockA, float* blockB, float* C, const int valid_m, const int valid_n, const int block_m, const int block_n, const int block_K, const int N, int row_index, int col_index, float* C_)
// {
//     for ( int i = 0; i < valid_m; i++)
//     for ( int j = 0; j < valid_n; j++)
//     {
//         float sum = 0.;
//         for ( int k = 0; k < block_K; k++)
//         {
//             float a = *blockA(i, k);
//             float b = *blockB(k, j);
//             if (a == 0 || b == 0)
//             {
//                 printf("\e[31m[ERROR]\e[m Access invalid memory\n");
//             }
//             sum += a * b;

//         }
//         *C(i, j) += sum;
//         // *C(i, j) = *C(i, j) + sum;
//         // float cur = *(C + i * N + j);
//         // *(C + i * N + j) = cur + sum;
            
//         // *(C_ + (row_index + i) * N + col_index + j) += sum;
//             //
//         // float * gt_addr = (C_ + (row_index + i) * N + col_index + j);
//         // float * addr = C(i, j);
//         // if (gt_addr != addr)
//         // {
//         //     printf("\e[31m[ERROR]\e[m The address is not correct\n");
//         //     printf("gt_addr: %p, addr: %p\n", gt_addr, addr);
//         //     exit(1);
//         // }
//             //
//         // printf("element address: %p\n", C + i * N + j);
//         // print the pointer of the element
//         // print("")
//         // printf("\e[32m[INFO]\e[m  (%d, %d) = %f\n", row_index + i, col_index + j, cur + sum);
//     }
    
// }




/*
Pack A into the memory layout required by the kernel

A: The origianl matrix A
K: The hidden dimension of the original matrix A, we need it to retrieve elements, since A is row-major.
blockA: The packed matrix A. blockA is column-major, with static shape: MC * KC. NOTE some parts may not be valid
valid_m: The valid row of blockA
valid_k: The valid column of blockA
the valid elements are in matrix of size (valid_m * valid_k)
*/
void pack_blockA_panel(float* A, float* blockA, const int kc, const int mc)
{
    for ( int i = 0; i < mc; ++i)
    {
        for( int j = 0; j < kc; ++j)
        {
            *(blockA(i, j, kc)) = *A(i, j, KDIM);
        }
    }

}
void pack_blockA(float* A, float* blockA, const int mc, const int kc)
{
    for (int i = 0; i < mc; i += MR)
    {
        int valid_m = min(mc - i, MR);
        pack_blockA_panel(
            A(i, 0, KDIM),
            blockA + i * kc,
            kc,
            valid_m
        );
    }
} 

/*
Pack B into the memory layout required by the kernel
B: The origianl matrix B
K: The hidden dimension of the original matrix A, we need it to retrieve elements, since B is column-major.
blockB: The packed matrix B. blockA is column-major, with static shape: KC * NC. NOTE some parts may not be valid
valid_n: The valid cloumns of blockB
valid_k: The valid rows of blockA
The valid elements of blockB are from 0 to (valid_n * valid_k)
*/

void pack_blockB_panel(float *B, float *blockB, const int kc, const int nc)
{
    // int block_n = NR;
    for (int j = 0; j < nc; j++)
    {
        for (int i = 0; i < kc; i++)
        {
            *(blockB(i, j, kc)) = *B(i, j, KDIM);
        }
    }
}

void pack_blockB(float* B, float* blockB, const int nc, const int kc)
{   
    for (int jc = 0; jc < nc; jc += NR)
    {
        int valid_NR = min(nc - jc, NR);
        pack_blockB_panel(
            B(0, jc, KDIM),
            blockB + jc * kc, 
            kc,
            valid_NR
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
    for (int jc = 0; jc < N; jc += NC)
    {
        int valid_NC = min(N - jc, NC);
        for (int pc = 0; pc < K; pc += KC)
        {
            int valid_KC = min(K - pc, KC);
            pack_blockB(
                B(pc, jc, K),
                blockB_packed,
                valid_NC,
                valid_KC
            );
            for (int ic = 0; ic < M; ic += MC)
            {
                int valid_MC = min(M - ic, MC); 
                pack_blockA(
                    A(ic, pc, K),
                    blockA_packed,
                    valid_MC,
                    valid_KC
                );
                for (int jr = 0; jr < valid_NC; jr += NR)
                {
                    int valid_NR = min(valid_NC - jr, NR);
                    for (int ir = 0; ir < valid_MC; ir += MR)
                    {
                        int valid_MR = min(valid_MC - ir, MR);
                        float *blockA_panel = blockA_packed + ir * valid_KC;
                        float *blockB_panel = blockB_packed + jr * valid_KC;
                        kernel_6x16(blockA_panel, blockB_panel, C(ic + ir, jc + jr, NDIM), valid_MR, valid_NR, valid_KC);
                    // void kernel_6x16(float* blockA, float* blockB, float* C, const int mc, const int nc, const int kc) 

                    }
                }
            }
        }
    }
}



void matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float acc = 0;
            for (int p = 0; p < K; p++) {
                // C[j * M + i] += A[p * M + i] * B[j * K + p];
                acc += *A(i, p, KDIM) * *B(p, j, KDIM);
            }
            *C(i, j, NDIM) = acc;
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
    if (avg_err > 1e-3) return 0;
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
    float* A = (float*)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
    float* B = (float*)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    init_rand(A, M, K);
    init_rand(B, K, N);
    

    printf("\e[31m[INFO]\e[m KC: %d, MC: %d, NC: %d, MR: %d, NR: %d\n", KC, MC, NC, MR, NR);

#ifdef TEST
    matmul_naive(A, B, C_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;

    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
    // init blockB_packed and blockA_packed
    // init_const(blockA_packed, UN_INIT, MC, KC);
    // init_const(blockB_packed, UN_INIT, KC, NC);
        
    
        init_const(C, 0.0, M, N);
        // init_const(C, -1000, M, N);
        uint64_t start = timer();
        matmul_kernel(A, B, C, M, N, K);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("\033[94mGFLOPS\033[0m = %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
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
            break;
        }
#endif
        printf("\n");
    }

    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);

    return 0;
}