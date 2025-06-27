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
    #define MDIM 1000
#endif

#ifndef NDIM
    #define NDIM 1000
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
#define A(i, j) ( A + ( i ) * K + ( j ) )
#define B(i, j) ( B + ( j ) * K + ( i ) )
#define C(i, j) ( C + ( i ) * N + ( j ) )

// column-major shape (block_m, K)
#define blockA(i, j) ( blockA + ( j ) * block_m + ( i ) )
// row-major shape (K, block_n)
#define blockB(i, j) ( blockB + ( i ) * block_n + ( j ) )



static float blockA_packed[MC * KC] __attribute__((aligned(64)));
static float blockB_packed[KC * NC] __attribute__((aligned(64)));

/*
blockA should be column-major order
blockB should be row-major order
*/
void microkernel_16x6(float* blockA, float* blockB, float* C, const int valid_m, const int valid_n, const int block_m, const int block_n, const int block_K, const int N) {
    __m256 C_buffer[6][2];
    __m256 a_packFloat8;
    __m256 b0_packFloat8;
    __m256 b1_packFloat8;
    __m256i masks[2];
    // load C from memory to registers
    // i: row index
    if (valid_n != NR)
    {
        const unsigned int bit_mask = 65535;
        masks[0] = _mm256_setr_epi32(bit_mask << (valid_n + 15),
                                     bit_mask << (valid_n + 14),
                                     bit_mask << (valid_n + 13),
                                     bit_mask << (valid_n + 12),
                                     bit_mask << (valid_n + 11),
                                     bit_mask << (valid_n + 10),
                                     bit_mask << (valid_n + 9),
                                     bit_mask << (valid_n + 8));
        masks[1] = _mm256_setr_epi32(bit_mask << (valid_n + 7),
                                     bit_mask << (valid_n + 6),
                                     bit_mask << (valid_n + 5),
                                     bit_mask << (valid_n + 4),
                                     bit_mask << (valid_n + 3),
                                     bit_mask << (valid_n + 2),
                                     bit_mask << (valid_n + 1),
                                     bit_mask <<  valid_n);
        for (int i =  0; i < valid_m; i++)
        {
            C_buffer[i][0] = _mm256_maskload_ps(C(i, 0), masks[0]);
            C_buffer[i][1] = _mm256_maskload_ps(C(i, 8), masks[1]);
        }
                                     
    } 
    else
    {
        for (int i = 0; i < valid_m; i++) {
            C_buffer[i][0] = _mm256_loadu_ps(C(i, 0));
            C_buffer[i][1] = _mm256_loadu_ps(C(i, 8));
        }
    }

    for (int p = 0; p < block_K; p++) {
        b0_packFloat8 = _mm256_loadu_ps(blockB(p, 0));
        b1_packFloat8 = _mm256_loadu_ps(blockB(p, 8));

        a_packFloat8 = _mm256_broadcast_ss(blockA(0, p));
        C_buffer[0][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[0][0]);
        C_buffer[0][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[0][1]);

        a_packFloat8 = _mm256_broadcast_ss(blockA(1, p));
        C_buffer[1][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[1][0]);
        C_buffer[1][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[1][1]);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(2, p));
        C_buffer[2][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[2][0]);
        C_buffer[2][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[2][1]);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(3, p));
        C_buffer[3][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[3][0]);
        C_buffer[3][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[3][1]);

        a_packFloat8 = _mm256_broadcast_ss(blockA(4, p));
        C_buffer[4][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[4][0]);
        C_buffer[4][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[4][1]);
        
        a_packFloat8 = _mm256_broadcast_ss(blockA(5, p));
        C_buffer[5][0] = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C_buffer[5][0]);
        C_buffer[5][1] = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C_buffer[5][1]);
    }
    if (valid_n != NR)
    {
        for (int i = 0; i < valid_m; i++) {
            _mm256_maskstore_ps(C(i, 0), masks[0], C_buffer[i][0]);
            _mm256_maskstore_ps(C(i, 8), masks[1], C_buffer[i][1]);

        }
    }
    else
    {
        for (int i = 0; i < valid_m; i++) {
            _mm256_storeu_ps(C(i, 0), C_buffer[i][0]);
            _mm256_storeu_ps(C(i, 8), C_buffer[i][1]);
        }
    }
}

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
void pack_blockA_panel(float* A, float* blockA, const int kc, const int mc, const int K)
{
    // int block_m = MR;
    for( int j = 0; j < kc; ++j)
    {
        for ( int i = 0; i < mc; ++i)
        {
            *(blockA) = *A(i, j);
            blockA++;
            // *(blockA(i, j )) = *A(i, j);
        }
    }

}
void pack_blockA(float* A, float* blockA, const int mc, const int kc, const int K)
{
    for (int i = 0; i < mc; i += MR)
    {
        int valid_m = min(mc - i, MR);
        // printf("\e[31m[ERROR]\e[m The valid_m = %d\n", valid_m);
        // if (valid_m != MC)
        // {
        //     printf("\e[31m[ERROR]\e[m The valid_m is not MC\n");
        // }
        pack_blockA_panel(
            A(i, 0),
            blockA + i * kc,
            kc,
            valid_m,
            K
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

void pack_blockB_panel(float *B, float *blockB, const int nc, const int kc, const int K)
{
    // int block_n = NR;
    for (int i = 0; i < kc; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            *(blockB++) = *B(i, j);
            // *(blockB( i, j )) = *B(i, j);
        }
    }
}

void pack_blockB(float* B, float* blockB, const int nc, const int kc, const int K)
{   
    for (int jc = 0; jc < nc; jc += NR)
    {
        int valid_NR = min(nc - jc, NR);
        pack_blockB_panel(
            B(0, jc),
            blockB + jc * kc, 
            valid_NR,
            kc,
            K
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
                B(pc, jc),
                blockB_packed,
                valid_NC,
                valid_KC,
                K
            );
            for (int ic = 0; ic < M; ic += MC)
            {
                int valid_MC = min(M - ic, MC); 
                pack_blockA(
                    A(ic, pc),
                    blockA_packed,
                    valid_MC,
                    valid_KC,
                    K
                );
                for (int jr = 0; jr < valid_NC; jr += NR)
                {
                    int valid_NR = min(valid_NC - jr, NR);
                    for (int ir = 0; ir < valid_MC; ir += MR)
                    {
                        int valid_MR = min(valid_MC - ir, MR);
                        float *blockA_panel = blockA_packed + ir * valid_KC;
                        float *blockB_panel = blockB_packed + jr * valid_KC;
                        // printf("C[%d, %d] + [%d, %d]\n", ic + ir, jc + jr, valid_MR, valid_NR);
                        microkernel_16x6(blockA_panel, blockB_panel, C(ic + ir, jc + jr), valid_MR, valid_NR, valid_MR, valid_NR, valid_KC, N);
                        // kernel_16x6(blockA_panel, blockB_panel, C(ic + ir, jc + jr), valid_MR, valid_NR, valid_MR, valid_NR, valid_KC, N, ic+ir, jc+jr, C);
                        // kernel_16x6(blockA_panel, blockB_panel, C((ic + ir), (jc + jr)), valid_MR, valid_NR, valid_MR, valid_NR, valid_KC, N, ic+ir, jc+jr, C);
                        // kernel_16x6(blockA_panel, blockB_panel, C((ic + ir), (jc + jr)), valid_MR, valid_NR, valid_MR, valid_NR, valid_KC, N, ic+ir, jc+jr, C);
                        // kernel_16x6(blockA_panel, blockB_panel, C((ic + ir), (jc + jr)), valid_MR, valid_NR, MR, NR, valid_KC, N, ic+ir, jc+jr, C);

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
                acc += *A(i, p) * *B(p, j);
            }
            *C(i, j) = acc;
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
    printf("average error: %f\n", acc_err / (M * N));
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
        printf("GFLOPS = %.3f for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
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