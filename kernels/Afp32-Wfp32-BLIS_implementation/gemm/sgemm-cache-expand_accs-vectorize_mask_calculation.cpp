#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "lib.h"

#define MEM_ALIGN 64
#define UN_INIT -10.0

#define MR 6
#define NR 16
// #define MC MR *256
// #define NC NR * 64
#define MC MR * 64
#define NC NR * 256
// #define MC 200
// #define NC 400

#define KC 2000

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
#define NITER 100
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))

// row-major order: matrix A and C
// column-major order: matrix B
// A: M x K B: K x N C: M x N
#define A(i, j) (A + (i) * K + (j))
#define B(i, j) (B + (j) * K + (i))
#define C(i, j) (C + (i) * N + (j))

// column-major shape (block_m, K)
#define blockA(i, j) (blockA + (j) * block_m + (i))
// row-major shape (K, block_n)
#define blockB(i, j) (blockB + (i) * block_n + (j))

static float blockA_packed[MC * KC] __attribute__((aligned(64)));
static float blockB_packed[KC * NC] __attribute__((aligned(64)));

/*
blockA should be column-major order
blockB should be row-major order
*/
void microkernel_16x6(float *blockA, float *blockB, float *C, const int valid_m,
                      const int valid_n, const int block_m, const int block_n,
                      const int block_K, const int N) {
  // Explicitly unroll the array
  // __m256 C_buffer[6][2];
  __m256 C00 = _mm256_setzero_ps();
  __m256 C01 = _mm256_setzero_ps();
  __m256 C10 = _mm256_setzero_ps();
  __m256 C11 = _mm256_setzero_ps();
  __m256 C20 = _mm256_setzero_ps();
  __m256 C21 = _mm256_setzero_ps();
  __m256 C30 = _mm256_setzero_ps();
  __m256 C31 = _mm256_setzero_ps();
  __m256 C40 = _mm256_setzero_ps();
  __m256 C41 = _mm256_setzero_ps();
  __m256 C50 = _mm256_setzero_ps();
  __m256 C51 = _mm256_setzero_ps();

  __m256 a_packFloat8;
  __m256 b0_packFloat8;
  __m256 b1_packFloat8;
  // __m256i masks[2];
  // Explicitly unroll the array
  __m256i mask0;
  __m256i mask1;
  // load C from memory to registers
  // i: row index
  if (valid_n != NR) {
    static int8_t mask[32]
        __attribute__((aligned(64))) = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                                        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
    mask0 = _mm256_cvtepi8_epi32(_mm_loadu_si64(&mask[16 - valid_n]));
    mask1 = _mm256_cvtepi8_epi32(_mm_loadu_si64(&mask[16 - valid_n + 8]));
    switch (valid_m) {
    case 6:
      C00 = _mm256_maskload_ps(C(0, 0), mask0);
      C01 = _mm256_maskload_ps(C(0, 8), mask1);
      C10 = _mm256_maskload_ps(C(1, 0), mask0);
      C11 = _mm256_maskload_ps(C(1, 8), mask1);
      C20 = _mm256_maskload_ps(C(2, 0), mask0);
      C21 = _mm256_maskload_ps(C(2, 8), mask1);
      C30 = _mm256_maskload_ps(C(3, 0), mask0);
      C31 = _mm256_maskload_ps(C(3, 8), mask1);
      C40 = _mm256_maskload_ps(C(4, 0), mask0);
      C41 = _mm256_maskload_ps(C(4, 8), mask1);
      C50 = _mm256_maskload_ps(C(5, 0), mask0);
      C51 = _mm256_maskload_ps(C(5, 8), mask1);
      break;
    case 5:
      C00 = _mm256_maskload_ps(C(0, 0), mask0);
      C01 = _mm256_maskload_ps(C(0, 8), mask1);
      C10 = _mm256_maskload_ps(C(1, 0), mask0);
      C11 = _mm256_maskload_ps(C(1, 8), mask1);
      C20 = _mm256_maskload_ps(C(2, 0), mask0);
      C21 = _mm256_maskload_ps(C(2, 8), mask1);
      C30 = _mm256_maskload_ps(C(3, 0), mask0);
      C31 = _mm256_maskload_ps(C(3, 8), mask1);
      C40 = _mm256_maskload_ps(C(4, 0), mask0);
      C41 = _mm256_maskload_ps(C(4, 8), mask1);
      break;
    case 4:
      C00 = _mm256_maskload_ps(C(0, 0), mask0);
      C01 = _mm256_maskload_ps(C(0, 8), mask1);
      C10 = _mm256_maskload_ps(C(1, 0), mask0);
      C11 = _mm256_maskload_ps(C(1, 8), mask1);
      C20 = _mm256_maskload_ps(C(2, 0), mask0);
      C21 = _mm256_maskload_ps(C(2, 8), mask1);
      C30 = _mm256_maskload_ps(C(3, 0), mask0);
      C31 = _mm256_maskload_ps(C(3, 8), mask1);
      break;
    case 3:
      C00 = _mm256_maskload_ps(C(0, 0), mask0);
      C01 = _mm256_maskload_ps(C(0, 8), mask1);
      C10 = _mm256_maskload_ps(C(1, 0), mask0);
      C11 = _mm256_maskload_ps(C(1, 8), mask1);
      C20 = _mm256_maskload_ps(C(2, 0), mask0);
      C21 = _mm256_maskload_ps(C(2, 8), mask1);
      break;
    case 2:
      C00 = _mm256_maskload_ps(C(0, 0), mask0);
      C01 = _mm256_maskload_ps(C(0, 8), mask1);
      C10 = _mm256_maskload_ps(C(1, 0), mask0);
      C11 = _mm256_maskload_ps(C(1, 8), mask1);
      break;
    case 1:
      C00 = _mm256_maskload_ps(C(0, 0), mask0);
      C01 = _mm256_maskload_ps(C(0, 8), mask1);
      break;
    }

  } else {
    switch (valid_m) {
    case 6:
      C00 = _mm256_loadu_ps(C(0, 0));
      C01 = _mm256_loadu_ps(C(0, 8));
      C10 = _mm256_loadu_ps(C(1, 0));
      C11 = _mm256_loadu_ps(C(1, 8));
      C20 = _mm256_loadu_ps(C(2, 0));
      C21 = _mm256_loadu_ps(C(2, 8));
      C30 = _mm256_loadu_ps(C(3, 0));
      C31 = _mm256_loadu_ps(C(3, 8));
      C40 = _mm256_loadu_ps(C(4, 0));
      C41 = _mm256_loadu_ps(C(4, 8));
      C50 = _mm256_loadu_ps(C(5, 0));
      C51 = _mm256_loadu_ps(C(5, 8));
      break;
    case 5:
      C00 = _mm256_loadu_ps(C(0, 0));
      C01 = _mm256_loadu_ps(C(0, 8));
      C10 = _mm256_loadu_ps(C(1, 0));
      C11 = _mm256_loadu_ps(C(1, 8));
      C20 = _mm256_loadu_ps(C(2, 0));
      C21 = _mm256_loadu_ps(C(2, 8));
      C30 = _mm256_loadu_ps(C(3, 0));
      C31 = _mm256_loadu_ps(C(3, 8));
      C40 = _mm256_loadu_ps(C(4, 0));
      C41 = _mm256_loadu_ps(C(4, 8));
      break;
    case 4:
      C00 = _mm256_loadu_ps(C(0, 0));
      C01 = _mm256_loadu_ps(C(0, 8));
      C10 = _mm256_loadu_ps(C(1, 0));
      C11 = _mm256_loadu_ps(C(1, 8));
      C20 = _mm256_loadu_ps(C(2, 0));
      C21 = _mm256_loadu_ps(C(2, 8));
      C30 = _mm256_loadu_ps(C(3, 0));
      C31 = _mm256_loadu_ps(C(3, 8));
      break;
    case 3:
      C00 = _mm256_loadu_ps(C(0, 0));
      C01 = _mm256_loadu_ps(C(0, 8));
      C10 = _mm256_loadu_ps(C(1, 0));
      C11 = _mm256_loadu_ps(C(1, 8));
      C20 = _mm256_loadu_ps(C(2, 0));
      C21 = _mm256_loadu_ps(C(2, 8));
      break;
    case 2:
      C00 = _mm256_loadu_ps(C(0, 0));
      C01 = _mm256_loadu_ps(C(0, 8));
      C10 = _mm256_loadu_ps(C(1, 0));
      C11 = _mm256_loadu_ps(C(1, 8));
      break;
    case 1:
      C00 = _mm256_loadu_ps(C(0, 0));
      C01 = _mm256_loadu_ps(C(0, 8));
      break;
    }
  }

  for (int p = 0; p < block_K; p++) {
    b0_packFloat8 = _mm256_loadu_ps(blockB(p, 0));
    b1_packFloat8 = _mm256_loadu_ps(blockB(p, 8));

    a_packFloat8 = _mm256_broadcast_ss(blockA(0, p));
    C00 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C00);
    C01 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C01);

    a_packFloat8 = _mm256_broadcast_ss(blockA(1, p));
    C10 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C10);
    C11 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C11);

    a_packFloat8 = _mm256_broadcast_ss(blockA(2, p));
    C20 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C20);
    C21 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C21);

    a_packFloat8 = _mm256_broadcast_ss(blockA(3, p));
    C30 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C30);
    C31 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C31);

    a_packFloat8 = _mm256_broadcast_ss(blockA(4, p));
    C40 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C40);
    C41 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C41);

    a_packFloat8 = _mm256_broadcast_ss(blockA(5, p));
    C50 = _mm256_fmadd_ps(a_packFloat8, b0_packFloat8, C50);
    C51 = _mm256_fmadd_ps(a_packFloat8, b1_packFloat8, C51);
  }
  if (valid_n != NR) {
    switch (valid_m) {
    case 6:
      _mm256_maskstore_ps(C(0, 0), mask0, C00);
      _mm256_maskstore_ps(C(0, 8), mask1, C01);
      _mm256_maskstore_ps(C(1, 0), mask0, C10);
      _mm256_maskstore_ps(C(1, 8), mask1, C11);
      _mm256_maskstore_ps(C(2, 0), mask0, C20);
      _mm256_maskstore_ps(C(2, 8), mask1, C21);
      _mm256_maskstore_ps(C(3, 0), mask0, C30);
      _mm256_maskstore_ps(C(3, 8), mask1, C31);
      _mm256_maskstore_ps(C(4, 0), mask0, C40);
      _mm256_maskstore_ps(C(4, 8), mask1, C41);
      _mm256_maskstore_ps(C(5, 0), mask0, C50);
      _mm256_maskstore_ps(C(5, 8), mask1, C51);
      break;
    case 5:
      _mm256_maskstore_ps(C(0, 0), mask0, C00);
      _mm256_maskstore_ps(C(0, 8), mask1, C01);
      _mm256_maskstore_ps(C(1, 0), mask0, C10);
      _mm256_maskstore_ps(C(1, 8), mask1, C11);
      _mm256_maskstore_ps(C(2, 0), mask0, C20);
      _mm256_maskstore_ps(C(2, 8), mask1, C21);
      _mm256_maskstore_ps(C(3, 0), mask0, C30);
      _mm256_maskstore_ps(C(3, 8), mask1, C31);
      _mm256_maskstore_ps(C(4, 0), mask0, C40);
      _mm256_maskstore_ps(C(4, 8), mask1, C41);
      break;
    case 4:
      _mm256_maskstore_ps(C(0, 0), mask0, C00);
      _mm256_maskstore_ps(C(0, 8), mask1, C01);
      _mm256_maskstore_ps(C(1, 0), mask0, C10);
      _mm256_maskstore_ps(C(1, 8), mask1, C11);
      _mm256_maskstore_ps(C(2, 0), mask0, C20);
      _mm256_maskstore_ps(C(2, 8), mask1, C21);
      _mm256_maskstore_ps(C(3, 0), mask0, C30);
      _mm256_maskstore_ps(C(3, 8), mask1, C31);
      break;
    case 3:
      _mm256_maskstore_ps(C(0, 0), mask0, C00);
      _mm256_maskstore_ps(C(0, 8), mask1, C01);
      _mm256_maskstore_ps(C(1, 0), mask0, C10);
      _mm256_maskstore_ps(C(1, 8), mask1, C11);
      _mm256_maskstore_ps(C(2, 0), mask0, C20);
      _mm256_maskstore_ps(C(2, 8), mask1, C21);
      break;
    case 2:
      _mm256_maskstore_ps(C(0, 0), mask0, C00);
      _mm256_maskstore_ps(C(0, 8), mask1, C01);
      _mm256_maskstore_ps(C(1, 0), mask0, C10);
      _mm256_maskstore_ps(C(1, 8), mask1, C11);
      break;
    case 1:
      _mm256_maskstore_ps(C(0, 0), mask0, C00);
      _mm256_maskstore_ps(C(0, 8), mask1, C01);
      break;
    }
  } else {
    switch (valid_m) {
    case 6:
      _mm256_storeu_ps(C(0, 0), C00);
      _mm256_storeu_ps(C(0, 8), C01);
      _mm256_storeu_ps(C(1, 0), C10);
      _mm256_storeu_ps(C(1, 8), C11);
      _mm256_storeu_ps(C(2, 0), C20);
      _mm256_storeu_ps(C(2, 8), C21);
      _mm256_storeu_ps(C(3, 0), C30);
      _mm256_storeu_ps(C(3, 8), C31);
      _mm256_storeu_ps(C(4, 0), C40);
      _mm256_storeu_ps(C(4, 8), C41);
      _mm256_storeu_ps(C(5, 0), C50);
      _mm256_storeu_ps(C(5, 8), C51);
      break;
    case 5:
      _mm256_storeu_ps(C(0, 0), C00);
      _mm256_storeu_ps(C(0, 8), C01);
      _mm256_storeu_ps(C(1, 0), C10);
      _mm256_storeu_ps(C(1, 8), C11);
      _mm256_storeu_ps(C(2, 0), C20);
      _mm256_storeu_ps(C(2, 8), C21);
      _mm256_storeu_ps(C(3, 0), C30);
      _mm256_storeu_ps(C(3, 8), C31);
      _mm256_storeu_ps(C(4, 0), C40);
      _mm256_storeu_ps(C(4, 8), C41);
      break;
    case 4:
      _mm256_storeu_ps(C(0, 0), C00);
      _mm256_storeu_ps(C(0, 8), C01);
      _mm256_storeu_ps(C(1, 0), C10);
      _mm256_storeu_ps(C(1, 8), C11);
      _mm256_storeu_ps(C(2, 0), C20);
      _mm256_storeu_ps(C(2, 8), C21);
      _mm256_storeu_ps(C(3, 0), C30);
      _mm256_storeu_ps(C(3, 8), C31);
      break;
    case 3:
      _mm256_storeu_ps(C(0, 0), C00);
      _mm256_storeu_ps(C(0, 8), C01);
      _mm256_storeu_ps(C(1, 0), C10);
      _mm256_storeu_ps(C(1, 8), C11);
      _mm256_storeu_ps(C(2, 0), C20);
      _mm256_storeu_ps(C(2, 8), C21);
      break;
    case 2:
      _mm256_storeu_ps(C(0, 0), C00);
      _mm256_storeu_ps(C(0, 8), C01);
      _mm256_storeu_ps(C(1, 0), C10);
      _mm256_storeu_ps(C(1, 8), C11);
      break;
    case 1:
      _mm256_storeu_ps(C(0, 0), C00);
      _mm256_storeu_ps(C(0, 8), C01);
      break;
    }
  }
}

