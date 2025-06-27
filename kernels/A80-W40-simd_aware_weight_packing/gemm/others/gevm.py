import os
# Limit NumPy to single thread (must be set before importing numpy)
os.environ["OMP_NUM_THREADS"] = "1"
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["MKL_NUM_THREADS"] = "1"
os.environ["VECLIB_MAXIMUM_THREADS"] = "1"
os.environ["NUMEXPR_NUM_THREADS"] = "1"

import numpy as np
import time

# Matrix dimensions
m, k, n = 1, 1024, 1000
iterations = 10

# Generate random float32 matrices
A = np.random.rand(m, k).astype(np.float32)
B = np.random.rand(k, n).astype(np.float32)

# Theoretical FLOPs per operation
flops_per_op = 2 * m * k * n
print(f"Theoretical FLOPs per matmul: {flops_per_op}")

# Warm-up
_ = A @ B

# Timing loop
total_time = 0.0
for _ in range(iterations):
    start = time.perf_counter()
    _ = A @ B
    end = time.perf_counter()
    total_time += (end - start)

# Compute average time and performance
avg_time = total_time / iterations
flops_per_sec = flops_per_op / avg_time

print(f"Average elapsed time: {avg_time:.6f} seconds")
print(f"Average performance (single-thread): {flops_per_sec / 1e9:.2f} GFLOP/s")