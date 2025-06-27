# Introduction

This directory contains **Quantized GEMM/GEMV kernels** designed for the **A81W41** data type configuration. These kernels leverage **SIMD optimizations**, including efficient **weight packing** and **repacking strategies**, to achieve high performance on modern x86-64 architectures.

---

## GEMM Kernels

`gemm-mul_sum_us8-unroll_i_by_6-add_logic_for_remainder-impl_3_407gflops.cpp` is the final kernel implementation.

### Key Optimization Techniques

| Technique | Explanation |
|---------|-------------|
| **mul_sum_us8 / mul_sum_i8** | Refers to the integer accumulation strategy used during quantized computation.<br>- `mul_sum_us8`: Optimized for A81W41 where weights are unsigned (positive), enabling more efficient multiplication and sum accumulation.<br>- `mul_sum_i8`: Used in configurations like A80W40 where both activation and weight values can be signed. |
| **unroll_i_by_6** | Loop unrolling along the `i` dimension improves instruction-level parallelism and reduces overhead from loop control. Unrolling by factor 6 was found to yield optimal balance between register usage and throughput. |
| **add_logic_for_remainder** | Handles cases where input dimensions are not perfectly divisible by tile sizes.<br>Uses a `switch` statement instead of chained `if` conditions for better branch prediction and lower latency. |

---

## GEMV Kernels

`gemv-A8141-v1.cpp` is the final kernel implementation.
