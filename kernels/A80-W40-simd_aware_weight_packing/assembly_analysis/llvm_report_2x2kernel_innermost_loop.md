Iterations:        100
Instructions:      11200
Total Cycles:      2536
Total uOps:        12200

Dispatch Width:    6
uOps Per Cycle:    4.81
IPC:               4.42
Block RThroughput: 23.0


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%eax, %edx
 1      8     0.33    *                   vmovdqu	(%rdi,%rax), %ymm7
 0      1     0.00                        vmovdqa	%ymm12, %ymm10
 1      1     0.50                        sarl	$5, %edx
 1      1     0.33                        movslq	%edx, %rdx
 1      7     0.33    *                   vmovss	(%rsi,%rdx,4), %xmm5
 1      7     0.33    *                   vmovss	(%rcx,%rdx,4), %xmm14
 1      1     1.00                        leaq	(%rdx,%r12), %r13
 1      1     1.00                        leaq	(,%rdx,4), %r15
 1      0     0.20                        movl	%eax, %edx
 1      1     1.00                        leaq	(,%r13,4), %r14
 1      7     0.33    *                   vmovss	(%rsi,%r13,4), %xmm13
 1      7     0.33    *                   vmovss	(%rcx,%r13,4), %xmm11
 1      1     0.50                        sarl	%edx
 1      7     0.33    *                   vmovss	4(%rsi,%r14), %xmm3
 2      12    0.50           *            vmovss	%xmm14, -4(%rsp)
 1      1     0.50                        vpsignb	%ymm7, %ymm7, %ymm14
 1      1     0.33                        movslq	%edx, %rdx
 1      7     0.33    *                   vmovss	4(%rcx,%r14), %xmm15
 2      12    0.50           *            vmovss	%xmm11, -20(%rsp)
 0      1     0.00                        vmovdqa	%ymm12, %ymm11
 1      8     0.33    *                   vmovdqu	(%r9,%rdx), %ymm1
 2      12    0.50           *            vmovss	%xmm3, -16(%rsp)
 1      7     0.33    *                   vmovss	4(%rcx,%r15), %xmm2
 1      1     0.20                        addq	%r9, %rdx
 2      12    0.50           *            vmovss	%xmm13, -12(%rsp)
 1      7     0.33    *                   vmovss	4(%rsi,%r15), %xmm9
 1      8     0.33    *                   vmovdqu	(%rdx,%rbx), %ymm0
 1      1     0.50                        vpsrlw	$4, %ymm1, %ymm4
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm1, %ymm1
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm1, %ymm1
 2      12    0.50           *            vmovss	%xmm15, -24(%rsp)
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm4, %ymm4
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm4, %ymm4
 2      12    0.50           *            vmovss	%xmm2, -8(%rsp)
 1      1     0.50                        vpsignb	%ymm7, %ymm1, %ymm3
 1      1     0.50                        vpsrlw	$4, %ymm0, %ymm2
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm0, %ymm0
 1      5     0.50                        {vex}	vpdpbusd	%ymm3, %ymm14, %ymm10
 1      8     0.33    *                   vmovdqu	32(%rdi,%rax), %ymm3
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm0, %ymm0
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm2, %ymm2
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm2, %ymm2
 1      1     0.50                        vpsignb	%ymm3, %ymm3, %ymm13
 1      1     0.50                        vpsignb	%ymm7, %ymm0, %ymm7
 1      4     0.50                        vcvtdq2ps	%ymm10, %ymm15
 1      1     0.50                        vpsignb	%ymm3, %ymm4, %ymm10
 1      1     0.50                        vpsignb	%ymm3, %ymm2, %ymm3
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm13, %ymm11
 2      11    0.50    *                   vmulss	-4(%rsp), %xmm5, %xmm10
 1      3     1.00                        vbroadcastss	%xmm10, %ymm10
 2      12    0.50    *                   vfmadd213ps	-64(%rsp), %ymm15, %ymm10
 2      11    0.50    *                   vmulss	-8(%rsp), %xmm9, %xmm15
 1      4     0.50                        vcvtdq2ps	%ymm11, %ymm11
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm10, %ymm11
 0      1     0.00                        vmovdqa	%ymm12, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm7, %ymm14, %ymm10
 2      12    0.50           *            vmovaps	%ymm11, -64(%rsp)
 1      7     0.33    *                   vmovss	-20(%rsp), %xmm11
 1      4     0.50                        vmulss	%xmm11, %xmm5, %xmm5
 1      4     0.50                        vcvtdq2ps	%ymm10, %ymm7
 0      1     0.00                        vmovdqa	%ymm12, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm3, %ymm13, %ymm10
 1      3     1.00                        vbroadcastss	%xmm5, %ymm5
 2      12    0.50    *                   vfmadd213ps	-96(%rsp), %ymm7, %ymm5
 1      7     0.33    *                   vmovss	-24(%rsp), %xmm15
 1      7     0.33    *                   vmovss	-12(%rsp), %xmm13
 1      7     0.33    *                   vmovss	-4(%rsp), %xmm14
 1      4     0.50                        vmulss	%xmm15, %xmm9, %xmm9
 1      4     0.50                        vcvtdq2ps	%ymm10, %ymm3
 0      1     0.00                        vmovaps	%ymm3, %ymm7
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      4     0.50                        vfmadd132ps	%ymm9, %ymm5, %ymm7
 0      1     0.00                        vmovdqa	%ymm12, %ymm5
 2      12    0.50           *            vmovaps	%ymm7, -96(%rsp)
 1      8     0.33    *                   vmovdqu	(%r10,%rax), %ymm7
 1      1     0.50                        vpsignb	%ymm7, %ymm7, %ymm9
 1      1     0.50                        vpsignb	%ymm7, %ymm1, %ymm1
 1      1     0.50                        vpsignb	%ymm7, %ymm0, %ymm0
 1      5     0.50                        {vex}	vpdpbusd	%ymm1, %ymm9, %ymm5
 0      1     0.00                        vmovdqa	%ymm12, %ymm7
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm9, %ymm7
 1      4     0.50                        vcvtdq2ps	%ymm5, %ymm1
 1      8     0.33    *                   vmovdqu	32(%r10,%rax), %ymm5
 0      1     0.00                        addq	$64, %rax
 1      4     0.50                        vcvtdq2ps	%ymm7, %ymm0
 1      4     0.50                        vmulss	%xmm11, %xmm13, %xmm7
 1      1     0.50                        vpsignb	%ymm5, %ymm4, %ymm3
 1      1     0.50                        vpsignb	%ymm5, %ymm5, %ymm10
 0      1     0.00                        vmovdqa	%ymm12, %ymm4
 1      5     0.50                        {vex}	vpdpbusd	%ymm3, %ymm10, %ymm4
 1      4     0.50                        vmulss	%xmm13, %xmm14, %xmm3
 1      1     0.50                        vpsignb	%ymm5, %ymm2, %ymm2
 1      3     1.00                        vbroadcastss	%xmm7, %ymm7
 1      4     0.50                        vfmadd132ps	%ymm0, %ymm6, %ymm7
 1      3     1.00                        vbroadcastss	%xmm3, %ymm3
 1      4     0.50                        vfmadd132ps	%ymm3, %ymm8, %ymm1
 1      7     0.33    *                   vmovss	-16(%rsp), %xmm3
 1      7     0.33    *                   vmovss	-8(%rsp), %xmm8
 1      4     0.50                        vcvtdq2ps	%ymm4, %ymm4
 1      4     0.50                        vmulss	%xmm3, %xmm8, %xmm8
 1      4     0.50                        vmulss	%xmm15, %xmm3, %xmm6
 1      3     1.00                        vbroadcastss	%xmm8, %ymm8
 1      4     0.50                        vfmadd132ps	%ymm4, %ymm1, %ymm8
 0      1     0.00                        vmovdqa	%ymm12, %ymm1
 1      3     1.00                        vbroadcastss	%xmm6, %ymm6
 1      5     0.50                        {vex}	vpdpbusd	%ymm2, %ymm10, %ymm1
 1      4     0.50                        vcvtdq2ps	%ymm1, %ymm1
 1      4     0.50                        vfmadd132ps	%ymm1, %ymm7, %ymm6
 1      1     0.20                        cmpl	%eax, %r11d
 1      1     0.50                        jg	.L3


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
25.02  25.03  10.66  10.67  4.00   16.91  3.54   4.00   4.00   4.00   3.50   10.67   -     

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   Instructions:
 -      -      -      -      -      -     0.04    -      -      -     0.96    -      -     movl	%eax, %edx
 -      -     0.33   0.32    -      -      -      -      -      -      -     0.35    -     vmovdqu	(%rdi,%rax), %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm10
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     sarl	$5, %edx
 -      -      -      -      -     0.02    -      -      -      -     0.98    -      -     movslq	%edx, %rdx
 -      -     0.32   0.35    -      -      -      -      -      -      -     0.33    -     vmovss	(%rsi,%rdx,4), %xmm5
 -      -     0.35   0.33    -      -      -      -      -      -      -     0.32    -     vmovss	(%rcx,%rdx,4), %xmm14
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%rdx,%r12), %r13
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(,%rdx,4), %r15
 -     0.01    -      -      -     0.95    -      -      -      -     0.04    -      -     movl	%eax, %edx
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(,%r13,4), %r14
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovss	(%rsi,%r13,4), %xmm13
 -      -     0.34   0.33    -      -      -      -      -      -      -     0.33    -     vmovss	(%rcx,%r13,4), %xmm11
