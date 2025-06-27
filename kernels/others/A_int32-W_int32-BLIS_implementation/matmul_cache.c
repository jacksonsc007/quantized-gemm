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



static int32_t blockA_packed[MC * KC] __attribute__((aligned(64)));
static int32_t blockB_packed[KC * NC] __attribute__((aligned(64)));

/*
blockA should be column-major order
blockB should be row-major order
*/
void kernel_16x6(int32_t* blockA, int32_t* blockB, int32_t* C, const int valid_m, const int valid_n, const int block_m, const int block_n, const int block_K, const int N) {
    __m256i C_buffer[6][2];
    __m256i a_pack;
    __m256i b0_pack;
    __m256i b1_pack;
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
            C_buffer[i][0] = _mm256_maskload_epi32(C(i, 0), masks[0]);
            C_buffer[i][1] = _mm256_maskload_epi32(C(i, 8), masks[1]);
        }
                                     
    } 
    else
    {
        for (int i = 0; i < valid_m; i++) {
            C_buffer[i][0] = _mm256_loadu_si256((__m256i*)C(i, 0));
            C_buffer[i][1] = _mm256_loadu_si256((__m256i*)C(i, 8));
        }
    }

    for (int p = 0; p < block_K; p++) {

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
    if (valid_n != NR)
    {
        for (int i = 0; i < valid_m; i++) {
            _mm256_maskstore_epi32(C(i, 0), masks[0], C_buffer[i][0]);
            _mm256_maskstore_epi32(C(i, 8), masks[1], C_buffer[i][1]);

        }
    }
    else
    {
        for (int i = 0; i < valid_m; i++) {
            _mm256_storeu_si256((__m256i*)C(i, 0), C_buffer[i][0]);
            _mm256_storeu_si256((__m256i*)C(i, 8), C_buffer[i][1]);
        }
    }
}

// void kernel_16x6(int32_t* blockA, int32_t* blockB, int32_t* C, const int valid_m, const int valid_n, const int block_m, const int block_n, const int block_K, const int N, int row_index, int col_index, int32_t* C_)
// {
//     for ( int i = 0; i < valid_m; i++)
//     for ( int j = 0; j < valid_n; j++)
//     {
//         int32_t sum = 0.;
//         for ( int k = 0; k < block_K; k++)
//         {
//             int32_t a = *blockA(i, k);
//             int32_t b = *blockB(k, j);
//             if (a == 0 || b == 0)
//             {
//                 printf("\e[31m[ERROR]\e[m Access invalid memory\n");
//             }
//             sum += a * b;

//         }
//         *C(i, j) += sum;
//         // *C(i, j) = *C(i, j) + sum;
//         // int32_t cur = *(C + i * N + j);
//         // *(C + i * N + j) = cur + sum;
            
//         // *(C_ + (row_index + i) * N + col_index + j) += sum;
//             //
//         // int32_t * gt_addr = (C_ + (row_index + i) * N + col_index + j);
//         // int32_t * addr = C(i, j);
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
void pack_blockA_panel(int32_t* A, int32_t* blockA, const int kc, const int mc, const int K)
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
void pack_blockA(int32_t* A, int32_t* blockA, const int mc, const int kc, const int K)
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

void pack_blockB_panel(int32_t *B, int32_t *blockB, const int nc, const int kc, const int K)
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

void pack_blockB(int32_t* B, int32_t* blockB, const int nc, const int kc, const int K)
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
void matmul_kernel(int32_t* A, int32_t* B, int32_t* C, const int M, const int N, const int K) {
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
                        int32_t *blockA_panel = blockA_packed + ir * valid_KC;
                        int32_t *blockB_panel = blockB_packed + jr * valid_KC;
                        // printf("C[%d, %d] + [%d, %d]\n", ic + ir, jc + jr, valid_MR, valid_NR);
                        kernel_16x6(blockA_panel, blockB_panel, C(ic + ir, jc + jr), valid_MR, valid_NR, valid_MR, valid_NR, valid_KC, N);
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
    init_const(A, M, K, 10);
    init_const(B, K, N, 3);
    // printf(exp_setting);

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