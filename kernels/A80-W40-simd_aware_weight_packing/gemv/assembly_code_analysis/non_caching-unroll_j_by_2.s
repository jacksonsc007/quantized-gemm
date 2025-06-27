.L4:
	movl	%eax, %r12d
	vmovdqu	(%rdi,%rax), %ymm12
	vmovdqu	32(%rdi,%rax), %ymm11
	sarl	$5, %r12d
	movslq	%r12d, %r12
	vpsignb	%ymm12, %ymm12, %ymm0
	vpsignb	%ymm11, %ymm11, %ymm5
	leaq	(%r12,%r9), %rdx
	leaq	(%r12,%rbx), %r15
	vmovss	(%rsi,%r12,4), %xmm3
	vmovss	4(%rsi,%r12,4), %xmm2
	vmulss	(%r8,%r15,4), %xmm3, %xmm15
	vmulss	4(%r8,%r15,4), %xmm2, %xmm7
	vmulss	(%r8,%rdx,4), %xmm3, %xmm3
	vmulss	4(%r8,%rdx,4), %xmm2, %xmm2
	movl	%eax, %edx
	addq	$64, %rax
	sarl	%edx
	movslq	%edx, %rdx
	vbroadcastss	%xmm15, %ymm15
	vmovdqu	(%r11,%rdx), %ymm10
	addq	%r10, %rdx
	vmovd	%xmm7, %r12d
	vmovdqu	(%rdx,%rcx), %ymm6
	vbroadcastss	%xmm3, %ymm3
	vpsrlw	$4, %ymm10, %ymm4
	vpand	%ymm14, %ymm10, %ymm10
	vbroadcastss	%xmm2, %ymm2
	vpsrlw	$4, %ymm6, %ymm7
	vpand	%ymm14, %ymm6, %ymm6
	vpaddb	%ymm10, %ymm13, %ymm10
	vpaddb	%ymm6, %ymm13, %ymm6
	vpsignb	%ymm12, %ymm10, %ymm10
	vpand	%ymm4, %ymm14, %ymm4
	vpsignb	%ymm12, %ymm6, %ymm6
	vpmaddubsw	%ymm10, %ymm0, %ymm10
	vpand	%ymm7, %ymm14, %ymm7
	vpmaddubsw	%ymm6, %ymm0, %ymm0
	vpmaddwd	%ymm10, %ymm8, %ymm10
	vpmaddwd	%ymm0, %ymm8, %ymm0
	vcvtdq2ps	%ymm10, %ymm10
	vfmadd231ps	%ymm15, %ymm10, %ymm9
	vpaddb	%ymm4, %ymm13, %ymm4
	vcvtdq2ps	%ymm0, %ymm0
	vfmadd231ps	%ymm3, %ymm0, %ymm1
	vpaddb	%ymm7, %ymm13, %ymm7
	vmovd	%r12d, %xmm15
	vpsignb	%ymm11, %ymm4, %ymm4
	vpsignb	%ymm11, %ymm7, %ymm7
	vbroadcastss	%xmm15, %ymm10
	vpmaddubsw	%ymm4, %ymm5, %ymm4
	vpmaddubsw	%ymm7, %ymm5, %ymm5
	vpmaddwd	%ymm4, %ymm8, %ymm4
	vpmaddwd	%ymm5, %ymm8, %ymm5
	vcvtdq2ps	%ymm4, %ymm4
	vcvtdq2ps	%ymm5, %ymm5
	vfmadd231ps	%ymm10, %ymm4, %ymm9
	vfmadd231ps	%ymm2, %ymm5, %ymm1
	cmpl	%eax, %r13d
	jg	.L4