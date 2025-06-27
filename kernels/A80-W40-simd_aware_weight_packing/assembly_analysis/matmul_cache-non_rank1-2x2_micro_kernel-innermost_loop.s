.L3:
	movl	%eax, %edx
	vmovdqu	(%rdi,%rax), %ymm7
	vmovdqa	%ymm12, %ymm10
	sarl	$5, %edx
	movslq	%edx, %rdx
	vmovss	(%rsi,%rdx,4), %xmm5
	vmovss	(%rcx,%rdx,4), %xmm14
	leaq	(%rdx,%r12), %r13
	leaq	0(,%rdx,4), %r15
	movl	%eax, %edx
	leaq	0(,%r13,4), %r14
	vmovss	(%rsi,%r13,4), %xmm13
	vmovss	(%rcx,%r13,4), %xmm11
	sarl	%edx
	vmovss	4(%rsi,%r14), %xmm3
	vmovss	%xmm14, -4(%rsp)
	vpsignb	%ymm7, %ymm7, %ymm14
	movslq	%edx, %rdx
	vmovss	4(%rcx,%r14), %xmm15
	vmovss	%xmm11, -20(%rsp)
	vmovdqa	%ymm12, %ymm11
	vmovdqu	(%r9,%rdx), %ymm1
	vmovss	%xmm3, -16(%rsp)
	vmovss	4(%rcx,%r15), %xmm2
	addq	%r9, %rdx
	vmovss	%xmm13, -12(%rsp)
	vmovss	4(%rsi,%r15), %xmm9
	vmovdqu	(%rdx,%rbx), %ymm0
	vpsrlw	$4, %ymm1, %ymm4
	vpand	.LC0(%rip), %ymm1, %ymm1
	vpaddb	.LC1(%rip), %ymm1, %ymm1
	vmovss	%xmm15, -24(%rsp)
	vpand	.LC0(%rip), %ymm4, %ymm4
	vpaddb	.LC1(%rip), %ymm4, %ymm4
	vmovss	%xmm2, -8(%rsp)
	vpsignb	%ymm7, %ymm1, %ymm3
	vpsrlw	$4, %ymm0, %ymm2
	vpand	.LC0(%rip), %ymm0, %ymm0
	{vex} vpdpbusd	%ymm3, %ymm14, %ymm10
	vmovdqu	32(%rdi,%rax), %ymm3
	vpaddb	.LC1(%rip), %ymm0, %ymm0
	vpand	.LC0(%rip), %ymm2, %ymm2
	vpaddb	.LC1(%rip), %ymm2, %ymm2
	vpsignb	%ymm3, %ymm3, %ymm13
	vpsignb	%ymm7, %ymm0, %ymm7
	vcvtdq2ps	%ymm10, %ymm15
	vpsignb	%ymm3, %ymm4, %ymm10
	vpsignb	%ymm3, %ymm2, %ymm3
	{vex} vpdpbusd	%ymm10, %ymm13, %ymm11
	vmulss	-4(%rsp), %xmm5, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	-64(%rsp), %ymm15, %ymm10
	vmulss	-8(%rsp), %xmm9, %xmm15
	vcvtdq2ps	%ymm11, %ymm11
	vbroadcastss	%xmm15, %ymm15
	vfmadd132ps	%ymm15, %ymm10, %ymm11
	vmovdqa	%ymm12, %ymm10
	{vex} vpdpbusd	%ymm7, %ymm14, %ymm10
	vmovaps	%ymm11, -64(%rsp)
	vmovss	-20(%rsp), %xmm11
	vmulss	%xmm11, %xmm5, %xmm5
	vcvtdq2ps	%ymm10, %ymm7
	vmovdqa	%ymm12, %ymm10
	{vex} vpdpbusd	%ymm3, %ymm13, %ymm10
	vbroadcastss	%xmm5, %ymm5
	vfmadd213ps	-96(%rsp), %ymm7, %ymm5
	vmovss	-24(%rsp), %xmm15
	vmovss	-12(%rsp), %xmm13
	vmovss	-4(%rsp), %xmm14
	vmulss	%xmm15, %xmm9, %xmm9
	vcvtdq2ps	%ymm10, %ymm3
	vmovaps	%ymm3, %ymm7
	vbroadcastss	%xmm9, %ymm9
	vfmadd132ps	%ymm9, %ymm5, %ymm7
	vmovdqa	%ymm12, %ymm5
	vmovaps	%ymm7, -96(%rsp)
	vmovdqu	(%r10,%rax), %ymm7
	vpsignb	%ymm7, %ymm7, %ymm9
	vpsignb	%ymm7, %ymm1, %ymm1
	vpsignb	%ymm7, %ymm0, %ymm0
	{vex} vpdpbusd	%ymm1, %ymm9, %ymm5
	vmovdqa	%ymm12, %ymm7
	{vex} vpdpbusd	%ymm0, %ymm9, %ymm7
	vcvtdq2ps	%ymm5, %ymm1
	vmovdqu	32(%r10,%rax), %ymm5
	addq	$64, %rax
	vcvtdq2ps	%ymm7, %ymm0
	vmulss	%xmm11, %xmm13, %xmm7
	vpsignb	%ymm5, %ymm4, %ymm3
	vpsignb	%ymm5, %ymm5, %ymm10
	vmovdqa	%ymm12, %ymm4
	{vex} vpdpbusd	%ymm3, %ymm10, %ymm4
	vmulss	%xmm13, %xmm14, %xmm3
	vpsignb	%ymm5, %ymm2, %ymm2
	vbroadcastss	%xmm7, %ymm7
	vfmadd132ps	%ymm0, %ymm6, %ymm7
	vbroadcastss	%xmm3, %ymm3
	vfmadd132ps	%ymm3, %ymm8, %ymm1
	vmovss	-16(%rsp), %xmm3
	vmovss	-8(%rsp), %xmm8
	vcvtdq2ps	%ymm4, %ymm4
	vmulss	%xmm3, %xmm8, %xmm8
	vmulss	%xmm15, %xmm3, %xmm6
	vbroadcastss	%xmm8, %ymm8
	vfmadd132ps	%ymm4, %ymm1, %ymm8
	vmovdqa	%ymm12, %ymm1
	vbroadcastss	%xmm6, %ymm6
	{vex} vpdpbusd	%ymm2, %ymm10, %ymm1
	vcvtdq2ps	%ymm1, %ymm1
	vfmadd132ps	%ymm1, %ymm7, %ymm6
	cmpl	%eax, %r11d
	jg	.L3