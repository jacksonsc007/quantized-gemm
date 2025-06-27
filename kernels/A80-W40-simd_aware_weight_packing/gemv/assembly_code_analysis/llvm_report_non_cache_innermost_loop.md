Iterations:        100
Instructions:      3600
Total Cycles:      846
Total uOps:        3700

Dispatch Width:    6
uOps Per Cycle:    4.37
IPC:               4.26
Block RThroughput: 7.5


Cycles with backend pressure increase [ 70.33% ]
Throughput Bottlenecks: 
  Resource Pressure       [ 68.91% ]
  - ADLPPort00  [ 68.68% ]
  - ADLPPort01  [ 68.68% ]
  - ADLPPort02  [ 0.12% ]
  - ADLPPort03  [ 0.12% ]
  - ADLPPort05  [ 9.69% ]
  - ADLPPort06  [ 3.55% ]
  - ADLPPort11  [ 0.12% ]
  Data Dependencies:      [ 38.77% ]
  - Register Dependencies [ 38.77% ]
  - Memory Dependencies   [ 0.00% ]

Critical sequence based on the simulation:

              Instruction                                 Dependency Information
 +----< 28.   vfmadd231ps	%ymm4, %ymm8, %ymm0
 |
 |    < loop carried > 
 |
 |      0.    movl	%eax, %edx
 |      1.    vmovdqu	(%rdi,%rax), %ymm9
 |      2.    sarl	$5, %edx
 |      3.    movslq	%edx, %rdx
 |      4.    vpsignb	%ymm9, %ymm9, %ymm8
 |      5.    leaq	(%rdx,%r11), %rcx
 |      6.    vmovss	(%rsi,%rdx,4), %xmm4
 |      7.    vmovss	4(%rsi,%rdx,4), %xmm3
 |      8.    movl	%eax, %edx
 |      9.    vmulss	(%r8,%rcx,4), %xmm4, %xmm4
 |      10.   sarl	%edx
 |      11.   vmulss	4(%r8,%rcx,4), %xmm3, %xmm3
 |      12.   movslq	%edx, %rdx
 |      13.   vmovdqu	(%r10,%rdx), %ymm1
 |      14.   vpsrlw	$4, %ymm1, %ymm2
 |      15.   vpand	%ymm7, %ymm1, %ymm1
 |      16.   vpaddb	%ymm1, %ymm6, %ymm1
 |      17.   vbroadcastss	%xmm4, %ymm4
 |      18.   vpand	%ymm2, %ymm7, %ymm2
 +----> 19.   vpsignb	%ymm9, %ymm1, %ymm1               ## RESOURCE interference:  ADLPPort00 [ probability: 71% ]
 |      20.   vmovdqu	32(%rdi,%rax), %ymm9
 |      21.   vpaddb	%ymm2, %ymm6, %ymm2
 |      22.   vbroadcastss	%xmm3, %ymm3
 +----> 23.   vpmaddubsw	%ymm1, %ymm8, %ymm1               ## REGISTER dependency:  %ymm1
 |      24.   addq	$64, %rax
 +----> 25.   vpmaddwd	%ymm1, %ymm5, %ymm1               ## REGISTER dependency:  %ymm1
 |      26.   vpsignb	%ymm9, %ymm2, %ymm2
 +----> 27.   vcvtdq2ps	%ymm1, %ymm8                      ## REGISTER dependency:  %ymm1
 |      28.   vfmadd231ps	%ymm4, %ymm8, %ymm0
 |      29.   vpsignb	%ymm9, %ymm9, %ymm1
 +----> 30.   vpmaddubsw	%ymm2, %ymm1, %ymm1               ## RESOURCE interference:  ADLPPort01 [ probability: 43% ]
 +----> 31.   vpmaddwd	%ymm1, %ymm5, %ymm1               ## REGISTER dependency:  %ymm1
 |      32.   vcvtdq2ps	%ymm1, %ymm1
 |      33.   vfmadd231ps	%ymm3, %ymm1, %ymm0
 |      34.   cmpl	%eax, %r9d
 |      35.   jg	.L4
 |
 |    < loop carried > 
 |
 +----> 19.   vpsignb	%ymm9, %ymm1, %ymm1               ## RESOURCE interference:  ADLPPort01 [ probability: 43% ]


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%eax, %edx
 1      8     0.33    *                   vmovdqu	(%rdi,%rax), %ymm9
 1      1     0.50                        sarl	$5, %edx
 1      1     0.33                        movslq	%edx, %rdx
 1      1     0.50                        vpsignb	%ymm9, %ymm9, %ymm8
 1      1     1.00                        leaq	(%rdx,%r11), %rcx
 1      7     0.33    *                   vmovss	(%rsi,%rdx,4), %xmm4
 1      7     0.33    *                   vmovss	4(%rsi,%rdx,4), %xmm3
 1      0     0.20                        movl	%eax, %edx
 2      11    0.50    *                   vmulss	(%r8,%rcx,4), %xmm4, %xmm4
 1      1     0.50                        sarl	%edx
 2      11    0.50    *                   vmulss	4(%r8,%rcx,4), %xmm3, %xmm3
 1      1     0.33                        movslq	%edx, %rdx
 1      8     0.33    *                   vmovdqu	(%r10,%rdx), %ymm1
 1      1     0.50                        vpsrlw	$4, %ymm1, %ymm2
 1      1     0.33                        vpand	%ymm7, %ymm1, %ymm1
 1      1     0.33                        vpaddb	%ymm1, %ymm6, %ymm1
 1      3     1.00                        vbroadcastss	%xmm4, %ymm4
 1      1     0.33                        vpand	%ymm2, %ymm7, %ymm2
 1      1     0.50                        vpsignb	%ymm9, %ymm1, %ymm1
 1      8     0.33    *                   vmovdqu	32(%rdi,%rax), %ymm9
 1      1     0.33                        vpaddb	%ymm2, %ymm6, %ymm2
 1      3     1.00                        vbroadcastss	%xmm3, %ymm3
 1      5     0.50                        vpmaddubsw	%ymm1, %ymm8, %ymm1
 0      1     0.00                        addq	$64, %rax
 1      5     0.50                        vpmaddwd	%ymm1, %ymm5, %ymm1
 1      1     0.50                        vpsignb	%ymm9, %ymm2, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm1, %ymm8
 1      4     0.50                        vfmadd231ps	%ymm4, %ymm8, %ymm0
 1      1     0.50                        vpsignb	%ymm9, %ymm9, %ymm1
 1      5     0.50                        vpmaddubsw	%ymm2, %ymm1, %ymm1
 1      5     0.50                        vpmaddwd	%ymm1, %ymm5, %ymm1
 1      4     0.50                        vcvtdq2ps	%ymm1, %ymm1
 1      4     0.50                        vfmadd231ps	%ymm3, %ymm1, %ymm0
 1      1     0.20                        cmpl	%eax, %r9d
 1      1     0.50                        jg	.L4


