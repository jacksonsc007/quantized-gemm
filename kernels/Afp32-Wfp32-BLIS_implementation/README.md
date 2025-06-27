# Introduction
This directory contains optimized GEMM (General Matrix Multiply) and GEMV (General Matrix-Vector Multiply) kernels for single-precision floating-point (`FP32`) data.

These implementations explore various optimization techniques such as SIMD vectorization using AVX2, cache blocking based on the BLIS design, accumulator expansion, mask vectorization, and multithreading. Performance metrics are provided in effective GFLOPS (eGFLOPS) for single-threaded execution.

---

## GEMM Kernels

| Kernel File | Description | eGFLOPS (1 thread) |
|-------------|-------------|--------------------|
| `sgemm-naive.cpp` | Naive implementation of GEMM with a triple-nested loop in `ijk` order. | 0.93 |
| `sgemm-micro_kernel.cpp` | Optimized micro-kernel using AVX2 intrinsics. Designed to work only with matrices whose dimensions are multiples of the kernel size. | ~72 |
| `sgemm-cache.cpp` | Extends `sgemm-micro_kernel.cpp` with cache-blocking strategy inspired by BLIS for better memory hierarchy utilization. | ~107 |
| `sgemm-cache-expand_accs.cpp` | Based on `sgemm-cache.cpp`, with explicit unrolling and expansion of accumulators. | ~96 |
| `sgemm-cache-vectorize_mask_calculation.cpp` | Based on `sgemm-cache.cpp`, with vectorized mask computation for handling irregular matrix boundaries. | ~107 |
| `sgemm-cache-expand_accs-vectorize_mask_calculation.cpp` | Combines both accumulator expansion and vectorized mask calculation from the above kernels. | 145–157 |

---

## GEMV Kernels

The naming convention for GEMV kernels includes memory layout information:
- `r`: row-major
- `c`: column-major  
The format follows: **[activation layout][weight layout][result layout]** (e.g., `rcr` means activation is row-major, weight is column-major, result is row-major).

| Kernel File | Description | eGFLOPS (1 thread) |
|-------------|-------------|--------------------|
| `sgemv-naive.cpp` | Naive implementation of GEMV with a double-nested loop in `jk` order. | 4.8 |
| `sgemv-avx.cpp` | Vectorized GEMV kernel using AVX2 instructions. | 14.2 |
| `sgemv_rcr-avx_v2.cpp` | Same as `sgemv-avx.cpp`, but with an alternative OpenMP scheduling strategy. | 13.2 |

---

## Resources

For detailed explanations of kernel design and optimization strategies, refer to this excellent tutorial:
- [Advanced GEMM Optimization on Modern x86-64 Multi-Core Processors](https://salykova.github.io/matmul-cpu)