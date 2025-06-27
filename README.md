## Introduction 📖

This repository provides quantized GEMM/GEMV kernels commonly used in large language models with `q4` quantization schemes, along with baseline float32 implementations.

Please note:
1. The kernel performance has been optimized specifically for my own hardware setup; results may not be reproducible on other systems.
2. This project is primarily educational and research-oriented—it is **not intended for production use**.

## Roadmap ⛵
- [ ] Add support for additional architectures (e.g., ARM NEON, RISC-V).
- [ ] Further optimize QGEMV kernels for better efficiency and scalability.

## Benchmark Results

We consider the general matrix multiplication formula:

$$
O = A \times W
$$

where $A$ represents the activation matrix, $W$ the weight matrix, and $O$ the resulting output. Unless otherwise specified:
- Activations are stored in **row-major order**
- Weights are stored in **column-major order**
- Output is returned in **row-major order**

### Experimental Setup

| Component         | Specification                     |
|------------------|-----------------------------------|
| Operating System  | Ubuntu 22.04                      |
| CPU               | Intel Core i5-13600KF             |
| DRAM              | 3600 MT/s, Dual-channel           |

---

### GEMM Benchmarks

**Input Shape:** (1024 × 1024) @ (1024 × 1024)

- Table 1: Single-threaded Performance (`num_threads = 1`)

| Kernel Type | Backend                 | eGFLOPS |
| ----------- | ----------------------- | ------- |
| Afp32Wfp32  | cpu-BLIS                | 157     |
| A80W40      | ggml-cpu                | 62      |
| A80W40      | ggml-cpu-aarch64-repack | 170     |
| A80W40      | sp                      | 115     |
| A80W40      | sp-repack               | **210** |
| A81W41      | ggml-cpu                | 69      |
| A81W41      | sp                      | 126     |
| A81W41      | sp-repack               | **405** |
| A81W41      | sp-repack-fp16_1        | 320     |
| A81W41      | sp-repack-fp16_2        | 375     |
| A81W41      | sp-repack-fp16_3        | 366     |

> 🔹 **Bolded values** indicate performance surpassing the FP32 baseline.  
> 🔹 `ggml-cpu-aarch64-repack` is the backend used when running `llama-run` on CPU.  
> 🔹 `sp` denotes **SIMD-aware weight packing**, and `repack` indicates an additional **weight repacking strategy**.  
> 🔹 `fp16_*` refers to FP16-based computation strategies:
> - `fp16_1`: First FP16 implementation—introduces pressure on execution ports P0 and P1 during FP16-to-FP32 conversion, leading to reduced performance compared to FP32.
> - `fp16_2`: Uses a lookup table to avoid execution port contention, resulting in better performance than `fp16_1`.
> - `fp16_3`: Identical to `fp16_2`, but without memory alignment optimizations.

> ⚠️ OpenMP pragmas such as `#define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for ...")` can affect single-threaded performance. In some cases, removing these directives improves performance slightly. The results shown here reflect both configurations, as deviations are relatively minor and do not alter overall trends.

---

- Table 2: Multi-threaded Performance (`num_threads = 16`)

| Kernel Type | Backend                                | eGFLOPS |
| ----------- | -------------------------------------- | ------- |
| Afp32Wfp32  | cpu-BLIS                               | 970     |
| A80W40      | ggml-cpu                               | 590     |
| A80W40      | ggml-cpu-aarch64-repack                | 910     |
| A80W40      | ggml-cpu-aarch64-repack-dynamic_openmp | 1221    |
| A80W40      | sp                                     | 950     |
| A80W40      | sp-repack                              | 1693    |
| A81W41      | ggml-cpu                               | 620     |
| A81W41      | sp                                     | 890     |
| A81W41      | sp-repack                              | 2925    |
| A81W41      | sp-repack-fp16_1                       | 2440    |
| A81W41      | sp-repack-fp16_2                       | 2861    |

> 📌 Key Observations:
> 1. Appropriate OpenMP settings significantly impact multi-threaded performance:
> ```cpp
> // Recommended OpenMP settings
> #define OMP_SCHEDULE dynamic
> #define PRAGMA_OMP_PARALLEL_FOR _Pragma("omp parallel for schedule(OMP_SCHEDULE) num_threads(NTHREADS)")
> ```
> 2. GGML lacks dynamic scheduling for multi-threading, which becomes problematic on hybrid CPUs like the i5-13600KF with both P-cores and E-cores. Use `OMP_SCHEDULE=dynamic` for optimal load balancing in such environments.
> 3. Loop ordering matters in A81W41 kernels:
> ```c
> for (int j = 0; j < N; j += 4)
>     for (int i = 0; i < M; i += 4)
> ```
> 4. Multi-threaded performance exhibits variability; results presented represent median values based on subjective judgment.