0.02    -      -      -      -      -     0.98    -      -      -      -      -      -     sarl	%edx
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovss	4(%rsi,%r14), %xmm3
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm14, -4(%rsp)
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm7, %ymm7, %ymm14
 -      -      -      -      -     0.49    -      -      -      -     0.51    -      -     movslq	%edx, %rdx
 -      -     0.34   0.33    -      -      -      -      -      -      -     0.33    -     vmovss	4(%rcx,%r14), %xmm15
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm11, -20(%rsp)
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm11
 -      -     0.35   0.32    -      -      -      -      -      -      -     0.33    -     vmovdqu	(%r9,%rdx), %ymm1
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm3, -16(%rsp)
 -      -     0.33   0.33    -      -      -      -      -      -      -     0.34    -     vmovss	4(%rcx,%r15), %xmm2
 -      -      -      -      -     0.47   0.03    -      -      -     0.50    -      -     addq	%r9, %rdx
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm13, -12(%rsp)
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovss	4(%rsi,%r15), %xmm9
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovdqu	(%rdx,%rbx), %ymm0
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm1, %ymm4
 -      -     0.32   0.36    -     1.00    -      -      -      -      -     0.32    -     vpand	.LC0(%rip), %ymm1, %ymm1
 -      -     0.33   0.33    -     1.00    -      -      -      -      -     0.34    -     vpaddb	.LC1(%rip), %ymm1, %ymm1
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm15, -24(%rsp)
0.99    -     0.31   0.35    -     0.01    -      -      -      -      -     0.34    -     vpand	.LC0(%rip), %ymm4, %ymm4
0.01    -     0.34   0.34    -     0.99    -      -      -      -      -     0.32    -     vpaddb	.LC1(%rip), %ymm4, %ymm4
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm2, -8(%rsp)
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm7, %ymm1, %ymm3
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm0, %ymm2
0.01    -     0.36   0.31    -     0.99    -      -      -      -      -     0.33    -     vpand	.LC0(%rip), %ymm0, %ymm0
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm3, %ymm14, %ymm10
 -      -     0.32   0.32    -      -      -      -      -      -      -     0.36    -     vmovdqu	32(%rdi,%rax), %ymm3
 -     0.01   0.33   0.33    -     0.99    -      -      -      -      -     0.34    -     vpaddb	.LC1(%rip), %ymm0, %ymm0
 -      -     0.33   0.34    -     1.00    -      -      -      -      -     0.33    -     vpand	.LC0(%rip), %ymm2, %ymm2
 -      -     0.33   0.33    -     1.00    -      -      -      -      -     0.34    -     vpaddb	.LC1(%rip), %ymm2, %ymm2
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm3, %ymm13
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm7, %ymm0, %ymm7
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm10, %ymm15
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm4, %ymm10
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm2, %ymm3
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm10, %ymm13, %ymm11
1.00    -     0.33   0.35    -      -      -      -      -      -      -     0.32    -     vmulss	-4(%rsp), %xmm5, %xmm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm10, %ymm10
 -     1.00   0.35   0.33    -      -      -      -      -      -      -     0.32    -     vfmadd213ps	-64(%rsp), %ymm15, %ymm10
 -     1.00   0.35   0.32    -      -      -      -      -      -      -     0.33    -     vmulss	-8(%rsp), %xmm9, %xmm15
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm11, %ymm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm15
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm15, %ymm10, %ymm11
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm10
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm7, %ymm14, %ymm10
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm11, -64(%rsp)
 -      -     0.34   0.33    -      -      -      -      -      -      -     0.33    -     vmovss	-20(%rsp), %xmm11
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm11, %xmm5, %xmm5
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm10, %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm10
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm3, %ymm13, %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm5, %ymm5
0.01   0.99   0.31   0.33    -      -      -      -      -      -      -     0.36    -     vfmadd213ps	-96(%rsp), %ymm7, %ymm5
 -      -     0.33   0.33    -      -      -      -      -      -      -     0.34    -     vmovss	-24(%rsp), %xmm15
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovss	-12(%rsp), %xmm13
 -      -     0.34   0.32    -      -      -      -      -      -      -     0.34    -     vmovss	-4(%rsp), %xmm14
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm15, %xmm9, %xmm9
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm10, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovaps	%ymm3, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm9, %ymm5, %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm5
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm7, -96(%rsp)
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovdqu	(%r10,%rax), %ymm7
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm7, %ymm7, %ymm9
0.02   0.98    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm7, %ymm1, %ymm1
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm7, %ymm0, %ymm0
1.00    -      -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm1, %ymm9, %ymm5
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm7
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm9, %ymm7
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm5, %ymm1
 -      -     0.33   0.33    -      -      -      -      -      -      -     0.34    -     vmovdqu	32(%r10,%rax), %ymm5
 -      -      -      -      -      -      -      -      -      -      -      -      -     addq	$64, %rax
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm7, %ymm0
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm11, %xmm13, %xmm7
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm5, %ymm4, %ymm3
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm5, %ymm5, %ymm10
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm4
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm3, %ymm10, %ymm4
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm13, %xmm14, %xmm3
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm5, %ymm2, %ymm2
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm7, %ymm7
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm0, %ymm6, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm3, %ymm3
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm3, %ymm8, %ymm1
 -      -     0.33   0.34    -      -      -      -      -      -      -     0.33    -     vmovss	-16(%rsp), %xmm3
 -      -     0.34   0.33    -      -      -      -      -      -      -     0.33    -     vmovss	-8(%rsp), %xmm8
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm4, %ymm4
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm3, %xmm8, %xmm8
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm15, %xmm3, %xmm6
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm8, %ymm8
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm4, %ymm1, %ymm8
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm12, %ymm1
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm6, %ymm6
1.00    -      -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm2, %ymm10, %ymm1
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm1, %ymm1
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm1, %ymm7, %ymm6
 -      -      -      -      -      -     0.49    -      -      -     0.51    -      -     cmpl	%eax, %r11d
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L3