/*
Pack A into the memory layout required by the kernel

A: The origianl matrix A
K: The hidden dimension of the original matrix A, we need it to retrieve
elements, since A is row-major. blockA: The packed matrix A. blockA is
column-major, with static shape: MC * KC. NOTE some parts may not be valid
valid_m: The valid row of blockA
valid_k: The valid column of blockA
the valid elements are in matrix of size (valid_m * valid_k)
*/
void pack_blockA_panel(float *A, float *blockA, const int kc, const int mc,
                       const int K) {
  // int block_m = MR;
  for (int j = 0; j < kc; ++j) {
    for (int i = 0; i < mc; ++i) {
      *(blockA) = *A(i, j);
      blockA++;
      // *(blockA(i, j )) = *A(i, j);
    }
  }
}
void pack_blockA(float *A, float *blockA, const int mc, const int kc,
                 const int K) {
  for (int i = 0; i < mc; i += MR) {
    int valid_m = min(mc - i, MR);
    // printf("\e[31m[ERROR]\e[m The valid_m = %d\n", valid_m);
    // if (valid_m != MC)
    // {
    //     printf("\e[31m[ERROR]\e[m The valid_m is not MC\n");
    // }
    pack_blockA_panel(A(i, 0), blockA + i * kc, kc, valid_m, K);
  }
}

