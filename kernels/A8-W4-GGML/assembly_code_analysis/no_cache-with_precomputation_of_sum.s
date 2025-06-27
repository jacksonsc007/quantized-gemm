.L5:
	movl	%edx, %eax
	movl	%edx, %esi
	sarl	$5, %eax
	sarl	%esi
	cltq
	movslq	%esi, %rsi
	leaq	(%r10,%rax), %rcx
	addq	%r8, %rax
	vmovss	(%r11,%rcx,4), %xmm0
	vmulss	(%rbx,%rax,4), %xmm0, %xmm1
	vmovdqu	(%rdi,%rsi), %xmm0
	vpsrlw	$4, %xmm0, %xmm2
	vinserti128	$0x1, %xmm2, %ymm0, %ymm0
	vmovdqa	%ymm5, %ymm2
	vpand	%ymm0, %ymm6, %ymm0
	{vex} vpdpbusd	(%r9,%rdx), %ymm0, %ymm2
	vbroadcastss	%xmm1, %ymm0
	addq	$32, %rdx
	vmovss	0(%r13,%rax,4), %xmm1
	vfmadd231ss	(%r15,%rcx,4), %xmm1, %xmm4
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd231ps	%ymm0, %ymm2, %ymm3
	cmpl	%edx, %r14d
	jg	.L5