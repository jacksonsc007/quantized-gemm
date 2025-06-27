	.file	"A81W41-ink_repack_scheme-use_fp16-imp1.cpp"
	.text
	.p2align 4
	.type	_Z13matmul_repackPvS_Pfiii._omp_fn.0, @function
_Z13matmul_repackPvS_Pfiii._omp_fn.0:
.LFB6669:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rax
	movl	$1, %ecx
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$1248, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movl	32(%rdi), %ebx
	movslq	36(%rdi), %rsi
	movq	%fs:40, %rdx
	movq	%rdx, 1240(%rsp)
	xorl	%edx, %edx
	movq	16(%rdi), %r15
	movq	(%rax), %r14
	leaq	1232(%rsp), %r9
	movl	%ebx, 188(%rsp)
	movl	28(%rdi), %ebx
	movl	$6, %edx
	leaq	1224(%rsp), %r8
	movq	8(%rdi), %rdi
	movl	%esi, 56(%rsp)
	movl	%ebx, 184(%rsp)
	movq	%rdi, 104(%rsp)
	movl	24(%rax), %edi
	movq	%r9, 8(%rsp)
	movl	%edi, 60(%rsp)
	xorl	%edi, %edi
	movq	%r8, 16(%rsp)
	call	GOMP_loop_nonmonotonic_dynamic_start@PLT
	testb	%al, %al
	je	.L2
	leal	(%rbx,%rbx,2), %eax
	vmovdqa	.LC0(%rip), %ymm8
	movq	%r14, %r12
	leaq	.L9(%rip), %r13
	addl	%eax, %eax
	movq	%r15, %r14
	cltq
	leaq	(%rax,%rax,8), %rax
	salq	$3, %rax
	movq	%rax, 24(%rsp)
	leal	-1(%rbx), %eax
	movq	%rax, 32(%rsp)
.L5:
	movq	1224(%rsp), %rdx
	movl	1232(%rsp), %eax
	movl	188(%rsp), %ecx
	movl	%edx, 176(%rsp)
	movl	%eax, 44(%rsp)
	testl	%ecx, %ecx
	jle	.L3
	movl	184(%rsp), %ebx
	movl	60(%rsp), %esi
	movl	%ebx, %eax
	imull	%edx, %esi
	imull	%edx, %eax
	cltq
	leaq	(%rax,%rax,8), %rcx
	addq	32(%rsp), %rax
	leaq	0(,%rcx,8), %rdi
	leal	1(%rdx), %ecx
	imull	%ebx, %ecx
	movq	%rdi, 168(%rsp)
	leaq	(%rax,%rax,8), %rax
	leaq	72(%r14,%rax,8), %r15
	movslq	%ebx, %rax
	movq	%rax, 120(%rsp)
	movl	%esi, %eax
	movslq	%ecx, %rcx
	leaq	(%rcx,%rcx,8), %rcx
	leaq	0(,%rcx,8), %rdi
	leal	2(%rdx), %ecx
	imull	%ebx, %ecx
	movq	%rdi, 160(%rsp)
	movslq	%ecx, %rcx
	leaq	(%rcx,%rcx,8), %rcx
	leaq	0(,%rcx,8), %rdi
	leal	3(%rdx), %ecx
	imull	%ebx, %ecx
	movq	%rdi, 152(%rsp)
	movslq	%ecx, %rcx
	leaq	(%rcx,%rcx,8), %rcx
	leaq	0(,%rcx,8), %rdi
	leal	4(%rdx), %ecx
	addl	$5, %edx
	imull	%ebx, %ecx
	movq	%rdi, 144(%rsp)
	imull	%ebx, %edx
	movslq	%ecx, %rcx
	movslq	%edx, %rdx
	leaq	(%rcx,%rcx,8), %rcx
	leaq	(%rdx,%rdx,8), %rdx
	leaq	0(,%rcx,8), %rdi
	leaq	0(,%rdx,8), %rcx
	movq	%rdi, 136(%rsp)
	movq	%rcx, 128(%rsp)
.L15:
	movl	$6, %edx
	movl	56(%rsp), %ebx
	subl	176(%rsp), %ebx
	cmpl	%edx, %ebx
	movl	%edx, %edi
	cmovle	%ebx, %edi
	movslq	%eax, %rbx
	xorl	%r10d, %r10d
	xorl	%r11d, %r11d
	movq	%rbx, 96(%rsp)
	movl	60(%rsp), %ebx
	movl	%edi, 180(%rsp)
	addl	%ebx, %eax
	movslq	%eax, %rsi
	addl	%ebx, %eax
	movq	%rsi, 88(%rsp)
	movslq	%eax, %rsi
	addl	%ebx, %eax
	movslq	%eax, %rcx
	addl	%ebx, %eax
	movq	%rsi, 80(%rsp)
	movslq	%eax, %rsi
	addl	%ebx, %eax
	movq	%rcx, 72(%rsp)
	xorl	%ebx, %ebx
	movl	%eax, 40(%rsp)
	cltq
	movq	%rax, 48(%rsp)
	movl	%edi, %eax
	movq	%rsi, 64(%rsp)
	movq	%rax, 112(%rsp)
	.p2align 4,,10
	.p2align 3
.L4:
	movl	184(%rsp), %eax
	testl	%eax, %eax
	jle	.L29
	movq	160(%rsp), %rdi
	movq	152(%rsp), %rsi
	vxorps	%xmm6, %xmm6, %xmm6
	leaq	(%r10,%r10,4), %r9
	movq	168(%rsp), %rax
	movq	136(%rsp), %rcx
	salq	$6, %r9
	vmovaps	%ymm6, 1024(%rsp)
	movq	128(%rsp), %rdx
	leaq	(%r14,%rdi), %r8
	leaq	(%r14,%rsi), %rdi
	movq	144(%rsp), %rsi
	addq	104(%rsp), %r9
	addq	%r14, %rax
	addq	%r14, %rcx
	vmovaps	%ymm6, 1056(%rsp)
	vmovaps	%ymm6, 1088(%rsp)
	addq	%r14, %rsi
	addq	%r14, %rdx
	vmovaps	%ymm6, 1120(%rsp)
	vmovaps	%ymm6, 1152(%rsp)
	vmovaps	%ymm6, 1184(%rsp)
	.p2align 4,,10
	.p2align 3
