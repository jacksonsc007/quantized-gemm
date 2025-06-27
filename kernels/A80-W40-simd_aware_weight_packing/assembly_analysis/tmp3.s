.L5:
	movl	%edx, %eax
	vmovdqu	(%rsi,%rdx), %ymm3
	sarl	$5, %eax
	cltq
	vpsignb	%ymm3, %ymm3, %ymm9
	leaq	(%rbx,%rax), %rcx
	addq	%r11, %rax
	vmovss	(%rdi,%rcx,4), %xmm8
	vmovss	4(%rdi,%rcx,4), %xmm4
	vmulss	(%r10,%rax,4), %xmm8, %xmm8
	vmulss	4(%r10,%rax,4), %xmm4, %xmm4
	movl	%edx, %eax
	sarl	%eax
	cltq
	vmovdqu	(%r9,%rax), %ymm0
	vbroadcastss	%xmm8, %ymm8
	vpsrlw	$4, %ymm0, %ymm2
	vpand	%ymm7, %ymm0, %ymm0
	vbroadcastss	%xmm4, %ymm4
	vpaddb	%ymm0, %ymm6, %ymm0
	vpsignb	%ymm3, %ymm0, %ymm0
	vmovdqa	%ymm5, %ymm3
	{vex} vpdpbusd	%ymm0, %ymm9, %ymm3
	vmovdqu	32(%rsi,%rdx), %ymm9
	vpand	%ymm2, %ymm7, %ymm0
	vmovdqa	%ymm5, %ymm2
	vpaddb	%ymm0, %ymm6, %ymm0
	addq	$64, %rdx
	vpsignb	%ymm9, %ymm0, %ymm0
	vpsignb	%ymm9, %ymm9, %ymm10
	{vex} vpdpbusd	%ymm0, %ymm10, %ymm2
	vcvtdq2ps	%ymm3, %ymm3
	vfmadd231ps	%ymm8, %ymm3, %ymm1
	vcvtdq2ps	%ymm2, %ymm0
	vfmadd231ps	%ymm4, %ymm0, %ymm1
	cmpl	%edx, %r8d
	jg	.L5