/*
Pack B into the memory layout required by the kernel
B: The origianl matrix B
K: The hidden dimension of the original matrix A, we need it to retrieve
elements, since B is column-major. blockB: The packed matrix B. blockA is
column-major, with static shape: KC * NC. NOTE some parts may not be valid
valid_n: The valid cloumns of blockB
valid_k: The valid rows of blockA
The valid elements of blockB are from 0 to (valid_n * valid_k)
*/

void pack_blockB_panel(float *B, float *blockB, const int nc, const int kc,
                       const int K) {
  // int block_n = NR;
  for (int i = 0; i < kc; i++) {
    for (int j = 0; j < nc; j++) {
      *(blockB++) = *B(i, j);
      // *(blockB( i, j )) = *B(i, j);
    }
  }
}

void pack_blockB(float *B, float *blockB, const int nc, const int kc,
                 const int K) {
  for (int jc = 0; jc < nc; jc += NR) {
    int valid_NR = min(nc - jc, NR);
    pack_blockB_panel(B(0, jc), blockB + jc * kc, valid_NR, kc, K);
  }
}

/*
j: column index
i: row index
p: hidden index
c: Cache level
r: register level
*/
void matmul_kernel(float *A, float *B, float *C, const int M, const int N,
                   const int K) {
  for (int jc = 0; jc < N; jc += NC) {
    int valid_NC = min(N - jc, NC);
    for (int pc = 0; pc < K; pc += KC) {
      int valid_KC = min(K - pc, KC);
      pack_blockB(B(pc, jc), blockB_packed, valid_NC, valid_KC, K);
      for (int ic = 0; ic < M; ic += MC) {
        int valid_MC = min(M - ic, MC);
        pack_blockA(A(ic, pc), blockA_packed, valid_MC, valid_KC, K);
        // PRAGMA_OMP_PARALLEL_FOR
        #pragma omp parallel for collapse(2) schedule(dynamic) num_threads(NTHREADS)
        for (int jr = 0; jr < valid_NC; jr += NR) {
          for (int ir = 0; ir < valid_MC; ir += MR) {
            int valid_NR = min(valid_NC - jr, NR);
            int valid_MR = min(valid_MC - ir, MR);
            float *blockA_panel = blockA_packed + ir * valid_KC;
            float *blockB_panel = blockB_packed + jr * valid_KC;
            microkernel_16x6(blockA_panel, blockB_panel, C(ic + ir, jc + jr),
                             valid_MR, valid_NR, valid_MR, valid_NR, valid_KC,
                             N);
          }
        }
      }
    }
  }
}

