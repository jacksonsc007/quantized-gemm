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

// #define print

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
    #define MDIM 1
#endif

#ifndef NDIM
    #define NDIM 10240
#endif

#ifndef KDIM
    #define KDIM 10240
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


void float_matmul_naive(float* A, float* B, float* C, const int M, const int N, const int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int p = 0; p < K; p++) {
                C[j * M + i] += A[p * M + i] * B[j * K + p];
            }
        }
    }
}

void print_q8_1_data(block_q8_1 * GGML_RESTRICT blocks, size_t n_blocks) {

    for (size_t b = 0; b < n_blocks; ++b) {
         block_q8_1 blk = blocks[b];
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

    init_rand_min_max(A, M, K, -128, 127);
    init_rand_uint8(B, K / 2, N);
    // init_rand(sA, M, K / Q_BLK_SIZE);
    // init_rand(sB, K / Q_BLK_SIZE, N);
    // init_rand(MinB, K / Q_BLK_SIZE, N);
    init_const(sA, 2.0, M, K / Q_BLK_SIZE);
    init_const(sB, 3.0, K / Q_BLK_SIZE, N);
    init_const(MinB, 1.0, K / Q_BLK_SIZE, N);
    compute_scaled_sum(A, sA, scaledSumA, M, K);

    // ggml initialization
    const ggml_type weight_q_type = GGML_TYPE_Q4_1;
    const ggml_type activation_q_type = GGML_TYPE_Q8_1;

    size_t ctx_size = 0;
    ctx_size += ggml_row_size(weight_q_type, K * N); // quantized weight
    ctx_size += ggml_row_size(activation_q_type, K * M); // quantized activation
    ctx_size += ggml_row_size(GGML_TYPE_F32, M * N); // fp32 output
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

    
    struct ggml_tensor * q4_weight = ggml_new_tensor_2d(ctx, weight_q_type, K, N);
    struct ggml_tensor * q8_activation = ggml_new_tensor_2d(ctx, activation_q_type, K, M);
    
    printf("\e[32m[INFO]\e[m adopt pseudo quantized weight\n");
    block_q8_1 * q8_blocks = (block_q8_1 *)q8_activation->data;
    int32_t num_q8_blocks = q8_activation->ne[0] * q8_activation->ne[1] / QK8_1;
    for (int block_idx = 0; block_idx < num_q8_blocks; ++block_idx) {
        float scale = sA[block_idx];
        float scaled_sum = scaledSumA[block_idx];
        block_q8_1* blk = &q8_blocks[block_idx];
        blk -> d = GGML_FP32_TO_FP16(scale);
        blk -> s = GGML_FP32_TO_FP16(scaled_sum);
        memcpy(blk->qs, A + block_idx * QK8_1, QK8_1 * sizeof(int8_t));
    }
    
    // print_q8_1_data(q8_blocks, 1);
    printf("\e[32m[INFO]\e[m adopt pseudo quantized activation\n");
    block_q4_1 * q4_blocks = (block_q4_1 *)q4_weight->data;
    int32_t num_q4_blocks = q4_weight->ne[0] * q4_weight->ne[1] / QK4_1;
    for (int block_idx = 0; block_idx < num_q4_blocks; ++block_idx) {
        block_q4_1 * blk = &q4_blocks[block_idx];
        float scale = sB[block_idx];
        float min_value = MinB[block_idx];
        blk -> d = GGML_FP32_TO_FP16(scale);
        blk -> m = GGML_FP32_TO_FP16(min_value);
        memcpy(blk -> qs, B + block_idx * QK4_1 / 2, QK4_1 / 2 * sizeof(uint8_t));
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
    matmul_naive_no_packing(A, B, C_ref, sA, sB, MinB, M, N, K);
    // matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
    // matmul_naive_no_packing_2(A, B, C_ref, sA, scaledSumA, sB, MinB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    // warm up
    ggml_graph_compute_with_ctx(ctx, graph, benchmark_params.n_threads);
    printf("\n\e[32m[INFO]\e[m M=%d, K=%d, N=%d\n",  M, K, N);
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
        #ifdef print
        print_mat(result, M * N);
        print_mat(C_ref, M * N);
        #endif
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