Timeline view:
                    0123456789          0123456789          0123456789          0123456789
Index     0123456789          0123456789          0123456789          0123456789          

[0,0]     DER  .    .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   movl	%eax, %edx
[0,1]     DeeeeeeeeER    .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovdqu	(%rdi,%rax), %ymm7
[0,2]     DeE-------R    .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm12, %ymm10
[0,3]     DeE-------R    .    .    .    .    .    .    .    .    .    .    .    .    .   .   sarl	$5, %edx
[0,4]     D=eE------R    .    .    .    .    .    .    .    .    .    .    .    .    .   .   movslq	%edx, %rdx
[0,5]     D==eeeeeeeER   .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rsi,%rdx,4), %xmm5
[0,6]     D==eeeeeeeER   .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rcx,%rdx,4), %xmm14
[0,7]     .D=eE------R   .    .    .    .    .    .    .    .    .    .    .    .    .   .   leaq	(%rdx,%r12), %r13
[0,8]     .D==eE-----R   .    .    .    .    .    .    .    .    .    .    .    .    .   .   leaq	(,%rdx,4), %r15
[0,9]     .DE--------R   .    .    .    .    .    .    .    .    .    .    .    .    .   .   movl	%eax, %edx
[0,10]    .D===eE----R   .    .    .    .    .    .    .    .    .    .    .    .    .   .   leaq	(,%r13,4), %r14
[0,11]    .D==eeeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rsi,%r13,4), %xmm13
[0,12]    .D==eeeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rcx,%r13,4), %xmm11
[0,13]    . DeE-------R  .    .    .    .    .    .    .    .    .    .    .    .    .   .   sarl	%edx
[0,14]    . D===eeeeeeeER.    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rsi,%r14), %xmm3
[0,15]    . D=======eeeeeeeeeeeeER .    .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm14, -4(%rsp)
[0,16]    . D======eE------------R .    .    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm7, %ymm7, %ymm14
[0,17]    . D=eE-----------------R .    .    .    .    .    .    .    .    .    .    .   .   movslq	%edx, %rdx
[0,18]    .  D==eeeeeeeE---------R .    .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rcx,%r14), %xmm15
[0,19]    .  D=======eeeeeeeeeeeeER.    .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm11, -20(%rsp)
[0,20]    .  DeE------------------R.    .    .    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm12, %ymm11
[0,21]    .  D=eeeeeeeeE----------R.    .    .    .    .    .    .    .    .    .    .   .   vmovdqu	(%r9,%rdx), %ymm1
[0,22]    .  D=========eeeeeeeeeeeeER   .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm3, -16(%rsp)
[0,23]    .   DeeeeeeeE-------------R   .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rcx,%r15), %xmm2
[0,24]    .   DeE-------------------R   .    .    .    .    .    .    .    .    .    .   .   addq	%r9, %rdx
[0,25]    .   D========eeeeeeeeeeeeER   .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm13, -12(%rsp)
[0,26]    .   DeeeeeeeE-------------R   .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rsi,%r15), %xmm9
[0,27]    .   D=eeeeeeeeE-----------R   .    .    .    .    .    .    .    .    .    .   .   vmovdqu	(%rdx,%rbx), %ymm0
[0,28]    .    D=======eE-----------R   .    .    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm1, %ymm4
[0,29]    .    D=eeeeeeeeeE---------R   .    .    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm1, %ymm1
[0,30]    .    D====eeeeeeeeeE------R   .    .    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm1, %ymm1
[0,31]    .    .D=======eeeeeeeeeeeeER  .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm15, -24(%rsp)
[0,32]    .    .D======eeeeeeeeeE----R  .    .    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm4, %ymm4
[0,33]    .    .D=========eeeeeeeeeE-R  .    .    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm4, %ymm4
[0,34]    .    . D========eeeeeeeeeeeeER.    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm2, -8(%rsp)
[0,35]    .    . D===========eE--------R.    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm7, %ymm1, %ymm3
[0,36]    .    . D======eE-------------R.    .    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[0,37]    .    . DeeeeeeeeeE-----------R.    .    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm0, %ymm0
[0,38]    .    .  D===========eeeeeE---R.    .    .    .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm3, %ymm14, %ymm10
[0,39]    .    .  DeeeeeeeeE-----------R.    .    .    .    .    .    .    .    .    .   .   vmovdqu	32(%rdi,%rax), %ymm3
[0,40]    .    .  D==eeeeeeeeeE--------R.    .    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm0, %ymm0
[0,41]    .    .  D=====eeeeeeeeeE-----R.    .    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm2, %ymm2
[0,42]    .    .   D=======eeeeeeeeeE--R.    .    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm2, %ymm2
[0,43]    .    .   D=======eE----------R.    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm3, %ymm13
[0,44]    .    .   D==========eE-------R.    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm7, %ymm0, %ymm7
[0,45]    .    .   D===============eeeeER    .    .    .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm10, %ymm15
[0,46]    .    .   D===============eE---R    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm4, %ymm10
[0,47]    .    .    D===============eE--R    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm2, %ymm3
[0,48]    .    .    D===============eeeeeER  .    .    .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm10, %ymm13, %ymm11
[0,49]    .    .    DeeeeeeeeeeeE---------R  .    .    .    .    .    .    .    .    .   .   vmulss	-4(%rsp), %xmm5, %xmm10
[0,50]    .    .    D===========eeeE------R  .    .    .    .    .    .    .    .    .   .   vbroadcastss	%xmm10, %ymm10
[0,51]    .    .    .D===============eeeeeeeeeeeeER    .    .    .    .    .    .    .   .   vfmadd213ps	-64(%rsp), %ymm15, %ymm10
[0,52]    .    .    .DeeeeeeeeeeeE----------------R    .    .    .    .    .    .    .   .   vmulss	-8(%rsp), %xmm9, %xmm15
[0,53]    .    .    .D===================eeeeE----R    .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm11, %ymm11
[0,54]    .    .    .D===========eeeE-------------R    .    .    .    .    .    .    .   .   vbroadcastss	%xmm15, %ymm15
[0,55]    .    .    . D==========================eeeeER.    .    .    .    .    .    .   .   vfmadd132ps	%ymm15, %ymm10, %ymm11
[0,56]    .    .    . DeE-----------------------------R.    .    .    .    .    .    .   .   vmovdqa	%ymm12, %ymm10
[0,57]    .    .    . D========eeeeeE-----------------R.    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm7, %ymm14, %ymm10
[0,58]    .    .    . D==============================eeeeeeeeeeeeER   .    .    .    .   .   vmovaps	%ymm11, -64(%rsp)
[0,59]    .    .    . DeeeeeeeE-----------------------------------R   .    .    .    .   .   vmovss	-20(%rsp), %xmm11
[0,60]    .    .    . D========eeeeE------------------------------R   .    .    .    .   .   vmulss	%xmm11, %xmm5, %xmm5
[0,61]    .    .    .  D=============eeeeE------------------------R   .    .    .    .   .   vcvtdq2ps	%ymm10, %ymm7
[0,62]    .    .    .  DeE----------------------------------------R   .    .    .    .   .   vmovdqa	%ymm12, %ymm10
[0,63]    .    .    .  D==============eeeeeE----------------------R   .    .    .    .   .   {vex}	vpdpbusd	%ymm3, %ymm13, %ymm10
[0,64]    .    .    .  D===========eeeE---------------------------R   .    .    .    .   .   vbroadcastss	%xmm5, %ymm5
[0,65]    .    .    .  D==============eeeeeeeeeeeeE---------------R   .    .    .    .   .   vfmadd213ps	-96(%rsp), %ymm7, %ymm5
[0,66]    .    .    .  DeeeeeeeE----------------------------------R   .    .    .    .   .   vmovss	-24(%rsp), %xmm15
[0,67]    .    .    .   DeeeeeeeE---------------------------------R   .    .    .    .   .   vmovss	-12(%rsp), %xmm13
[0,68]    .    .    .   DeeeeeeeE---------------------------------R   .    .    .    .   .   vmovss	-4(%rsp), %xmm14
[0,69]    .    .    .   D=======eeeeE-----------------------------R   .    .    .    .   .   vmulss	%xmm15, %xmm9, %xmm9
[0,70]    .    .    .   D==================eeeeE------------------R   .    .    .    .   .   vcvtdq2ps	%ymm10, %ymm3
[0,71]    .    .    .   D======================eE-----------------R   .    .    .    .   .   vmovaps	%ymm3, %ymm7
[0,72]    .    .    .   D===========eeeE--------------------------R   .    .    .    .   .   vbroadcastss	%xmm9, %ymm9
[0,73]    .    .    .   D=========================eeeeE-----------R   .    .    .    .   .   vfmadd132ps	%ymm9, %ymm5, %ymm7
[0,74]    .    .    .    DeE--------------------------------------R   .    .    .    .   .   vmovdqa	%ymm12, %ymm5
[0,75]    .    .    .    D============================eeeeeeeeeeeeER  .    .    .    .   .   vmovaps	%ymm7, -96(%rsp)
[0,76]    .    .    .    DeeeeeeeeE--------------------------------R  .    .    .    .   .   vmovdqu	(%r10,%rax), %ymm7
[0,77]    .    .    .    D========eE-------------------------------R  .    .    .    .   .   vpsignb	%ymm7, %ymm7, %ymm9
[0,78]    .    .    .    D========eE-------------------------------R  .    .    .    .   .   vpsignb	%ymm7, %ymm1, %ymm1
[0,79]    .    .    .    D=============eE--------------------------R  .    .    .    .   .   vpsignb	%ymm7, %ymm0, %ymm0
[0,80]    .    .    .    .D============eeeeeE----------------------R  .    .    .    .   .   {vex}	vpdpbusd	%ymm1, %ymm9, %ymm5
[0,81]    .    .    .    .DeE--------------------------------------R  .    .    .    .   .   vmovdqa	%ymm12, %ymm7
[0,82]    .    .    .    .D=============eeeeeE---------------------R  .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm7
[0,83]    .    .    .    .D=================eeeeE------------------R  .    .    .    .   .   vcvtdq2ps	%ymm5, %ymm1
[0,84]    .    .    .    .DeeeeeeeeE-------------------------------R  .    .    .    .   .   vmovdqu	32(%r10,%rax), %ymm5
[0,85]    .    .    .    .DeE--------------------------------------R  .    .    .    .   .   addq	$64, %rax
[0,86]    .    .    .    .D==================eeeeE-----------------R  .    .    .    .   .   vcvtdq2ps	%ymm7, %ymm0
[0,87]    .    .    .    .D=====eeeeE------------------------------R  .    .    .    .   .   vmulss	%xmm11, %xmm13, %xmm7
[0,88]    .    .    .    . D============eE-------------------------R  .    .    .    .   .   vpsignb	%ymm5, %ymm4, %ymm3
[0,89]    .    .    .    . D=============eE------------------------R  .    .    .    .   .   vpsignb	%ymm5, %ymm5, %ymm10
[0,90]    .    .    .    . DeE-------------------------------------R  .    .    .    .   .   vmovdqa	%ymm12, %ymm4
[0,91]    .    .    .    . D==============eeeeeE-------------------R  .    .    .    .   .   {vex}	vpdpbusd	%ymm3, %ymm10, %ymm4
[0,92]    .    .    .    . D=====eeeeE-----------------------------R  .    .    .    .   .   vmulss	%xmm13, %xmm14, %xmm3
[0,93]    .    .    .    . D==============eE-----------------------R  .    .    .    .   .   vpsignb	%ymm5, %ymm2, %ymm2
[0,94]    .    .    .    . D=========eeeE--------------------------R  .    .    .    .   .   vbroadcastss	%xmm7, %ymm7
[0,95]    .    .    .    .  D====================eeeeE-------------R  .    .    .    .   .   vfmadd132ps	%ymm0, %ymm6, %ymm7
[0,96]    .    .    .    .  D=========eeeE-------------------------R  .    .    .    .   .   vbroadcastss	%xmm3, %ymm3
[0,97]    .    .    .    .  D===================eeeeE--------------R  .    .    .    .   .   vfmadd132ps	%ymm3, %ymm8, %ymm1
[0,98]    .    .    .    .  DeeeeeeeE------------------------------R  .    .    .    .   .   vmovss	-16(%rsp), %xmm3
[0,99]    .    .    .    .  DeeeeeeeE------------------------------R  .    .    .    .   .   vmovss	-8(%rsp), %xmm8
[0,100]   .    .    .    .  D==================eeeeE---------------R  .    .    .    .   .   vcvtdq2ps	%ymm4, %ymm4
[0,101]   .    .    .    .   D=============eeeeE-------------------R  .    .    .    .   .   vmulss	%xmm3, %xmm8, %xmm8
[0,102]   .    .    .    .   D==============eeeeE------------------R  .    .    .    .   .   vmulss	%xmm15, %xmm3, %xmm6
[0,103]   .    .    .    .   D=================eeeE----------------R  .    .    .    .   .   vbroadcastss	%xmm8, %ymm8
[0,104]   .    .    .    .   D======================eeeeE----------R  .    .    .    .   .   vfmadd132ps	%ymm4, %ymm1, %ymm8
[0,105]   .    .    .    .   DeE-----------------------------------R  .    .    .    .   .   vmovdqa	%ymm12, %ymm1
[0,106]   .    .    .    .   D==================eeeE---------------R  .    .    .    .   .   vbroadcastss	%xmm6, %ymm6
[0,107]   .    .    .    .   D===============eeeeeE----------------R  .    .    .    .   .   {vex}	vpdpbusd	%ymm2, %ymm10, %ymm1
[0,108]   .    .    .    .    D===================eeeeE------------R  .    .    .    .   .   vcvtdq2ps	%ymm1, %ymm1
[0,109]   .    .    .    .    D=======================eeeeE--------R  .    .    .    .   .   vfmadd132ps	%ymm1, %ymm7, %ymm6
[0,110]   .    .    .    .    DeE----------------------------------R  .    .    .    .   .   cmpl	%eax, %r11d
[0,111]   .    .    .    .    D=eE---------------------------------R  .    .    .    .   .   jg	.L3
[1,0]     .    .    .    .    DE-----------------------------------R  .    .    .    .   .   movl	%eax, %edx
[1,1]     .    .    .    .    DeeeeeeeeE---------------------------R  .    .    .    .   .   vmovdqu	(%rdi,%rax), %ymm7
[1,2]     .    .    .    .    .DeE---------------------------------R  .    .    .    .   .   vmovdqa	%ymm12, %ymm10
[1,3]     .    .    .    .    .D=eE--------------------------------R  .    .    .    .   .   sarl	$5, %edx
[1,4]     .    .    .    .    .D==eE-------------------------------R  .    .    .    .   .   movslq	%edx, %rdx
[1,5]     .    .    .    .    .D===eeeeeeeE------------------------R  .    .    .    .   .   vmovss	(%rsi,%rdx,4), %xmm5
[1,6]     .    .    .    .    .D===eeeeeeeE------------------------R  .    .    .    .   .   vmovss	(%rcx,%rdx,4), %xmm14
[1,7]     .    .    .    .    .D==============eE-------------------R  .    .    .    .   .   leaq	(%rdx,%r12), %r13
[1,8]     .    .    .    .    .D===================eE--------------R  .    .    .    .   .   leaq	(,%rdx,4), %r15
[1,9]     .    .    .    .    . DE---------------------------------R  .    .    .    .   .   movl	%eax, %edx
[1,10]    .    .    .    .    . D====================eE------------R  .    .    .    .   .   leaq	(,%r13,4), %r14
[1,11]    .    .    .    .    . D==============eeeeeeeE------------R  .    .    .    .   .   vmovss	(%rsi,%r13,4), %xmm13
[1,12]    .    .    .    .    . D==============eeeeeeeE------------R  .    .    .    .   .   vmovss	(%rcx,%r13,4), %xmm11
[1,13]    .    .    .    .    . DeE--------------------------------R  .    .    .    .   .   sarl	%edx
[1,14]    .    .    .    .    . D=====================eeeeeeeE-----R  .    .    .    .   .   vmovss	4(%rsi,%r14), %xmm3
[1,15]    .    .    .    .    .  D====================eeeeeeeeeeeeER  .    .    .    .   .   vmovss	%xmm14, -4(%rsp)
[1,16]    .    .    .    .    .  D============eE-------------------R  .    .    .    .   .   vpsignb	%ymm7, %ymm7, %ymm14
[1,17]    .    .    .    .    .  DeE-------------------------------R  .    .    .    .   .   movslq	%edx, %rdx
[1,18]    .    .    .    .    .  D====================eeeeeeeE-----R  .    .    .    .   .   vmovss	4(%rcx,%r14), %xmm15
[1,19]    .    .    .    .    .   D====================eeeeeeeeeeeeER .    .    .    .   .   vmovss	%xmm11, -20(%rsp)
[1,20]    .    .    .    .    .   DeE-------------------------------R .    .    .    .   .   vmovdqa	%ymm12, %ymm11
[1,21]    .    .    .    .    .   DeeeeeeeeE------------------------R .    .    .    .   .   vmovdqu	(%r9,%rdx), %ymm1
[1,22]    .    .    .    .    .   D==========================eeeeeeeeeeeeER.    .    .   .   vmovss	%xmm3, -16(%rsp)
[1,23]    .    .    .    .    .   D=================eeeeeeeE--------------R.    .    .   .   vmovss	4(%rcx,%r15), %xmm2
[1,24]    .    .    .    .    .    DeE------------------------------------R.    .    .   .   addq	%r9, %rdx
[1,25]    .    .    .    .    .    D=========================eeeeeeeeeeeeER.    .    .   .   vmovss	%xmm13, -12(%rsp)
[1,26]    .    .    .    .    .    D================eeeeeeeE--------------R.    .    .   .   vmovss	4(%rsi,%r15), %xmm9
[1,27]    .    .    .    .    .    D=eeeeeeeeE----------------------------R.    .    .   .   vmovdqu	(%rdx,%rbx), %ymm0
[1,28]    .    .    .    .    .    D===========eE-------------------------R.    .    .   .   vpsrlw	$4, %ymm1, %ymm4
[1,29]    .    .    .    .    .    .D==eeeeeeeeeE-------------------------R.    .    .   .   vpand	.LC0(%rip), %ymm1, %ymm1
[1,30]    .    .    .    .    .    .D=====eeeeeeeeeE----------------------R.    .    .   .   vpaddb	.LC1(%rip), %ymm1, %ymm1
[1,31]    .    .    .    .    .    .D=========================eeeeeeeeeeeeER    .    .   .   vmovss	%xmm15, -24(%rsp)
[1,32]    .    .    .    .    .    . D==========eeeeeeeeeE-----------------R    .    .   .   vpand	.LC0(%rip), %ymm4, %ymm4
[1,33]    .    .    .    .    .    . D=============eeeeeeeeeE--------------R    .    .   .   vpaddb	.LC1(%rip), %ymm4, %ymm4
[1,34]    .    .    .    .    .    . D========================eeeeeeeeeeeeER    .    .   .   vmovss	%xmm2, -8(%rsp)
[1,35]    .    .    .    .    .    .  D============eE----------------------R    .    .   .   vpsignb	%ymm7, %ymm1, %ymm3
[1,36]    .    .    .    .    .    .  D=============eE---------------------R    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[1,37]    .    .    .    .    .    .  D=eeeeeeeeeE-------------------------R    .    .   .   vpand	.LC0(%rip), %ymm0, %ymm0
[1,38]    .    .    .    .    .    .  D==============eeeeeE----------------R    .    .   .   {vex}	vpdpbusd	%ymm3, %ymm14, %ymm10
[1,39]    .    .    .    .    .    .  DeeeeeeeeE---------------------------R    .    .   .   vmovdqu	32(%rdi,%rax), %ymm3
[1,40]    .    .    .    .    .    .   D===eeeeeeeeeE----------------------R    .    .   .   vpaddb	.LC1(%rip), %ymm0, %ymm0
[1,41]    .    .    .    .    .    .   D============eeeeeeeeeE-------------R    .    .   .   vpand	.LC0(%rip), %ymm2, %ymm2
[1,42]    .    .    .    .    .    .   D===============eeeeeeeeeE----------R    .    .   .   vpaddb	.LC1(%rip), %ymm2, %ymm2
[1,43]    .    .    .    .    .    .    D=============eE-------------------R    .    .   .   vpsignb	%ymm3, %ymm3, %ymm13
[1,44]    .    .    .    .    .    .    D==============eE------------------R    .    .   .   vpsignb	%ymm7, %ymm0, %ymm7
[1,45]    .    .    .    .    .    .    D=================eeeeE------------R    .    .   .   vcvtdq2ps	%ymm10, %ymm15
[1,46]    .    .    .    .    .    .    D===================eE-------------R    .    .   .   vpsignb	%ymm3, %ymm4, %ymm10
[1,47]    .    .    .    .    .    .    D=======================eE---------R    .    .   .   vpsignb	%ymm3, %ymm2, %ymm3
[1,48]    .    .    .    .    .    .    D====================eeeeeE--------R    .    .   .   {vex}	vpdpbusd	%ymm10, %ymm13, %ymm11
[1,49]    .    .    .    .    .    .    .D=============eeeeeeeeeeeE--------R    .    .   .   vmulss	-4(%rsp), %xmm5, %xmm10
[1,50]    .    .    .    .    .    .    .D========================eeeE-----R    .    .   .   vbroadcastss	%xmm10, %ymm10
[1,51]    .    .    .    .    .    .    .D========================eeeeeeeeeeeeER.    .   .   vfmadd213ps	-64(%rsp), %ymm15, %ymm10
[1,52]    .    .    .    .    .    .    . D=============eeeeeeeeeeeE-----------R.    .   .   vmulss	-8(%rsp), %xmm9, %xmm15
[1,53]    .    .    .    .    .    .    . D=======================eeeeE--------R.    .   .   vcvtdq2ps	%ymm11, %ymm11
[1,54]    .    .    .    .    .    .    . D========================eeeE--------R.    .   .   vbroadcastss	%xmm15, %ymm15
[1,55]    .    .    .    .    .    .    . D===================================eeeeER .   .   vfmadd132ps	%ymm15, %ymm10, %ymm11
[1,56]    .    .    .    .    .    .    . DeE--------------------------------------R .   .   vmovdqa	%ymm12, %ymm10
[1,57]    .    .    .    .    .    .    . D=============eeeeeE---------------------R .   .   {vex}	vpdpbusd	%ymm7, %ymm14, %ymm10
Truncated display due to cycle limit


