/*
Activation: int8 quantization
weight: int4 quantization

# matrix A
- each ele is int8.
- element size: (M, K)
- storage memory type: int8
- storage size: (M, K)

# matrix B
- each ele is int4.
- element size: (K, M)
- storage memory type: uint8
- storage size: (K/2, M)

## scaling factor of B
- each ele is float32.
- element size: (K / 32, M)
- storage memory type: float32
- storage size: (K / 32, M)



quantization block size: 32
To get 32 ele from matrix A, we need 32 storage unit (int8).
To get 32 ele from matrix B, we need 16 storage unit (uint8).
*/

#include <cstring>
#include <vector>
#include <assert.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include "stdbool.h"
#include "ggml.h"
#include "ggml-cpu.h"

#include "lib.h"

#ifndef NTHREADS
#define NTHREADS 1
#endif

#ifndef MR
#define MR 4
#endif

#ifndef NR
#define NR 4
#endif

#ifndef MC
#define MC MR * 11 * NTHREADS
#endif

#ifndef NC
#define NC NR * 25 *  NTHREADS
#endif

#ifndef KC
#define KC 1024
#endif

#define MEM_ALIGN 64
#ifndef MDIM
    #define MDIM 1000
#endif

#ifndef NDIM
    #define NDIM 1000
#endif

#ifndef KDIM
    #define KDIM 1024
#endif

#ifndef NITER
    #define NITER 5
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C, SA
// column-major order: matrix B, SB
// A: M x K B: K x N C: M x N

#define A(i, j, ld) ( A + ( i ) * ( ld ) + ( j ) )
#define B(i, j, ld) ( B + ( j ) * ( ld ) + ( i ) )
#define C(i, j, ld) ( C + ( i ) * ( ld ) + ( j ) )
#define SA(i, j, ld) (SA + (i) * (ld) + (j))
#define ScaledSumA(i, j, ld) (ScaledSumA + (i) * (ld) + (j))
#define SB(i, j, ld) (SB + (j) * (ld) + (i))
#define MinB(i, j, ld) (MinB + (j) * (ld) + (i))

struct benchmark_params_struct {
    int32_t n_threads     = NTHREADS;
    int32_t n_iterations  = 10;
};
 
// static void ggml_graph_compute_helper(std::vector<uint8_t> & buf, ggml_cgraph * graph, int n_threads) {
//     struct ggml_cplan plan = ggml_graph_plan(graph, n_threads);

//     if (plan.work_size > 0) {
//         buf.resize(plan.work_size);
//         plan.work_data = buf.data();
//     }

//     ggml_graph_compute(graph, &plan);
// }

void matmul_kernel_ggml(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) 
{

}





void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB, float* MinB,
    const int M, const int N, const int K) {
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(K % Q_BLK_SIZE == 0);
    for (int i = 0; i < M; ++i) 
    for (int j = 0; j < N; ++j)
    {
        float sum = 0;
        // load 1 quantization blocks at once
        for (int p = 0; p < K; p += Q_BLK_SIZE )
        {
            // SA: row-major
            // SB: column-major
            int block_idx = p / Q_BLK_SIZE;
            float s_a = *SA(i, block_idx, KDIM / Q_BLK_SIZE);
            float s_b = *SB(block_idx, j, KDIM / Q_BLK_SIZE);
                
                
            float min_b = *MinB(block_idx, j, KDIM / Q_BLK_SIZE);
            float sum_a = 0; // the sum of quantized int8 value
            // 1 elements in B contains two int4 weights, matching 2 int8 activations 
            for (int q = 0; q < Q_BLK_SIZE/2; q ++ )
            {
                int8_t a_low = *A(i, p + q, KDIM); // get (p + q)-th element
                int8_t a_high = *A(i, p + q + Q_BLK_SIZE / 2, KDIM); // get (p + q + 16)
                uint8_t b = *B(p / 2 + q, j, KDIM / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f); 
                int8_t b_high = (b >> 4); 
                sum += (float)a_low * (float)b_low * s_a * s_b;
                sum += (float)a_high * (float)b_high * s_a * s_b;

                sum_a += (float) a_low;
                sum_a += (float) a_high;
            }
            sum += sum_a * s_a * min_b;
        }    
        *C(i, j, NDIM) = sum;
    }
}

