.L5:
	movl	%edx, %eax
	movl	%edx, %edi
	sarl	$5, %eax
	sarl	%edi
	cltq
	movslq	%edi, %rdi
	leaq	(%rbx,%rax), %rcx
	addq	%r11, %rax
	leaq	0(,%rcx,4), %rsi
	vmovss	(%r10,%rcx,4), %xmm2
	vmulss	0(%r13,%rax,4), %xmm2, %xmm2
	vmovss	4(%r10,%rsi), %xmm1
	vmulss	4(%r13,%rax,4), %xmm1, %xmm4
	vmovdqu	(%r9,%rdi), %ymm1
	vpand	%ymm7, %ymm1, %ymm3
	vpsrlw	$4, %ymm1, %ymm9
	vmovdqa	%ymm6, %ymm1
	{vex} vpdpbusd	(%r8,%rdx), %ymm3, %ymm1
	vpand	%ymm9, %ymm7, %ymm9
	vmovdqa	%ymm6, %ymm3
	vbroadcastss	%xmm2, %ymm2
	{vex} vpdpbusd	32(%r8,%rdx), %ymm9, %ymm3
	addq	$64, %rdx
	vcvtdq2ps	%ymm1, %ymm1
	vfmadd231ps	%ymm2, %ymm1, %ymm0
	vbroadcastss	%xmm4, %ymm1
	vmovss	(%r14,%rax,4), %xmm4
	vfmadd231ss	(%r15,%rcx,4), %xmm4, %xmm5
	vcvtdq2ps	%ymm3, %ymm3
	vmovss	4(%r14,%rax,4), %xmm4
	vfmadd231ss	4(%r15,%rsi), %xmm4, %xmm5
	vfmadd231ps	%ymm1, %ymm3, %ymm0
	cmpl	%edx, %r12d
	jg	.L5