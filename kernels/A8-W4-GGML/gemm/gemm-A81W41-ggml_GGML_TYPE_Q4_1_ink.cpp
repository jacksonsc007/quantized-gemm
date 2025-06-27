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

#include <blis/cblas.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <assert.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include "stdbool.h"
#include "ggml.h"
#include "ggml-cpu.h"
#include "ggml-quants.h"
#include "ggml-impl.h"

#include "lib.h"

#ifndef NTHREADS
#define NTHREADS 1
#endif

#define MEM_ALIGN 64

#ifndef MDIM
    #define MDIM 1000
#endif

#ifndef NDIM
    // #define NDIM 64
    #define NDIM 1000
#endif

#ifndef KDIM
    #define KDIM 1024
    // #define KDIM 64
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
};
/* 
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
} */

void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float * ScaledSumA,
    float* SB, float * MinB,
    const int M, const int N, const int K) {
    // assert(M % MR == 0);
    // assert(N % NR == 0);
    assert(K % Q_BLK_SIZE == 0);
    for (int i = 0; i < M; ++i) 
    for (int j = 0; j < N; ++j)
    {
        float sum = 0;
        // load 2 quantization blocks at once
        for (int p = 0; p < K; p += Q_BLK_SIZE * 2)
        {
            // SA: row-major
            // SB: column-major
            long long sum_int8_0 = 0;
            long long sum_int8_1 = 0;
            int block_idx = p / Q_BLK_SIZE;
            float s_a_0 = *SA(i, block_idx, KDIM / Q_BLK_SIZE);
            float s_a_1 = *SA(i, block_idx + 1, KDIM / Q_BLK_SIZE);
            float s_b_0 = *SB(block_idx, j, KDIM / Q_BLK_SIZE);
            float s_b_1 = *SB(block_idx + 1, j, KDIM / Q_BLK_SIZE);
                
                
            float min_b_0 = *MinB(block_idx, j, KDIM / Q_BLK_SIZE);
            float min_b_1 = *MinB(block_idx + 1, j, KDIM / Q_BLK_SIZE);
            float sum_a_0 = *ScaledSumA(i, block_idx, KDIM / Q_BLK_SIZE);
            float sum_a_1 = *ScaledSumA(i, block_idx + 1, KDIM / Q_BLK_SIZE);
            for (int q = 0; q < Q_BLK_SIZE; q++)
            {
                // due to q4_0 memory strategy, the first byte consists of w0 and w16, so we need to get a0 and a16
                // origin order: (w0,w1), (w2,w3), (w4,w5), (w6,w7), (w8, w9), ... (w15,w31)
                // QM_ARM order: (w0,w16),(w1,w17),(w2,w18),(w3,w19),(w4, w20),... (w15,w31)
                //               |--|
                //               4 bits
                //               |------|
                //               8 bits (byte)
                //            low|----------------------------------------------------------|high
                //               0                         128 bit
                int8_t a_low = *A(i, p + q, KDIM);
                int8_t a_high = *A(i, p + q + Q_BLK_SIZE, KDIM); 
                uint8_t b = *B(p / 2 + q, j, KDIM / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f); // first block
                int8_t b_high = (b >> 4);  // second block
                // int8_t b_high = (b & 0x0f) - 8;
                // int8_t b_low = (b >> 4) - 8;
                // if (s_a == -1.0 || s_b == -1.0 || a == -1 || b == -1)
                // {
                //     printf("\e[31m[ERROR]\e[m Access invalid memory\n");
                // }
                sum_int8_0 += (long long)a_low * (long long)b_low;
                sum_int8_1 += (long long)a_high * (long long)b_high;
            }
            sum += (float) sum_int8_0 * s_a_0 * s_b_0;
            sum += (float) sum_int8_1 * s_a_1 * s_b_1;
            sum += sum_a_0  * min_b_0;
            sum += sum_a_1  * min_b_1;
        }    
        *C(i, j, NDIM) = sum;
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

void print_q8_1_data(block_q8_0 * GGML_RESTRICT blocks, size_t n_blocks) {

    for (size_t b = 0; b < n_blocks; ++b) {
         block_q8_0 blk = blocks[b];
        float scale = GGML_FP16_TO_FP32 (blk.d);
        printf("Block %zu (scale = %.6f):\n", b, scale);
        for (int i = 0; i < QK8_0; ++i) {
            int32_t q = (int32_t) blk.qs[i];
            float val =  scale * (float) q;
            printf(" %6.2f", val);
            if ((i + 1) % 8 == 0) printf(" |");
            if ((i + 1) % 16 == 0) printf("\n");
        }
        printf("\n\n");
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
    float* C_float_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    // init_rand_min_max(A, M, K, -128, 127);
    // init_rand_uint8(B, K / 2, N);
    // init_rand(sA, M, K / Q_BLK_SIZE);
    // init_rand(sB, K / Q_BLK_SIZE, N);
    // init_rand(MinB, K / Q_BLK_SIZE, N);
    // compute_scaled_sum(A, sA, scaledSumA, M, K);

    // init_const(sB, -1, K / Q_BLK_SIZE, N);
    
    // ggml initialization
    const ggml_type weight_q_type = GGML_TYPE_Q4_1_ink;
    const ggml_type activation_q_type = GGML_TYPE_Q8_1_ink;

    size_t ctx_size = 0;
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * N); // fp32 weight
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * M); // fp32 activation
    ctx_size += ggml_row_size(GGML_TYPE_F32, M * N); // fp32 output
    ctx_size += ggml_row_size(weight_q_type, K * N); // int4 weight
    ctx_size += ggml_row_size(activation_q_type, K * M); // int8 activation
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
    struct ggml_tensor * weight = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, K, N);
    // ggml_set_f32(weight, 10.0f);
    init_rand((float *)weight->data, K, N );


    struct ggml_tensor * activation = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, K, M);
    // ggml_set_f32(activation, 200.0f);
    init_rand((float *)activation->data, K, M);

    
    int32_t nelements = K * N;
    // quantize the weight tensor to int4
    printf("\e[32m[INFO]\e[m quantize weight\n");
    struct ggml_tensor * q4_weight = ggml_new_tensor_2d(ctx, weight_q_type, K, N);
    ggml_quantize_chunk(
        weight_q_type, (const float *) weight->data, q4_weight->data, 0, nelements/q4_weight->ne[0], q4_weight->ne[0], nullptr
    );
    // quantize the activation tensor to int8
    printf("\e[32m[INFO]\e[m quantize activation\n");
    struct ggml_tensor * q8_activation = ggml_new_tensor_2d(ctx, activation_q_type, K, M);
    // print_q8_0_data((block_q8_0*)q8_activation->data, 2);
    nelements = K * M;
    ggml_quantize_chunk(
        activation_q_type, (const float *) activation->data, q8_activation->data, 0, nelements/q8_activation->ne[0], q8_activation->ne[0], nullptr
    );
    
    // print_q8_0_data((block_q8_0*)q8_activation->data, 2);
    printf("\e[32m[INFO]\e[m Copy quantized tensor for evaluation\n");
    block_q8_1_ink * q8_blocks = (block_q8_1_ink *)q8_activation->data;
    int32_t num_q8_blocks = q8_activation->ne[0] * q8_activation->ne[1] / QK8_1_ink;
    for (int block_idx = 0; block_idx < num_q8_blocks; ++block_idx) {
        block_q8_1_ink blk = q8_blocks[block_idx];
        float scale_1 = GGML_FP16_TO_FP32(blk.d1);
        float scaled_sum_1 = GGML_FP16_TO_FP32(blk.s1);
        float scale_2 = GGML_FP16_TO_FP32(blk.d2);
        float scaled_sum_2 = GGML_FP16_TO_FP32(blk.s2);

        sA[2 * block_idx] = scale_1;
        sA[2 * block_idx + 1] = scale_2;
        scaledSumA[2 * block_idx] = scaled_sum_1;
        scaledSumA[2 * block_idx + 1] = scaled_sum_2;
        memcpy(A + block_idx * QK8_1_ink, blk.qs1, QK8_1_ink / 2 * sizeof(int8_t));
        memcpy(A + block_idx * QK8_1_ink + QK8_1_ink / 2, blk.qs2, QK8_1_ink / 2 * sizeof(int8_t));
    }
    block_q4_1_ink * q4_blocks = (block_q4_1_ink *)q4_weight->data;
    int32_t num_q4_blocks = q4_weight->ne[0] * q4_weight->ne[1] / QK4_1_ink;
    for (int block_idx = 0; block_idx < num_q4_blocks; ++block_idx) {
        block_q4_1_ink blk = q4_blocks[block_idx];
        float scale_1 = GGML_FP16_TO_FP32(blk.d1);
        float min_value_1 = GGML_FP16_TO_FP32(blk.m1);
        float scale_2 = GGML_FP16_TO_FP32(blk.d2);
        float min_value_2 = GGML_FP16_TO_FP32(blk.m2);

        sB[2 * block_idx] = scale_1;
        sB[2 * block_idx + 1] = scale_2;
        MinB[2 * block_idx] = min_value_1;
        MinB[2 * block_idx + 1] = min_value_2;
        memcpy(B + block_idx * QK4_1_ink / 2, blk.qs, QK4_1_ink / 2 * sizeof(uint8_t));
    }
    


    // Set up a the compute graph
    printf("\e[32m[INFO]\e[m Building Graph\n");
    // struct ggml_tensor * output = ggml_mul_mat(ctx, q4_weight, activation);
    struct ggml_tensor * ggml_result = ggml_mul_mat(ctx, q4_weight, q8_activation);
    struct ggml_cgraph * graph = ggml_new_graph(ctx);
    ggml_build_forward_expand(graph, ggml_result);
    struct benchmark_params_struct benchmark_params;
    printf("n_threads=%i\n", benchmark_params.n_threads);
    std::vector<uint8_t> work_buffer;

    
    // buffers to clear the cache
    const int M_ = 1000;
    const int K_ = 1000;
    const int N_ = 1000;
    float* A_ = (float*)_mm_malloc(M_ * K_ * sizeof(float), MEM_ALIGN);
    float* B_ = (float*)_mm_malloc(K_ * N_ * sizeof(float), MEM_ALIGN);
    float* C_ = (float*)_mm_malloc(M_ * N_ * sizeof(float), MEM_ALIGN);
    init_rand(A_, M_, K_);
    init_rand(B_, K_, N_);


