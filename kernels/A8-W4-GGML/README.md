# Introduction

This directory contains the following components:

1. Performance evaluation of various custom GEMM/GEMV kernels, along with official kernels of GGML.
2. Implementation of **FP16 support** in Ink's custom kernels using GGML’s built-in conversion APIs:
   - `GGML_FP16_TO_FP32`
   - `GGML_FP32_TO_FP16`

---
## GEMM Kernels

Below is a table summarizing each GEMM kernel file and its purpose:

| File Name | Description |
|----------|-------------|
| `gemm-A80W40-ggml.cpp` | Official A80W40 kernel based on the `cpu` backend in GGML. |
| `gemm-A81W41-ggml.cpp` | Official A81W41 kernel based on the `cpu` backend in GGML. |
| `gemm-A80W40-ggml-aarch64-repack.cpp` | Official A80W40-repack kernel based on the `aarch64` backend in GGML; used by default in `llama-run`. |
| `gemm-A80W40-llama-aarch64-repack-dynamic_openmp_scheme-better_kernel.cpp` | The kernel is the same as above, except that dynamic OpenMP scheduling in used for multi-threaded case to better utilize CPUs with P/E cores. |
| `gemm-A80W40-llama-aarch64-repack-dynamic_openmp_scheme-worse_kernel.cpp` | Similar to the "better" kernel, but uses `mul_sum_i8_pairs_int32x8` instead of `mul_sum_i8_pairs_acc_int32x8` (note: GGML updates this frequently). |
| `gemm-A80W40-ink_repack_scheme.cpp` | Custom A80W40 kernel using `SIMD-aware weight packing` and repacking strategy. |
| `gemm-A81W41-ink_repack_scheme.cpp` | Custom A81W41 kernel using `SIMD-aware weight packing` and repacking strategy. |
| `gemm-A81W41-ggml_GGML_TYPE_Q4_1_ink.cpp` | Invokes the A81W41 kernel with `SIMD-aware weight packing` within the GGML framework.|
| `gemm-A81W41-ink_repack_scheme-use_fp16-imp1.cpp` | Converts FP32 to FP16 during packing; uses `_mm256_cvtph_ps` to convert back to FP32 inside the kernel. |
| `gemm-A81W41-ink_repack_scheme-use_fp16-imp2.cpp` | Converts FP32 to FP16 during packing; uses `GGML_FP16_TO_FP32` to convert back to FP32 inside the kernel. |
| `gemm-A81W41-ink_repack_scheme-use_fp16-imp3_use_malloc.cpp` | Identical to `imp2`, but memory is not aligned — used to evaluate the impact of memory alignment when using AVX intrinsics. |

---

## GEMV Kernels

Below is a summary of GEMV-related files:

| File Name | Description |
|----------|-------------|
| `gemv-A80W40-ggml.cpp` | Official A80W40 kernel based on the `cpu` backend in GGML. |
| `gemv-A80W40-ggml_ink.cpp` | Invokes the the A80W40 kernel with `SIMD-aware weight packing` within the GGML framework |
| `gemv-A81W41-ggml_impl_1.cpp` & `gemv-A81W41-ggml_impl_2.cpp` | Official A81W41 kernels based on the `cpu` backend in GGML. The only difference between them lies in initialization logic. |
| *(Other GEMV files)* | Naming follows similar conventions as described in the GEMM section. |
