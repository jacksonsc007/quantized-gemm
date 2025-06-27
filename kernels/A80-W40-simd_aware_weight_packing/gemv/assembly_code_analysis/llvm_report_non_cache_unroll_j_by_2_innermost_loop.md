Iterations:        100
Instructions:      6000
Total Cycles:      1577
Total uOps:        6300

Dispatch Width:    6
uOps Per Cycle:    3.99
IPC:               3.80
Block RThroughput: 14.0


Cycles with backend pressure increase [ 86.81% ]
Throughput Bottlenecks: 
  Resource Pressure       [ 86.18% ]
  - ADLPPort00  [ 85.92% ]
  - ADLPPort01  [ 86.18% ]
  - ADLPPort05  [ 46.16% ]
  - ADLPPort06  [ 3.17% ]
  Data Dependencies:      [ 43.56% ]
  - Register Dependencies [ 43.56% ]
  - Memory Dependencies   [ 0.00% ]

Critical sequence based on the simulation:

              Instruction                                 Dependency Information
 +----< 39.   vpmaddwd	%ymm0, %ymm8, %ymm0
 |
 |    < loop carried > 
 |
 |      0.    movl	%eax, %r12d
 |      1.    vmovdqu	(%rdi,%rax), %ymm12
 |      2.    vmovdqu	32(%rdi,%rax), %ymm11
 |      3.    sarl	$5, %r12d
 |      4.    movslq	%r12d, %r12
 |      5.    vpsignb	%ymm12, %ymm12, %ymm0
 |      6.    vpsignb	%ymm11, %ymm11, %ymm5
 +----> 7.    leaq	(%r12,%r9), %rdx                  ## RESOURCE interference:  ADLPPort01 [ probability: 25% ]
 |      8.    leaq	(%r12,%rbx), %r15
 |      9.    vmovss	(%rsi,%r12,4), %xmm3
 |      10.   vmovss	4(%rsi,%r12,4), %xmm2
 |      11.   vmulss	(%r8,%r15,4), %xmm3, %xmm15
 |      12.   vmulss	4(%r8,%r15,4), %xmm2, %xmm7
 +----> 13.   vmulss	(%r8,%rdx,4), %xmm3, %xmm3        ## REGISTER dependency:  %rdx
 +----> 14.   vmulss	4(%r8,%rdx,4), %xmm2, %xmm2       ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 |      15.   movl	%eax, %edx
 |      16.   addq	$64, %rax
 |      17.   sarl	%edx
 |      18.   movslq	%edx, %rdx
 |      19.   vbroadcastss	%xmm15, %ymm15
 |      20.   vmovdqu	(%r11,%rdx), %ymm10
 |      21.   addq	%r10, %rdx
 |      22.   vmovd	%xmm7, %r12d
 |      23.   vmovdqu	(%rdx,%rcx), %ymm6
 |      24.   vbroadcastss	%xmm3, %ymm3
 +----> 25.   vpsrlw	$4, %ymm10, %ymm4                 ## RESOURCE interference:  ADLPPort01 [ probability: 72% ]
 |      26.   vpand	%ymm14, %ymm10, %ymm10
 |      27.   vbroadcastss	%xmm2, %ymm2
 |      28.   vpsrlw	$4, %ymm6, %ymm7
 |      29.   vpand	%ymm6, %ymm14, %ymm6
 |      30.   vpaddb	%ymm10, %ymm13, %ymm10
 |      31.   vpaddb	%ymm6, %ymm13, %ymm6
 |      32.   vpsignb	%ymm12, %ymm10, %ymm10
 +----> 33.   vpand	%ymm4, %ymm14, %ymm4              ## REGISTER dependency:  %ymm4
 |      34.   vpsignb	%ymm12, %ymm6, %ymm6
 |      35.   vpmaddubsw	%ymm10, %ymm0, %ymm10
 |      36.   vpand	%ymm7, %ymm14, %ymm7
 |      37.   vpmaddubsw	%ymm6, %ymm0, %ymm0
 |      38.   vpmaddwd	%ymm10, %ymm8, %ymm10
 |      39.   vpmaddwd	%ymm0, %ymm8, %ymm0
 |      40.   vcvtdq2ps	%ymm10, %ymm10
 |      41.   vfmadd231ps	%ymm15, %ymm10, %ymm9
 +----> 42.   vpaddb	%ymm4, %ymm13, %ymm4              ## REGISTER dependency:  %ymm4
 |      43.   vcvtdq2ps	%ymm0, %ymm0
 |      44.   vfmadd231ps	%ymm3, %ymm0, %ymm1
 |      45.   vpaddb	%ymm7, %ymm13, %ymm7
 |      46.   vmovd	%r12d, %xmm15
 +----> 47.   vpsignb	%ymm11, %ymm4, %ymm4              ## REGISTER dependency:  %ymm4
 |      48.   vpsignb	%ymm11, %ymm7, %ymm7
 |      49.   vbroadcastss	%xmm15, %ymm10
 +----> 50.   vpmaddubsw	%ymm4, %ymm5, %ymm4               ## REGISTER dependency:  %ymm4
 |      51.   vpmaddubsw	%ymm7, %ymm5, %ymm5
 +----> 52.   vpmaddwd	%ymm4, %ymm8, %ymm4               ## REGISTER dependency:  %ymm4
 |      53.   vpmaddwd	%ymm5, %ymm8, %ymm5
 +----> 54.   vcvtdq2ps	%ymm4, %ymm4                      ## REGISTER dependency:  %ymm4
 |      55.   vcvtdq2ps	%ymm5, %ymm5
 |      56.   vfmadd231ps	%ymm10, %ymm4, %ymm9
 |      57.   vfmadd231ps	%ymm2, %ymm5, %ymm1
 |      58.   cmpl	%eax, %r13d
 |      59.   jg	.L4
 |
 |    < loop carried > 
 |
 +----> 28.   vpsrlw	$4, %ymm6, %ymm7                  ## RESOURCE interference:  ADLPPort00 [ probability: 72% ]


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%eax, %r12d
 1      8     0.33    *                   vmovdqu	(%rdi,%rax), %ymm12
 1      8     0.33    *                   vmovdqu	32(%rdi,%rax), %ymm11
 1      1     0.50                        sarl	$5, %r12d
 1      1     0.33                        movslq	%r12d, %r12
 1      1     0.50                        vpsignb	%ymm12, %ymm12, %ymm0
 1      1     0.50                        vpsignb	%ymm11, %ymm11, %ymm5
 1      1     1.00                        leaq	(%r12,%r9), %rdx
 1      1     1.00                        leaq	(%r12,%rbx), %r15
 1      7     0.33    *                   vmovss	(%rsi,%r12,4), %xmm3
 1      7     0.33    *                   vmovss	4(%rsi,%r12,4), %xmm2
 2      11    0.50    *                   vmulss	(%r8,%r15,4), %xmm3, %xmm15
 2      11    0.50    *                   vmulss	4(%r8,%r15,4), %xmm2, %xmm7
 2      11    0.50    *                   vmulss	(%r8,%rdx,4), %xmm3, %xmm3
 2      11    0.50    *                   vmulss	4(%r8,%rdx,4), %xmm2, %xmm2
 1      0     0.20                        movl	%eax, %edx
 0      1     0.00                        addq	$64, %rax
 1      1     0.50                        sarl	%edx
 1      1     0.33                        movslq	%edx, %rdx
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      8     0.33    *                   vmovdqu	(%r11,%rdx), %ymm10
 1      1     0.20                        addq	%r10, %rdx
 1      3     1.00                        vmovd	%xmm7, %r12d
 1      8     0.33    *                   vmovdqu	(%rdx,%rcx), %ymm6
 1      3     1.00                        vbroadcastss	%xmm3, %ymm3
 1      1     0.50                        vpsrlw	$4, %ymm10, %ymm4
 1      1     0.33                        vpand	%ymm14, %ymm10, %ymm10
 1      3     1.00                        vbroadcastss	%xmm2, %ymm2
 1      1     0.50                        vpsrlw	$4, %ymm6, %ymm7
 1      1     0.33                        vpand	%ymm6, %ymm14, %ymm6
 1      1     0.33                        vpaddb	%ymm10, %ymm13, %ymm10
 1      1     0.33                        vpaddb	%ymm6, %ymm13, %ymm6
 1      1     0.50                        vpsignb	%ymm12, %ymm10, %ymm10
 1      1     0.33                        vpand	%ymm4, %ymm14, %ymm4
 1      1     0.50                        vpsignb	%ymm12, %ymm6, %ymm6
 1      5     0.50                        vpmaddubsw	%ymm10, %ymm0, %ymm10
 1      1     0.33                        vpand	%ymm7, %ymm14, %ymm7
 1      5     0.50                        vpmaddubsw	%ymm6, %ymm0, %ymm0
 1      5     0.50                        vpmaddwd	%ymm10, %ymm8, %ymm10
 1      5     0.50                        vpmaddwd	%ymm0, %ymm8, %ymm0
 1      4     0.50                        vcvtdq2ps	%ymm10, %ymm10
 1      4     0.50                        vfmadd231ps	%ymm15, %ymm10, %ymm9
 1      1     0.33                        vpaddb	%ymm4, %ymm13, %ymm4
 1      4     0.50                        vcvtdq2ps	%ymm0, %ymm0
 1      4     0.50                        vfmadd231ps	%ymm3, %ymm0, %ymm1
 1      1     0.33                        vpaddb	%ymm7, %ymm13, %ymm7
 1      3     1.00                        vmovd	%r12d, %xmm15
 1      1     0.50                        vpsignb	%ymm11, %ymm4, %ymm4
 1      1     0.50                        vpsignb	%ymm11, %ymm7, %ymm7
 1      3     1.00                        vbroadcastss	%xmm15, %ymm10
 1      5     0.50                        vpmaddubsw	%ymm4, %ymm5, %ymm4
 1      5     0.50                        vpmaddubsw	%ymm7, %ymm5, %ymm5
 1      5     0.50                        vpmaddwd	%ymm4, %ymm8, %ymm4
 1      5     0.50                        vpmaddwd	%ymm5, %ymm8, %ymm5
 1      4     0.50                        vcvtdq2ps	%ymm4, %ymm4
 1      4     0.50                        vcvtdq2ps	%ymm5, %ymm5
 1      4     0.50                        vfmadd231ps	%ymm10, %ymm4, %ymm9
 1      4     0.50                        vfmadd231ps	%ymm2, %ymm5, %ymm1
 1      1     0.20                        cmpl	%eax, %r13d
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
15.53  15.53  3.33   3.33    -     12.99  4.49    -      -      -     4.46   3.34    -     

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   Instructions:
 -      -      -      -      -     0.01   0.03    -      -      -     0.96    -      -     movl	%eax, %r12d
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovdqu	(%rdi,%rax), %ymm12
 -      -     0.34   0.34    -      -      -      -      -      -      -     0.32    -     vmovdqu	32(%rdi,%rax), %ymm11