---

### GEMV Benchmarks

**Input Shape:** (1 × 10240) @ (10240 × 10240)

- Table 1: Single-threaded Performance (`num_threads = 1`)

| Kernel Type | Backend                 | eGFLOPS |
| ----------- | ----------------------- | ------- |
| Afp32Wfp32  | cpu                     | 14      |
| A80W40      | ggml-cpu                | 64      |
| A80W40      | ggml-cpu-aarch64-repack | 75      |
| A80W40      | sp                      | 63      |
| A80W40      | sp-repack               | 80      |
| A80W40      | sp-repack-fp16_1        | 101     |
| A80W40      | sp-repack-fp16_2        | 100     |
| A81W41      | ggml-cpu                | 58      |
| A81W41      | sp                      | 57      |
| A81W41      | sp-repack               | 74      |
| A81W41      | sp-repack-fp16_1        | 96      |
| A81W41      | sp-repack-fp16_2        | 97      |

- Table 2: Multi-threaded Performance (`num_threads = 16`)

| Kernel Type | Backend                                | eGFLOPS |
| ----------- | -------------------------------------- | ------- |
| Afp32Wfp32  | cpu                                    | 24      |
| A80W40      | ggml-cpu                               | 144     |
| A80W40      | ggml-cpu-aarch64-repack                | 136     |
| A80W40      | sp                                     | 135     |
| A80W40      | sp-repack                              | 140     |
| A80W40      | sp-repack-fp16_1                       | 157     |
| A80W40      | sp-repack-fp16_2                       | 156     |
| A81W41      | ggml-cpu                               | 130     |
| A81W41      | sp                                     | 114     |
| A81W41      | sp-repack                              | 122     |
| A81W41      | sp-repack-fp16_1                       | 144     |
| A81W41      | sp-repack-fp16_2                       | 145     |

> 📌 A81W41 shows slightly degraded GEMV performance compared to A80W40 due to increased memory traffic.

---

## Build && Run
To build this project:
```fish
cd kernels
bash benchmark.sh <num_threads>
```

To evaluate the kernel performance, execute the executable files under `bin`:
```fish
A8-W4-GGML/bin/gemm/gemm-A81W41-ink_repack_scheme-use_fp16-imp2
```

## Specification

| File Name | Description |
|-----------|-------------|
| [theoretical_peak_analysis.ipynb](https://github.com/jacksonsc007/quantized-gemm/blob/main/kernels/theoretical_peak_analysis.ipynb) | Provides a detailed and thorough analysis of the theoretical peak performance achievable by each kernel. This part bears even more significance than specific kernel implementation. |
| [Afp32-Wfp32-BLIS_implementation](https://github.com/jacksonsc007/quantized-gemm/tree/main/kernels/Afp32-Wfp32-BLIS_implementation) | Provides the kernel where both the activation and weight are float32. |
| [A80-W40-simd_aware_weight_packing](https://github.com/jacksonsc007/quantized-gemm/tree/main/kernels/A80-W40-simd_aware_weight_packing) | Provides the A80W40 Quantized kernels with `SIMD-aware weight packing`. |
| [A81-W41-simd_aware_weight_packing](https://github.com/jacksonsc007/quantized-gemm/tree/main/kernels/A81-W41-simd_aware_weight_packing) | Provides the A81W41 Quantized kernels with `SIMD-aware weight packing`. |
| [A80-W40-simd_aware_weight_packing-repack](https://github.com/jacksonsc007/quantized-gemm/tree/main/kernels/A80-W40-simd_aware_weight_packing-repack) | Provides the A80W40 Quantized kernels with `SIMD-aware weight packing` and `repacking`. |
| [A81-W41-simd_aware_weight_packing-repack](https://github.com/jacksonsc007/quantized-gemm/tree/main/kernels/A81-W41-simd_aware_weight_packing-repack) | Provides the A81W41 Quantized kernels with `SIMD-aware weight packing` and `repacking`. |
| [A8-W4-GGML](https://github.com/jacksonsc007/quantized-gemm/tree/main/kernels/A8-W4-GGML) | Benchmarks native kernels provided by GGML, offers `FP16` support to custom kernels, and compares their performances. |

---

## Suffix Elucidation

- **A81W41**: Applies `q81` quantization to activations and `q41` to weights.
- **A80W40**: Applies `q80` quantization to activations and `q40` to weights.

---

## Acknowledgments 🫶

1. [sgemm.c](https://github.com/salykova/sgemm.c) – Hardcore tutorial on optimiation and inspiration for microkernel design and tuning methodology.
2. [GGML](https://github.com/ggml-org/ggml) – For providing foundational quantization and low-level primitives.
3. [Cursor](https://github.com/cursor/cursor) – Greatly simplified development of loop-unrolled kernels with AI-assisted coding features.
