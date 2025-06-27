.L4:
	movl	%eax, %edx
	vmovdqu	(%rdi,%rax), %ymm9
	sarl	$5, %edx
	movslq	%edx, %rdx
	vpsignb	%ymm9, %ymm9, %ymm8
	leaq	(%rdx,%r11), %rcx
	vmovss	(%rsi,%rdx,4), %xmm4
	vmovss	4(%rsi,%rdx,4), %xmm3
	movl	%eax, %edx
	vmulss	(%r8,%rcx,4), %xmm4, %xmm4
	sarl	%edx
	vmulss	4(%r8,%rcx,4), %xmm3, %xmm3
	movslq	%edx, %rdx
	vmovdqu	(%r10,%rdx), %ymm1
	vpsrlw	$4, %ymm1, %ymm2
	vpand	%ymm7, %ymm1, %ymm1
	vpaddb	%ymm1, %ymm6, %ymm1
	vbroadcastss	%xmm4, %ymm4
	vpand	%ymm2, %ymm7, %ymm2
	vpsignb	%ymm9, %ymm1, %ymm1
	vmovdqu	32(%rdi,%rax), %ymm9
	vpaddb	%ymm2, %ymm6, %ymm2
	vbroadcastss	%xmm3, %ymm3
	vpmaddubsw	%ymm1, %ymm8, %ymm1
	addq	$64, %rax
	vpmaddwd	%ymm1, %ymm5, %ymm1
	vpsignb	%ymm9, %ymm2, %ymm2
	vcvtdq2ps	%ymm1, %ymm8
	vfmadd231ps	%ymm4, %ymm8, %ymm0
	vpsignb	%ymm9, %ymm9, %ymm1
	vpmaddubsw	%ymm2, %ymm1, %ymm1
	vpmaddwd	%ymm1, %ymm5, %ymm1
	vcvtdq2ps	%ymm1, %ymm1
	vfmadd231ps	%ymm3, %ymm1, %ymm0
	cmpl	%eax, %r9d
	jg	.L4