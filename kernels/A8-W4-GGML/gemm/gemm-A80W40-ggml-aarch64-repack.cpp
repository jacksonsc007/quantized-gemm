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
#include <stdexcept>
#include "ggml-cpu.h"
#include "ggml-quants.h"
#include "ggml-impl.h"

#include "lib.h"
#include "profile.h"

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
    #define MDIM 1024
#endif

#ifndef NDIM
    #define NDIM 1024
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


void matmul_naive_no_packing(
    int8_t* A, uint8_t* B, float* C,
    float* SA, float* SB,
    const int M, const int N, const int K) 
{

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
            // 1 elements in B contains two int4 weights, matching 2 int8 activations 
            for (int q = 0; q < Q_BLK_SIZE / 2; q += 1)
            {
                int8_t a_low = *A(i, p + q, KDIM); // get (p + q)-th element
                int8_t a_high = *A(i, p + q + Q_BLK_SIZE / 2, KDIM); // get (p + q + 16)
                uint8_t b = *B(p / 2 + q , j, KDIM / 2);
                // NOTE: '-' has higher priority than '&' and '>>'
                int8_t b_low = (b & 0x0f) - 8; 
                int8_t b_high = (b >> 4) - 8; 
                float a_low_f = (float)a_low * s_a;
                float a_high_f = (float)a_high * s_a;
                float b_low_f = (float)b_low * s_b;
                float b_high_f = (float)b_high * s_b;
                sum += a_low_f * b_low_f + a_high_f * b_high_f;

            }
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

using buft_list_t = std::vector<std::pair<ggml_backend_dev_t, ggml_backend_buffer_type_t>>;
static buft_list_t make_cpu_buft_list(const std::vector<ggml_backend_dev_t> & devices) {
    buft_list_t buft_list;

    // add ACCEL buffer types
    for (size_t i = 0; i < ggml_backend_dev_count(); ++i) {
        ggml_backend_dev_t dev = ggml_backend_dev_get(i);
        if (ggml_backend_dev_type(dev) == GGML_BACKEND_DEVICE_TYPE_ACCEL) {
            auto * buft = ggml_backend_dev_buffer_type(dev);
            // skip
            if (buft != ggml_backend_cpu_buffer_type()) {
                buft_list.emplace_back(dev, buft);
            }
        }
    }

    // add a host buffer type
    // storing the tensors in a host buffer is useful when the processing of large batches
    // is offloaded to a GPU device, since it reduces the time spent on data transfers
    // generally, this will be done using the first device in the list
    // a better approach would be to handle this on a weight-by-weight basis using the offload_op
    // function of the device to determine if it would benefit from being stored in a host buffer
    for (auto * dev : devices) {
        ggml_backend_buffer_type_t buft = ggml_backend_dev_host_buffer_type(dev);
        if (buft) {
            buft_list.emplace_back(dev, buft);
            break;
        }
    }

    // add extra buffer types, only if no GPU device is present
    // ref: https://github.com/ggml-org/llama.cpp/issues/12481#issuecomment-2743136094
    auto * cpu_dev = ggml_backend_dev_by_type(GGML_BACKEND_DEVICE_TYPE_CPU);
    auto * cpu_reg = ggml_backend_dev_backend_reg(cpu_dev);
    auto ggml_backend_dev_get_extra_bufts_fn = (ggml_backend_dev_get_extra_bufts_t)
        ggml_backend_reg_get_proc_address(cpu_reg, "ggml_backend_dev_get_extra_bufts");
    if (ggml_backend_dev_get_extra_bufts_fn) {
        ggml_backend_buffer_type_t * extra_bufts = ggml_backend_dev_get_extra_bufts_fn(cpu_dev);
        while (extra_bufts && *extra_bufts) {
            buft_list.emplace_back(cpu_dev, *extra_bufts);
            ++extra_bufts;
        }
    }

    // add the CPU buffer type
    for (size_t i = 0; i < ggml_backend_dev_count(); ++i) {
        ggml_backend_dev_t dev = ggml_backend_dev_get(i);
        if (ggml_backend_dev_type(dev) == GGML_BACKEND_DEVICE_TYPE_CPU) {
            buft_list.emplace_back(dev, ggml_backend_dev_buffer_type(dev));
        }
    }

    return buft_list;
}

int main() {
    const int M = MDIM;
    const int N = NDIM;
    const int K = KDIM;
    int8_t* A  = (int8_t*)_mm_malloc(M * K * sizeof(int8_t), MEM_ALIGN);
    uint8_t* B = (uint8_t*)_mm_malloc(K / 2 * N * sizeof(uint8_t), MEM_ALIGN);
    float* sA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    // float* scaledSumA  = (float*)_mm_malloc(M * K / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* sB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    // float* MinB  = (float*)_mm_malloc(K * N / Q_BLK_SIZE * sizeof(float), MEM_ALIGN);
    float* C = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
    float* C_ref = (float*)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);

    /*
    S1: Data preparation.
    */
    const ggml_type weight_q_type = GGML_TYPE_Q4_0;
    const ggml_type activation_q_type = GGML_TYPE_Q8_0;

    size_t ctx_size = 0;
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * N); // fp32 weight
    ctx_size += ggml_row_size(GGML_TYPE_F32, K * M); // fp32 activation
    ctx_size += ggml_row_size(GGML_TYPE_F32, M * N); // fp32 output
    ctx_size += ggml_row_size(weight_q_type, K * N); // fp32 activation
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
    init_rand((float *)weight->data, K, N);


    struct ggml_tensor * activation = ggml_new_tensor_2d(ctx, GGML_TYPE_F32, K, M);
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
    block_q8_0 * q8_blocks = (block_q8_0 *)q8_activation->data;
    int32_t num_q8_blocks = q8_activation->ne[0] * q8_activation->ne[1] / QK8_0;
    for (int block_idx = 0; block_idx < num_q8_blocks; ++block_idx) {
        block_q8_0 blk = q8_blocks[block_idx];
        float scale = GGML_FP16_TO_FP32(blk.d);
        sA[block_idx] = scale;
        memcpy(A + block_idx * QK8_0, blk.qs, QK8_0 * sizeof(int8_t));
    }
    block_q4_0 * q4_blocks = (block_q4_0 *)q4_weight->data;
    int32_t num_q4_blocks = q4_weight->ne[0] * q4_weight->ne[1] / QK4_0;
    for (int block_idx = 0; block_idx < num_q4_blocks; ++block_idx) {
        block_q4_0 blk = q4_blocks[block_idx];
        float scale = GGML_FP16_TO_FP32(blk.d);
        sB[block_idx] = scale;
        memcpy(B + block_idx * QK4_0 / 2, blk.qs, QK4_0 / 2 * sizeof(uint8_t));
    }
    


    /*
    S2: Backend Computation.
    */
    printf("\e[32m[INFO]\e[m Allocate backend\n");
    std::vector<ggml_backend_dev_t> devices;
    buft_list_t buft_list = make_cpu_buft_list(devices);
    ggml_backend_buffer_type_t aarch64_buft = buft_list[0].second;

    // 1. Initialize backend
    ggml_backend_t backend = NULL;

    // if there aren't GPU Backends fallback to CPU backend
    if (!backend) {
        backend = ggml_backend_cpu_init();
    }

    // Calculate the size needed to allocate
    size_t ctx_size_backend = 0;
    ctx_size_backend += 2 * ggml_tensor_overhead(); // tensors
    // no need to allocate anything else!

    // 2. Allocate `ggml_context` to store tensor data
    struct ggml_init_params params_backend = {
        /*.mem_size   =*/ ctx_size_backend,
        /*.mem_buffer =*/ NULL,
        /*.no_alloc   =*/ true, // the tensors will be allocated later by ggml_backend_alloc_ctx_tensors()
    };
    struct ggml_context * ctx_backend = ggml_init(params_backend);

    // Create tensors metadata (only there shapes and data type)
    struct ggml_tensor * activation_fp32 = ggml_new_tensor_2d(ctx_backend, GGML_TYPE_F32, K, M);
    struct ggml_tensor * weight_int4 = ggml_new_tensor_2d(ctx_backend, weight_q_type, K, N);

    // 4. Allocate a `ggml_backend_buffer` to store all tensors
    // ggml_backend_buffer_t buffer = ggml_backend_alloc_ctx_tensors(ctx_backend, backend);
    ggml_backend_buffer_t buf = ggml_backend_alloc_ctx_tensors_from_buft(ctx_backend, aarch64_buft);
    if (buf == nullptr) {
        throw std::runtime_error(std::string("unable to allocate ") + ggml_backend_buft_name(aarch64_buft) + " buffer");
    }

    // 5. Copy tensor data from main memory (RAM) to backend buffer
    ggml_backend_tensor_set(weight_int4, q4_weight->data, 0, ggml_nbytes(weight_int4));
    // ggml_backend_tensor_set(activation_fp32, activation->data, 0, ggml_nbytes(activation_fp32));

    // 6. Create a `ggml_cgraph` for mul_mat operation
    struct ggml_cgraph * gf = NULL;
    struct ggml_context * ctx_cgraph = NULL;
    {
        // create a temporally context to build the graph
        struct ggml_init_params params0 = {
            /*.mem_size   =*/ ggml_tensor_overhead()*GGML_DEFAULT_GRAPH_SIZE + ggml_graph_overhead(),
            /*.mem_buffer =*/ NULL,
            /*.no_alloc   =*/ true, // the tensors will be allocated later by ggml_gallocr_alloc_graph()
        };
        ctx_cgraph = ggml_init(params0);
        gf = ggml_new_graph(ctx_cgraph);

        // result = a*b^T
        // Pay attention: ggml_mul_mat(A, B) ==> B will be transposed internally
        // the result is transposed
        struct ggml_tensor * result0 = ggml_mul_mat(ctx_cgraph, weight_int4, activation);

        // Add "result" tensor and all of its dependencies to the cgraph
        ggml_build_forward_expand(gf, result0);
    }
    // 7. Create a `ggml_gallocr` for cgraph computation
    ggml_gallocr_t allocr = ggml_gallocr_new(ggml_backend_get_default_buffer_type(backend));
    ggml_gallocr_alloc_graph(allocr, gf);

    // (we skip step 8. Optionally: schedule the cgraph using `ggml_backend_sched`)

    // 9. Run the computation
    int n_threads = NTHREADS; // Optional: number of threads to perform some operations with multi-threading
    if (ggml_backend_is_cpu(backend)) {
        ggml_backend_cpu_set_n_threads(backend, n_threads);
    }

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
    matmul_naive_no_packing(A, B, C_ref, sA, sB, M, N, K);
    // matmul_naive_no_packing_2(A, B, C_ref, sA, scaledSumA, sB, MinB, M, N, K);
