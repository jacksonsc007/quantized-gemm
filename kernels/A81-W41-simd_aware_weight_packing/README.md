# Introduction

This directory contains **Quantized GEMM (General Matrix Multiply) and GEMV (General Matrix-Vector Multiply) kernels** optimized for the **A81W41** data type configuration. These implementations leverage **SIMD-aware weight packing**.

---

## GEMM Kernels
`gemm-no_cache-unroll_i_j_by_4-use_loop.cpp` is selected as the final kernel implementation.

The following table describes the available GEMM kernels and their optimization features:

| Kernel File | Description |
|-------------|-------------|
| `gemm-no_cache-unroll_i_j_by_4-use_loop.cpp` | Final selected kernel implementation. Implements loop unrolling along both the `i` and `j` dimensions by a factor of 4, and uses a structured loop-based approach for better maintainability and performance. |
| `gemm-good-no_cache-unroll_i_j_by_4-fully_expanded.cpp` | Variant with **explicitly unrolled accumulators**, avoiding arrays for accumulator storage. Despite its more manual structure, this version performs **worse** than the loop-based version. |
| `micro_kernel_{num_rows}x{num_cols}.cpp` | Refers to a micro-kernel implementation inspired by the **BLIS design**, typically used in the innermost loop of high-performance GEMM implementations. This function processes a small fixed-size block of computation. |
| `micro_kernel_4xN.cpp` | Micro-kernel with **4 rows** and a variable number of columns (`N`).|

### Terms Elucidation

| Term | Explanation |
|---------|-------------|
| **cache** | Indicates that a **cache-blocking strategy** is employed to improve data locality and reduce cache misses. However, it does not obtain any improvment.|
| **no_cache / no_caching** | No cache blocking is applied|
| **unroll_i_j_by_4** | Loop unrolling along both the `i` and `j` dimensions by a factor of 4 to increase instruction-level parallelism and reduce loop overhead. |

---

## GEMV Kernels
`non_caching-unroll_by_2.cpp` is selected as the final kernel implementation, due to slightly better single-thread performance.

> 🔍 **Note**: While **loop unrolling improves single-threaded performance**, it provides **limited or no benefit in multi-threaded scenarios** due to increased register pressure and memory bandwidth contention.
