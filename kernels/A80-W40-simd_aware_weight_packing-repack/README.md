# Introduction

This directory contains **Quantized GEMM (General Matrix Multiply) and GEMV (General Matrix-Vector Multiply) kernels** optimized for the **A80W40** data type configuration. These implementations utilize **SIMD-aware weight packing and repacking strategies** to maximize performance on modern x86-64 architectures.

---

## GEMM Kernel

| File | Description |
|------|-------------|
| `unroll_i_j_by_2-deal_with_remainder-210gflops.cpp` | Final selected kernel implementation achieving up to **210 GFLOPS** in single-threaded mode. Applies loop unrolling along both `i` and `j` dimensions by a factor of 2, and includes logic to efficiently handle irregular input sizes. |

### Glossary

| Technique | Explanation |
|---------|-------------|
| **unroll_i_j_by_2** | Loop unrolling along both the `i` and `j` dimensions by a factor of 2. This improves instruction-level parallelism and reduces loop overhead. |
| **deal_with_remainder** | Handles cases where matrix dimensions are not perfectly divisible by tile or unroll factors. Ensures robustness across arbitrary input sizes while maintaining performance. |

---

## GEMV Kernel

| File | Description |
|------|-------------|
| `gemv-v1.cpp` | Final optimized GEMV kernel tailored for the A80W40 quantization format. Implements SIMD vectorization, weight repacking. |
