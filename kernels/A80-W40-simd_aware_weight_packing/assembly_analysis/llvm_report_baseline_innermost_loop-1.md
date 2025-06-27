Iterations:        100
Instructions:      3700
Total Cycles:      827
Total uOps:        3600

Dispatch Width:    6
uOps Per Cycle:    4.35
IPC:               4.47
Block RThroughput: 6.5


Cycles with backend pressure increase [ 62.15% ]
Throughput Bottlenecks: 
  Resource Pressure       [ 60.94% ]
  - ADLPPort00  [ 59.73% ]
  - ADLPPort01  [ 60.58% ]
  - ADLPPort02  [ 1.33% ]
  - ADLPPort03  [ 1.33% ]
  - ADLPPort05  [ 59.49% ]
  - ADLPPort06  [ 4.23% ]
  - ADLPPort10  [ 0.12% ]
  - ADLPPort11  [ 1.33% ]
  Data Dependencies:      [ 22.61% ]
  - Register Dependencies [ 22.61% ]
  - Memory Dependencies   [ 0.00% ]

Critical sequence based on the simulation:

              Instruction                                 Dependency Information
 +----< 5.    leaq	(%rbx,%rax), %rcx
 |
 |    < loop carried > 
 |
 |      0.    movl	%edx, %eax
 |      1.    vmovdqu	(%rsi,%rdx), %ymm3
 |      2.    sarl	$5, %eax
 +----> 3.    cltq                                        ## RESOURCE interference:  ADLPPort01 [ probability: 21% ]
 |      4.    vpsignb	%ymm3, %ymm3, %ymm9
 +----> 5.    leaq	(%rbx,%rax), %rcx                 ## REGISTER dependency:  %rax
 |      6.    addq	%r11, %rax
 +----> 7.    vmovss	(%rdi,%rcx,4), %xmm8              ## REGISTER dependency:  %rcx
 |      8.    vmovss	4(%rdi,%rcx,4), %xmm4
 +----> 9.    vmulss	(%r10,%rax,4), %xmm8, %xmm8       ## RESOURCE interference:  ADLPPort02 [ probability: 1% ]
 |      10.   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
 |      11.   movl	%edx, %eax
 |      12.   sarl	%eax
 |      13.   cltq
 |      14.   vmovdqu	(%r9,%rax), %ymm0
 +----> 15.   vbroadcastss	%xmm8, %ymm8              ## REGISTER dependency:  %xmm8
 |      16.   vpsrlw	$4, %ymm0, %ymm2
 |      17.   vpand	%ymm7, %ymm0, %ymm0
 +----> 18.   vbroadcastss	%xmm4, %ymm4              ## RESOURCE interference:  ADLPPort05 [ probability: 49% ]
 |      19.   vpaddb	%ymm0, %ymm6, %ymm0
 |      20.   vpsignb	%ymm3, %ymm0, %ymm0
 |      21.   vmovdqa	%ymm5, %ymm3
 |      22.   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
 |      23.   vmovdqu	32(%rsi,%rdx), %ymm9
 |      24.   vpand	%ymm2, %ymm7, %ymm0
 |      25.   vmovdqa	%ymm5, %ymm2
 |      26.   vpaddb	%ymm0, %ymm6, %ymm0
 |      27.   addq	$64, %rdx
 |      28.   vpsignb	%ymm9, %ymm0, %ymm0
 |      29.   vpsignb	%ymm9, %ymm9, %ymm10
 |      30.   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
 |      31.   vcvtdq2ps	%ymm3, %ymm3
 |      32.   vfmadd231ps	%ymm8, %ymm3, %ymm1
 |      33.   vcvtdq2ps	%ymm2, %ymm0
 +----> 34.   vfmadd231ps	%ymm4, %ymm0, %ymm1       ## REGISTER dependency:  %ymm4
 |      35.   cmpl	%edx, %r8d
 |      36.   jg	.L5
 |
 |    < loop carried > 
 |
 +----> 29.   vpsignb	%ymm9, %ymm9, %ymm10              ## RESOURCE interference:  ADLPPort00 [ probability: 16% ]


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%edx, %eax
 1      8     0.33    *                   vmovdqu	(%rsi,%rdx), %ymm3
 1      1     0.50                        sarl	$5, %eax
 1      1     0.50                        cltq
 1      1     0.50                        vpsignb	%ymm3, %ymm3, %ymm9
 1      1     1.00                        leaq	(%rbx,%rax), %rcx
 1      1     0.20                        addq	%r11, %rax
 1      7     0.33    *                   vmovss	(%rdi,%rcx,4), %xmm8
 1      7     0.33    *                   vmovss	4(%rdi,%rcx,4), %xmm4
 2      11    0.50    *                   vmulss	(%r10,%rax,4), %xmm8, %xmm8
 2      11    0.50    *                   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
 1      0     0.20                        movl	%edx, %eax
 1      1     0.50                        sarl	%eax
 1      1     0.50                        cltq
 1      8     0.33    *                   vmovdqu	(%r9,%rax), %ymm0
 1      3     1.00                        vbroadcastss	%xmm8, %ymm8
 1      1     0.50                        vpsrlw	$4, %ymm0, %ymm2
 1      1     0.33                        vpand	%ymm7, %ymm0, %ymm0
 1      3     1.00                        vbroadcastss	%xmm4, %ymm4
 1      1     0.33                        vpaddb	%ymm0, %ymm6, %ymm0
 1      1     0.50                        vpsignb	%ymm3, %ymm0, %ymm0
 0      1     0.00                        vmovdqa	%ymm5, %ymm3
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
 1      8     0.33    *                   vmovdqu	32(%rsi,%rdx), %ymm9
 1      1     0.33                        vpand	%ymm2, %ymm7, %ymm0
 0      1     0.00                        vmovdqa	%ymm5, %ymm2
 1      1     0.33                        vpaddb	%ymm0, %ymm6, %ymm0
 0      1     0.00                        addq	$64, %rdx
 1      1     0.50                        vpsignb	%ymm9, %ymm0, %ymm0
 1      1     0.50                        vpsignb	%ymm9, %ymm9, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm3
 1      4     0.50                        vfmadd231ps	%ymm8, %ymm3, %ymm1
 1      4     0.50                        vcvtdq2ps	%ymm2, %ymm0
 1      4     0.50                        vfmadd231ps	%ymm4, %ymm0, %ymm1
 1      1     0.20                        cmpl	%edx, %r8d
 1      1     0.50                        jg	.L5


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
7.40   7.43   2.33   2.33    -     7.21   3.69    -      -      -     3.27   2.34    -     

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   Instructions:
 -      -      -      -      -     0.01   0.27    -      -      -     0.72    -      -     movl	%edx, %eax
 -      -     0.33   0.29    -      -      -      -      -      -      -     0.38    -     vmovdqu	(%rsi,%rdx), %ymm3
