Iterations:        100
Instructions:      39200
Total Cycles:      9179
Total uOps:        48600

Dispatch Width:    6
uOps Per Cycle:    5.29
IPC:               4.27
Block RThroughput: 86.0


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%edi, %eax
 1      5     0.33    *                   movq	88(%rsp), %r15
 0      1     0.00                        addq	$64, %rdx
 1      1     0.50                        sarl	$5, %eax
 1      1     0.50                        cltq
 1      1     0.20                        addq	%rax, %r15
 1      7     0.33    *                   vmovss	4(%rsi,%rax,4), %xmm4
 1      7     0.33    *                   vmovss	(%rsi,%rax,4), %xmm15
 1      7     0.33    *                   vmovss	(%rsi,%r15,4), %xmm13
 1      7     0.33    *                   vmovss	4(%rsi,%r15,4), %xmm5
 1      5     0.33    *                   movq	80(%rsp), %r15
 2      12    0.50           *            vmovss	%xmm5, 704(%rsp)
 2      12    0.50           *            vmovss	%xmm13, 708(%rsp)
 1      1     0.20                        addq	%rax, %r15
 1      7     0.33    *                   vmovss	(%rsi,%r15,4), %xmm6
 1      7     0.33    *                   vmovss	4(%rsi,%r15,4), %xmm14
 1      5     0.33    *                   movq	72(%rsp), %r15
 2      12    0.50           *            vmovss	%xmm6, 700(%rsp)
 2      12    0.50           *            vmovss	%xmm14, 696(%rsp)
 1      1     0.20                        addq	%rax, %r15
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      7     0.33    *                   vmovss	(%rsi,%r15,4), %xmm7
 1      7     0.33    *                   vmovss	4(%rsi,%r15,4), %xmm11
 1      1     1.00                        leaq	(%r11,%rax), %r15
 1      7     0.33    *                   vmovss	(%rcx,%r15,4), %xmm10
 1      7     0.33    *                   vmovss	4(%rcx,%r15,4), %xmm12
 1      1     1.00                        leaq	(%r9,%rax), %r15
 1      7     0.33    *                   vmovss	(%rcx,%r15,4), %xmm2
 1      7     0.33    *                   vmovss	4(%rcx,%r15,4), %xmm1
 1      1     1.00                        leaq	(%r10,%rax), %r15
 1      1     0.20                        addq	%r8, %rax
 1      7     0.33    *                   vmovss	(%rcx,%rax,4), %xmm9
 1      7     0.33    *                   vmovss	4(%rcx,%rax,4), %xmm5
 1      0     0.20                        movl	%edi, %eax
 2      12    0.50           *            vmovss	%xmm7, 676(%rsp)
 1      7     0.33    *                   vmovss	(%rcx,%r15,4), %xmm3
 1      1     0.50                        sarl	%eax
 1      1     0.20                        addl	$64, %edi
 1      7     0.33    *                   vmovss	4(%rcx,%r15,4), %xmm0
 2      12    0.50           *            vmovss	%xmm11, 680(%rsp)
 1      1     0.50                        cltq
 2      12    0.50           *            vmovss	%xmm2, 732(%rsp)
 2      12    0.50           *            vmovss	%xmm1, 728(%rsp)
 2      12    0.50           *            vmovss	%xmm3, 724(%rsp)
 1      8     0.33    *                   vmovdqu	(%r14,%rax), %ymm3
 2      12    0.50           *            vmovss	%xmm0, 720(%rsp)
 2      12    0.50           *            vmovss	%xmm9, 716(%rsp)
 1      1     0.50                        vpsrlw	$4, %ymm3, %ymm7
 2      12    0.50           *            vmovss	%xmm5, 712(%rsp)
 2      6     0.33    *                   addq	120(%rsp), %rax
 1      5     0.33    *                   movq	112(%rsp), %r15
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm3, %ymm3
 2      12    0.50           *            vmovss	%xmm10, 692(%rsp)
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm3, %ymm3
 1      8     0.33    *                   vmovdqu	(%rax,%r15), %ymm2
 1      5     0.33    *                   movq	104(%rsp), %r15
 2      12    0.50           *            vmovss	%xmm12, 688(%rsp)
 2      12    0.50           *            vmovss	%xmm15, 684(%rsp)
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm7, %ymm7
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm7, %ymm7
 1      8     0.33    *                   vmovdqu	(%rax,%r15), %ymm1
 1      5     0.33    *                   movq	96(%rsp), %r15
 1      1     0.50                        vpsrlw	$4, %ymm2, %ymm6
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm2, %ymm2
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm2, %ymm2
 1      8     0.33    *                   vmovdqu	(%rax,%r15), %ymm0
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm6, %ymm6
 1      1     0.50                        vpsrlw	$4, %ymm1, %ymm5
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm6, %ymm6
 1      1     0.50                        vpsrlw	$4, %ymm0, %ymm9
 2      12    0.50           *            vmovdqa	%ymm9, 128(%rsp)
 1      4     0.50                        vmulss	%xmm15, %xmm10, %xmm9
 1      8     0.33    *                   vmovdqu	-64(%rdx), %ymm10
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm13
 1      3     1.00                        vmovd	%xmm9, %r15d
 1      4     0.50                        vmulss	%xmm12, %xmm4, %xmm9
 1      1     0.50                        vpsignb	%ymm10, %ymm3, %ymm12
 1      3     1.00                        vmovd	%xmm9, %eax
 0      1     0.00                        vmovdqa	%ymm8, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm13, %ymm9
 1      4     0.50                        vcvtdq2ps	%ymm9, %ymm15
 1      8     0.33    *                   vmovdqu	-32(%rdx), %ymm9
 1      1     0.50                        vpsignb	%ymm9, %ymm9, %ymm12
 1      1     0.50                        vpsignb	%ymm9, %ymm7, %ymm11
 1      5     0.50                        {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 1      3     1.00                        vmovd	%r15d, %xmm11
 1      3     1.00                        vbroadcastss	%xmm11, %ymm11
 2      12    0.50    *                   vfmadd213ps	640(%rsp), %ymm15, %ymm11
 1      3     1.00                        vmovd	%eax, %xmm15
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm11, %ymm14
 1      7     0.33    *                   vmovss	684(%rsp), %xmm15
 2      11    0.50    *                   vmulss	728(%rsp), %xmm4, %xmm11
 2      12    0.50           *            vmovaps	%ymm14, 640(%rsp)
 2      12    0.50           *            vmovaps	%ymm14, 736(%rsp)
 1      3     1.00                        vmovd	%xmm11, %eax
 1      1     0.50                        vpsignb	%ymm9, %ymm6, %ymm11
 2      11    0.50    *                   vmulss	732(%rsp), %xmm15, %xmm14
 1      1     0.50                        vpsignb	%ymm10, %ymm2, %ymm15
 1      3     1.00                        vmovd	%xmm14, %r15d
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 1      3     1.00                        vmovd	%r15d, %xmm11
 1      3     1.00                        vbroadcastss	%xmm11, %ymm11
 2      12    0.50    *                   vfmadd213ps	608(%rsp), %ymm15, %ymm11
 1      3     1.00                        vmovd	%eax, %xmm15
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm11, %ymm14
 1      7     0.33    *                   vmovss	684(%rsp), %xmm15
 2      12    0.50           *            vmovaps	%ymm14, 608(%rsp)
 2      12    0.50           *            vmovaps	%ymm14, 768(%rsp)
 2      11    0.50    *                   vmulss	724(%rsp), %xmm15, %xmm14
 1      3     1.00                        vmovd	%xmm14, %r15d
 2      11    0.50    *                   vmulss	720(%rsp), %xmm4, %xmm14
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm1, %ymm1
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm1, %ymm1
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm5, %ymm5
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm5, %ymm5
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm0, %ymm0
 1      1     0.50                        vpsignb	%ymm10, %ymm1, %ymm15
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm0, %ymm0
 1      1     0.50                        vpsignb	%ymm9, %ymm5, %ymm11
 1      3     1.00                        vmovd	%xmm14, %eax
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      1     0.50                        vpsignb	%ymm10, %ymm0, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 1      3     1.00                        vmovd	%r15d, %xmm11
 1      3     1.00                        vbroadcastss	%xmm11, %ymm11
 2      12    0.50    *                   vfmadd213ps	576(%rsp), %ymm15, %ymm11
 1      3     1.00                        vmovd	%eax, %xmm15
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm11, %ymm14
 1      7     0.33    *                   vmovss	684(%rsp), %xmm15
 2      11    0.50    *                   vmulss	716(%rsp), %xmm15, %xmm15
 2      12    0.50           *            vmovaps	%ymm14, 576(%rsp)
 2      12    0.50           *            vmovaps	%ymm14, 800(%rsp)
 2      11    0.50    *                   vmulss	712(%rsp), %xmm4, %xmm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm4
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm13, %ymm4
 0      1     0.00                        vmovdqa	%ymm8, %ymm10
 1      4     0.50                        vcvtdq2ps	%ymm4, %ymm11
 1      8     0.33    *                   vmovdqa	128(%rsp), %ymm4
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm4, %ymm4
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm4, %ymm4
 1      1     0.50                        vpsignb	%ymm9, %ymm4, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm12, %ymm10
 1      3     1.00                        vbroadcastss	%xmm15, %ymm9
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15
 1      7     0.33    *                   vmovss	688(%rsp), %xmm12
 2      12    0.50    *                   vfmadd213ps	544(%rsp), %ymm11, %ymm9
 1      3     1.00                        vbroadcastss	%xmm14, %ymm11
 2      11    0.50    *                   vmulss	704(%rsp), %xmm12, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm10, %ymm10
 1      4     0.50                        vfmadd132ps	%ymm11, %ymm9, %ymm10
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 2      12    0.50           *            vmovaps	%ymm10, 544(%rsp)
 2      12    0.50           *            vmovaps	%ymm10, 832(%rsp)
 1      7     0.33    *                   vmovss	692(%rsp), %xmm10
 2      11    0.50    *                   vmulss	708(%rsp), %xmm10, %xmm9
 1      8     0.33    *                   vmovdqu	-64(%rdx,%rbx), %ymm10
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm11
 1      1     0.50                        vpsignb	%ymm10, %ymm3, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm11, %ymm12
 1      8     0.33    *                   vmovdqu	-32(%rdx,%rbx), %ymm10
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm10
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 2      12    0.50    *                   vfmadd213ps	512(%rsp), %ymm14, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      7     0.33    *                   vmovss	704(%rsp), %xmm15
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm12
 1      7     0.33    *                   vmovss	708(%rsp), %xmm13
 2      11    0.50    *                   vmulss	732(%rsp), %xmm13, %xmm9
 2      12    0.50           *            vmovaps	%ymm12, 512(%rsp)
 2      12    0.50           *            vmovaps	%ymm12, 864(%rsp)
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 2      11    0.50    *                   vmulss	728(%rsp), %xmm15, %xmm13
 2      9     0.50    *                   vpsignb	-64(%rdx,%rbx), %ymm2, %ymm12
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm6, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      7     0.33    *                   vmovss	704(%rsp), %xmm15
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 2      12    0.50    *                   vfmadd213ps	480(%rsp), %ymm14, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm12
 1      7     0.33    *                   vmovss	708(%rsp), %xmm13
 2      11    0.50    *                   vmulss	724(%rsp), %xmm13, %xmm9
 2      11    0.50    *                   vmulss	720(%rsp), %xmm15, %xmm13
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm5, %ymm15
 2      12    0.50           *            vmovaps	%ymm12, 480(%rsp)
 2      12    0.50           *            vmovaps	%ymm12, 896(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%rbx), %ymm1, %ymm12
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      7     0.33    *                   vmovss	704(%rsp), %xmm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 2      12    0.50    *                   vfmadd213ps	448(%rsp), %ymm14, %ymm9
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm12
 2      9     0.50    *                   vpsignb	-64(%rdx,%rbx), %ymm0, %ymm9
 1      7     0.33    *                   vmovss	708(%rsp), %xmm13
 2      11    0.50    *                   vmulss	716(%rsp), %xmm13, %xmm14
 2      11    0.50    *                   vmulss	712(%rsp), %xmm15, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm4, %ymm9
 2      12    0.50           *            vmovaps	%ymm12, 448(%rsp)
 2      12    0.50           *            vmovaps	%ymm12, 928(%rsp)
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm12
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm10, %ymm15
 1      3     1.00                        vbroadcastss	%xmm14, %ymm9
 1      3     1.00                        vbroadcastss	%xmm13, %ymm10
 2      12    0.50    *                   vfmadd213ps	416(%rsp), %ymm12, %ymm9
 1      7     0.33    *                   vmovss	688(%rsp), %xmm12
 2      11    0.50    *                   vmulss	696(%rsp), %xmm12, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm11
 0      1     0.00                        vmovaps	%ymm11, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm10, %ymm9, %ymm15
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      7     0.33    *                   vmovss	692(%rsp), %xmm10
 2      11    0.50    *                   vmulss	700(%rsp), %xmm10, %xmm9
 2      12    0.50           *            vmovaps	%ymm15, 416(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 960(%rsp)
 1      8     0.33    *                   vmovdqu	-64(%rdx,%r13), %ymm15
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      1     0.50                        vpsignb	%ymm15, %ymm15, %ymm11
 1      1     0.50                        vpsignb	%ymm15, %ymm3, %ymm10
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm11, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm14
 1      8     0.33    *                   vmovdqu	-32(%rdx,%r13), %ymm15
 2      12    0.50    *                   vfmadd213ps	384(%rsp), %ymm14, %ymm9
 1      1     0.50                        vpsignb	%ymm15, %ymm15, %ymm10
 1      1     0.50                        vpsignb	%ymm15, %ymm7, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 0      1     0.00                        vmovaps	%ymm12, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm15
 2      12    0.50           *            vmovaps	%ymm15, 384(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 992(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%r13), %ymm2, %ymm12
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      7     0.33    *                   vmovss	700(%rsp), %xmm13
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      7     0.33    *                   vmovss	696(%rsp), %xmm14
 2      11    0.50    *                   vmulss	732(%rsp), %xmm13, %xmm9
 2      11    0.50    *                   vmulss	728(%rsp), %xmm14, %xmm13
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm14
 2      9     0.50    *                   vpsignb	-32(%rdx,%r13), %ymm6, %ymm15
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 2      12    0.50    *                   vfmadd213ps	352(%rsp), %ymm14, %ymm9
 1      7     0.33    *                   vmovss	696(%rsp), %xmm14
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 0      1     0.00                        vmovaps	%ymm12, %ymm15
 2      9     0.50    *                   vpsignb	-64(%rdx,%r13), %ymm1, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm15
 1      7     0.33    *                   vmovss	700(%rsp), %xmm13
 2      11    0.50    *                   vmulss	724(%rsp), %xmm13, %xmm9
 2      11    0.50    *                   vmulss	720(%rsp), %xmm14, %xmm13
 2      12    0.50           *            vmovaps	%ymm15, 352(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 1024(%rsp)
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm14
 2      9     0.50    *                   vpsignb	-32(%rdx,%r13), %ymm5, %ymm15
 2      12    0.50    *                   vfmadd213ps	320(%rsp), %ymm14, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 0      1     0.00                        vmovaps	%ymm12, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm15
 2      9     0.50    *                   vpsignb	-64(%rdx,%r13), %ymm0, %ymm9
 1      7     0.33    *                   vmovss	700(%rsp), %xmm13
 2      11    0.50    *                   vmulss	716(%rsp), %xmm13, %xmm14
 2      12    0.50           *            vmovaps	%ymm15, 320(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 1056(%rsp)
 1      7     0.33    *                   vmovss	696(%rsp), %xmm15
 2      11    0.50    *                   vmulss	712(%rsp), %xmm15, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
 2      9     0.50    *                   vpsignb	-32(%rdx,%r13), %ymm4, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm11
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm10, %ymm11
 1      3     1.00                        vbroadcastss	%xmm14, %ymm9
 1      3     1.00                        vbroadcastss	%xmm13, %ymm10
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm12
 2      12    0.50    *                   vfmadd213ps	288(%rsp), %ymm12, %ymm9
 1      7     0.33    *                   vmovss	676(%rsp), %xmm15
 1      7     0.33    *                   vmovss	688(%rsp), %xmm12
 1      4     0.50                        vcvtdq2ps	%ymm11, %ymm11
 2      11    0.50    *                   vmulss	680(%rsp), %xmm12, %xmm13
 1      4     0.50                        vfmadd132ps	%ymm10, %ymm9, %ymm11
 1      7     0.33    *                   vmovss	692(%rsp), %xmm10
 1      4     0.50                        vmulss	%xmm15, %xmm10, %xmm14
 1      8     0.33    *                   vmovdqu	-64(%rdx,%r12), %ymm10
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm10
 2      12    0.50           *            vmovaps	%ymm11, 288(%rsp)
 2      12    0.50           *            vmovaps	%ymm11, 1088(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 2      9     0.50    *                   vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm10, %ymm3
 2      9     0.50    *                   vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
 2      9     0.50    *                   vpsignb	-32(%rdx,%r12), %ymm4, %ymm4
 2      11    0.50    *                   vmulss	732(%rsp), %xmm15, %xmm12
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm11
 1      8     0.33    *                   vmovdqu	-32(%rdx,%r12), %ymm3
 1      1     0.50                        vpsignb	%ymm3, %ymm3, %ymm9
 1      1     0.50                        vpsignb	%ymm3, %ymm7, %ymm7
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 1      5     0.50                        {vex}	vpdpbusd	%ymm7, %ymm9, %ymm3
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm7
 1      3     1.00                        vbroadcastss	%xmm14, %ymm3
 2      12    0.50    *                   vfmadd213ps	256(%rsp), %ymm11, %ymm3
 1      3     1.00                        vbroadcastss	%xmm13, %ymm11
 1      7     0.33    *                   vmovss	680(%rsp), %xmm13
 1      4     0.50                        vfmadd132ps	%ymm11, %ymm3, %ymm7
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 2      11    0.50    *                   vmulss	728(%rsp), %xmm13, %xmm11
 2      12    0.50           *            vmovaps	%ymm7, 256(%rsp)
 2      12    0.50           *            vmovaps	%ymm7, 1120(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm2, %ymm7
 1      3     1.00                        vbroadcastss	%xmm12, %ymm2
 1      5     0.50                        {vex}	vpdpbusd	%ymm7, %ymm10, %ymm3
 0      1     0.00                        vmovdqa	%ymm8, %ymm7
 1      5     0.50                        {vex}	vpdpbusd	%ymm6, %ymm9, %ymm7
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm3
 2      12    0.50    *                   vfmadd213ps	224(%rsp), %ymm2, %ymm3
 1      3     1.00                        vbroadcastss	%xmm11, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm7, %ymm6
 2      11    0.50    *                   vmulss	724(%rsp), %xmm15, %xmm7
 1      4     0.50                        vfmadd132ps	%ymm2, %ymm3, %ymm6
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm1, %ymm3
 0      1     0.00                        vmovdqa	%ymm8, %ymm2
 1      5     0.50                        {vex}	vpdpbusd	%ymm3, %ymm10, %ymm2
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 1      3     1.00                        vbroadcastss	%xmm7, %ymm1
 1      5     0.50                        {vex}	vpdpbusd	%ymm5, %ymm9, %ymm3
 2      12    0.50           *            vmovaps	%ymm6, 224(%rsp)
 2      12    0.50           *            vmovaps	%ymm6, 1152(%rsp)
 2      11    0.50    *                   vmulss	720(%rsp), %xmm13, %xmm6
 1      4     0.50                        vcvtdq2ps	%ymm2, %ymm2
 2      12    0.50    *                   vfmadd213ps	192(%rsp), %ymm1, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm3
 1      3     1.00                        vbroadcastss	%xmm6, %ymm1
 1      4     0.50                        vfmadd132ps	%ymm1, %ymm2, %ymm3
 0      1     0.00                        vmovdqa	%ymm8, %ymm1
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm10, %ymm1
 0      1     0.00                        vmovdqa	%ymm8, %ymm0
 2      11    0.50    *                   vmulss	716(%rsp), %xmm15, %xmm2
 1      5     0.50                        {vex}	vpdpbusd	%ymm4, %ymm9, %ymm0
 2      12    0.50           *            vmovaps	%ymm3, 192(%rsp)
 2      12    0.50           *            vmovaps	%ymm3, 1184(%rsp)
 2      11    0.50    *                   vmulss	712(%rsp), %xmm13, %xmm3
 1      3     1.00                        vbroadcastss	%xmm2, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm1, %ymm1
 2      12    0.50    *                   vfmadd213ps	160(%rsp), %ymm2, %ymm1
 1      4     0.50                        vcvtdq2ps	%ymm0, %ymm0
 1      3     1.00                        vbroadcastss	%xmm3, %ymm2
 1      4     0.50                        vfmadd132ps	%ymm2, %ymm1, %ymm0
 2      12    0.50           *            vmovaps	%ymm0, 160(%rsp)
 2      12    0.50           *            vmovaps	%ymm0, 1216(%rsp)
 2      6     0.33    *                   cmpl	%edi, 24(%rbp)
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
91.29  91.29  47.00  47.00  24.00  56.43  5.25   24.00  24.00  24.00  4.74   47.00   -     

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   Instructions:
 -      -      -      -      -      -     0.26    -      -      -     0.74    -      -     movl	%edi, %eax
 -      -     0.01   0.02    -      -      -      -      -      -      -     0.97    -     movq	88(%rsp), %r15
 -      -      -      -      -      -      -      -      -      -      -      -      -     addq	$64, %rdx
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     sarl	$5, %eax
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     cltq
0.01    -      -      -      -      -      -      -      -      -     0.99    -      -     addq	%rax, %r15
 -      -     0.26   0.50    -      -      -      -      -      -      -     0.24    -     vmovss	4(%rsi,%rax,4), %xmm4
 -      -     0.73   0.25    -      -      -      -      -      -      -     0.02    -     vmovss	(%rsi,%rax,4), %xmm15
 -      -     0.48   0.50    -      -      -      -      -      -      -     0.02    -     vmovss	(%rsi,%r15,4), %xmm13
 -      -     0.49   0.51    -      -      -      -      -      -      -      -      -     vmovss	4(%rsi,%r15,4), %xmm5
 -      -     0.50   0.49    -      -      -      -      -      -      -     0.01    -     movq	80(%rsp), %r15
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm5, 704(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm13, 708(%rsp)
 -      -      -      -      -     0.01   0.25    -      -      -     0.74    -      -     addq	%rax, %r15
 -      -     0.25   0.73    -      -      -      -      -      -      -     0.02    -     vmovss	(%rsi,%r15,4), %xmm6
 -      -     0.49   0.51    -      -      -      -      -      -      -      -      -     vmovss	4(%rsi,%r15,4), %xmm14
 -      -     0.01   0.03    -      -      -      -      -      -      -     0.96    -     movq	72(%rsp), %r15
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm6, 700(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm14, 696(%rsp)
 -      -      -      -      -     0.26   0.48    -      -      -     0.26    -      -     addq	%rax, %r15
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
 -      -     0.27   0.25    -      -      -      -      -      -      -     0.48    -     vmovss	(%rsi,%r15,4), %xmm7
 -      -     0.25   0.49    -      -      -      -      -      -      -     0.26    -     vmovss	4(%rsi,%r15,4), %xmm11
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%r11,%rax), %r15
 -      -     0.73   0.01    -      -      -      -      -      -      -     0.26    -     vmovss	(%rcx,%r15,4), %xmm10
 -      -     0.01   0.26    -      -      -      -      -      -      -     0.73    -     vmovss	4(%rcx,%r15,4), %xmm12
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%r9,%rax), %r15
 -      -     0.48   0.25    -      -      -      -      -      -      -     0.27    -     vmovss	(%rcx,%r15,4), %xmm2
 -      -     0.25   0.26    -      -      -      -      -      -      -     0.49    -     vmovss	4(%rcx,%r15,4), %xmm1
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%r10,%rax), %r15
 -      -      -      -      -     0.49   0.48    -      -      -     0.03    -      -     addq	%r8, %rax
 -      -     0.50   0.49    -      -      -      -      -      -      -     0.01    -     vmovss	(%rcx,%rax,4), %xmm9
 -      -     0.26   0.74    -      -      -      -      -      -      -      -      -     vmovss	4(%rcx,%rax,4), %xmm5
 -      -      -      -      -     0.48   0.04    -      -      -     0.48    -      -     movl	%edi, %eax
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm7, 676(%rsp)
 -      -     0.49   0.26    -      -      -      -      -      -      -     0.25    -     vmovss	(%rcx,%r15,4), %xmm3
0.01    -      -      -      -      -     0.99    -      -      -      -      -      -     sarl	%eax
 -      -      -      -      -     0.74    -      -      -      -     0.26    -      -     addl	$64, %edi
 -      -     0.26   0.25    -      -      -      -      -      -      -     0.49    -     vmovss	4(%rcx,%r15,4), %xmm0
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm11, 680(%rsp)
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     cltq
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm2, 732(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm1, 728(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm3, 724(%rsp)
 -      -     0.72   0.27    -      -      -      -      -      -      -     0.01    -     vmovdqu	(%r14,%rax), %ymm3
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm0, 720(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm9, 716(%rsp)
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm3, %ymm7
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm5, 712(%rsp)
0.01    -     0.01   0.02    -      -     0.01    -      -      -     0.98   0.97    -     addq	120(%rsp), %rax
 -      -     0.01   0.98    -      -      -      -      -      -      -     0.01    -     movq	112(%rsp), %r15
 -      -     0.51   0.25    -     1.00    -      -      -      -      -     0.24    -     vpand	.LC0(%rip), %ymm3, %ymm3
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm10, 692(%rsp)
 -     0.01   0.50   0.49    -     0.99    -      -      -      -      -     0.01    -     vpaddb	.LC1(%rip), %ymm3, %ymm3
 -      -     0.74   0.26    -      -      -      -      -      -      -      -      -     vmovdqu	(%rax,%r15), %ymm2
 -      -     0.50   0.01    -      -      -      -      -      -      -     0.49    -     movq	104(%rsp), %r15
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm12, 688(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm15, 684(%rsp)
 -     0.25   0.50   0.49    -     0.75    -      -      -      -      -     0.01    -     vpand	.LC0(%rip), %ymm7, %ymm7
 -     0.48   0.25   0.50    -     0.52    -      -      -      -      -     0.25    -     vpaddb	.LC1(%rip), %ymm7, %ymm7
 -      -     0.25   0.25    -      -      -      -      -      -      -     0.50    -     vmovdqu	(%rax,%r15), %ymm1
 -      -     0.26   0.73    -      -      -      -      -      -      -     0.01    -     movq	96(%rsp), %r15
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm2, %ymm6
 -     0.01   0.03   0.25    -     0.99    -      -      -      -      -     0.72    -     vpand	.LC0(%rip), %ymm2, %ymm2
0.01   0.01   0.24   0.49    -     0.98    -      -      -      -      -     0.27    -     vpaddb	.LC1(%rip), %ymm2, %ymm2
 -      -     0.97   0.01    -      -      -      -      -      -      -     0.02    -     vmovdqu	(%rax,%r15), %ymm0
 -     0.02   0.74   0.01    -     0.98    -      -      -      -      -     0.25    -     vpand	.LC0(%rip), %ymm6, %ymm6
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm1, %ymm5
 -     0.74    -     0.75    -     0.26    -      -      -      -      -     0.25    -     vpaddb	.LC1(%rip), %ymm6, %ymm6
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm0, %ymm9
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovdqa	%ymm9, 128(%rsp)
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm15, %xmm10, %xmm9
 -      -      -     0.74    -      -      -      -      -      -      -     0.26    -     vmovdqu	-64(%rdx), %ymm10
0.03   0.97    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm10, %ymm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm9, %r15d
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm12, %xmm4, %xmm9
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm3, %ymm12
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm9, %eax
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm9
1.00    -      -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm12, %ymm13, %ymm9
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm9, %ymm15
 -      -     0.50    -      -      -      -      -      -      -      -     0.50    -     vmovdqu	-32(%rdx), %ymm9
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm9, %ymm12
0.28   0.72    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm7, %ymm11
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%r15d, %xmm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm11, %ymm11
0.25   0.75   0.25   0.73    -      -      -      -      -      -      -     0.02    -     vfmadd213ps	640(%rsp), %ymm15, %ymm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%eax, %xmm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm15
0.52   0.48    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm14
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm15, %ymm11, %ymm14
 -      -      -     0.01    -      -      -      -      -      -      -     0.99    -     vmovss	684(%rsp), %xmm15
0.50   0.50   0.26   0.50    -      -      -      -      -      -      -     0.24    -     vmulss	728(%rsp), %xmm4, %xmm11
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm14, 640(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm14, 736(%rsp)
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm11, %eax
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm6, %ymm11
0.49   0.51   0.25   0.26    -      -      -      -      -      -      -     0.49    -     vmulss	732(%rsp), %xmm15, %xmm14
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm2, %ymm15
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm14, %r15d
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm15
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%r15d, %xmm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm11, %ymm11
0.99   0.01   0.51   0.48    -      -      -      -      -      -      -     0.01    -     vfmadd213ps	608(%rsp), %ymm15, %ymm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%eax, %xmm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm15
0.98   0.02    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm14
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm15, %ymm11, %ymm14
 -      -     0.26   0.72    -      -      -      -      -      -      -     0.02    -     vmovss	684(%rsp), %xmm15
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm14, 608(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm14, 768(%rsp)
0.25   0.75   0.75   0.25    -      -      -      -      -      -      -      -      -     vmulss	724(%rsp), %xmm15, %xmm14
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm14, %r15d
0.25   0.75   0.49   0.49    -      -      -      -      -      -      -     0.02    -     vmulss	720(%rsp), %xmm4, %xmm14
0.01    -      -     0.02    -     0.99    -      -      -      -      -     0.98    -     vpand	.LC0(%rip), %ymm1, %ymm1
 -      -     0.25   0.24    -     1.00    -      -      -      -      -     0.51    -     vpaddb	.LC1(%rip), %ymm1, %ymm1
 -      -     0.26   0.26    -     1.00    -      -      -      -      -     0.48    -     vpand	.LC0(%rip), %ymm5, %ymm5
 -      -     0.26   0.01    -     1.00    -      -      -      -      -     0.73    -     vpaddb	.LC1(%rip), %ymm5, %ymm5
 -     0.01   0.26   0.72    -     0.99    -      -      -      -      -     0.02    -     vpand	.LC0(%rip), %ymm0, %ymm0
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm1, %ymm15
 -      -     0.73   0.26    -     1.00    -      -      -      -      -     0.01    -     vpaddb	.LC1(%rip), %ymm0, %ymm0
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm5, %ymm11
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vmovd	%xmm14, %eax
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm0, %ymm10
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm15
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%r15d, %xmm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm11, %ymm11
0.99   0.01   0.02    -      -      -      -      -      -      -      -     0.98    -     vfmadd213ps	576(%rsp), %ymm15, %ymm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vmovd	%eax, %xmm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm15
0.01   0.99    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm14
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm15, %ymm11, %ymm14
 -      -     0.25   0.02    -      -      -      -      -      -      -     0.73    -     vmovss	684(%rsp), %xmm15
0.48   0.52    -     0.03    -      -      -      -      -      -      -     0.97    -     vmulss	716(%rsp), %xmm15, %xmm15
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm14, 576(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm14, 800(%rsp)
 -     1.00   0.01   0.97    -      -      -      -      -      -      -     0.02    -     vmulss	712(%rsp), %xmm4, %xmm14
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm4
0.27   0.73    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm10, %ymm13, %ymm4
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm10
0.03   0.97    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm4, %ymm11
 -      -     0.99    -      -      -      -      -      -      -      -     0.01    -     vmovdqa	128(%rsp), %ymm4
 -      -     0.01   0.97    -     1.00    -      -      -      -      -     0.02    -     vpand	.LC0(%rip), %ymm4, %ymm4
 -      -     0.26   0.48    -     1.00    -      -      -      -      -     0.26    -     vpaddb	.LC1(%rip), %ymm4, %ymm4
0.49   0.51    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm4, %ymm9
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm12, %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm15, %ymm9
0.25   0.75   0.99    -      -      -      -      -      -      -      -     0.01    -     vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15
 -      -     0.97   0.02    -      -      -      -      -      -      -     0.01    -     vmovss	688(%rsp), %xmm12
1.00    -     0.01   0.97    -      -      -      -      -      -      -     0.02    -     vfmadd213ps	544(%rsp), %ymm11, %ymm9
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm14, %ymm11
0.48   0.52    -     0.49    -      -      -      -      -      -      -     0.51    -     vmulss	704(%rsp), %xmm12, %xmm13
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
0.27   0.73    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm10, %ymm10
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm11, %ymm9, %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm10, 544(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm10, 832(%rsp)
 -      -      -     0.99    -      -      -      -      -      -      -     0.01    -     vmovss	692(%rsp), %xmm10
0.25   0.75    -     0.99    -      -      -      -      -      -      -     0.01    -     vmulss	708(%rsp), %xmm10, %xmm9
 -      -     0.26   0.72    -      -      -      -      -      -      -     0.02    -     vmovdqu	-64(%rdx,%rbx), %ymm10
0.72   0.28    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm10, %ymm11
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm3, %ymm10
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm10, %ymm11, %ymm12
 -      -     0.50   0.02    -      -      -      -      -      -      -     0.48    -     vmovdqu	-32(%rdx,%rbx), %ymm10
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm10, %ymm10
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm14
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
0.97   0.03   0.25   0.25    -      -      -      -      -      -      -     0.50    -     vfmadd213ps	512(%rsp), %ymm14, %ymm9
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 -      -     0.02   0.49    -      -      -      -      -      -      -     0.49    -     vmovss	704(%rsp), %xmm15
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm12
 -      -     0.49   0.49    -      -      -      -      -      -      -     0.02    -     vmovss	708(%rsp), %xmm13
0.50   0.50   0.26   0.01    -      -      -      -      -      -      -     0.73    -     vmulss	732(%rsp), %xmm13, %xmm9
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm12, 512(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm12, 864(%rsp)
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
0.50   0.50   0.01   0.99    -      -      -      -      -      -      -      -      -     vmulss	728(%rsp), %xmm15, %xmm13
0.51   0.49   0.99    -      -      -      -      -      -      -      -     0.01    -     vpsignb	-64(%rdx,%rbx), %ymm2, %ymm12
0.49   0.51    -     0.26    -      -      -      -      -      -      -     0.74    -     vpsignb	-32(%rdx,%rbx), %ymm6, %ymm15
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 -      -     0.26   0.49    -      -      -      -      -      -      -     0.25    -     vmovss	704(%rsp), %xmm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm14
 -     1.00    -     0.99    -      -      -      -      -      -      -     0.01    -     vfmadd213ps	480(%rsp), %ymm14, %ymm9
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm14
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm12
 -      -     0.49   0.02    -      -      -      -      -      -      -     0.49    -     vmovss	708(%rsp), %xmm13
0.27   0.73   0.74   0.24    -      -      -      -      -      -      -     0.02    -     vmulss	724(%rsp), %xmm13, %xmm9
0.24   0.76    -     0.02    -      -      -      -      -      -      -     0.98    -     vmulss	720(%rsp), %xmm15, %xmm13
0.26   0.74   0.03   0.97    -      -      -      -      -      -      -      -      -     vpsignb	-32(%rdx,%rbx), %ymm5, %ymm15
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm12, 480(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm12, 896(%rsp)
0.26   0.74   0.97    -      -      -      -      -      -      -      -     0.03    -     vpsignb	-64(%rdx,%rbx), %ymm1, %ymm12
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
1.00    -      -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 -      -     0.72   0.27    -      -      -      -      -      -      -     0.01    -     vmovss	704(%rsp), %xmm15
0.98   0.02    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm14, %ymm14
0.02   0.98   0.49   0.49    -      -      -      -      -      -      -     0.02    -     vfmadd213ps	448(%rsp), %ymm14, %ymm9
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm12
0.25   0.75   0.02   0.01    -      -      -      -      -      -      -     0.97    -     vpsignb	-64(%rdx,%rbx), %ymm0, %ymm9
 -      -     0.26   0.25    -      -      -      -      -      -      -     0.49    -     vmovss	708(%rsp), %xmm13
0.51   0.49   0.02   0.97    -      -      -      -      -      -      -     0.01    -     vmulss	716(%rsp), %xmm13, %xmm14
0.25   0.75   0.74   0.01    -      -      -      -      -      -      -     0.25    -     vmulss	712(%rsp), %xmm15, %xmm13
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
0.25   0.75   0.01   0.74    -      -      -      -      -      -      -     0.25    -     vpsignb	-32(%rdx,%rbx), %ymm4, %ymm9
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm12, 448(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm12, 928(%rsp)
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm12
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
0.24   0.76    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm10, %ymm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm14, %ymm9
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm10
0.74   0.26   0.24   0.52    -      -      -      -      -      -      -     0.24    -     vfmadd213ps	416(%rsp), %ymm12, %ymm9
 -      -     0.01   0.98    -      -      -      -      -      -      -     0.01    -     vmovss	688(%rsp), %xmm12
0.51   0.49   0.99   0.01    -      -      -      -      -      -      -      -      -     vmulss	696(%rsp), %xmm12, %xmm13
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm11
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovaps	%ymm11, %ymm15
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm10, %ymm9, %ymm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
 -      -     0.25   0.02    -      -      -      -      -      -      -     0.73    -     vmovss	692(%rsp), %xmm10
0.01   0.99    -     0.01    -      -      -      -      -      -      -     0.99    -     vmulss	700(%rsp), %xmm10, %xmm9
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm15, 416(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm15, 960(%rsp)
 -      -     0.48   0.26    -      -      -      -      -      -      -     0.26    -     vmovdqu	-64(%rdx,%r13), %ymm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
0.02   0.98    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm15, %ymm15, %ymm11
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm15, %ymm3, %ymm10
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
0.24   0.76    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm10, %ymm11, %ymm15
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm14
 -      -     0.02    -      -      -      -      -      -      -      -     0.98    -     vmovdqu	-32(%rdx,%r13), %ymm15
0.27   0.73   0.52   0.48    -      -      -      -      -      -      -      -      -     vfmadd213ps	384(%rsp), %ymm14, %ymm9
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm15, %ymm15, %ymm10
0.03   0.97    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm15, %ymm7, %ymm15
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovaps	%ymm12, %ymm15
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm15
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm15, 384(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm15, 992(%rsp)
0.73   0.27   0.50    -      -      -      -      -      -      -      -     0.50    -     vpsignb	-64(%rdx,%r13), %ymm2, %ymm12
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
 -      -     0.73   0.27    -      -      -      -      -      -      -      -      -     vmovss	700(%rsp), %xmm13
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
 -      -     0.02   0.01    -      -      -      -      -      -      -     0.97    -     vmovss	696(%rsp), %xmm14
0.26   0.74    -     0.26    -      -      -      -      -      -      -     0.74    -     vmulss	732(%rsp), %xmm13, %xmm9
0.75   0.25   0.49   0.27    -      -      -      -      -      -      -     0.24    -     vmulss	728(%rsp), %xmm14, %xmm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm14
0.25   0.75   0.03   0.24    -      -      -      -      -      -      -     0.73    -     vpsignb	-32(%rdx,%r13), %ymm6, %ymm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
0.27   0.73    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
 -     1.00   0.01   0.49    -      -      -      -      -      -      -     0.50    -     vfmadd213ps	352(%rsp), %ymm14, %ymm9
 -      -     0.01   0.98    -      -      -      -      -      -      -     0.01    -     vmovss	696(%rsp), %xmm14
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovaps	%ymm12, %ymm15
0.75   0.25   0.48   0.25    -      -      -      -      -      -      -     0.27    -     vpsignb	-64(%rdx,%r13), %ymm1, %ymm12
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm15
 -      -     0.98   0.02    -      -      -      -      -      -      -      -      -     vmovss	700(%rsp), %xmm13
0.25   0.75   0.02   0.26    -      -      -      -      -      -      -     0.72    -     vmulss	724(%rsp), %xmm13, %xmm9
0.74   0.26   0.49   0.24    -      -      -      -      -      -      -     0.27    -     vmulss	720(%rsp), %xmm14, %xmm13
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm15, 352(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm15, 1024(%rsp)
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
0.73   0.27    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm14
0.25   0.75   0.01   0.25    -      -      -      -      -      -      -     0.74    -     vpsignb	-32(%rdx,%r13), %ymm5, %ymm15
0.27   0.73   0.01   0.97    -      -      -      -      -      -      -     0.02    -     vfmadd213ps	320(%rsp), %ymm14, %ymm9
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovaps	%ymm12, %ymm15
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm15
0.26   0.74   0.50   0.26    -      -      -      -      -      -      -     0.24    -     vpsignb	-64(%rdx,%r13), %ymm0, %ymm9
 -      -     0.49   0.02    -      -      -      -      -      -      -     0.49    -     vmovss	700(%rsp), %xmm13
0.50   0.50   0.26    -      -      -      -      -      -      -      -     0.74    -     vmulss	716(%rsp), %xmm13, %xmm14
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm15, 320(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm15, 1056(%rsp)
 -      -      -     0.50    -      -      -      -      -      -      -     0.50    -     vmovss	696(%rsp), %xmm15
0.51   0.49   0.25   0.25    -      -      -      -      -      -      -     0.50    -     vmulss	712(%rsp), %xmm15, %xmm13
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
0.49   0.51   0.26   0.25    -      -      -      -      -      -      -     0.49    -     vpsignb	-32(%rdx,%r13), %ymm4, %ymm9
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm11
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm10, %ymm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm14, %ymm9
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm10
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm12
0.25   0.75   0.72   0.03    -      -      -      -      -      -      -     0.25    -     vfmadd213ps	288(%rsp), %ymm12, %ymm9
 -      -     0.02   0.49    -      -      -      -      -      -      -     0.49    -     vmovss	676(%rsp), %xmm15
 -      -     0.97   0.02    -      -      -      -      -      -      -     0.01    -     vmovss	688(%rsp), %xmm12
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm11, %ymm11
0.50   0.50   0.26   0.24    -      -      -      -      -      -      -     0.50    -     vmulss	680(%rsp), %xmm12, %xmm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm10, %ymm9, %ymm11
 -      -     0.02   0.01    -      -      -      -      -      -      -     0.97    -     vmovss	692(%rsp), %xmm10
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm15, %xmm10, %xmm14
 -      -     0.98   0.01    -      -      -      -      -      -      -     0.01    -     vmovdqu	-64(%rdx,%r12), %ymm10
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm10, %ymm10
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm11, 288(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm11, 1088(%rsp)
 -     1.00   0.72   0.02    -      -      -      -      -      -      -     0.26    -     vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
0.03   0.97   0.03   0.25    -      -      -      -      -      -      -     0.72    -     vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm10, %ymm3
0.49   0.51   0.49    -      -      -      -      -      -      -      -     0.51    -     vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
 -     1.00   0.03   0.97    -      -      -      -      -      -      -      -      -     vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
0.49   0.51   0.26    -      -      -      -      -      -      -      -     0.74    -     vpsignb	-32(%rdx,%r12), %ymm4, %ymm4
0.27   0.73   0.25   0.49    -      -      -      -      -      -      -     0.26    -     vmulss	732(%rsp), %xmm15, %xmm12
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm11
 -      -     0.73    -      -      -      -      -      -      -      -     0.27    -     vmovdqu	-32(%rdx,%r12), %ymm3
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm3, %ymm9
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm7, %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm7, %ymm9, %ymm3
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm14, %ymm3
0.24   0.76   0.49    -      -      -      -      -      -      -      -     0.51    -     vfmadd213ps	256(%rsp), %ymm11, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm11
 -      -     0.24   0.50    -      -      -      -      -      -      -     0.26    -     vmovss	680(%rsp), %xmm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm11, %ymm3, %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
0.01   0.99   0.96   0.03    -      -      -      -      -      -      -     0.01    -     vmulss	728(%rsp), %xmm13, %xmm11
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm7, 256(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm7, 1120(%rsp)
0.49   0.51   0.01   0.74    -      -      -      -      -      -      -     0.25    -     vpsignb	-64(%rdx,%r12), %ymm2, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm12, %ymm2
0.97   0.03    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm7, %ymm10, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm7
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm6, %ymm9, %ymm7
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm3
0.01   0.99   0.24   0.24    -      -      -      -      -      -      -     0.52    -     vfmadd213ps	224(%rsp), %ymm2, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm11, %ymm2
0.97   0.03    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm7, %ymm6
0.50   0.50   0.72   0.03    -      -      -      -      -      -      -     0.25    -     vmulss	724(%rsp), %xmm15, %xmm7
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm2, %ymm3, %ymm6
0.26   0.74   0.02   0.74    -      -      -      -      -      -      -     0.24    -     vpsignb	-64(%rdx,%r12), %ymm1, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm2
1.00    -      -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm3, %ymm10, %ymm2
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm7, %ymm1
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm5, %ymm9, %ymm3
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm6, 224(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm6, 1152(%rsp)
0.01   0.99    -     0.73    -      -      -      -      -      -      -     0.27    -     vmulss	720(%rsp), %xmm13, %xmm6
0.97   0.03    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm2, %ymm2
0.03   0.97    -     0.49    -      -      -      -      -      -      -     0.51    -     vfmadd213ps	192(%rsp), %ymm1, %ymm2
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm6, %ymm1
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm1, %ymm2, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm1
0.02   0.98    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm10, %ymm1
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm0
0.01   0.99    -     0.02    -      -      -      -      -      -      -     0.98    -     vmulss	716(%rsp), %xmm15, %xmm2
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm4, %ymm9, %ymm0
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm3, 192(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm3, 1184(%rsp)
0.26   0.74   0.04    -      -      -      -      -      -      -      -     0.96    -     vmulss	712(%rsp), %xmm13, %xmm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm2, %ymm2
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm1, %ymm1
0.99   0.01   0.75    -      -      -      -      -      -      -      -     0.25    -     vfmadd213ps	160(%rsp), %ymm2, %ymm1
0.27   0.73    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm0, %ymm0
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm3, %ymm2
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm2, %ymm1, %ymm0
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm0, 160(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm0, 1216(%rsp)
 -      -     0.98    -      -      -     0.74    -      -      -     0.26   0.02    -     cmpl	%edi, 24(%rbp)
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L4


Timeline view:
                    0123456789          0123456789          0123456789          0123456789
Index     0123456789          0123456789          0123456789          0123456789          

[0,0]     DER  .    .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   movl	%edi, %eax
[0,1]     DeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   movq	88(%rsp), %r15
[0,2]     DeE----R  .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   addq	$64, %rdx
[0,3]     DeE----R  .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   sarl	$5, %eax
[0,4]     D=eE---R  .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   cltq
[0,5]     D=====eER .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   addq	%rax, %r15
[0,6]     D==eeeeeeeER   .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rsi,%rax,4), %xmm4
[0,7]     .D=eeeeeeeER   .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rsi,%rax,4), %xmm15
[0,8]     .D=====eeeeeeeER    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rsi,%r15,4), %xmm13
[0,9]     .D=====eeeeeeeER    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rsi,%r15,4), %xmm5
[0,10]    .DeeeeeE-------R    .    .    .    .    .    .    .    .    .    .    .    .   .   movq	80(%rsp), %r15
[0,11]    .D============eeeeeeeeeeeeER  .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm5, 704(%rsp)
[0,12]    . D===========eeeeeeeeeeeeER  .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm13, 708(%rsp)
[0,13]    . D====eE------------------R  .    .    .    .    .    .    .    .    .    .   .   addq	%rax, %r15
[0,14]    . D=====eeeeeeeE-----------R  .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rsi,%r15,4), %xmm6
[0,15]    . D=====eeeeeeeE-----------R  .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rsi,%r15,4), %xmm14
[0,16]    . DeeeeeE------------------R  .    .    .    .    .    .    .    .    .    .   .   movq	72(%rsp), %r15
[0,17]    .  D===========eeeeeeeeeeeeER .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm6, 700(%rsp)
[0,18]    .  D===========eeeeeeeeeeeeER .    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm14, 696(%rsp)
[0,19]    .  D====eE------------------R .    .    .    .    .    .    .    .    .    .   .   addq	%rax, %r15
[0,20]    .  DeE----------------------R .    .    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm8, %ymm14
[0,21]    .  D=====eeeeeeeE-----------R .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rsi,%r15,4), %xmm7
[0,22]    .   D====eeeeeeeE-----------R .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rsi,%r15,4), %xmm11
[0,23]    .   DeE---------------------R .    .    .    .    .    .    .    .    .    .   .   leaq	(%r11,%rax), %r15
[0,24]    .   D=eeeeeeeE--------------R .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rcx,%r15,4), %xmm10
[0,25]    .   D=eeeeeeeE--------------R .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rcx,%r15,4), %xmm12
[0,26]    .   D=eE--------------------R .    .    .    .    .    .    .    .    .    .   .   leaq	(%r9,%rax), %r15
[0,27]    .   D==eeeeeeeE-------------R .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rcx,%r15,4), %xmm2
[0,28]    .    D==eeeeeeeE------------R .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rcx,%r15,4), %xmm1
[0,29]    .    D=eE-------------------R .    .    .    .    .    .    .    .    .    .   .   leaq	(%r10,%rax), %r15
[0,30]    .    DeE--------------------R .    .    .    .    .    .    .    .    .    .   .   addq	%r8, %rax
[0,31]    .    D===eeeeeeeE-----------R .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rcx,%rax,4), %xmm9
[0,32]    .    D====eeeeeeeE----------R .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rcx,%rax,4), %xmm5
[0,33]    .    DE---------------------R .    .    .    .    .    .    .    .    .    .   .   movl	%edi, %eax
[0,34]    .    .D=========eeeeeeeeeeeeER.    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm7, 676(%rsp)
[0,35]    .    .D===eeeeeeeE-----------R.    .    .    .    .    .    .    .    .    .   .   vmovss	(%rcx,%r15,4), %xmm3
[0,36]    .    .DeE--------------------R.    .    .    .    .    .    .    .    .    .   .   sarl	%eax
[0,37]    .    .DeE--------------------R.    .    .    .    .    .    .    .    .    .   .   addl	$64, %edi
[0,38]    .    .D===eeeeeeeE-----------R.    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rcx,%r15,4), %xmm0
[0,39]    .    . D========eeeeeeeeeeeeER.    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm11, 680(%rsp)
[0,40]    .    . DeE-------------------R.    .    .    .    .    .    .    .    .    .   .   cltq
[0,41]    .    . D=========eeeeeeeeeeeeER    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm2, 732(%rsp)
[0,42]    .    .  D========eeeeeeeeeeeeER    .    .    .    .    .    .    .    .    .   .   vmovss	%xmm1, 728(%rsp)
[0,43]    .    .  D=========eeeeeeeeeeeeER   .    .    .    .    .    .    .    .    .   .   vmovss	%xmm3, 724(%rsp)
[0,44]    .    .  D==eeeeeeeeE-----------R   .    .    .    .    .    .    .    .    .   .   vmovdqu	(%r14,%rax), %ymm3
[0,45]    .    .   D========eeeeeeeeeeeeER   .    .    .    .    .    .    .    .    .   .   vmovss	%xmm0, 720(%rsp)
[0,46]    .    .   D=========eeeeeeeeeeeeER  .    .    .    .    .    .    .    .    .   .   vmovss	%xmm9, 716(%rsp)
[0,47]    .    .   D=========eE-----------R  .    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm3, %ymm7
[0,48]    .    .    D========eeeeeeeeeeeeER  .    .    .    .    .    .    .    .    .   .   vmovss	%xmm5, 712(%rsp)
[0,49]    .    .    DeeeeeeE--------------R  .    .    .    .    .    .    .    .    .   .   addq	120(%rsp), %rax
[0,50]    .    .    DeeeeeE---------------R  .    .    .    .    .    .    .    .    .   .   movq	112(%rsp), %r15
[0,51]    .    .    .D=eeeeeeeeeE---------R  .    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm3, %ymm3
[0,52]    .    .    .D========eeeeeeeeeeeeER .    .    .    .    .    .    .    .    .   .   vmovss	%xmm10, 692(%rsp)
[0,53]    .    .    .D====eeeeeeeeeE-------R .    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm3, %ymm3
[0,54]    .    .    . D====eeeeeeeeE-------R .    .    .    .    .    .    .    .    .   .   vmovdqu	(%rax,%r15), %ymm2
[0,55]    .    .    . DeeeeeE--------------R .    .    .    .    .    .    .    .    .   .   movq	104(%rsp), %r15
[0,56]    .    .    . D=======eeeeeeeeeeeeER .    .    .    .    .    .    .    .    .   .   vmovss	%xmm12, 688(%rsp)
[0,57]    .    .    . D========eeeeeeeeeeeeER.    .    .    .    .    .    .    .    .   .   vmovss	%xmm15, 684(%rsp)
[0,58]    .    .    .  D=====eeeeeeeeeE-----R.    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm7, %ymm7
[0,59]    .    .    .  D========eeeeeeeeeE--R.    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm7, %ymm7
[0,60]    .    .    .  D====eeeeeeeeE-------R.    .    .    .    .    .    .    .    .   .   vmovdqu	(%rax,%r15), %ymm1
[0,61]    .    .    .  DeeeeeE--------------R.    .    .    .    .    .    .    .    .   .   movq	96(%rsp), %r15
[0,62]    .    .    .   D==========eE-------R.    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm2, %ymm6
[0,63]    .    .    .   D====eeeeeeeeeE-----R.    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm2, %ymm2
[0,64]    .    .    .   D=======eeeeeeeeeE--R.    .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm2, %ymm2
[0,65]    .    .    .   D====eeeeeeeeE------R.    .    .    .    .    .    .    .    .   .   vmovdqu	(%rax,%r15), %ymm0
[0,66]    .    .    .    D=========eeeeeeeeeER    .    .    .    .    .    .    .    .   .   vpand	.LC0(%rip), %ymm6, %ymm6
[0,67]    .    .    .    D==========eE-------R    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm1, %ymm5
[0,68]    .    .    .    D============eeeeeeeeeER .    .    .    .    .    .    .    .   .   vpaddb	.LC1(%rip), %ymm6, %ymm6
[0,69]    .    .    .    D===========eE---------R .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm9
[0,70]    .    .    .    .D===========eeeeeeeeeeeeER   .    .    .    .    .    .    .   .   vmovdqa	%ymm9, 128(%rsp)
[0,71]    .    .    .    .DeeeeE-------------------R   .    .    .    .    .    .    .   .   vmulss	%xmm15, %xmm10, %xmm9
[0,72]    .    .    .    .DeeeeeeeeE---------------R   .    .    .    .    .    .    .   .   vmovdqu	-64(%rdx), %ymm10
[0,73]    .    .    .    .D========eE--------------R   .    .    .    .    .    .    .   .   vpsignb	%ymm10, %ymm10, %ymm13
[0,74]    .    .    .    .D====eeeE----------------R   .    .    .    .    .    .    .   .   vmovd	%xmm9, %r15d
[0,75]    .    .    .    . DeeeeE------------------R   .    .    .    .    .    .    .   .   vmulss	%xmm12, %xmm4, %xmm9
[0,76]    .    .    .    . D========eE-------------R   .    .    .    .    .    .    .   .   vpsignb	%ymm10, %ymm3, %ymm12
[0,77]    .    .    .    . D=====eeeE--------------R   .    .    .    .    .    .    .   .   vmovd	%xmm9, %eax
[0,78]    .    .    .    . DeE---------------------R   .    .    .    .    .    .    .   .   vmovdqa	%ymm8, %ymm9
[0,79]    .    .    .    . D=========eeeeeE--------R   .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm12, %ymm13, %ymm9
[0,80]    .    .    .    . D==============eeeeE----R   .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm9, %ymm15
[0,81]    .    .    .    . DeeeeeeeeE--------------R   .    .    .    .    .    .    .   .   vmovdqu	-32(%rdx), %ymm9
[0,82]    .    .    .    .  D=========eE-----------R   .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm9, %ymm12
[0,83]    .    .    .    .  D============eE--------R   .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm7, %ymm11
[0,84]    .    .    .    .  D=============eeeeeE---R   .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
[0,85]    .    .    .    .  D=====eeeE-------------R   .    .    .    .    .    .    .   .   vmovd	%r15d, %xmm11
[0,86]    .    .    .    .  D========eeeE----------R   .    .    .    .    .    .    .   .   vbroadcastss	%xmm11, %ymm11
[0,87]    .    .    .    .   D=============eeeeeeeeeeeeER   .    .    .    .    .    .   .   vfmadd213ps	640(%rsp), %ymm15, %ymm11
[0,88]    .    .    .    .   D======eeeE----------------R   .    .    .    .    .    .   .   vmovd	%eax, %xmm15
[0,89]    .    .    .    .   D=========eeeE-------------R   .    .    .    .    .    .   .   vbroadcastss	%xmm15, %ymm15
[0,90]    .    .    .    .   D=================eeeeE----R   .    .    .    .    .    .   .   vcvtdq2ps	%ymm14, %ymm14
[0,91]    .    .    .    .   D=========================eeeeER    .    .    .    .    .   .   vfmadd132ps	%ymm15, %ymm11, %ymm14
[0,92]    .    .    .    .    DeeeeeeeE---------------------R    .    .    .    .    .   .   vmovss	684(%rsp), %xmm15
[0,93]    .    .    .    .    DeeeeeeeeeeeE-----------------R    .    .    .    .    .   .   vmulss	728(%rsp), %xmm4, %xmm11
[0,94]    .    .    .    .    D============================eeeeeeeeeeeeER  .    .    .   .   vmovaps	%ymm14, 640(%rsp)
[0,95]    .    .    .    .    .D===========================eeeeeeeeeeeeER  .    .    .   .   vmovaps	%ymm14, 736(%rsp)
[0,96]    .    .    .    .    .D============eeeE------------------------R  .    .    .   .   vmovd	%xmm11, %eax
[0,97]    .    .    .    .    .D===============eE-----------------------R  .    .    .   .   vpsignb	%ymm9, %ymm6, %ymm11
[0,98]    .    .    .    .    .DeeeeeeeeeeeE----------------------------R  .    .    .   .   vmulss	732(%rsp), %xmm15, %xmm14
[0,99]    .    .    .    .    . D========eE-----------------------------R  .    .    .   .   vpsignb	%ymm10, %ymm2, %ymm15
[0,100]   .    .    .    .    . D============eeeE-----------------------R  .    .    .   .   vmovd	%xmm14, %r15d
[0,101]   .    .    .    .    . DeE-------------------------------------R  .    .    .   .   vmovdqa	%ymm8, %ymm14
[0,102]   .    .    .    .    . D==========eeeeeE-----------------------R  .    .    .   .   {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
[0,103]   .    .    .    .    . D===============eeeeE-------------------R  .    .    .   .   vcvtdq2ps	%ymm14, %ymm15
[0,104]   .    .    .    .    . DeE-------------------------------------R  .    .    .   .   vmovdqa	%ymm8, %ymm14
[0,105]   .    .    .    .    . D===============eeeeeE------------------R  .    .    .   .   {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
[0,106]   .    .    .    .    . D===============eeeE--------------------R  .    .    .   .   vmovd	%r15d, %xmm11
[0,107]   .    .    .    .    .  D=================eeeE-----------------R  .    .    .   .   vbroadcastss	%xmm11, %ymm11
[0,108]   .    .    .    .    .  D=================eeeeeeeeeeeeE--------R  .    .    .   .   vfmadd213ps	608(%rsp), %ymm15, %ymm11
[0,109]   .    .    .    .    .  D=============eeeE---------------------R  .    .    .   .   vmovd	%eax, %xmm15
[0,110]   .    .    .    .    .  D================eeeE------------------R  .    .    .   .   vbroadcastss	%xmm15, %ymm15
[0,111]   .    .    .    .    .  D===================eeeeE--------------R  .    .    .   .   vcvtdq2ps	%ymm14, %ymm14
[0,112]   .    .    .    .    .   D============================eeeeE----R  .    .    .   .   vfmadd132ps	%ymm15, %ymm11, %ymm14
[0,113]   .    .    .    .    .   DeeeeeeeE-----------------------------R  .    .    .   .   vmovss	684(%rsp), %xmm15
[0,114]   .    .    .    .    .   D================================eeeeeeeeeeeeER    .   .   vmovaps	%ymm14, 608(%rsp)
[0,115]   .    .    .    .    .   D================================eeeeeeeeeeeeER    .   .   vmovaps	%ymm14, 768(%rsp)
[0,116]   .    .    .    .    .    D==eeeeeeeeeeeE------------------------------R    .   .   vmulss	724(%rsp), %xmm15, %xmm14
[0,117]   .    .    .    .    .    D=============eeeE---------------------------R    .   .   vmovd	%xmm14, %r15d
[0,118]   .    .    .    .    .    D===eeeeeeeeeeeE-----------------------------R    .   .   vmulss	720(%rsp), %xmm4, %xmm14
[0,119]   .    .    .    .    .    .D==eeeeeeeeeE-------------------------------R    .   .   vpand	.LC0(%rip), %ymm1, %ymm1
[0,120]   .    .    .    .    .    .D=====eeeeeeeeeE----------------------------R    .   .   vpaddb	.LC1(%rip), %ymm1, %ymm1
[0,121]   .    .    .    .    .    .D===eeeeeeeeeE------------------------------R    .   .   vpand	.LC0(%rip), %ymm5, %ymm5
[0,122]   .    .    .    .    .    . D=====eeeeeeeeeE---------------------------R    .   .   vpaddb	.LC1(%rip), %ymm5, %ymm5
[0,123]   .    .    .    .    .    . D==eeeeeeeeeE------------------------------R    .   .   vpand	.LC0(%rip), %ymm0, %ymm0
[0,124]   .    .    .    .    .    . D=============eE---------------------------R    .   .   vpsignb	%ymm10, %ymm1, %ymm15
[0,125]   .    .    .    .    .    .  D=====eeeeeeeeeE--------------------------R    .   .   vpaddb	.LC1(%rip), %ymm0, %ymm0
[0,126]   .    .    .    .    .    .  D=============eE--------------------------R    .   .   vpsignb	%ymm9, %ymm5, %ymm11
[0,127]   .    .    .    .    .    .  D===========eeeE--------------------------R    .   .   vmovd	%xmm14, %eax
[0,128]   .    .    .    .    .    .  DeE---------------------------------------R    .   .   vmovdqa	%ymm8, %ymm14
[0,129]   .    .    .    .    .    .  D==============eE-------------------------R    .   .   vpsignb	%ymm10, %ymm0, %ymm10
[0,130]   .    .    .    .    .    .  D=============eeeeeE----------------------R    .   .   {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
[0,131]   .    .    .    .    .    .   D=================eeeeE------------------R    .   .   vcvtdq2ps	%ymm14, %ymm15
[0,132]   .    .    .    .    .    .   DeE--------------------------------------R    .   .   vmovdqa	%ymm8, %ymm14
[0,133]   .    .    .    .    .    .   D==============eeeeeE--------------------R    .   .   {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
[0,134]   .    .    .    .    .    .   D============eeeE------------------------R    .   .   vmovd	%r15d, %xmm11
[0,135]   .    .    .    .    .    .   D===============eeeE---------------------R    .   .   vbroadcastss	%xmm11, %ymm11
[0,136]   .    .    .    .    .    .   D=================eeeeeeeeeeeeE----------R    .   .   vfmadd213ps	576(%rsp), %ymm15, %ymm11
[0,137]   .    .    .    .    .    .    D============eeeE-----------------------R    .   .   vmovd	%eax, %xmm15
[0,138]   .    .    .    .    .    .    D===============eeeE--------------------R    .   .   vbroadcastss	%xmm15, %ymm15
[0,139]   .    .    .    .    .    .    D==================eeeeE----------------R    .   .   vcvtdq2ps	%ymm14, %ymm14
[0,140]   .    .    .    .    .    .    D============================eeeeE------R    .   .   vfmadd132ps	%ymm15, %ymm11, %ymm14
[0,141]   .    .    .    .    .    .    DeeeeeeeE-------------------------------R    .   .   vmovss	684(%rsp), %xmm15
[0,142]   .    .    .    .    .    .    .D==eeeeeeeeeeeE------------------------R    .   .   vmulss	716(%rsp), %xmm15, %xmm15
[0,143]   .    .    .    .    .    .    .D===============================eeeeeeeeeeeeER  .   vmovaps	%ymm14, 576(%rsp)
[0,144]   .    .    .    .    .    .    .D===============================eeeeeeeeeeeeER  .   vmovaps	%ymm14, 800(%rsp)
[0,145]   .    .    .    .    .    .    . D==eeeeeeeeeeeE-----------------------------R  .   vmulss	712(%rsp), %xmm4, %xmm14
[0,146]   .    .    .    .    .    .    . DeE-----------------------------------------R  .   vmovdqa	%ymm8, %ymm4
[0,147]   .    .    .    .    .    .    . D===========eeeeeE--------------------------R  .   {vex}	vpdpbusd	%ymm10, %ymm13, %ymm4
[0,148]   .    .    .    .    .    .    . DeE-----------------------------------------R  .   vmovdqa	%ymm8, %ymm10
[0,149]   .    .    .    .    .    .    . D================eeeeE----------------------R  .   vcvtdq2ps	%ymm4, %ymm11
[0,150]   .    .    .    .    .    .    . DeeeeeeeeE----------------------------------R  .   vmovdqa	128(%rsp), %ymm4
[0,151]   .    .    .    .    .    .    .  D=eeeeeeeeeE-------------------------------R  .   vpand	.LC0(%rip), %ymm4, %ymm4
[0,152]   .    .    .    .    .    .    .  D=====eeeeeeeeeE---------------------------R  .   vpaddb	.LC1(%rip), %ymm4, %ymm4
[0,153]   .    .    .    .    .    .    .  D==============eE--------------------------R  .   vpsignb	%ymm9, %ymm4, %ymm9
[0,154]   .    .    .    .    .    .    .  D================eeeeeE--------------------R  .   {vex}	vpdpbusd	%ymm9, %ymm12, %ymm10
[0,155]   .    .    .    .    .    .    .   D============eeeE-------------------------R  .   vbroadcastss	%xmm15, %ymm9
[0,156]   .    .    .    .    .    .    .   D=eeeeeeeeeE------------------------------R  .   vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15
[0,157]   .    .    .    .    .    .    .   DeeeeeeeE---------------------------------R  .   vmovss	688(%rsp), %xmm12
[0,158]   .    .    .    .    .    .    .   D===============eeeeeeeeeeeeE-------------R  .   vfmadd213ps	544(%rsp), %ymm11, %ymm9
[0,159]   .    .    .    .    .    .    .    D============eeeE------------------------R  .   vbroadcastss	%xmm14, %ymm11
[0,160]   .    .    .    .    .    .    .    DeeeeeeeeeeeE----------------------------R  .   vmulss	704(%rsp), %xmm12, %xmm13
[0,161]   .    .    .    .    .    .    .    DeE--------------------------------------R  .   vmovdqa	%ymm8, %ymm12
[0,162]   .    .    .    .    .    .    .    D===================eeeeE----------------R  .   vcvtdq2ps	%ymm10, %ymm10
[0,163]   .    .    .    .    .    .    .    D==========================eeeeE---------R  .   vfmadd132ps	%ymm11, %ymm9, %ymm10
[0,164]   .    .    .    .    .    .    .    D=============eeeE-----------------------R  .   vbroadcastss	%xmm13, %ymm13
[0,165]   .    .    .    .    .    .    .    .D=============================eeeeeeeeeeeeER   vmovaps	%ymm10, 544(%rsp)
[0,166]   .    .    .    .    .    .    .    .D=============================eeeeeeeeeeeeER   vmovaps	%ymm10, 832(%rsp)
[0,167]   .    .    .    .    .    .    .    .DeeeeeeeE----------------------------------R   vmovss	692(%rsp), %xmm10
[0,168]   .    .    .    .    .    .    .    . D=eeeeeeeeeeeE----------------------------R   vmulss	708(%rsp), %xmm10, %xmm9
[0,169]   .    .    .    .    .    .    .    . DeeeeeeeeE--------------------------------R   vmovdqu	-64(%rdx,%rbx), %ymm10
[0,170]   .    .    .    .    .    .    .    . D========eE-------------------------------R   vpsignb	%ymm10, %ymm10, %ymm11
[0,171]   .    .    .    .    .    .    .    . D========eE-------------------------------R   vpsignb	%ymm10, %ymm3, %ymm10
[0,172]   .    .    .    .    .    .    .    . D==========eeeeeE-------------------------R   {vex}	vpdpbusd	%ymm10, %ymm11, %ymm12
[0,173]   .    .    .    .    .    .    .    .  DeeeeeeeeE-------------------------------R   vmovdqu	-32(%rdx,%rbx), %ymm10
[0,174]   .    .    .    .    .    .    .    .  D===========eeeE-------------------------R   vbroadcastss	%xmm9, %ymm9
[0,175]   .    .    .    .    .    .    .    .  D============eE--------------------------R   vpsignb	%ymm10, %ymm10, %ymm10
[0,176]   .    .    .    .    .    .    .    .  D==============eeeeE---------------------R   vcvtdq2ps	%ymm12, %ymm14
[0,177]   .    .    .    .    .    .    .    .  DeE--------------------------------------R   vmovdqa	%ymm8, %ymm12
[0,178]   .    .    .    .    .    .    .    .  D===============eeeeeeeeeeeeE------------R   vfmadd213ps	512(%rsp), %ymm14, %ymm9
[0,179]   .    .    .    .    .    .    .    .   DeE-------------------------------------R   vmovdqa	%ymm8, %ymm14
[0,180]   .    .    .    .    .    .    .    .   D============eeeeeE---------------------R   {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
[0,181]   .    .    .    .    .    .    .    .   DeeeeeeeE-------------------------------R   vmovss	704(%rsp), %xmm15
[0,182]   .    .    .    .    .    .    .    .   D=================eeeeE-----------------R   vcvtdq2ps	%ymm12, %ymm12
[0,183]   .    .    .    .    .    .    .    .   D==========================eeeeE--------R   vfmadd132ps	%ymm13, %ymm9, %ymm12
[0,184]   .    .    .    .    .    .    .    .   DeeeeeeeE-------------------------------R   vmovss	708(%rsp), %xmm13
[0,185]   .    .    .    .    .    .    .    .    D====eeeeeeeeeeeE----------------------R   vmulss	732(%rsp), %xmm13, %xmm9
Truncated display due to cycle limit


Average Wait times (based on the timeline view):
[0]: Executions
[1]: Average time spent waiting in a scheduler's queue
[2]: Average time spent waiting in a scheduler's queue while ready
[3]: Average time elapsed from WB until retire stage

      [0]    [1]    [2]    [3]
0.     10    1.0    1.0    64.4      movl	%edi, %eax
1.     10    1.0    1.0    59.7      movq	88(%rsp), %r15
2.     10    1.0    1.0    63.7      addq	$64, %rdx
3.     10    1.5    0.7    63.2      sarl	$5, %eax
4.     10    4.6    2.4    59.8      cltq
5.     10    6.6    0.0    57.9      addq	%rax, %r15
6.     10    5.2    0.0    53.2      vmovss	4(%rsi,%rax,4), %xmm4
7.     10    5.4    0.3    52.9      vmovss	(%rsi,%rax,4), %xmm15
8.     10    7.1    0.2    51.4      vmovss	(%rsi,%r15,4), %xmm13
9.     10    7.8    0.9    50.7      vmovss	4(%rsi,%r15,4), %xmm5
10.    10    1.0    1.0    59.2      movq	80(%rsp), %r15
11.    10    54.9   0.9    0.0       vmovss	%xmm5, 704(%rsp)
12.    10    54.6   0.0    0.0       vmovss	%xmm13, 708(%rsp)
13.    10    5.6    0.0    60.0      addq	%rax, %r15
14.    10    7.4    1.1    51.9      vmovss	(%rsi,%r15,4), %xmm6
15.    10    7.5    1.6    51.4      vmovss	4(%rsi,%r15,4), %xmm14
16.    10    1.2    1.2    59.7      movq	72(%rsp), %r15
17.    10    54.6   0.9    0.0       vmovss	%xmm6, 700(%rsp)
18.    10    54.3   0.0    0.0       vmovss	%xmm14, 696(%rsp)
19.    10    5.3    0.0    59.6      addq	%rax, %r15
20.    10    1.0    1.0    63.9      vmovdqa	%ymm8, %ymm14
21.    10    7.7    1.4    51.2      vmovss	(%rsi,%r15,4), %xmm7
22.    10    7.5    1.5    51.1      vmovss	4(%rsi,%r15,4), %xmm11
23.    10    29.0   26.9   35.6      leaq	(%r11,%rax), %r15
24.    10    29.7   0.0    28.6      vmovss	(%rcx,%r15,4), %xmm10
25.    10    29.5   0.0    28.6      vmovss	4(%rcx,%r15,4), %xmm12
26.    10    29.5   28.0   34.3      leaq	(%r9,%rax), %r15
27.    10    30.2   0.0    27.3      vmovss	(%rcx,%r15,4), %xmm2
28.    10    29.4   0.1    27.2      vmovss	4(%rcx,%r15,4), %xmm1
29.    10    29.7   29.2   32.9      leaq	(%r10,%rax), %r15
30.    10    1.5    1.0    61.1      addq	%r8, %rax
31.    10    5.3    2.9    51.2      vmovss	(%rcx,%rax,4), %xmm9
32.    10    5.7    3.9    50.2      vmovss	4(%rcx,%rax,4), %xmm5
33.    10    1.0    1.0    61.9      movl	%edi, %eax
34.    10    51.5   0.9    0.0       vmovss	%xmm7, 676(%rsp)
35.    10    29.8   0.2    26.7      vmovss	(%rcx,%r15,4), %xmm3
36.    10    1.4    0.8    61.1      sarl	%eax
37.    10    1.0    1.0    60.9      addl	$64, %edi
38.    10    29.2   0.2    26.7      vmovss	4(%rcx,%r15,4), %xmm0
39.    10    50.5   0.0    0.0       vmovss	%xmm11, 680(%rsp)
40.    10    2.4    1.0    59.1      cltq
41.    10    50.9   1.0    0.0       vmovss	%xmm2, 732(%rsp)
42.    10    50.5   0.0    0.0       vmovss	%xmm1, 728(%rsp)
43.    10    51.5   1.0    0.0       vmovss	%xmm3, 724(%rsp)
44.    10    4.1    2.3    50.8      vmovdqu	(%r14,%rax), %ymm3
45.    10    50.5   0.0    0.0       vmovss	%xmm0, 720(%rsp)
46.    10    51.5   1.0    0.0       vmovss	%xmm9, 716(%rsp)
47.    10    26.7   15.0   35.8      vpsrlw	$4, %ymm3, %ymm7
48.    10    50.5   0.0    0.0       vmovss	%xmm5, 712(%rsp)
49.    10    3.1    2.6    53.4      addq	120(%rsp), %rax
50.    10    2.8    2.8    54.7      movq	112(%rsp), %r15
51.    10    3.9    0.2    48.6      vpand	.LC0(%rip), %ymm3, %ymm3
52.    10    50.5   1.0    0.0       vmovss	%xmm10, 692(%rsp)
53.    10    12.1   5.2    41.4      vpaddb	.LC1(%rip), %ymm3, %ymm3
54.    10    7.1    0.0    46.4      vmovdqu	(%rax,%r15), %ymm2
55.    10    1.7    1.7    54.8      movq	104(%rsp), %r15
56.    10    49.5   0.0    0.0       vmovss	%xmm12, 688(%rsp)
57.    10    50.5   1.0    0.0       vmovss	%xmm15, 684(%rsp)
58.    10    22.9   0.2    29.6      vpand	.LC0(%rip), %ymm7, %ymm7
59.    10    26.1   0.2    26.4      vpaddb	.LC1(%rip), %ymm7, %ymm7
60.    10    6.4    0.2    47.1      vmovdqu	(%rax,%r15), %ymm1
61.    10    1.2    1.2    55.3      movq	96(%rsp), %r15
62.    10    24.3   11.2   35.2      vpsrlw	$4, %ymm2, %ymm6
63.    10    10.5   3.4    41.0      vpand	.LC0(%rip), %ymm2, %ymm2
64.    10    13.8   0.3    37.7      vpaddb	.LC1(%rip), %ymm2, %ymm2
65.    10    6.2    0.9    46.3      vmovdqu	(%rax,%r15), %ymm0
66.    10    24.2   0.9    26.4      vpand	.LC0(%rip), %ymm6, %ymm6
67.    10    24.3   11.9   34.3      vpsrlw	$4, %ymm1, %ymm5
68.    10    27.3   0.1    23.6      vpaddb	.LC1(%rip), %ymm6, %ymm6
69.    10    26.4   13.2   32.5      vpsrlw	$4, %ymm0, %ymm9
70.    10    47.2   0.0    0.0       vmovdqa	%ymm9, 128(%rsp)
71.    10    25.7   1.5    29.5      vmulss	%xmm15, %xmm10, %xmm9
72.    10    1.0    1.0    50.1      vmovdqu	-64(%rdx), %ymm10
73.    10    24.9   15.9   33.2      vpsignb	%ymm10, %ymm10, %ymm13
74.    10    29.6   0.0    26.5      vmovd	%xmm9, %r15d
75.    10    25.3   2.0    28.9      vmulss	%xmm12, %xmm4, %xmm9
76.    10    26.5   11.4   30.7      vpsignb	%ymm10, %ymm3, %ymm12
77.    10    29.8   0.5    25.4      vmovd	%xmm9, %eax
78.    10    1.0    1.0    56.1      vmovdqa	%ymm8, %ymm9
79.    10    27.4   0.0    25.7      {vex}	vpdpbusd	%ymm12, %ymm13, %ymm9
80.    10    32.4   0.0    21.7      vcvtdq2ps	%ymm9, %ymm15
81.    10    1.2    1.2    48.9      vmovdqu	-32(%rdx), %ymm9
82.    10    26.2   17.9   30.0      vpsignb	%ymm9, %ymm9, %ymm12
83.    10    30.2   0.1    26.0      vpsignb	%ymm9, %ymm7, %ymm11
84.    10    31.4   0.2    20.8      {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
85.    10    30.6   0.0    23.5      vmovd	%r15d, %xmm11
86.    10    33.6   0.0    20.5      vbroadcastss	%xmm11, %ymm11
87.    10    33.3   0.1    11.4      vfmadd213ps	640(%rsp), %ymm15, %ymm11
88.    10    30.8   0.0    22.9      vmovd	%eax, %xmm15
89.    10    33.8   0.0    19.9      vbroadcastss	%xmm15, %ymm15
90.    10    35.3   0.0    17.3      vcvtdq2ps	%ymm14, %ymm14
91.    10    45.2   0.0    7.8       vfmadd132ps	%ymm15, %ymm11, %ymm14
92.    10    1.2    1.2    47.9      vmovss	684(%rsp), %xmm15
93.    10    24.4   24.4   20.7      vmulss	728(%rsp), %xmm4, %xmm11
94.    10    48.2   0.0    0.0       vmovaps	%ymm14, 640(%rsp)
95.    10    47.3   0.0    0.0       vmovaps	%ymm14, 736(%rsp)
96.    10    34.6   0.2    21.7      vmovd	%xmm11, %eax
97.    10    30.7   0.4    27.6      vpsignb	%ymm9, %ymm6, %ymm11
98.    10    23.8   22.7   24.4      vmulss	732(%rsp), %xmm15, %xmm14
99.    10    24.7   9.9    32.6      vpsignb	%ymm10, %ymm2, %ymm15
100.   10    34.7   0.8    20.6      vmovd	%xmm14, %r15d
101.   10    1.0    1.0    56.3      vmovdqa	%ymm8, %ymm14
102.   10    26.1   0.4    27.2      {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
103.   10    31.7   0.6    22.6      vcvtdq2ps	%ymm14, %ymm15
104.   10    1.0    1.0    56.2      vmovdqa	%ymm8, %ymm14
105.   10    32.0   1.4    21.2      {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
106.   10    37.6   0.0    17.6      vmovd	%r15d, %xmm11
107.   10    39.7   0.0    14.6      vbroadcastss	%xmm11, %ymm11
108.   10    39.7   0.0    5.6       vfmadd213ps	608(%rsp), %ymm15, %ymm11
109.   10    35.6   0.0    18.7      vmovd	%eax, %xmm15
110.   10    38.5   0.0    15.7      vbroadcastss	%xmm15, %ymm15
111.   10    36.0   0.0    17.2      vcvtdq2ps	%ymm14, %ymm14
112.   10    50.7   0.0    1.6       vfmadd132ps	%ymm15, %ymm11, %ymm14
113.   10    1.3    1.3    48.0      vmovss	684(%rsp), %xmm15
114.   10    54.7   0.0    0.0       vmovaps	%ymm14, 608(%rsp)
115.   10    54.6   0.0    0.0       vmovaps	%ymm14, 768(%rsp)
116.   10    23.2   21.9   31.5      vmulss	724(%rsp), %xmm15, %xmm14
117.   10    34.7   0.5    28.0      vmovd	%xmm14, %r15d
118.   10    24.4   24.4   30.2      vmulss	720(%rsp), %xmm4, %xmm14
119.   10    1.8    1.8    53.9      vpand	.LC0(%rip), %ymm1, %ymm1
120.   10    4.9    0.1    50.8      vpaddb	.LC1(%rip), %ymm1, %ymm1
121.   10    16.0   2.8    39.6      vpand	.LC0(%rip), %ymm5, %ymm5
122.   10    18.3   0.2    36.4      vpaddb	.LC1(%rip), %ymm5, %ymm5
123.   10    2.4    2.4    52.3      vpand	.LC0(%rip), %ymm0, %ymm0
124.   10    26.0   13.2   36.6      vpsignb	%ymm10, %ymm1, %ymm15
125.   10    4.9    0.5    48.8      vpaddb	.LC1(%rip), %ymm0, %ymm0
126.   10    27.4   1.1    34.3      vpsignb	%ymm9, %ymm5, %ymm11
127.   10    33.2   0.7    26.5      vmovd	%xmm14, %eax
128.   10    1.0    1.0    60.7      vmovdqa	%ymm8, %ymm14
129.   10    27.6   13.7   34.1      vpsignb	%ymm10, %ymm0, %ymm10
130.   10    28.8   2.8    28.8      {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
131.   10    32.9   0.0    24.8      vcvtdq2ps	%ymm14, %ymm15
132.   10    1.0    1.0    59.7      vmovdqa	%ymm8, %ymm14
133.   10    29.0   1.6    27.7      {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
134.   10    34.2   0.5    24.5      vmovd	%r15d, %xmm11
135.   10    37.2   0.0    21.5      vbroadcastss	%xmm11, %ymm11
136.   10    37.3   0.0    12.3      vfmadd213ps	576(%rsp), %ymm15, %ymm11
137.   10    34.6   0.4    23.1      vmovd	%eax, %xmm15
138.   10    37.6   0.0    20.1      vbroadcastss	%xmm15, %ymm15
139.   10    33.0   0.0    23.7      vcvtdq2ps	%ymm14, %ymm14
140.   10    48.4   0.0    8.3       vfmadd132ps	%ymm15, %ymm11, %ymm14
141.   10    1.1    1.1    52.5      vmovss	684(%rsp), %xmm15
142.   10    25.4   24.2   23.3      vmulss	716(%rsp), %xmm15, %xmm15
143.   10    51.4   0.0    0.0       vmovaps	%ymm14, 576(%rsp)
144.   10    51.3   0.0    0.0       vmovaps	%ymm14, 800(%rsp)
145.   10    25.5   25.5   25.9      vmulss	712(%rsp), %xmm4, %xmm14
146.   10    1.0    1.0    60.4      vmovdqa	%ymm8, %ymm4
147.   10    28.0   3.4    29.4      {vex}	vpdpbusd	%ymm10, %ymm13, %ymm4
148.   10    1.0    1.0    60.4      vmovdqa	%ymm8, %ymm10
149.   10    33.0   0.0    25.4      vcvtdq2ps	%ymm4, %ymm11
150.   10    1.0    1.0    53.3      vmovdqa	128(%rsp), %ymm4
151.   10    3.0    0.9    49.4      vpand	.LC0(%rip), %ymm4, %ymm4
152.   10    7.3    1.3    45.1      vpaddb	.LC1(%rip), %ymm4, %ymm4
153.   10    28.4   12.1   32.0      vpsignb	%ymm9, %ymm4, %ymm9
154.   10    30.0   0.7    26.3      {vex}	vpdpbusd	%ymm9, %ymm12, %ymm10
155.   10    34.3   0.9    23.1      vbroadcastss	%xmm15, %ymm9
156.   10    26.9   17.7   24.5      vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15
157.   10    1.1    1.1    52.3      vmovss	688(%rsp), %xmm12
158.   10    34.6   0.2    13.7      vfmadd213ps	544(%rsp), %ymm11, %ymm9
159.   10    34.3   0.8    22.1      vbroadcastss	%xmm14, %ymm11
160.   10    28.0   26.9   20.4      vmulss	704(%rsp), %xmm12, %xmm13
161.   10    1.0    1.0    57.4      vmovdqa	%ymm8, %ymm12
162.   10    33.1   0.0    22.3      vcvtdq2ps	%ymm10, %ymm10
163.   10    45.6   0.0    9.7       vfmadd132ps	%ymm11, %ymm9, %ymm10
164.   10    39.1   0.2    17.2      vbroadcastss	%xmm13, %ymm13
165.   10    48.7   0.0    0.0       vmovaps	%ymm10, 544(%rsp)
166.   10    48.7   0.0    0.0       vmovaps	%ymm10, 832(%rsp)
167.   10    1.0    1.0    52.6      vmovss	692(%rsp), %xmm10
168.   10    28.1   27.0   20.6      vmulss	708(%rsp), %xmm10, %xmm9
169.   10    1.1    1.1    50.6      vmovdqu	-64(%rdx,%rbx), %ymm10
170.   10    28.2   19.1   30.5      vpsignb	%ymm10, %ymm10, %ymm11
171.   10    29.7   20.6   29.0      vpsignb	%ymm10, %ymm3, %ymm10
172.   10    30.7   0.1    23.9      {vex}	vpdpbusd	%ymm10, %ymm11, %ymm12
173.   10    1.8    1.8    48.9      vmovdqu	-32(%rdx,%rbx), %ymm10
174.   10    38.1   0.0    17.6      vbroadcastss	%xmm9, %ymm9
175.   10    30.2   20.4   27.5      vpsignb	%ymm10, %ymm10, %ymm10
176.   10    34.8   0.0    19.9      vcvtdq2ps	%ymm12, %ymm14
177.   10    1.0    1.0    56.7      vmovdqa	%ymm8, %ymm12
178.   10    38.4   0.1    8.2       vfmadd213ps	512(%rsp), %ymm14, %ymm9
179.   10    1.0    1.0    55.7      vmovdqa	%ymm8, %ymm14
180.   10    31.7   0.0    21.0      {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
181.   10    1.7    1.7    49.0      vmovss	704(%rsp), %xmm15
182.   10    36.7   0.0    17.0      vcvtdq2ps	%ymm12, %ymm12
183.   10    49.5   0.0    4.2       vfmadd132ps	%ymm13, %ymm9, %ymm12
184.   10    1.6    1.6    49.0      vmovss	708(%rsp), %xmm13
185.   10    28.9   27.2   16.8      vmulss	732(%rsp), %xmm13, %xmm9
186.   10    52.5   0.0    0.0       vmovaps	%ymm12, 512(%rsp)
187.   10    52.4   0.0    0.0       vmovaps	%ymm12, 864(%rsp)
188.   10    38.9   0.0    21.6      vbroadcastss	%xmm9, %ymm9
189.   10    29.1   28.4   23.4      vmulss	728(%rsp), %xmm15, %xmm13
190.   10    29.5   29.5   24.9      vpsignb	-64(%rdx,%rbx), %ymm2, %ymm12
191.   10    29.3   24.3   24.2      vpsignb	-32(%rdx,%rbx), %ymm6, %ymm15
192.   10    37.6   0.0    19.9      {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
193.   10    1.0    1.0    60.5      vmovdqa	%ymm8, %ymm12
194.   10    38.3   0.0    19.2      {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
195.   10    1.0    1.0    54.4      vmovss	704(%rsp), %xmm15
196.   10    39.3   0.3    20.1      vbroadcastss	%xmm13, %ymm13
197.   10    41.6   0.0    15.9      vcvtdq2ps	%ymm14, %ymm14
198.   10    41.7   0.1    7.8       vfmadd213ps	480(%rsp), %ymm14, %ymm9
199.   10    1.0    1.0    59.5      vmovdqa	%ymm8, %ymm14
200.   10    42.7   0.4    14.8      vcvtdq2ps	%ymm12, %ymm12
201.   10    53.6   0.0    3.9       vfmadd132ps	%ymm13, %ymm9, %ymm12
202.   10    1.0    1.0    53.5      vmovss	708(%rsp), %xmm13
203.   10    27.9   26.8   21.7      vmulss	724(%rsp), %xmm13, %xmm9
204.   10    28.6   28.5   21.0      vmulss	720(%rsp), %xmm15, %xmm13
205.   10    29.3   23.8   22.2      vpsignb	-32(%rdx,%rbx), %ymm5, %ymm15
206.   10    55.7   0.0    0.0       vmovaps	%ymm12, 480(%rsp)
207.   10    55.7   0.0    0.0       vmovaps	%ymm12, 896(%rsp)
208.   10    29.0   29.0   29.6      vpsignb	-64(%rdx,%rbx), %ymm1, %ymm12
209.   10    36.9   0.0    26.8      vbroadcastss	%xmm9, %ymm9
210.   10    37.2   0.1    24.5      {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
211.   10    1.0    1.0    64.7      vmovdqa	%ymm8, %ymm12
212.   10    37.9   0.3    25.8      vbroadcastss	%xmm13, %ymm13
213.   10    36.4   0.0    25.3      {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
214.   10    1.0    1.0    58.6      vmovss	704(%rsp), %xmm15
215.   10    42.1   0.0    20.5      vcvtdq2ps	%ymm14, %ymm14
216.   10    41.3   0.1    12.4      vfmadd213ps	448(%rsp), %ymm14, %ymm9
217.   10    40.7   0.3    21.0      vcvtdq2ps	%ymm12, %ymm12
218.   10    53.3   0.0    8.4       vfmadd132ps	%ymm13, %ymm9, %ymm12
219.   10    27.9   27.9   28.7      vpsignb	-64(%rdx,%rbx), %ymm0, %ymm9
220.   10    1.2    1.2    56.5      vmovss	708(%rsp), %xmm13
221.   10    27.8   25.6   25.9      vmulss	716(%rsp), %xmm13, %xmm14
222.   10    28.3   28.3   25.3      vmulss	712(%rsp), %xmm15, %xmm13
223.   10    1.0    1.0    62.6      vmovdqa	%ymm8, %ymm15
224.   10    36.3   0.4    23.3      {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
225.   10    28.2   28.2   26.5      vpsignb	-32(%rdx,%rbx), %ymm4, %ymm9
226.   10    55.2   0.0    0.0       vmovaps	%ymm12, 448(%rsp)
227.   10    55.2   0.0    0.0       vmovaps	%ymm12, 928(%rsp)
228.   10    39.7   0.3    22.6      vcvtdq2ps	%ymm15, %ymm12
229.   10    1.0    1.0    64.3      vmovdqa	%ymm8, %ymm15
230.   10    36.2   0.0    25.1      {vex}	vpdpbusd	%ymm9, %ymm10, %ymm15
231.   10    36.7   0.0    26.5      vbroadcastss	%xmm14, %ymm9
232.   10    37.7   0.4    25.5      vbroadcastss	%xmm13, %ymm10
233.   10    40.2   0.6    14.0      vfmadd213ps	416(%rsp), %ymm12, %ymm9
234.   10    1.0    1.0    57.3      vmovss	688(%rsp), %xmm12
235.   10    27.0   25.1   27.2      vmulss	696(%rsp), %xmm12, %xmm13
236.   10    1.0    1.0    63.2      vmovdqa	%ymm8, %ymm12
237.   10    40.1   0.0    21.1      vcvtdq2ps	%ymm15, %ymm11
238.   10    44.1   0.0    20.1      vmovaps	%ymm11, %ymm15
239.   10    51.2   0.0    10.0      vfmadd132ps	%ymm10, %ymm9, %ymm15
240.   10    38.1   0.1    24.1      vbroadcastss	%xmm13, %ymm13
241.   10    1.0    1.0    56.3      vmovss	692(%rsp), %xmm10
242.   10    26.6   24.7   26.6      vmulss	700(%rsp), %xmm10, %xmm9
243.   10    54.2   0.0    0.0       vmovaps	%ymm15, 416(%rsp)
244.   10    53.3   0.0    0.0       vmovaps	%ymm15, 960(%rsp)
245.   10    1.0    1.0    56.2      vmovdqu	-64(%rdx,%r13), %ymm15
246.   10    37.1   0.5    25.1      vbroadcastss	%xmm9, %ymm9
247.   10    27.0   18.0   37.2      vpsignb	%ymm15, %ymm15, %ymm11
248.   10    28.3   19.3   35.9      vpsignb	%ymm15, %ymm3, %ymm10
249.   10    1.0    1.0    62.3      vmovdqa	%ymm8, %ymm15
250.   10    28.7   0.3    30.6      {vex}	vpdpbusd	%ymm10, %ymm11, %ymm15
251.   10    34.5   0.8    25.8      vcvtdq2ps	%ymm15, %ymm14
252.   10    1.0    1.0    55.2      vmovdqu	-32(%rdx,%r13), %ymm15
253.   10    36.7   0.6    15.5      vfmadd213ps	384(%rsp), %ymm14, %ymm9
254.   10    29.3   20.3   33.9      vpsignb	%ymm15, %ymm15, %ymm10
255.   10    30.1   22.0   32.2      vpsignb	%ymm15, %ymm7, %ymm15
256.   10    35.1   4.0    23.2      {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
257.   10    40.0   0.0    19.2      vcvtdq2ps	%ymm12, %ymm12
258.   10    44.0   0.0    18.2      vmovaps	%ymm12, %ymm15
259.   10    47.7   0.0    11.5      vfmadd132ps	%ymm13, %ymm9, %ymm15
260.   10    52.3   0.4    0.0       vmovaps	%ymm15, 384(%rsp)
261.   10    51.4   0.0    0.0       vmovaps	%ymm15, 992(%rsp)
262.   10    34.7   34.7   19.6      vpsignb	-64(%rdx,%r13), %ymm2, %ymm12
263.   10    1.0    1.0    61.3      vmovdqa	%ymm8, %ymm15
264.   10    1.0    1.0    55.3      vmovss	700(%rsp), %xmm13
265.   10    43.7   0.0    14.6      {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
266.   10    1.0    1.0    60.4      vmovdqa	%ymm8, %ymm12
267.   10    1.0    1.0    54.4      vmovss	696(%rsp), %xmm14
268.   10    35.0   34.0   16.3      vmulss	732(%rsp), %xmm13, %xmm9
269.   10    36.1   34.2   15.2      vmulss	728(%rsp), %xmm14, %xmm13
270.   10    47.7   0.0    10.6      vcvtdq2ps	%ymm15, %ymm14
271.   10    35.4   35.4   16.9      vpsignb	-32(%rdx,%r13), %ymm6, %ymm15
272.   10    45.0   0.0    13.3      vbroadcastss	%xmm9, %ymm9
273.   10    44.7   0.3    11.6      {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
274.   10    46.3   0.2    12.0      vbroadcastss	%xmm13, %ymm13
275.   10    46.5   0.2    1.8       vfmadd213ps	352(%rsp), %ymm14, %ymm9
276.   10    1.0    1.0    52.2      vmovss	696(%rsp), %xmm14
277.   10    48.6   0.0    7.6       vcvtdq2ps	%ymm12, %ymm12
278.   10    52.6   0.0    6.6       vmovaps	%ymm12, %ymm15
279.   10    35.0   35.0   16.2      vpsignb	-64(%rdx,%r13), %ymm1, %ymm12
280.   10    57.5   0.0    0.0       vfmadd132ps	%ymm13, %ymm9, %ymm15
281.   10    1.0    1.0    53.4      vmovss	700(%rsp), %xmm13
282.   10    34.0   32.3   16.1      vmulss	724(%rsp), %xmm13, %xmm9
283.   10    34.7   34.0   15.4      vmulss	720(%rsp), %xmm14, %xmm13
284.   10    60.4   0.0    0.0       vmovaps	%ymm15, 352(%rsp)
285.   10    60.1   0.0    0.0       vmovaps	%ymm15, 1024(%rsp)
286.   10    1.0    1.0    70.1      vmovdqa	%ymm8, %ymm15
287.   10    44.0   0.0    25.1      vbroadcastss	%xmm9, %ymm9
288.   10    42.1   0.4    25.0      {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
289.   10    1.0    1.0    69.4      vmovdqa	%ymm8, %ymm12
290.   10    44.3   0.3    24.1      vbroadcastss	%xmm13, %ymm13
291.   10    46.4   0.0    21.0      vcvtdq2ps	%ymm15, %ymm14
292.   10    33.2   33.2   28.9      vpsignb	-32(%rdx,%r13), %ymm5, %ymm15
293.   10    46.1   0.3    12.7      vfmadd213ps	320(%rsp), %ymm14, %ymm9
294.   10    41.5   0.1    23.8      {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
295.   10    46.5   0.0    19.8      vcvtdq2ps	%ymm12, %ymm12
296.   10    50.3   0.0    18.8      vmovaps	%ymm12, %ymm15
297.   10    57.1   0.0    8.7       vfmadd132ps	%ymm13, %ymm9, %ymm15
298.   10    32.7   32.7   27.9      vpsignb	-64(%rdx,%r13), %ymm0, %ymm9
299.   10    1.0    1.0    61.6      vmovss	700(%rsp), %xmm13
300.   10    33.2   31.5   25.1      vmulss	716(%rsp), %xmm13, %xmm14
301.   10    59.9   0.0    0.0       vmovaps	%ymm15, 320(%rsp)
302.   10    59.9   0.0    0.0       vmovaps	%ymm15, 1056(%rsp)
303.   10    1.0    1.0    63.6      vmovss	696(%rsp), %xmm15
304.   10    32.1   30.8   27.8      vmulss	712(%rsp), %xmm15, %xmm13
305.   10    1.0    1.0    68.9      vmovdqa	%ymm8, %ymm15
306.   10    40.0   0.3    25.9      {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
307.   10    32.6   32.6   29.3      vpsignb	-32(%rdx,%r13), %ymm4, %ymm9
308.   10    1.0    1.0    68.6      vmovdqa	%ymm8, %ymm11
309.   10    41.8   0.5    23.8      {vex}	vpdpbusd	%ymm9, %ymm10, %ymm11
310.   10    41.5   0.0    25.4      vbroadcastss	%xmm14, %ymm9
311.   10    42.5   0.4    24.4      vbroadcastss	%xmm13, %ymm10
312.   10    44.0   0.0    21.9      vcvtdq2ps	%ymm15, %ymm12
313.   10    44.3   0.3    13.6      vfmadd213ps	288(%rsp), %ymm12, %ymm9
314.   10    1.0    1.0    61.6      vmovss	676(%rsp), %xmm15
315.   10    1.2    1.2    60.7      vmovss	688(%rsp), %xmm12
316.   10    45.1   0.0    19.8      vcvtdq2ps	%ymm11, %ymm11
317.   10    31.1   28.9   26.8      vmulss	680(%rsp), %xmm12, %xmm13
318.   10    55.3   0.0    9.6       vfmadd132ps	%ymm10, %ymm9, %ymm11
319.   10    1.0    1.0    60.6      vmovss	692(%rsp), %xmm10
320.   10    31.6   24.3   32.3      vmulss	%xmm15, %xmm10, %xmm14
321.   10    1.1    1.1    58.8      vmovdqu	-64(%rdx,%r12), %ymm10
322.   10    30.7   21.6   36.2      vpsignb	%ymm10, %ymm10, %ymm10
323.   10    58.3   0.0    0.0       vmovaps	%ymm11, 288(%rsp)
324.   10    57.3   0.0    0.0       vmovaps	%ymm11, 1088(%rsp)
325.   10    31.8   31.8   28.5      vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
326.   10    1.0    1.0    67.3      vmovdqa	%ymm8, %ymm3
327.   10    34.0   34.0   26.3      vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
328.   10    40.2   0.4    23.1      {vex}	vpdpbusd	%ymm9, %ymm10, %ymm3
329.   10    34.0   34.0   25.3      vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
330.   10    35.4   35.4   23.9      vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
331.   10    38.3   38.3   20.0      vpsignb	-32(%rdx,%r12), %ymm4, %ymm4
332.   10    39.1   39.1   17.2      vmulss	732(%rsp), %xmm15, %xmm12
333.   10    44.2   0.0    19.1      vcvtdq2ps	%ymm3, %ymm11
334.   10    1.0    1.0    58.3      vmovdqu	-32(%rdx,%r12), %ymm3
335.   10    38.7   30.7   26.6      vpsignb	%ymm3, %ymm3, %ymm9
336.   10    40.2   32.2   25.1      vpsignb	%ymm3, %ymm7, %ymm7
337.   10    1.0    1.0    64.3      vmovdqa	%ymm8, %ymm3
338.   10    41.4   0.2    19.9      {vex}	vpdpbusd	%ymm7, %ymm9, %ymm3
339.   10    46.4   0.0    15.9      vcvtdq2ps	%ymm3, %ymm7
340.   10    33.9   2.6    29.1      vbroadcastss	%xmm14, %ymm3
341.   10    42.5   0.3    10.8      vfmadd213ps	256(%rsp), %ymm11, %ymm3
342.   10    36.5   0.4    25.8      vbroadcastss	%xmm13, %ymm11
343.   10    1.0    1.0    57.3      vmovss	680(%rsp), %xmm13
344.   10    54.2   0.0    6.8       vfmadd132ps	%ymm11, %ymm3, %ymm7
345.   10    1.0    1.0    63.0      vmovdqa	%ymm8, %ymm3
346.   10    38.7   37.7   14.6      vmulss	728(%rsp), %xmm13, %xmm11
347.   10    57.5   0.0    0.0       vmovaps	%ymm7, 256(%rsp)
348.   10    57.2   0.0    0.0       vmovaps	%ymm7, 1120(%rsp)
349.   10    39.4   39.4   20.1      vpsignb	-64(%rdx,%r12), %ymm2, %ymm7
350.   10    46.1   0.0    19.4      vbroadcastss	%xmm12, %ymm2
351.   10    48.1   0.0    15.1      {vex}	vpdpbusd	%ymm7, %ymm10, %ymm3
352.   10    1.0    1.0    66.2      vmovdqa	%ymm8, %ymm7
353.   10    39.3   2.5    23.5      {vex}	vpdpbusd	%ymm6, %ymm9, %ymm7
354.   10    52.4   0.0    11.1      vcvtdq2ps	%ymm3, %ymm3
355.   10    51.7   0.0    3.1       vfmadd213ps	224(%rsp), %ymm2, %ymm3
356.   10    46.7   0.0    16.8      vbroadcastss	%xmm11, %ymm2
357.   10    42.6   0.0    19.5      vcvtdq2ps	%ymm7, %ymm6
358.   10    38.4   38.4   16.7      vmulss	724(%rsp), %xmm15, %xmm7
359.   10    62.1   0.0    0.4       vfmadd132ps	%ymm2, %ymm3, %ymm6
360.   10    37.8   37.8   19.6      vpsignb	-64(%rdx,%r12), %ymm1, %ymm3
361.   10    1.0    1.0    64.4      vmovdqa	%ymm8, %ymm2
362.   10    46.5   0.0    14.6      {vex}	vpdpbusd	%ymm3, %ymm10, %ymm2
363.   10    1.0    1.0    64.1      vmovdqa	%ymm8, %ymm3
364.   10    47.7   0.0    15.0      vbroadcastss	%xmm7, %ymm1
365.   10    37.9   4.0    22.8      {vex}	vpdpbusd	%ymm5, %ymm9, %ymm3
366.   10    65.0   0.0    0.0       vmovaps	%ymm6, 224(%rsp)
367.   10    64.7   0.0    0.0       vmovaps	%ymm6, 1152(%rsp)
368.   10    37.8   37.8   27.5      vmulss	720(%rsp), %xmm13, %xmm6
369.   10    49.8   0.0    22.2      vcvtdq2ps	%ymm2, %ymm2
370.   10    49.7   0.2    14.0      vfmadd213ps	192(%rsp), %ymm1, %ymm2
371.   10    40.9   0.0    30.4      vcvtdq2ps	%ymm3, %ymm3
372.   10    47.5   0.0    24.5      vbroadcastss	%xmm6, %ymm1
373.   10    60.6   0.0    10.0      vfmadd132ps	%ymm1, %ymm2, %ymm3
374.   10    1.0    1.0    72.3      vmovdqa	%ymm8, %ymm1
375.   10    36.6   4.6    32.4      {vex}	vpdpbusd	%ymm0, %ymm10, %ymm1
376.   10    1.0    1.0    72.0      vmovdqa	%ymm8, %ymm0
377.   10    37.5   37.5   25.4      vmulss	716(%rsp), %xmm15, %xmm2
378.   10    38.1   2.6    30.5      {vex}	vpdpbusd	%ymm4, %ymm9, %ymm0
379.   10    63.6   0.0    0.0       vmovaps	%ymm3, 192(%rsp)
380.   10    62.9   0.0    0.0       vmovaps	%ymm3, 1184(%rsp)
381.   10    38.1   38.1   25.5      vmulss	712(%rsp), %xmm13, %xmm3
382.   10    47.2   0.0    24.4      vbroadcastss	%xmm2, %ymm2
383.   10    40.2   0.0    30.4      vcvtdq2ps	%ymm1, %ymm1
384.   10    46.9   0.4    15.0      vfmadd213ps	160(%rsp), %ymm2, %ymm1
385.   10    41.1   0.0    28.5      vcvtdq2ps	%ymm0, %ymm0
386.   10    48.1   0.0    22.5      vbroadcastss	%xmm3, %ymm2
387.   10    58.3   0.0    11.0      vfmadd132ps	%ymm2, %ymm1, %ymm0
388.   10    61.9   0.0    0.0       vmovaps	%ymm0, 160(%rsp)
389.   10    61.6   0.0    0.0       vmovaps	%ymm0, 1216(%rsp)
390.   10    1.0    1.0    66.3      cmpl	%edi, 24(%rbp)
391.   10    6.6    0.0    65.3      jg	.L4
       10    29.3   5.5    29.0      <total>
