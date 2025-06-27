matmul_repack:
.LFB5533:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	leal	7(%r8), %r12d
	pushq	%rbx
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	leal	63(%r9), %esi
	andq	$-32, %rsp
	testl	%r9d, %r9d
	cmovns	%r9d, %esi
	movq	%rdx, -16(%rsp)
	movl	%ecx, -8(%rsp)
	sarl	$6, %esi
	testl	%r8d, %r8d
	cmovns	%r8d, %r12d
	sarl	$3, %r12d
	testl	%ecx, %ecx
	jle	.L139
	movl	%r8d, %r14d
	cmpl	$7, %r8d
	jle	.L139
	movslq	%esi, %r13
	movq	%rdi, %r10
	movl	%r9d, %r11d
	xorl	%r15d, %r15d
	leaq	0(%r13,%r13,8), %rax
	movl	$0, -4(%rsp)
	vpxor	%xmm7, %xmm7, %xmm7
	vmovdqa	.LC10(%rip), %ymm4
	salq	$3, %rax
	vmovdqa	.LC11(%rip), %ymm3
	movq	%rax, -24(%rsp)
.L131:
	movq	-16(%rsp), %rdi
	movslq	%r15d, %rax
	xorl	%r8d, %r8d
	leaq	(%rdi,%rax,4), %r9
	xorl	%edi, %edi
	.p2align 4,,10
	.p2align 3
.L134:
	cmpl	$63, %r11d
	jle	.L135
	leaq	(%rdi,%rdi,4), %rdx
	movq	%r10, %rax
	vxorps	%xmm0, %xmm0, %xmm0
	xorl	%ecx, %ecx
	salq	$6, %rdx
	addq	%rbx, %rdx
	.p2align 4,,10
	.p2align 3