#ifdef TEST
    // matmul_naive(A, B, C_ref, sA, sB, M, N, K);
    // matmul_naive_no_packing(A, B, C_ref, sA, sB, MinB, M, N, K);
    // matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
    matmul_naive_no_packing(A, B, C_ref, sA, scaledSumA, sB, MinB, M, N, K);
    float_matmul_naive((float *)activation->data, (float *) weight->data, C_float_ref, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    // warm up
    ggml_graph_compute_with_ctx(ctx, graph, benchmark_params.n_threads);
    printf("\n\e[32m[INFO]\e[m M=%d, K=%d, N=%d\n",  M, K, N);
    printf("\n\e[32m[INFO]\e[m llama.cpp q41q81 ink modified\n");
    for (int i = 0; i < NITER; i++) {

        // flush cache lines
        float_matmul_naive(A_, B_, C_, M_, N_, K_);

        float * result = (float *)ggml_result->data;
        init_const(result, 0.0, M, N);

        uint64_t start = timer();
        ggml_graph_compute_with_ctx(ctx, graph, benchmark_params.n_threads);
        uint64_t end = timer();

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        // print_mat(result, 5);
        // print_mat(C_ref, 5);
        // print_mat(C_float_ref, 5);
#ifdef TEST
        int res = compare_mats(result, C_ref, (long)M * (long)N);
        // if (!res) break;
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