Average Wait times (based on the timeline view):
[0]: Executions
[1]: Average time spent waiting in a scheduler's queue
[2]: Average time spent waiting in a scheduler's queue while ready
[3]: Average time elapsed from WB until retire stage

      [0]    [1]    [2]    [3]
0.     10    1.0    1.0    48.0      movl	%eax, %edx
1.     10    1.1    1.1    40.5      vmovdqu	(%rdi,%rax), %ymm7
2.     10    1.0    1.0    47.5      vmovdqa	%ymm12, %ymm10
3.     10    2.2    1.5    46.3      sarl	$5, %edx
4.     10    3.2    0.0    45.3      movslq	%edx, %rdx
5.     10    3.9    0.0    38.4      vmovss	(%rsi,%rdx,4), %xmm5
6.     10    3.9    0.0    38.4      vmovss	(%rcx,%rdx,4), %xmm14
7.     10    29.4   25.9   18.5      leaq	(%rdx,%r12), %r13
8.     10    30.6   27.3   17.1      leaq	(,%rdx,4), %r15
9.     10    1.0    1.0    47.6      movl	%eax, %edx
10.    10    33.2   3.1    14.4      leaq	(,%r13,4), %r14
11.    10    29.8   0.0    11.6      vmovss	(%rsi,%r13,4), %xmm13
12.    10    29.8   0.0    11.6      vmovss	(%rcx,%r13,4), %xmm11
13.    10    1.5    1.2    45.5      sarl	%edx
14.    10    33.3   0.0    7.7       vmovss	4(%rsi,%r14), %xmm3
15.    10    37.6   0.8    0.0       vmovss	%xmm14, -4(%rsp)
16.    10    26.8   20.1   21.5      vpsignb	%ymm7, %ymm7, %ymm14
17.    10    1.9    0.0    46.4      movslq	%edx, %rdx
18.    10    32.5   0.0    9.4       vmovss	4(%rcx,%r14), %xmm15
19.    10    36.8   0.1    0.0       vmovss	%xmm11, -20(%rsp)
20.    10    1.0    1.0    46.8      vmovdqa	%ymm12, %ymm11
21.    10    2.2    0.0    38.6      vmovdqu	(%r9,%rdx), %ymm1
22.    10    38.9   0.0    0.0       vmovss	%xmm3, -16(%rsp)
23.    10    28.8   0.0    14.7      vmovss	4(%rcx,%r15), %xmm2
24.    10    1.3    0.1    47.9      addq	%r9, %rdx
25.    10    38.2   0.0    0.0       vmovss	%xmm13, -12(%rsp)
26.    10    28.2   0.0    14.7      vmovss	4(%rsi,%r15), %xmm9
27.    10    2.0    0.0    39.9      vmovdqu	(%rdx,%rbx), %ymm0
28.    10    25.7   17.2   22.8      vpsrlw	$4, %ymm1, %ymm4
29.    10    3.0    0.8    37.2      vpand	.LC0(%rip), %ymm1, %ymm1
30.    10    7.1    1.4    32.8      vpaddb	.LC1(%rip), %ymm1, %ymm1
31.    10    37.5   1.0    0.0       vmovss	%xmm15, -24(%rsp)
32.    10    25.3   0.9    14.9      vpand	.LC0(%rip), %ymm4, %ymm4
33.    10    28.0   0.0    11.9      vpaddb	.LC1(%rip), %ymm4, %ymm4
34.    10    36.7   0.0    0.0       vmovss	%xmm2, -8(%rsp)
35.    10    26.0   11.6   21.4      vpsignb	%ymm7, %ymm1, %ymm3
36.    10    27.2   19.9   20.2      vpsrlw	$4, %ymm0, %ymm2
37.    10    2.1    1.1    37.0      vpand	.LC0(%rip), %ymm0, %ymm0
38.    10    28.0   1.7    14.7      {vex}	vpdpbusd	%ymm3, %ymm14, %ymm10
39.    10    1.0    1.0    38.7      vmovdqu	32(%rdi,%rax), %ymm3
40.    10    6.3    1.9    32.1      vpaddb	.LC1(%rip), %ymm0, %ymm0
41.    10    26.7   0.8    11.4      vpand	.LC0(%rip), %ymm2, %ymm2
42.    10    29.3   0.0    8.4       vpaddb	.LC1(%rip), %ymm2, %ymm2
43.    10    27.3   19.6   18.1      vpsignb	%ymm3, %ymm3, %ymm13
44.    10    28.5   14.2   16.9      vpsignb	%ymm7, %ymm0, %ymm7
45.    10    31.4   0.0    10.8      vcvtdq2ps	%ymm10, %ymm15
46.    10    34.0   0.0    11.2      vpsignb	%ymm3, %ymm4, %ymm10
47.    10    37.3   0.0    7.5       vpsignb	%ymm3, %ymm2, %ymm3
48.    10    34.6   0.0    6.4       {vex}	vpdpbusd	%ymm10, %ymm13, %ymm11
49.    10    27.4   27.4   7.3       vmulss	-4(%rsp), %xmm5, %xmm10
50.    10    38.1   0.0    4.3       vbroadcastss	%xmm10, %ymm10
51.    10    38.2   0.2    0.0       vfmadd213ps	-64(%rsp), %ymm15, %ymm10
52.    10    27.4   4.4    11.5      vmulss	-8(%rsp), %xmm9, %xmm15
53.    10    38.3   0.0    7.6       vcvtdq2ps	%ymm11, %ymm11
54.    10    38.1   0.0    8.5       vbroadcastss	%xmm15, %ymm15
55.    10    49.2   0.0    0.0       vfmadd132ps	%ymm15, %ymm10, %ymm11
56.    10    1.0    1.0    51.2      vmovdqa	%ymm12, %ymm10
57.    10    27.6   0.8    20.6      {vex}	vpdpbusd	%ymm7, %ymm14, %ymm10
58.    10    52.9   0.0    0.0       vmovaps	%ymm11, -64(%rsp)
59.    10    1.0    1.0    56.9      vmovss	-20(%rsp), %xmm11
60.    10    27.9   20.2   32.7      vmulss	%xmm11, %xmm5, %xmm5
61.    10    31.7   0.1    28.5      vcvtdq2ps	%ymm10, %ymm7
62.    10    1.0    1.0    62.2      vmovdqa	%ymm12, %ymm10
63.    10    35.4   0.1    23.8      {vex}	vpdpbusd	%ymm3, %ymm13, %ymm10
64.    10    31.2   0.0    29.7      vbroadcastss	%xmm5, %ymm5
65.    10    32.4   1.0    19.5      vfmadd213ps	-96(%rsp), %ymm7, %ymm5
66.    10    1.0    1.0    55.6      vmovss	-24(%rsp), %xmm15
67.    10    1.0    1.0    55.2      vmovss	-12(%rsp), %xmm13
68.    10    1.5    1.5    54.7      vmovss	-4(%rsp), %xmm14
69.    10    27.3   0.9    31.6      vmulss	%xmm15, %xmm9, %xmm9
70.    10    39.1   0.0    19.8      vcvtdq2ps	%ymm10, %ymm3
71.    10    43.1   0.0    18.8      vmovaps	%ymm3, %ymm7
72.    10    31.3   0.0    28.6      vbroadcastss	%xmm9, %ymm9
73.    10    44.0   0.0    14.6      vfmadd132ps	%ymm9, %ymm5, %ymm7
74.    10    1.0    1.0    60.2      vmovdqa	%ymm12, %ymm5
75.    10    50.3   0.0    0.0       vmovaps	%ymm7, -96(%rsp)
76.    10    1.2    1.2    52.8      vmovdqu	(%r10,%rax), %ymm7
77.    10    26.3   17.1   34.7      vpsignb	%ymm7, %ymm7, %ymm9
78.    10    27.2   18.0   33.8      vpsignb	%ymm7, %ymm1, %ymm1
79.    10    27.5   18.3   33.2      vpsignb	%ymm7, %ymm0, %ymm0
80.    10    28.0   0.5    28.3      {vex}	vpdpbusd	%ymm1, %ymm9, %ymm5
81.    10    1.0    1.0    59.3      vmovdqa	%ymm12, %ymm7
82.    10    29.9   1.8    26.4      {vex}	vpdpbusd	%ymm0, %ymm9, %ymm7
83.    10    32.7   0.0    24.3      vcvtdq2ps	%ymm5, %ymm1
84.    10    1.1    1.1    51.9      vmovdqu	32(%r10,%rax), %ymm5
85.    10    1.0    1.0    59.0      addq	$64, %rax
86.    10    34.6   0.0    22.4      vcvtdq2ps	%ymm7, %ymm0
87.    10    29.1   24.0   27.3      vmulss	%xmm11, %xmm13, %xmm7
88.    10    29.8   3.2    29.5      vpsignb	%ymm5, %ymm4, %ymm3
89.    10    30.8   22.4   28.5      vpsignb	%ymm5, %ymm5, %ymm10
90.    10    1.0    1.0    58.0      vmovdqa	%ymm12, %ymm4
91.    10    33.0   1.8    21.7      {vex}	vpdpbusd	%ymm3, %ymm10, %ymm4
92.    10    33.0   28.1   22.7      vmulss	%xmm13, %xmm14, %xmm3
93.    10    33.9   4.2    24.8      vpsignb	%ymm5, %ymm2, %ymm2
94.    10    32.8   1.0    23.3      vbroadcastss	%xmm7, %ymm7
95.    10    36.6   0.0    18.4      vfmadd132ps	%ymm0, %ymm6, %ymm7
96.    10    36.4   0.1    19.6      vbroadcastss	%xmm3, %ymm3
97.    10    39.5   0.0    14.9      vfmadd132ps	%ymm3, %ymm8, %ymm1
98.    10    1.0    1.0    50.4      vmovss	-16(%rsp), %xmm3
99.    10    1.0    1.0    50.4      vmovss	-8(%rsp), %xmm8
100.   10    36.4   0.0    17.7      vcvtdq2ps	%ymm4, %ymm4
101.   10    32.9   25.6   20.8      vmulss	%xmm3, %xmm8, %xmm8
102.   10    33.0   25.7   20.7      vmulss	%xmm15, %xmm3, %xmm6
103.   10    36.6   0.0    17.8      vbroadcastss	%xmm8, %ymm8
104.   10    42.2   0.0    10.9      vfmadd132ps	%ymm4, %ymm1, %ymm8
105.   10    1.0    1.0    55.1      vmovdqa	%ymm12, %ymm1
106.   10    37.3   0.9    16.8      vbroadcastss	%xmm6, %ymm6
107.   10    33.1   1.1    18.7      {vex}	vpdpbusd	%ymm2, %ymm10, %ymm1
108.   10    38.6   0.9    13.8      vcvtdq2ps	%ymm1, %ymm1
109.   10    42.6   0.0    9.8       vfmadd132ps	%ymm1, %ymm7, %ymm6
110.   10    1.0    1.0    53.7      cmpl	%eax, %r11d
111.   10    2.0    0.0    52.7      jg	.L3
       10    23.3   4.0    26.4      <total>