void matmul_naive(float *A, float *B, float *C, const int M, const int N,
                  const int K) {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      float acc = 0;
      for (int p = 0; p < K; p++) {
        // C[j * M + i] += A[p * M + i] * B[j * K + p];
        acc += *A(i, p) * *B(p, j);
      }
      *C(i, j) = acc;
    }
  }
}

int main() {
  const int M = MDIM;
  const int N = NDIM;
  const int K = KDIM;
  float *A = (float *)_mm_malloc(M * K * sizeof(float), MEM_ALIGN);
  float *B = (float *)_mm_malloc(K * N * sizeof(float), MEM_ALIGN);
  float *C = (float *)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
  float *C_ref = (float *)_mm_malloc(M * N * sizeof(float), MEM_ALIGN);
  init_rand(A, M, K);
  init_rand(B, K, N);

  printf("\e[31m[INFO]\e[m KC: %d, MC: %d, NC: %d, MR: %d, NR: %d\n", KC, MC,
         NC, MR, NR);

#ifdef TEST
  matmul_naive(A, B, C_ref, M, N, K);
#endif
  double FLOP = 2 * (double)M * N * K;

  for (int i = 0; i < NITER; i++) {
    init_const(C, 0.0, M, N);
    // init_const(C, -1000, M, N);
    uint64_t start = timer();
    matmul_kernel(A, B, C, M, N, K);
    uint64_t end = timer();

    double exec_time = (end - start) * 1e-9;
    double FLOPS = FLOP / exec_time;

    printf("GFLOPS = \e[32m%.3f \e[m M: %d, N: %d, K: %d\n\n", FLOPS / 1e9, M, N, K);
#ifdef TEST
    int res = compare_mats(C, C_ref, M, N);
#endif
    printf("\n");
  }

  _mm_free(A);
  _mm_free(B);
  _mm_free(C);
  _mm_free(C_ref);

  return 0;
}