0.01    -      -      -      -      -     0.99    -      -      -      -      -      -     sarl	$5, %eax
 -     0.02    -      -      -     0.98    -      -      -      -      -      -      -     cltq
0.14   0.86    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm3, %ymm9
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%rbx,%rax), %rcx
0.01    -      -      -      -      -     0.13    -      -      -     0.86    -      -     addq	%r11, %rax
 -      -     0.33   0.39    -      -      -      -      -      -      -     0.28    -     vmovss	(%rdi,%rcx,4), %xmm8
 -      -     0.39   0.28    -      -      -      -      -      -      -     0.33    -     vmovss	4(%rdi,%rcx,4), %xmm4
0.49   0.51   0.35   0.37    -      -      -      -      -      -      -     0.28    -     vmulss	(%r10,%rax,4), %xmm8, %xmm8
0.21   0.79   0.31   0.31    -      -      -      -      -      -      -     0.38    -     vmulss	4(%r10,%rax,4), %xmm4, %xmm4
 -      -      -      -      -      -     0.09    -      -      -     0.91    -      -     movl	%edx, %eax
0.01    -      -      -      -      -     0.99    -      -      -      -      -      -     sarl	%eax
 -     0.01    -      -      -     0.99    -      -      -      -      -      -      -     cltq
 -      -     0.37   0.34    -      -      -      -      -      -      -     0.29    -     vmovdqu	(%r9,%rax), %ymm0
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm8, %ymm8
0.53   0.47    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm0, %ymm2
0.03   0.04    -      -      -     0.93    -      -      -      -      -      -      -     vpand	%ymm7, %ymm0, %ymm0
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm4, %ymm4
0.18   0.02    -      -      -     0.80    -      -      -      -      -      -      -     vpaddb	%ymm0, %ymm6, %ymm0
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm0, %ymm0
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm5, %ymm3
0.48   0.52    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
 -      -     0.25   0.35    -      -      -      -      -      -      -     0.40    -     vmovdqu	32(%rsi,%rdx), %ymm9
0.02   0.27    -      -      -     0.71    -      -      -      -      -      -      -     vpand	%ymm2, %ymm7, %ymm0
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm5, %ymm2
0.05   0.16    -      -      -     0.79    -      -      -      -      -      -      -     vpaddb	%ymm0, %ymm6, %ymm0
 -      -      -      -      -      -      -      -      -      -      -      -      -     addq	$64, %rdx
0.37   0.63    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm0, %ymm0
0.42   0.58    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm9, %ymm10
0.66   0.34    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
0.73   0.27    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm3
0.98   0.02    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm8, %ymm3, %ymm1
0.68   0.32    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm2, %ymm0
0.89   0.11    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm4, %ymm0, %ymm1
 -      -      -      -      -      -     0.22    -      -      -     0.78    -      -     cmpl	%edx, %r8d
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L5