Resources:
[0]   - ADLPPort00
[1]   - ADLPPort01
[2]   - ADLPPort02
[3]   - ADLPPort03
[4]   - ADLPPort04
[5]   - ADLPPort05
[6]   - ADLPPort06
[7]   - ADLPPort07
[8]   - ADLPPort08
[9]   - ADLPPort09
[10]  - ADLPPort10
[11]  - ADLPPort11
[12]  - ADLPPortInvalid


Resource pressure per iteration:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   
8.20   8.22   2.33   2.33    -     5.95   3.83    -      -      -     3.80   2.34    -     

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   Instructions:
 -      -      -      -      -     0.03   0.02    -      -      -     0.95    -      -     movl	%eax, %edx
 -      -     0.47   0.18    -      -      -      -      -      -      -     0.35    -     vmovdqu	(%rdi,%rax), %ymm9
0.02    -      -      -      -      -     0.98    -      -      -      -      -      -     sarl	$5, %edx
 -      -      -      -      -     0.01    -      -      -      -     0.99    -      -     movslq	%edx, %rdx
0.02   0.98    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm9, %ymm8
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%rdx,%r11), %rcx
 -      -     0.34   0.33    -      -      -      -      -      -      -     0.33    -     vmovss	(%rsi,%rdx,4), %xmm4
 -      -     0.33   0.33    -      -      -      -      -      -      -     0.34    -     vmovss	4(%rsi,%rdx,4), %xmm3
 -     0.01    -      -      -     0.27    -      -      -      -     0.72    -      -     movl	%eax, %edx
0.62   0.38   0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmulss	(%r8,%rcx,4), %xmm4, %xmm4
0.01    -      -      -      -      -     0.99    -      -      -      -      -      -     sarl	%edx
0.18   0.82   0.20   0.48    -      -      -      -      -      -      -     0.32    -     vmulss	4(%r8,%rcx,4), %xmm3, %xmm3
 -      -      -      -      -     0.02    -      -      -      -     0.98    -      -     movslq	%edx, %rdx
 -      -     0.49   0.32    -      -      -      -      -      -      -     0.19    -     vmovdqu	(%r10,%rdx), %ymm1
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm1, %ymm2
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vpand	%ymm7, %ymm1, %ymm1
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vpaddb	%ymm1, %ymm6, %ymm1
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm4, %ymm4
0.01   0.06    -      -      -     0.93    -      -      -      -      -      -      -     vpand	%ymm2, %ymm7, %ymm2
0.76   0.24    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm1, %ymm1
 -      -     0.17   0.35    -      -      -      -      -      -      -     0.48    -     vmovdqu	32(%rdi,%rax), %ymm9
 -     0.31    -      -      -     0.69    -      -      -      -      -      -      -     vpaddb	%ymm2, %ymm6, %ymm2
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm3, %ymm3
0.66   0.34    -      -      -      -      -      -      -      -      -      -      -     vpmaddubsw	%ymm1, %ymm8, %ymm1
 -      -      -      -      -      -      -      -      -      -      -      -      -     addq	$64, %rax
0.55   0.45    -      -      -      -      -      -      -      -      -      -      -     vpmaddwd	%ymm1, %ymm5, %ymm1
0.19   0.81    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm2, %ymm2
0.40   0.60    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm1, %ymm8
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm4, %ymm8, %ymm0
0.34   0.66    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm9, %ymm1
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vpmaddubsw	%ymm2, %ymm1, %ymm1
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     vpmaddwd	%ymm1, %ymm5, %ymm1
0.95   0.05    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm1, %ymm1
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm3, %ymm1, %ymm0
 -      -      -      -      -      -     0.84    -      -      -     0.16    -      -     cmpl	%eax, %r9d
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L4