0.02    -      -      -      -      -     0.98    -      -      -      -      -      -     sarl	$5, %r12d
 -      -      -      -      -     0.01    -      -      -      -     0.99    -      -     movslq	%r12d, %r12
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm12, %ymm12, %ymm0
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm11, %ymm11, %ymm5
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%r12,%r9), %rdx
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%r12,%rbx), %r15
 -      -     0.33   0.33    -      -      -      -      -      -      -     0.34    -     vmovss	(%rsi,%r12,4), %xmm3
 -      -     0.32   0.34    -      -      -      -      -      -      -     0.34    -     vmovss	4(%rsi,%r12,4), %xmm2
0.26   0.74   0.37   0.31    -      -      -      -      -      -      -     0.32    -     vmulss	(%r8,%r15,4), %xmm3, %xmm15
0.49   0.51   0.32   0.33    -      -      -      -      -      -      -     0.35    -     vmulss	4(%r8,%r15,4), %xmm2, %xmm7
0.25   0.75   0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmulss	(%r8,%rdx,4), %xmm3, %xmm3
0.26   0.74   0.32   0.35    -      -      -      -      -      -      -     0.33    -     vmulss	4(%r8,%rdx,4), %xmm2, %xmm2
 -      -      -      -      -      -     0.48    -      -      -     0.52    -      -     movl	%eax, %edx
 -      -      -      -      -      -      -      -      -      -      -      -      -     addq	$64, %rax
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     sarl	%edx
 -      -      -      -      -     0.01    -      -      -      -     0.99    -      -     movslq	%edx, %rdx
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm15
 -      -     0.35   0.32    -      -      -      -      -      -      -     0.33    -     vmovdqu	(%r11,%rdx), %ymm10
 -      -      -      -      -      -     0.51    -      -      -     0.49    -      -     addq	%r10, %rdx
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm7, %r12d
 -      -     0.32   0.33    -      -      -      -      -      -      -     0.35    -     vmovdqu	(%rdx,%rcx), %ymm6
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm3, %ymm3
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm10, %ymm4
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vpand	%ymm14, %ymm10, %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm2, %ymm2
0.24   0.76    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm6, %ymm7
0.01    -      -      -      -     0.99    -      -      -      -      -      -      -     vpand	%ymm6, %ymm14, %ymm6
 -     0.01    -      -      -     0.99    -      -      -      -      -      -      -     vpaddb	%ymm10, %ymm13, %ymm10
 -     0.01    -      -      -     0.99    -      -      -      -      -      -      -     vpaddb	%ymm6, %ymm13, %ymm6
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm12, %ymm10, %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vpand	%ymm4, %ymm14, %ymm4
0.24   0.76    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm12, %ymm6, %ymm6
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     vpmaddubsw	%ymm10, %ymm0, %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vpand	%ymm7, %ymm14, %ymm7
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     vpmaddubsw	%ymm6, %ymm0, %ymm0
0.24   0.76    -      -      -      -      -      -      -      -      -      -      -     vpmaddwd	%ymm10, %ymm8, %ymm10
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vpmaddwd	%ymm0, %ymm8, %ymm0
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm10, %ymm10
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm15, %ymm10, %ymm9
0.01    -      -      -      -     0.99    -      -      -      -      -      -      -     vpaddb	%ymm4, %ymm13, %ymm4
0.76   0.24    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm0, %ymm0
0.76   0.24    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm3, %ymm0, %ymm1
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vpaddb	%ymm7, %ymm13, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%r12d, %xmm15
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm11, %ymm4, %ymm4
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm11, %ymm7, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm10
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vpmaddubsw	%ymm4, %ymm5, %ymm4
0.24   0.76    -      -      -      -      -      -      -      -      -      -      -     vpmaddubsw	%ymm7, %ymm5, %ymm5
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vpmaddwd	%ymm4, %ymm8, %ymm4
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpmaddwd	%ymm5, %ymm8, %ymm5
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm4, %ymm4
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm5, %ymm5
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm10, %ymm4, %ymm9
0.76   0.24    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm2, %ymm5, %ymm1
 -      -      -      -      -      -     0.49    -      -      -     0.51    -      -     cmpl	%eax, %r13d
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L4