void matmul_naive_no_packing_2(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* ScaledSumA, float* SB, float* MinB,
    const int M, const int N, const int K) {
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(K % Q_BLK_SIZE == 0);
    for (int i = 0; i < M; ++i) 
    for (int j = 0; j < N; ++j)
    {
        float sum = 0;
        // load 1 quantization blocks at once
        for (int p = 0; p < K; p += Q_BLK_SIZE )
        {
            // SA: row-major
            // SB: column-major
            int block_idx = p / Q_BLK_SIZE;
            float s_a = *SA(i, block_idx, KDIM / Q_BLK_SIZE);
            float s_b = *SB(block_idx, j, KDIM / Q_BLK_SIZE);
            float scaledSum = *ScaledSumA(i, block_idx, KDIM / Q_BLK_SIZE);
                
                
            float min_b = *MinB(block_idx, j, KDIM / Q_BLK_SIZE);
                
            // 1 elements in B contains two int4 weights, matching 2 int8 activations 
            for (int q = 0; q < Q_BLK_SIZE / 2; q += 1)
            {
                int8_t a_low = *A(i, p + q, KDIM); // get (p + q)-th element
                int8_t a_high = *A(i, p + q + Q_BLK_SIZE / 2, KDIM); // get (p + q + 16)
                uint8_t b = *B(p / 2 + q , j, KDIM / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f); 
                int8_t b_high = (b >> 4); 
                sum += (float)a_low * (float)b_low * s_a * s_b;
                sum += (float)a_high * (float)b_high * s_a * s_b;

            }
            sum += scaledSum * min_b;
        }    
        *C(i, j, NDIM) = sum;
    }
}




int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A  = (int8_t*)_mm_malloc(M * K * sizeof(int8_t), MEM_ALIGN);
    uint8_t* B = (uint8_t*)_mm_malloc(K / 2 * N * sizeof(uint8_t), MEM_ALIGN);
    float* sA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* scaledSumA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* MinB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    init_rand_min_max(A, M, K, -128, 127);
    init_rand_uint8(B, K / 2, N);
    init_rand(sA, M, K / Q_BLK_SIZE);
    init_rand(sB, K / Q_BLK_SIZE, N);
    init_rand(MinB, K / Q_BLK_SIZE, N);
    compute_scaled_sum(A, sA, scaledSumA, M, K);
    
    // ggml initialization
    const ggml_type qtype = GGML_TYPE_Q4_1;

    size_t ctx_size = 0;
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * N); // fp32 weight
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * M); // fp32 activation
    ctx_size += ggml_row_size(GGML_TYPE_F32, M * N); // fp32 output
    ctx_size += ggml_row_size(qtype, K * N); // fp32 activation
    ctx_size += 1024*1024*16;
    printf("Allocating Memory of size %zi bytes, %zi MB\n",ctx_size, (ctx_size/1024/1024));

    struct ggml_init_params params = {
        /*.mem_size   =*/ ctx_size,
        /*.mem_buffer =*/ NULL,
        /* no_alloc   =*/ 0
    };

    struct ggml_context * ctx;
    ctx = ggml_init(params);
    if (!ctx) {
        fprintf(stderr, "%s: ggml_init() failed\n", __func__);
        return 1;
    }
    printf("Creating new tensors\n");
    // printf("Creating new tensor m1\n");
    struct ggml_tensor * weight = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, KDIM, NDIM);
    ggml_set_f32(weight, 1.0f);

    // printf("Creating new tensor m2\n");
    struct ggml_tensor * activation = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, KDIM, MDIM);
    ggml_set_f32(activation, 2.0f);
    
    int32_t nelements = K * N;
    // printf("Creating new tensor q11 & Running quantize\n");
    struct ggml_tensor * q4_weight = ggml_new_tensor_2d(ctx, qtype, K, N);
    ggml_quantize_chunk(
        qtype, (const float *) weight->data, q4_weight->data, 0, nelements/q4_weight->ne[0], q4_weight->ne[0], nullptr
    );

    // Set up a the compute graph
    // printf("Creating new tensor q31\n");
    struct ggml_tensor * output = ggml_mul_mat(ctx, q4_weight, activation);
    struct ggml_cgraph * graph = ggml_new_graph(ctx);
    ggml_build_forward_expand(graph, output);
    struct benchmark_params_struct benchmark_params;
    printf("n_threads=%i\n", benchmark_params.n_threads);
    std::vector<uint8_t> work_buffer;

    
    

#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    // matmul_naive_no_packing(A, B, C_ref, sA, sB, MinB, M, N, K);
    matmul_naive_no_packing_2(A, B, C_ref, sA, scaledSumA, sB, MinB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    printf("\n\e[32m[INFO]\e[m MR=%d, NR=%d, MC=%d, NC=%d, KC=%d, M=%d, K=%d, N=%d\n", MR, NR, MC, NC, KC, M, K, N);
    for (int i = 0; i < NITER; i++) {


    // init_rand(blockA_packed, MR, K);
    // init_rand(blockB_packed, K, NR);
        
    
        init_const(C, 0.0, M, N);
        uint64_t start = timer();
        ggml_graph_compute_with_ctx(ctx, graph, benchmark_params.n_threads);
        struct ggml_tensor * result =  ggml_graph_node(graph, -1);
        memcpy(C, result->data, ggml_nbytes(result));
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        print_mat(C, 5, 5);
        print_mat(C_ref, 5, 5);
#ifdef TEST
        int res = compare_mats(C, C_ref, M, N);
        // if (!res) break;
#endif
        printf("\n");
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    _mm_free(C_ref);

    return 0;
}