#endif
    double FLOP = 2 * (double)M * N * K;
    // warm up
    printf("\n\e[32m[INFO]\e[m M=%d, K=%d, N=%d\n",  M, K, N);
    printf("\e[32m[INFO]\e[m llama.cpp A80W40 gemm with aarch64 backend \n");
    for (int i = 0; i < NITER; i++) {
        float_matmul_naive(A_, B_, C_, M_, N_, K_);

        uint64_t start = timer();
        ggml_backend_graph_compute(backend, gf);
        uint64_t end = timer();
        struct ggml_tensor * result_node = ggml_graph_node(gf, -1);
        // 10. Retrieve results (output tensors)
        // in this example, output tensor is always the last tensor in the graph
        float * result = (float *) malloc(ggml_nbytes(result_node));
        // because the tensor data is stored in device buffer, we need to copy it back to RAM
        ggml_backend_tensor_get(result_node, result, 0, ggml_nbytes(result_node));
        

        double exec_time = (end - start) * 1e-9;
        double FLOPS = FLOP / exec_time;

        // printf("Exec. time = %.3fms\n", exec_time * 1000);
        printf("GFLOPS= \e[32m%.3f\e[m for (%d, %d)x(%d, %d) \n", FLOPS / 1e9, M, K, K, N);
        print_mat(result, 5);
        print_mat(C_ref, 5);
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
    Profiler::getInstance().report_internal();
    return 0;
}
