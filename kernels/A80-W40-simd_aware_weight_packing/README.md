# Introduction

This repository contains an implementation of the **A80W40** kernel with **SIMD-aware weight packing**.

---

## GEMM Kernels

The file `gemm-no_cache-unroll_4x4x1x2.cpp` has been selected as the final kernel implementation due to its superior performance.

### Term Definitions

| Term / Phrase             | Meaning                                                                 |
|--------------------------|-------------------------------------------------------------------------|
| `cache`                  | Indicates that a cache-blocking strategy is employed.                   |
| `no_cache` / `no_caching`| No cache-blocking strategy is used.                                     |
| `micro_kernel_{num_rows}x{num_cols}` | Refers to the function executed in the innermost loop, processing a block of size `{num_rows}x{num_cols}`, inspired by BLIS. |
| `micro_kernel_4xN`       | Micro kernel with 4 rows and N columns.                                 |

### Available GEMM Implementations

| File Name                                  | Description |
|--------------------------------------------|-------------|
| `gemm-no_cache-baseline.cpp`               | A naive baseline implementation, useful for understanding the fundamental algorithm. |
| `gemm-no_cache-unroll_4x4x1x1.cpp`         | Loops over rows (`i`) and columns (`j`) are both unrolled by 4. |
| `gemm-no_cache-unroll_4x4x1x2.cpp`         | Loops over `i` and `j` are unrolled by 4; the `jj` loop is unrolled by 2 (selected final implementation). |

---

## GEMV Kernels

The file `gemv-non_caching-unroll_j_by_2.cpp` has been chosen as the final implementation for GEMV due to **slightly better performance**.

### Term Definitions

| Term / Phrase        | Meaning |
|----------------------|---------|
| `BLIS_cache`         | One of the cache-blocking strategies explored. |
| `Ink_cache`          | An alternative cache-blocking strategy. |
| `packing`            | Refers to copying a non-contiguous memory block into a contiguous chunk, often used to improve data locality and SIMD efficiency. |