.L133:
	vpand	64(%rdx), %ymm4, %ymm2
	vpbroadcastd	8(%rax), %ymm1
	addl	$1, %ecx
	addq	$72, %rax
	vmovdqu	64(%rdx), %ymm6
	vpbroadcastd	-60(%rax), %ymm15
	addq	$320, %rdx
	vpaddb	%ymm2, %ymm3, %ymm2
	vpsignb	%ymm1, %ymm1, %ymm14
	vpsignb	%ymm1, %ymm2, %ymm2
	vpsrlw	$4, %ymm6, %ymm5
	vmovdqa	%ymm7, %ymm1
	vmovdqu	-224(%rdx), %ymm6
	{vex} vpdpbusd	%ymm2, %ymm14, %ymm1
	vpand	%ymm5, %ymm4, %ymm5
	vpbroadcastd	-32(%rax), %ymm2
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsrlw	$4, %ymm6, %ymm13
	vmovdqu	-192(%rdx), %ymm6
	vpsignb	%ymm2, %ymm2, %ymm14
	vpsignb	%ymm2, %ymm5, %ymm5
	vmovdqa	%ymm7, %ymm2
	{vex} vpdpbusd	%ymm5, %ymm14, %ymm2
	vpsignb	%ymm15, %ymm15, %ymm14
	vpsrlw	$4, %ymm6, %ymm12
	vpand	%ymm13, %ymm4, %ymm13
	vpand	-224(%rdx), %ymm4, %ymm5
	vpaddb	%ymm13, %ymm3, %ymm13
	vpand	%ymm12, %ymm4, %ymm12
	vmovdqu	-160(%rdx), %ymm6
	vpaddb	%ymm12, %ymm3, %ymm12
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsrlw	$4, %ymm6, %ymm11
	vmovdqu	-128(%rdx), %ymm6
	vpsignb	%ymm15, %ymm5, %ymm5
	vpand	%ymm11, %ymm4, %ymm11
	{vex} vpdpbusd	%ymm5, %ymm14, %ymm1
	vpbroadcastd	-28(%rax), %ymm14
	vpaddb	%ymm11, %ymm3, %ymm11
	vpsrlw	$4, %ymm6, %ymm10
	vpand	%ymm10, %ymm4, %ymm10
	vmovdqu	-96(%rdx), %ymm6
	vpsignb	%ymm14, %ymm14, %ymm5
	vpsignb	%ymm14, %ymm13, %ymm13
	vpbroadcastd	-56(%rax), %ymm14
	vpaddb	%ymm10, %ymm3, %ymm10
	{vex} vpdpbusd	%ymm13, %ymm5, %ymm2
	vpand	-192(%rdx), %ymm4, %ymm5
	vpsrlw	$4, %ymm6, %ymm9
	vmovdqu	-64(%rdx), %ymm6
	vpsignb	%ymm14, %ymm14, %ymm13
	vpand	%ymm9, %ymm4, %ymm9
	vpaddb	%ymm5, %ymm3, %ymm5
	vpaddb	%ymm9, %ymm3, %ymm9
	vpsrlw	$4, %ymm6, %ymm8
	vmovdqu	-32(%rdx), %ymm6
	vpsignb	%ymm14, %ymm5, %ymm5
	vpand	%ymm8, %ymm4, %ymm8
	{vex} vpdpbusd	%ymm5, %ymm13, %ymm1
	vpbroadcastd	-24(%rax), %ymm13
	vpaddb	%ymm8, %ymm3, %ymm8
	vpsrlw	$4, %ymm6, %ymm6
	vpand	%ymm6, %ymm4, %ymm6
	vpsignb	%ymm13, %ymm13, %ymm5
	vpsignb	%ymm13, %ymm12, %ymm12
	vpbroadcastd	-52(%rax), %ymm13
	vpaddb	%ymm6, %ymm3, %ymm6
	{vex} vpdpbusd	%ymm12, %ymm5, %ymm2
	vpand	-160(%rdx), %ymm4, %ymm5
	vpsignb	%ymm13, %ymm13, %ymm12
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsignb	%ymm13, %ymm5, %ymm5
	{vex} vpdpbusd	%ymm5, %ymm12, %ymm1
	vpbroadcastd	-20(%rax), %ymm12
	vpsignb	%ymm12, %ymm12, %ymm5
	vpsignb	%ymm12, %ymm11, %ymm11
	{vex} vpdpbusd	%ymm11, %ymm5, %ymm2
	vpand	-128(%rdx), %ymm4, %ymm5
	vpbroadcastd	-48(%rax), %ymm11
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsignb	%ymm11, %ymm11, %ymm12
	vpsignb	%ymm11, %ymm5, %ymm5
	{vex} vpdpbusd	%ymm5, %ymm12, %ymm1
	vpbroadcastd	-16(%rax), %ymm5
	vpsignb	%ymm5, %ymm5, %ymm11
	vpsignb	%ymm5, %ymm10, %ymm10
	vpand	-96(%rdx), %ymm4, %ymm5
	{vex} vpdpbusd	%ymm10, %ymm11, %ymm2
	vpbroadcastd	-44(%rax), %ymm10
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsignb	%ymm10, %ymm5, %ymm5
	vpsignb	%ymm10, %ymm10, %ymm11
	{vex} vpdpbusd	%ymm5, %ymm11, %ymm1
	vpbroadcastd	-12(%rax), %ymm5
	vpsignb	%ymm5, %ymm5, %ymm10
	vpsignb	%ymm5, %ymm9, %ymm9
	vpand	-64(%rdx), %ymm4, %ymm5
	{vex} vpdpbusd	%ymm9, %ymm10, %ymm2
	vpbroadcastd	-40(%rax), %ymm9
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsignb	%ymm9, %ymm5, %ymm5
	vpsignb	%ymm9, %ymm9, %ymm10
	{vex} vpdpbusd	%ymm5, %ymm10, %ymm1
	vpbroadcastd	-8(%rax), %ymm5
	vpsignb	%ymm5, %ymm5, %ymm9
	vpsignb	%ymm5, %ymm8, %ymm8
	vpand	-32(%rdx), %ymm4, %ymm5
	{vex} vpdpbusd	%ymm8, %ymm9, %ymm2
	vpbroadcastd	-36(%rax), %ymm8
	vpaddb	%ymm5, %ymm3, %ymm5
	vpsignb	%ymm8, %ymm5, %ymm5
	vpsignb	%ymm8, %ymm8, %ymm9
	{vex} vpdpbusd	%ymm5, %ymm9, %ymm1
	vpbroadcastd	-4(%rax), %ymm5
	vpsignb	%ymm5, %ymm5, %ymm8
	vpsignb	%ymm5, %ymm6, %ymm6
	vbroadcastss	-72(%rax), %ymm5
	{vex} vpdpbusd	%ymm6, %ymm8, %ymm2
	vmulps	-320(%rdx), %ymm5, %ymm5
	vcvtdq2ps	%ymm1, %ymm1
	vfmadd231ps	%ymm5, %ymm1, %ymm0
	vcvtdq2ps	%ymm2, %ymm1
	vbroadcastss	-68(%rax), %ymm2
	vmulps	-288(%rdx), %ymm2, %ymm2
	vfmadd231ps	%ymm2, %ymm1, %ymm0
	cmpl	%ecx, %esi
	jg	.L133
.L132:
	addl	$1, %r8d
	vmovups	%ymm0, (%r9)
	addq	%r13, %rdi
	addq	$32, %r9
	cmpl	%r8d, %r12d
	jg	.L134
	addl	$1, -4(%rsp)
	addl	%r14d, %r15d
	movl	-4(%rsp), %eax
	addq	-24(%rsp), %r10
	cmpl	%eax, -8(%rsp)
	jne	.L131
	vzeroupper
.L139:
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L135:
	.cfi_restore_state
	vxorps	%xmm0, %xmm0, %xmm0
	jmp	.L132
	.cfi_endproc
.LFE5533:
	.size	matmul_repack, .-matmul_repack
	.p2align 4
	.globl	matmul_kernel
	.type	matmul_kernel, @function