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
#include <memory>

#include "lib.h"

using float16 = uint16_t;

#ifndef NTHREADS
#define NTHREADS 1
#endif

struct benchmark_params_struct {
    int32_t n_threads     = NTHREADS;
};

struct ggml_context_deleter_t {
  auto operator()(ggml_context *ctx) const noexcept -> void { ggml_free(ctx); }
};

using unique_ggml_context_t = std::unique_ptr<ggml_context, ggml_context_deleter_t>;

static inline auto make_unique_ggml_context(
  size_t mem_size, void *mem_buffer, bool no_alloc
) -> unique_ggml_context_t {
  return unique_ggml_context_t(ggml_init({mem_size, mem_buffer, no_alloc}));
}

int main() {
    const int M = 4096;
    const int N = 4096;
    const int K = 12288;

    // ggml initialization
    const ggml_type weight_q_type = GGML_TYPE_Q4_0_ink;
    const ggml_type activation_q_type = GGML_TYPE_Q8_0_ink;

    unique_ggml_context_t ctx_w;  // weight
    size_t ctx_w_size = 91168;
    ctx_w = make_unique_ggml_context(ctx_w_size, nullptr, true);

    ggml_new_tensor_2d(ctx_w.get(), GGML_TYPE_Q4_0, 4096, 12288);

    size_t ctx_size = 91168; 
    // ctx_size += ggml_row_size(GGML_TYPE_F32, K * N); // fp32 weight
    // ctx_size += ggml_row_size(GGML_TYPE_F32, K * M); // fp32 activation
    // ctx_size += ggml_row_size(GGML_TYPE_F32, M * N); // fp32 output
    // ctx_size += ggml_row_size(weight_q_type, K * N); // int4 weight
    // ctx_size += ggml_row_size(activation_q_type, K * M); // int8 activation
    // ctx_size += 1024*1024*16;
    printf("Allocating Memory of size %zi bytes, %zi MB\n",ctx_size, (ctx_size/1024/1024));

    struct ggml_init_params params = {
        /*.mem_size   =*/ ctx_size,
        /*.mem_buffer =*/ NULL,
        /* no_alloc   =*/ true
    };

    struct ggml_context * ctx;
    ctx = ggml_init(params);
    if (!ctx) {
        fprintf(stderr, "%s: ggml_init() failed\n", __func__);
        return 1;
    }

    struct ggml_tensor * weight = ggml_new_tensor_2d(ctx, GGML_TYPE_Q4_0, K, N);
    // ggml_set_f32(weight, 10.0f);
    // init_rand((float *)weight->data, K, N );


    struct ggml_tensor * activation = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, K, M);
    // ggml_set_f32(activation, 200.0f);
    // init_rand((float *)activation->data, K, M);

    
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
    
    // Set up a the compute graph
    printf("\e[32m[INFO]\e[m Building Graph\n");
    // struct ggml_tensor * output = ggml_mul_mat(ctx, q4_weight, activation);
    struct ggml_tensor * ggml_result = ggml_mul_mat(ctx, q4_weight, q8_activation);
    struct ggml_cgraph * graph = ggml_new_graph(ctx);
    ggml_build_forward_expand(graph, ggml_result);
    struct benchmark_params_struct benchmark_params;
    printf("n_threads=%i\n", benchmark_params.n_threads);
    std::vector<uint8_t> work_buffer;
    ggml_graph_compute_with_ctx(ctx, graph, benchmark_params.n_threads);

    return 0;
}