.L16:
	vmovdqu	64(%r9), %ymm6
	vpbroadcastd	8(%rcx), %ymm15
	addq	$72, %rax
	addq	$320, %r9
	addq	$72, %r8
	addq	$72, %rdi
	addq	$72, %rsi
	addq	$72, %rcx
	vpsrlw	$4, %ymm6, %ymm3
	vmovdqu	-64(%r9), %ymm0
	vmovdqu	-32(%r9), %ymm1
	addq	$72, %rdx
	vmovdqu	-224(%r9), %ymm6
	vpand	%ymm3, %ymm8, %ymm3
	vmovdqu	-192(%r9), %ymm7
	vpsrlw	$4, %ymm0, %ymm0
	vpsrlw	$4, %ymm1, %ymm1
	vmovdqa	%ymm0, 864(%rsp)
	vpsrlw	$4, %ymm6, %ymm2
	vpsrlw	$4, %ymm7, %ymm6
	vmovdqu	-160(%r9), %ymm7
	vpand	-256(%r9), %ymm8, %ymm0
	vpand	%ymm2, %ymm8, %ymm2
	vmovdqa	%ymm6, 992(%rsp)
	vpxor	%xmm6, %xmm6, %xmm6
	vmovdqa	%ymm1, 832(%rsp)
	vpsrlw	$4, %ymm7, %ymm4
	vmovdqu	-128(%r9), %ymm7
	vpxor	%xmm1, %xmm1, %xmm1
	vmovdqa	%ymm4, 960(%rsp)
	vpbroadcastd	-64(%rax), %ymm4
	vpsrlw	$4, %ymm7, %ymm5
	vmovdqu	-96(%r9), %ymm7
	vmovdqa	%ymm5, 928(%rsp)
	vpsrlw	$4, %ymm7, %ymm7
	vmovdqa	%ymm7, 896(%rsp)
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm0, %ymm7
	vpbroadcastd	-32(%rax), %ymm4
	vmovdqa	%ymm7, %ymm14
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm3, %ymm7
	vpbroadcastd	-64(%r8), %ymm4
	vmovdqa	%ymm7, %ymm13
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm0, %ymm7
	vpbroadcastd	-32(%r8), %ymm4
	vmovdqa	%ymm7, %ymm12
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm3, %ymm7
	vpbroadcastd	-64(%rdi), %ymm4
	vmovdqa	%ymm7, %ymm11
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm0, %ymm7
	vpbroadcastd	-32(%rdi), %ymm4
	vmovdqa	%ymm7, %ymm10
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm3, %ymm7
	vpbroadcastd	-64(%rsi), %ymm4
	{vex} vpdpbusd	%ymm4, %ymm0, %ymm6
	vpbroadcastd	-32(%rsi), %ymm4
	vmovdqa	%ymm7, %ymm9
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm4, %ymm3, %ymm7
	vpxor	%xmm4, %xmm4, %xmm4
	{vex} vpdpbusd	%ymm15, %ymm0, %ymm4
	vpbroadcastd	-32(%rcx), %ymm15
	vmovdqa	%ymm7, %ymm5
	vpxor	%xmm7, %xmm7, %xmm7
	{vex} vpdpbusd	%ymm15, %ymm3, %ymm7
	vpbroadcastd	-64(%rdx), %ymm15
	{vex} vpdpbusd	%ymm15, %ymm0, %ymm1
	vpbroadcastd	-32(%rdx), %ymm0
	vmovdqa	%ymm7, %ymm15
	vmovdqa	%ymm1, 800(%rsp)
	vpxor	%xmm1, %xmm1, %xmm1
	{vex} vpdpbusd	%ymm0, %ymm3, %ymm1
	vpbroadcastd	-60(%rax), %ymm3
	vpand	-224(%r9), %ymm8, %ymm0
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm14
	vpbroadcastd	-28(%rax), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm2, %ymm13
	vpbroadcastd	-60(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm12
	vpbroadcastd	-28(%r8), %ymm3
	vmovdqa	800(%rsp), %ymm7
	{vex} vpdpbusd	%ymm3, %ymm2, %ymm11
	vpbroadcastd	-60(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm10
	vpbroadcastd	-28(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm2, %ymm9
	vpbroadcastd	-60(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm6
	vpbroadcastd	-28(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm2, %ymm5
	vpbroadcastd	-60(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm4
	vpbroadcastd	-28(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm2, %ymm15
	vpbroadcastd	-60(%rdx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm7
	vpbroadcastd	-28(%rdx), %ymm0
	vmovdqa	%ymm1, %ymm3
	vpand	992(%rsp), %ymm8, %ymm1
	{vex} vpdpbusd	%ymm0, %ymm2, %ymm3
	vpbroadcastd	-56(%rax), %ymm2
	vpand	-192(%r9), %ymm8, %ymm0
	{vex} vpdpbusd	%ymm2, %ymm0, %ymm14
	vpbroadcastd	-24(%rax), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm1, %ymm13
	vpbroadcastd	-56(%r8), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm0, %ymm12
	vpbroadcastd	-24(%r8), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm1, %ymm11
	vpbroadcastd	-56(%rdi), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm0, %ymm10
	vpbroadcastd	-24(%rdi), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm1, %ymm9
	vpbroadcastd	-56(%rsi), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm0, %ymm6
	vpbroadcastd	-24(%rsi), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm1, %ymm5
	vpbroadcastd	-56(%rcx), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm0, %ymm4
	vpbroadcastd	-24(%rcx), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm1, %ymm15
	vpbroadcastd	-56(%rdx), %ymm2
	{vex} vpdpbusd	%ymm2, %ymm0, %ymm7
	vpbroadcastd	-24(%rdx), %ymm0
	vmovdqa	%ymm3, %ymm2
	vpbroadcastd	-20(%rax), %ymm3
	{vex} vpdpbusd	%ymm0, %ymm1, %ymm2
	vpbroadcastd	-52(%rax), %ymm1
	vpand	-160(%r9), %ymm8, %ymm0
	{vex} vpdpbusd	%ymm1, %ymm0, %ymm14
	vpand	960(%rsp), %ymm8, %ymm1
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm13
	vpbroadcastd	-52(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm12
	vpbroadcastd	-20(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm11
	vpbroadcastd	-52(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm10
	vpbroadcastd	-20(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm9
	vpbroadcastd	-52(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm6
	vpbroadcastd	-20(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm5
	vpbroadcastd	-52(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm4
	vpbroadcastd	-20(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm15
	vpbroadcastd	-52(%rdx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm7
	vpbroadcastd	-20(%rdx), %ymm0
	vpbroadcastd	-16(%rax), %ymm3
	{vex} vpdpbusd	%ymm0, %ymm1, %ymm2
	vpand	-128(%r9), %ymm8, %ymm0
	vpbroadcastd	-48(%rax), %ymm1
	{vex} vpdpbusd	%ymm1, %ymm0, %ymm14
	vpand	928(%rsp), %ymm8, %ymm1
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm13
	vpbroadcastd	-48(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm12
	vpbroadcastd	-16(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm11
	vpbroadcastd	-48(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm10
	vpbroadcastd	-16(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm9
	vpbroadcastd	-48(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm6
	vpbroadcastd	-16(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm5
	vpbroadcastd	-48(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm4
	vpbroadcastd	-16(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm15
	vpbroadcastd	-48(%rdx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm7
	vpbroadcastd	-16(%rdx), %ymm0
	vpbroadcastd	-12(%rax), %ymm3
	{vex} vpdpbusd	%ymm0, %ymm1, %ymm2
	vpand	-96(%r9), %ymm8, %ymm0
	vpbroadcastd	-44(%rax), %ymm1
	{vex} vpdpbusd	%ymm1, %ymm0, %ymm14
	vpand	896(%rsp), %ymm8, %ymm1
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm13
	vpbroadcastd	-44(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm12
	vpbroadcastd	-12(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm11
	vpbroadcastd	-44(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm10
	vpbroadcastd	-12(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm9
	vpbroadcastd	-44(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm6
	vpbroadcastd	-12(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm5
	vpbroadcastd	-44(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm4
	vpbroadcastd	-12(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm15
	vpbroadcastd	-44(%rdx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm7
	vpbroadcastd	-12(%rdx), %ymm0
	vpbroadcastd	-8(%rax), %ymm3
	{vex} vpdpbusd	%ymm0, %ymm1, %ymm2
	vpand	-64(%r9), %ymm8, %ymm0
	vpbroadcastd	-40(%rax), %ymm1
	{vex} vpdpbusd	%ymm1, %ymm0, %ymm14
	vpand	864(%rsp), %ymm8, %ymm1
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm13
	vpbroadcastd	-40(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm12
	vpbroadcastd	-8(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm11
	vpbroadcastd	-40(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm10
	vpbroadcastd	-8(%rdi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm9
	vpbroadcastd	-40(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm6
	vpbroadcastd	-8(%rsi), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm5
	vpbroadcastd	-40(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm4
	vpbroadcastd	-8(%rcx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm15
	vpbroadcastd	-40(%rdx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm7
	vpbroadcastd	-8(%rdx), %ymm0
	vpbroadcastd	-4(%rax), %ymm3
	{vex} vpdpbusd	%ymm0, %ymm1, %ymm2
	vpand	-32(%r9), %ymm8, %ymm0
	vpbroadcastd	-36(%rax), %ymm1
	{vex} vpdpbusd	%ymm1, %ymm0, %ymm14
	vpand	832(%rsp), %ymm8, %ymm1
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm13
	vpbroadcastd	-36(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm12
	vpbroadcastd	-4(%r8), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm11
	vpbroadcastd	-36(%rdi), %ymm3
	vmovdqa	%ymm14, 992(%rsp)
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm10
	vpbroadcastd	-4(%rdi), %ymm3
	vmovdqa	%ymm13, 960(%rsp)
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm9
	vpbroadcastd	-36(%rsi), %ymm3
	vmovdqa	%ymm12, 928(%rsp)
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm6
	vmovdqa	%ymm11, 896(%rsp)
	vmovdqa	%ymm10, 864(%rsp)
	vmovdqa	%ymm9, 832(%rsp)
	vmovdqa	%ymm6, 800(%rsp)
	vpbroadcastd	-4(%rsi), %ymm3
	vpbroadcastw	-70(%rax), %xmm11
	vpbroadcastw	-68(%rax), %xmm10
	vpbroadcastw	-66(%rax), %xmm14
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm5
	vpbroadcastd	-36(%rcx), %ymm3
	vpbroadcastw	-72(%r8), %xmm9
	vcvtph2ps	%xmm11, %ymm11
	vpbroadcastw	-70(%r8), %xmm6
	vcvtph2ps	%xmm10, %ymm10
	vcvtph2ps	%xmm14, %ymm14
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm4
	vpbroadcastd	-4(%rcx), %ymm3
	vcvtph2ps	%xmm9, %ymm9
	vcvtph2ps	%xmm6, %ymm6
	vmovdqa	%ymm5, 768(%rsp)
	vpbroadcastw	-68(%r8), %xmm5
	vmovdqa	%ymm4, 736(%rsp)
	vmovdqa	%ymm15, %ymm4
	vcvtph2ps	%xmm5, %ymm5
	{vex} vpdpbusd	%ymm3, %ymm1, %ymm4
	vpbroadcastd	-36(%rdx), %ymm3
	{vex} vpdpbusd	%ymm3, %ymm0, %ymm7
	vpbroadcastd	-4(%rdx), %ymm0
	vcvtph2ps	-304(%r9), %ymm3
	vmulps	%ymm3, %ymm11, %ymm11
	vmulps	%ymm3, %ymm6, %ymm6
	{vex} vpdpbusd	%ymm0, %ymm1, %ymm2
	vpbroadcastw	-72(%rax), %xmm1
	vcvtph2ps	-288(%r9), %ymm0
	vmovdqa	%ymm4, 704(%rsp)
	vcvtph2ps	%xmm1, %ymm1
	vcvtph2ps	-320(%r9), %ymm4
	vmulps	%ymm4, %ymm1, %ymm1
	vmulps	%ymm4, %ymm9, %ymm9
	vmovdqa	%ymm7, 672(%rsp)
	vmovaps	%ymm5, %ymm7
	vpbroadcastw	-66(%r8), %xmm5
	vmovdqa	%ymm2, 640(%rsp)
	vcvtph2ps	%xmm5, %ymm5
	vcvtph2ps	-272(%r9), %ymm2
	vmovaps	%ymm5, 608(%rsp)
	vpbroadcastw	-72(%rdi), %xmm5
	vcvtph2ps	%xmm5, %ymm12
	vpbroadcastw	-70(%rdi), %xmm5
	vmovaps	%ymm12, 576(%rsp)
	vcvtph2ps	%xmm5, %ymm13
	vpbroadcastw	-68(%rdi), %xmm5
	vmovaps	%ymm13, 544(%rsp)
	vcvtph2ps	%xmm5, %ymm5
	vmovaps	%ymm5, 512(%rsp)
	vpbroadcastw	-66(%rdi), %xmm5
	vcvtph2ps	%xmm5, %ymm12
	vpbroadcastw	-72(%rsi), %xmm5
	vmovaps	%ymm12, 480(%rsp)
	vcvtph2ps	%xmm5, %ymm13
	vmovaps	%ymm13, 448(%rsp)
	vpbroadcastw	-70(%rsi), %xmm5
	vcvtph2ps	%xmm5, %ymm15
	vpbroadcastw	-68(%rsi), %xmm5
	vmovaps	%ymm15, 416(%rsp)
	vcvtph2ps	%xmm5, %ymm5
	vmovaps	%ymm5, 384(%rsp)
	vpbroadcastw	-66(%rsi), %xmm5
	vcvtph2ps	%xmm5, %ymm12
	vpbroadcastw	-72(%rcx), %xmm5
	vmovaps	%ymm12, 352(%rsp)
	vcvtph2ps	%xmm5, %ymm13
	vpbroadcastw	-70(%rcx), %xmm5
	vmovaps	%ymm13, 320(%rsp)
	vcvtph2ps	%xmm5, %ymm5
	vmovaps	%ymm5, 288(%rsp)
	vpbroadcastw	-68(%rcx), %xmm5
	vcvtph2ps	%xmm5, %ymm12
	vpbroadcastw	-66(%rcx), %xmm5
	vmovaps	%ymm12, 256(%rsp)
	vpbroadcastw	-66(%rdx), %xmm12
	vcvtph2ps	%xmm5, %ymm13
	vpbroadcastw	-72(%rdx), %xmm5
	vcvtph2ps	%xmm12, %ymm12
	vmovaps	%ymm13, 224(%rsp)
	vpbroadcastw	-68(%rdx), %xmm13
	vcvtph2ps	%xmm5, %ymm15
	vpbroadcastw	-70(%rdx), %xmm5
	vmovaps	%ymm15, 192(%rsp)
	vcvtdq2ps	992(%rsp), %ymm15
	vfmadd213ps	1184(%rsp), %ymm15, %ymm1
	vcvtph2ps	%xmm13, %ymm13
	vcvtdq2ps	960(%rsp), %ymm15
	vcvtph2ps	%xmm5, %ymm5
	vmulps	%ymm3, %ymm5, %ymm5
	vfmadd132ps	%ymm11, %ymm1, %ymm15
	vcvtdq2ps	928(%rsp), %ymm1
	vfmadd213ps	1152(%rsp), %ymm1, %ymm9
	vcvtdq2ps	896(%rsp), %ymm1
	vfmadd132ps	%ymm0, %ymm15, %ymm10
	vfmadd132ps	%ymm6, %ymm9, %ymm1
	vcvtdq2ps	864(%rsp), %ymm6
	vfmadd132ps	%ymm2, %ymm10, %ymm14
	vfmadd231ps	%ymm7, %ymm0, %ymm1
	vmovaps	%ymm14, 1184(%rsp)
	vmovaps	%ymm1, %ymm14
	vfmadd231ps	608(%rsp), %ymm2, %ymm14
	vmulps	576(%rsp), %ymm4, %ymm1
	vfmadd213ps	1120(%rsp), %ymm1, %ymm6
	vcvtdq2ps	832(%rsp), %ymm1
	vmovaps	%ymm14, 1152(%rsp)
	vmulps	544(%rsp), %ymm3, %ymm9
	vfmadd132ps	%ymm9, %ymm6, %ymm1
	vcvtdq2ps	800(%rsp), %ymm6
	vfmadd231ps	512(%rsp), %ymm0, %ymm1
	vmulps	416(%rsp), %ymm3, %ymm9
	vmovaps	%ymm1, %ymm10
	vfmadd231ps	480(%rsp), %ymm2, %ymm10
	vmulps	448(%rsp), %ymm4, %ymm1
	vfmadd213ps	1088(%rsp), %ymm1, %ymm6
	vcvtdq2ps	768(%rsp), %ymm1
	vmovaps	%ymm10, 1120(%rsp)
	vfmadd132ps	%ymm9, %ymm6, %ymm1
	vfmadd231ps	384(%rsp), %ymm0, %ymm1
	vfmadd231ps	352(%rsp), %ymm2, %ymm1
	vmulps	320(%rsp), %ymm4, %ymm6
	vmulps	288(%rsp), %ymm3, %ymm9
	vmulps	192(%rsp), %ymm4, %ymm4
	vmovaps	%ymm1, 1088(%rsp)
	vcvtdq2ps	736(%rsp), %ymm1
	vfmadd213ps	1056(%rsp), %ymm1, %ymm6
	vcvtdq2ps	704(%rsp), %ymm1
	vfmadd132ps	%ymm9, %ymm6, %ymm1
	vfmadd231ps	256(%rsp), %ymm0, %ymm1
	vfmadd231ps	224(%rsp), %ymm2, %ymm1
	vmovaps	%ymm1, 1056(%rsp)
	vcvtdq2ps	672(%rsp), %ymm1
	vfmadd213ps	1024(%rsp), %ymm1, %ymm4
	vcvtdq2ps	640(%rsp), %ymm1
	vfmadd132ps	%ymm5, %ymm4, %ymm1
	vfmadd132ps	%ymm13, %ymm1, %ymm0
	vfmadd132ps	%ymm12, %ymm0, %ymm2
	vmovaps	%ymm2, 1024(%rsp)
	cmpq	%r15, %rax
	jne	.L16
.L17:
	cmpl	$6, 180(%rsp)
	ja	.L7
	movq	112(%rsp), %rax
	movslq	0(%r13,%rax,4), %rax
	addq	%r13, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L9:
	.long	.L7-.L9
	.long	.L14-.L9
	.long	.L13-.L9
	.long	.L12-.L9
	.long	.L11-.L9
	.long	.L10-.L9
	.long	.L8-.L9
	.text
.L8:
	movq	96(%rsp), %rdi
	movslq	%ebx, %rax
	movq	88(%rsp), %rsi
	vmovaps	1184(%rsp), %ymm6
	movq	80(%rsp), %rcx
	vmovaps	1024(%rsp), %ymm4
	leaq	(%rdi,%rax), %rdx
	movq	72(%rsp), %rdi
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rax,%rsi), %rdx
	movq	64(%rsp), %rsi
	vmovaps	1152(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rax,%rcx), %rdx
	vmovaps	1120(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rax,%rdi), %rdx
	vmovaps	1088(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rax,%rsi), %rdx
	addq	48(%rsp), %rax
	vmovaps	1056(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	vmovups	%ymm4, (%r12,%rax,4)
	.p2align 4,,10
	.p2align 3
.L7:
	addl	$1, %r11d
	addl	$8, %ebx
	addq	120(%rsp), %r10
	cmpl	%r11d, 188(%rsp)
	jne	.L4
	movq	24(%rsp), %rbx
	addl	$6, 176(%rsp)
	movl	40(%rsp), %eax
	addq	%rbx, 168(%rsp)
	addq	%rbx, 160(%rsp)
	addl	60(%rsp), %eax
	addq	%rbx, %r15
	addq	%rbx, 152(%rsp)
	movl	176(%rsp), %edi
	addq	%rbx, 144(%rsp)
	addq	%rbx, 136(%rsp)
	addq	%rbx, 128(%rsp)
	cmpl	%edi, 44(%rsp)
	jg	.L15
.L3:
	movq	8(%rsp), %rsi
	movq	16(%rsp), %rdi
	vzeroupper
	call	GOMP_loop_nonmonotonic_dynamic_next@PLT
	vmovdqa	.LC0(%rip), %ymm8
	testb	%al, %al
	jne	.L5
	vzeroupper
.L2:
	call	GOMP_loop_end_nowait@PLT
	movq	1240(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L30
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
.L11:
	.cfi_restore_state
	movq	96(%rsp), %rsi
	movslq	%ebx, %rax
	movq	88(%rsp), %rdi
	vmovaps	1184(%rsp), %ymm6
	leaq	(%rsi,%rax), %rdx
	movq	80(%rsp), %rsi
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rdi,%rax), %rdx
	vmovaps	1152(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rsi,%rax), %rdx
	addq	72(%rsp), %rax
	vmovaps	1120(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	vmovaps	1088(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rax,4)
	jmp	.L7
.L10:
	movq	96(%rsp), %rcx
	movslq	%ebx, %rax
	movq	88(%rsp), %rdi
	vmovaps	1184(%rsp), %ymm6
	movq	80(%rsp), %rsi
	leaq	(%rcx,%rax), %rdx
	movq	72(%rsp), %rcx
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rdi,%rax), %rdx
	vmovaps	1152(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rsi,%rax), %rdx
	vmovaps	1120(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	leaq	(%rcx,%rax), %rdx
	addq	64(%rsp), %rax
	vmovaps	1088(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rdx,4)
	vmovaps	1056(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rax,4)
	jmp	.L7
.L13:
	movq	96(%rsp), %rdi
	movslq	%ebx, %rax
	vmovaps	1184(%rsp), %ymm6
	leaq	(%rdi,%rax), %rdx
	addq	88(%rsp), %rax
	vmovups	%ymm6, (%r12,%rdx,4)
	vmovaps	1152(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rax,4)
	jmp	.L7
.L14:
	vmovaps	1184(%rsp), %ymm6
	movslq	%ebx, %rax
	addq	96(%rsp), %rax
	vmovups	%ymm6, (%r12,%rax,4)
	jmp	.L7
.L12:
	movq	96(%rsp), %rdi
	movslq	%ebx, %rax
	vmovaps	1184(%rsp), %ymm6
	leaq	(%rdi,%rax), %rdx
	movq	88(%rsp), %rdi
	vmovups	%ymm6, (%r12,%rdx,4)
	vmovaps	1152(%rsp), %ymm6
	leaq	(%rdi,%rax), %rdx
	addq	80(%rsp), %rax
	vmovups	%ymm6, (%r12,%rdx,4)
	vmovaps	1120(%rsp), %ymm6
	vmovups	%ymm6, (%r12,%rax,4)
	jmp	.L7
.L29:
	vxorps	%xmm6, %xmm6, %xmm6
	vmovaps	%ymm6, 1024(%rsp)
	vmovaps	%ymm6, 1056(%rsp)
	vmovaps	%ymm6, 1088(%rsp)
	vmovaps	%ymm6, 1120(%rsp)
	vmovaps	%ymm6, 1152(%rsp)
	vmovaps	%ymm6, 1184(%rsp)
	jmp	.L17
.L30:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6669:
	.size	_Z13matmul_repackPvS_Pfiii._omp_fn.0, .-_Z13matmul_repackPvS_Pfiii._omp_fn.0
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC1:
	.string	"\033[32m[INFO]\033[m number q blocks in A: %d\n"
	.text
	.p2align 4
	.globl	_Z18compute_scaled_sumPKaPKfPfii
	.type	_Z18compute_scaled_sumPKaPKfPfii, @function
_Z18compute_scaled_sumPKaPKfPfii:
.LFB6507:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	imull	%r8d, %ecx
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	.cfi_offset 15, -24
	movl	%ecx, %r15d
	pushq	%r14
	.cfi_offset 14, -32
	leal	31(%rcx), %r14d
	pushq	%r13
	.cfi_offset 13, -40
	movq	%rdx, %r13
	pushq	%r12
	.cfi_offset 12, -48
	movq	%rsi, %r12
	leaq	.LC1(%rip), %rsi
	pushq	%rbx
	.cfi_offset 3, -56
	movq	%rdi, %rbx
	movl	$1, %edi
	andq	$-32, %rsp
	testl	%ecx, %ecx
	cmovns	%ecx, %r14d
	xorl	%eax, %eax
	sarl	$5, %r14d
	movl	%r14d, %edx
	call	__printf_chk@PLT
	cmpl	$31, %r15d
	jle	.L35
	vxorps	%xmm2, %xmm2, %xmm2
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L33:
	movq	%rdx, %rax
	salq	$5, %rax
	vmovdqu	(%rbx,%rax), %ymm0
	vpmovsxbw	%xmm0, %ymm1
	vextracti128	$0x1, %ymm0, %xmm0
	vextracti128	$0x1, %ymm1, %xmm3
	vpmovsxbw	%xmm0, %ymm0
	vpmovsxwd	%xmm1, %ymm1
	vpmovsxwd	%xmm3, %ymm3
	vpaddd	%ymm1, %ymm3, %ymm3
	vpmovsxwd	%xmm0, %ymm1
	vextracti128	$0x1, %ymm0, %xmm0
	vpaddd	%ymm3, %ymm1, %ymm1
	vpmovsxwd	%xmm0, %ymm0
	vpaddd	%ymm1, %ymm0, %ymm0
	vextracti128	$0x1, %ymm0, %xmm1
	vpaddd	%xmm0, %xmm1, %xmm0
	vpsrldq	$8, %xmm0, %xmm1
	vpaddd	%xmm1, %xmm0, %xmm0
	vpsrldq	$4, %xmm0, %xmm1
	vpaddd	%xmm1, %xmm0, %xmm0
	vmovd	%xmm0, %eax
	vcvtsi2ssl	%eax, %xmm2, %xmm0
	vmulss	(%r12,%rdx,4), %xmm0, %xmm0
	vmovss	%xmm0, 0(%r13,%rdx,4)
	addq	$1, %rdx
	cmpl	%edx, %r14d
	jg	.L33
	vzeroupper
.L35:
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6507:
	.size	_Z18compute_scaled_sumPKaPKfPfii, .-_Z18compute_scaled_sumPKaPKfPfii
	.p2align 4
	.globl	_Z9init_randPfii
	.type	_Z9init_randPfii, @function
_Z9init_randPfii:
.LFB6508:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L43
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movslq	%esi, %rsi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	leaq	(%rdi,%rsi,4), %rbp
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L39:
	call	rand@PLT
	vxorps	%xmm1, %xmm1, %xmm1
	addq	$4, %rbx
	vcvtsi2ssl	%eax, %xmm1, %xmm0
	vmulss	.LC2(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rbx)
	cmpq	%rbp, %rbx
	jne	.L39
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L43:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE6508:
	.size	_Z9init_randPfii, .-_Z9init_randPfii
	.p2align 4
	.globl	_Z15init_rand_uint8Phii
	.type	_Z15init_rand_uint8Phii, @function
_Z15init_rand_uint8Phii:
.LFB6509:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L52
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movslq	%esi, %rsi
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	leaq	(%rdi,%rsi), %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L48:
	call	rand@PLT
	addq	$1, %rbp
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%rbp)
	cmpq	%rbx, %rbp
	jne	.L48
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L52:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE6509:
	.size	_Z15init_rand_uint8Phii, .-_Z15init_rand_uint8Phii
	.p2align 4
	.globl	_Z14init_rand_int8Paii
	.type	_Z14init_rand_int8Paii, @function
_Z14init_rand_int8Paii:
.LFB6510:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L61
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movslq	%esi, %rsi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	leaq	(%rdi,%rsi), %rbp
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L57:
	call	rand@PLT
	addq	$1, %rbx
	addl	$-128, %eax
	movb	%al, -1(%rbx)
	cmpq	%rbp, %rbx
	jne	.L57
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L61:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE6510:
	.size	_Z14init_rand_int8Paii, .-_Z14init_rand_int8Paii
	.p2align 4
	.globl	_Z17init_rand_min_maxPaiiii
	.type	_Z17init_rand_min_maxPaiiii, @function
_Z17init_rand_min_maxPaiiii:
.LFB6511:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L71
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	subl	%ecx, %r8d
	movslq	%esi, %rsi
	movl	%ecx, %r13d
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	leaq	(%rdi,%rsi), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	leal	1(%r8), %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	.p2align 4,,10
	.p2align 3
.L66:
	call	rand@PLT
	addq	$1, %rbx
	cltd
	idivl	%ebp
	addl	%r13d, %edx
	movb	%dl, -1(%rbx)
	cmpq	%rbx, %r12
	jne	.L66
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L71:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE6511:
	.size	_Z17init_rand_min_maxPaiiii, .-_Z17init_rand_min_maxPaiiii
	.p2align 4
	.globl	_Z16init_uint8_constPhhii
	.type	_Z16init_uint8_constPhhii, @function
_Z16init_uint8_constPhhii:
.LFB6512:
	.cfi_startproc
	endbr64
	testl	%edx, %edx
	jle	.L80
	testl	%ecx, %ecx
	jle	.L80
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movzbl	%sil, %r13d
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movl	%edx, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movslq	%ecx, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	xorl	%ebx, %ebx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	.p2align 4,,10
	.p2align 3
.L76:
	movq	%rbp, %rdx
	movl	%r13d, %esi
	addl	$1, %ebx
	call	memset@PLT
	movq	%rax, %rdi
	addq	%rbp, %rdi
	cmpl	%ebx, %r12d
	jne	.L76
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L80:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE6512:
	.size	_Z16init_uint8_constPhhii, .-_Z16init_uint8_constPhhii
	.p2align 4
	.globl	_Z15init_int8_constPaaii
	.type	_Z15init_int8_constPaaii, @function
_Z15init_int8_constPaaii:
.LFB6513:
	.cfi_startproc
	endbr64
	testl	%edx, %edx
	jle	.L89
	testl	%ecx, %ecx
	jle	.L89
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	movsbl	%sil, %r13d
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movl	%edx, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	movslq	%ecx, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	xorl	%ebx, %ebx
	subq	$8, %rsp
	.cfi_def_cfa_offset 48
	.p2align 4,,10
	.p2align 3
.L85:
	movq	%rbp, %rdx
	movl	%r13d, %esi
	addl	$1, %ebx
	call	memset@PLT
	movq	%rax, %rdi
	addq	%rbp, %rdi
	cmpl	%ebx, %r12d
	jne	.L85
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
.L89:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE6513:
	.size	_Z15init_int8_constPaaii, .-_Z15init_int8_constPaaii
	.p2align 4
	.globl	_Z10init_constPffii
	.type	_Z10init_constPffii, @function
_Z10init_constPffii:
.LFB6514:
	.cfi_startproc
	endbr64
	movl	%esi, %r9d
	testl	%esi, %esi
	jle	.L107
	movl	%edx, %esi
	testl	%edx, %edx
	jle	.L107
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	%edx, %eax
	movl	%edx, %r8d
	movl	%edx, %r11d
	shrl	$3, %eax
	andl	$-8, %r8d
	movq	%rdi, %rcx
	salq	$2, %r11
	leal	-1(%rax), %r10d
	xorl	%edi, %edi
	vbroadcastss	%xmm0, %ymm1
	vshufps	$0, %xmm0, %xmm0, %xmm2
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$1, %r10
	pushq	%r15
	pushq	%r14
	salq	$5, %r10
	pushq	%r13
	pushq	%r12
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	leal	-1(%rdx), %r12d
	pushq	%rbx
	.cfi_offset 3, -56
	movl	%r8d, %ebx
	salq	$2, %rbx
	.p2align 4,,10
	.p2align 3
.L94:
	movq	%rcx, %rax
	leaq	(%r10,%rcx), %rdx
	cmpl	$6, %r12d
	jbe	.L110
	.p2align 4,,10
	.p2align 3
.L95:
	vmovups	%ymm1, (%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L95
	leaq	(%rcx,%rbx), %r13
	cmpl	%r8d, %esi
	je	.L96
	movl	%r8d, %edx
	movl	%r8d, %eax
.L99:
	movl	%esi, %r14d
	subl	%edx, %r14d
	leal	-1(%r14), %r15d
	cmpl	$2, %r15d
	jbe	.L97
	vmovups	%xmm2, (%rcx,%rdx,4)
	movl	%r14d, %edx
	andl	$-4, %edx
	movl	%edx, %r15d
	addl	%edx, %eax
	leaq	0(%r13,%r15,4), %r13
	cmpl	%edx, %r14d
	je	.L96
.L97:
	leal	1(%rax), %edx
	vmovss	%xmm0, 0(%r13)
	cmpl	%esi, %edx
	jge	.L96
	addl	$2, %eax
	vmovss	%xmm0, 4(%r13)
	cmpl	%esi, %eax
	jge	.L96
	vmovss	%xmm0, 8(%r13)
.L96:
	addl	$1, %edi
	addq	%r11, %rcx
	cmpl	%edi, %r9d
	jne	.L94
	vzeroupper
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L110:
	.cfi_restore_state
	movq	%rcx, %r13
	xorl	%edx, %edx
	xorl	%eax, %eax
	jmp	.L99
.L107:
	.cfi_def_cfa 7, 8
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE6514:
	.size	_Z10init_constPffii, .-_Z10init_constPffii
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"acc error: % f average error: %f\n"
	.text
	.p2align 4
	.globl	_Z12compare_matsPfS_i
	.type	_Z12compare_matsPfS_i, @function
_Z12compare_matsPfS_i:
.LFB6515:
	.cfi_startproc
	endbr64
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	pushq	-8(%r10)
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x78,0x6
	subq	$40, %rsp
	testl	%edx, %edx
	jle	.L118
	leal	-1(%rdx), %eax
	movq	%rsi, %rcx
	cmpl	$6, %eax
	jbe	.L119
	movl	%edx, %esi
	vmovaps	.LC5(%rip), %ymm4
	xorl	%eax, %eax
	vxorps	%xmm1, %xmm1, %xmm1
	shrl	$3, %esi
	salq	$5, %rsi
	.p2align 4,,10
	.p2align 3
.L114:
	vmovups	(%rdi,%rax), %ymm5
	vsubps	(%rcx,%rax), %ymm5, %ymm0
	addq	$32, %rax
	vandps	%ymm4, %ymm0, %ymm0
	vaddss	%xmm1, %xmm0, %xmm3
	vshufps	$85, %xmm0, %xmm0, %xmm1
	vshufps	$255, %xmm0, %xmm0, %xmm2
	vaddss	%xmm3, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm3
	vextractf128	$0x1, %ymm0, %xmm0
	vaddss	%xmm3, %xmm1, %xmm1
	vaddss	%xmm2, %xmm1, %xmm1
	vshufps	$85, %xmm0, %xmm0, %xmm2
	vaddss	%xmm0, %xmm1, %xmm1
	vaddss	%xmm2, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm2
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm0, %xmm1, %xmm1
	cmpq	%rsi, %rax
	jne	.L114
	movl	%edx, %eax
	andl	$-8, %eax
	movl	%eax, %esi
	cmpl	%edx, %eax
	je	.L123
	vzeroupper
.L113:
	movl	%edx, %r8d
	subl	%eax, %r8d
	leal	-1(%r8), %r9d
	cmpl	$2, %r9d
	jbe	.L116
	vmovups	(%rdi,%rax,4), %xmm6
	vsubps	(%rcx,%rax,4), %xmm6, %xmm0
	movl	%r8d, %eax
	vandps	.LC6(%rip), %xmm0, %xmm0
	andl	$-4, %eax
	addl	%eax, %esi
	vaddss	%xmm0, %xmm1, %xmm1
	vshufps	$85, %xmm0, %xmm0, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm2
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm0, %xmm1, %xmm1
	cmpl	%eax, %r8d
	je	.L115
.L116:
	movslq	%esi, %r8
	vmovss	.LC7(%rip), %xmm2
	vmovss	(%rdi,%r8,4), %xmm0
	vsubss	(%rcx,%r8,4), %xmm0, %xmm0
	leaq	0(,%r8,4), %rax
	leal	1(%rsi), %r8d
	vandps	%xmm2, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	cmpl	%r8d, %edx
	jle	.L115
	vmovss	4(%rdi,%rax), %xmm0
	vsubss	4(%rcx,%rax), %xmm0, %xmm0
	addl	$2, %esi
	vandps	%xmm2, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	cmpl	%esi, %edx
	jle	.L115
	vmovss	8(%rdi,%rax), %xmm0
	vsubss	8(%rcx,%rax), %xmm0, %xmm0
	vandps	%xmm2, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
.L115:
	vcvtss2sd	%xmm1, %xmm1, %xmm2
.L112:
	vxorps	%xmm0, %xmm0, %xmm0
	movl	$1, %edi
	movl	$2, %eax
	vcvtsi2ssl	%edx, %xmm0, %xmm0
	leaq	.LC8(%rip), %rsi
	vdivss	%xmm0, %xmm1, %xmm1
	vmovsd	%xmm2, %xmm2, %xmm0
	vcvtss2sd	%xmm1, %xmm1, %xmm1
	vmovsd	%xmm1, -24(%rbp)
	call	__printf_chk@PLT
	vmovsd	-24(%rbp), %xmm1
	xorl	%eax, %eax
	movq	-8(%rbp), %r10
	.cfi_remember_state
	.cfi_def_cfa 10, 0
	vcomisd	.LC9(%rip), %xmm1
	leave
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	setbe	%al
	ret
	.p2align 4,,10
	.p2align 3
.L118:
	.cfi_restore_state
	vxorpd	%xmm2, %xmm2, %xmm2
	vxorps	%xmm1, %xmm1, %xmm1
	jmp	.L112
	.p2align 4,,10
	.p2align 3
.L123:
	vzeroupper
	jmp	.L115
.L119:
	xorl	%eax, %eax
	xorl	%esi, %esi
	vxorps	%xmm1, %xmm1, %xmm1
	jmp	.L113
	.cfi_endproc
.LFE6515:
	.size	_Z12compare_matsPfS_i, .-_Z12compare_matsPfS_i
	.p2align 4
	.globl	_Z5timerv
	.type	_Z5timerv, @function
_Z5timerv:
.LFB6516:
	.cfi_startproc
	endbr64
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	$4, %edi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rsi
	call	clock_gettime@PLT
	imulq	$1000000000, (%rsp), %rax
	addq	8(%rsp), %rax
	movq	24(%rsp), %rdx
	subq	%fs:40, %rdx
	jne	.L127
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L127:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6516:
	.size	_Z5timerv, .-_Z5timerv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC10:
	.string	"%.2f "
	.text
	.p2align 4
	.globl	_Z9print_matPfi
	.type	_Z9print_matPfi, @function
_Z9print_matPfi:
.LFB6517:
	.cfi_startproc
	endbr64
	testl	%esi, %esi
	jle	.L133
	leal	-1(%rsi), %eax
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	leaq	4(%rdi,%rax,4), %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	leaq	.LC10(%rip), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %rbx
	.p2align 4,,10
	.p2align 3
.L130:
	vxorpd	%xmm1, %xmm1, %xmm1
	movq	%rbp, %rsi
	movl	$1, %edi
	addq	$4, %rbx
	movl	$1, %eax
	vcvtss2sd	-4(%rbx), %xmm1, %xmm0
	call	__printf_chk@PLT
	cmpq	%r12, %rbx
	jne	.L130
	popq	%rbx
	.cfi_restore 3
	.cfi_def_cfa_offset 24
	movl	$10, %edi
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_restore 12
	.cfi_def_cfa_offset 8
	jmp	putchar@PLT
	.p2align 4,,10
	.p2align 3
.L133:
	movl	$10, %edi
	jmp	putchar@PLT
	.cfi_endproc
.LFE6517:
	.size	_Z9print_matPfi, .-_Z9print_matPfi
	.p2align 4
	.globl	_Z13matmul_repackPvS_Pfiii
	.type	_Z13matmul_repackPvS_Pfiii, @function
_Z13matmul_repackPvS_Pfiii:
.LFB6518:
	.cfi_startproc
	endbr64
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdx, %r10
	leal	63(%r9), %edx
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	xorl	%eax, %eax
	testl	%r9d, %r9d
	leal	7(%r8), %eax
	vmovd	%r8d, %xmm3
	cmovns	%r9d, %edx
	vmovq	%r10, %xmm4
	movq	%rdi, 16(%rsp)
	leaq	_Z13matmul_repackPvS_Pfiii._omp_fn.0(%rip), %rdi
	sarl	$6, %edx
	testl	%r8d, %r8d
	cmovns	%r8d, %eax
	vpinsrd	$1, %edx, %xmm3, %xmm0
	movl	$1, %edx
	sarl	$3, %eax
	vmovd	%eax, %xmm2
	vpinsrd	$1, %ecx, %xmm2, %xmm1
	xorl	%ecx, %ecx
	vpunpcklqdq	%xmm1, %xmm0, %xmm0
	vmovdqu	%xmm0, 24(%rsp)
	vpinsrq	$1, %rsi, %xmm4, %xmm0
	movq	%rsp, %rsi
	vmovdqa	%xmm0, (%rsp)
	call	GOMP_parallel@PLT
	movq	40(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L140
	addq	$56, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L140:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6518:
	.size	_Z13matmul_repackPvS_Pfiii, .-_Z13matmul_repackPvS_Pfiii
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	805306368
	.section	.rodata.cst32
	.align 32
.LC5:
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.set	.LC6,.LC5
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC7:
	.long	2147483647
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC9:
	.long	-755914244
	.long	1062232653
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
