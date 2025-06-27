	.file	"no_cache-with_precomputation_of_sum-unroll_i_by_2.c"
	.text
	.p2align 4
	.globl	matmul_kernel
	.type	matmul_kernel, @function
matmul_kernel:
.LFB5518:
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
	pushq	%rbx
	andq	$-32, %rsp
	subq	$8, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rsi, -48(%rsp)
	movq	%rdx, %rsi
	movq	%rdx, -96(%rsp)
	movl	24(%rbp), %eax
	movq	16(%rbp), %rbx
	movq	%rdi, -88(%rsp)
	movl	32(%rbp), %edx
	subl	$1, %eax
	movl	%eax, -76(%rsp)
	testl	%eax, %eax
	jle	.L13
	movl	$5, %eax
	sarx	%eax, 40(%rbp), %eax
	movl	%eax, -20(%rsp)
	movl	$1, %eax
	sarx	%eax, 40(%rbp), %eax
	movl	%eax, -24(%rsp)
	testl	%edx, %edx
	jle	.L13
	movq	%rcx, %r15
	leaq	4(%rsi), %rcx
	movl	40(%rbp), %eax
	movl	$1000, -56(%rsp)
	movq	%rcx, -112(%rsp)
	leal	-1(%rdx), %ecx
	movq	%r8, %r13
	movq	%r9, %r14
	movl	$0, -60(%rsp)
	vmovdqa	.LC1(%rip), %ymm14
	vpxor	%xmm11, %xmm11, %xmm11
	movl	$0, -68(%rsp)
	movl	$0, -52(%rsp)
	movq	%rcx, -104(%rsp)
	movl	%eax, -64(%rsp)
	addl	%eax, %eax
	movl	%eax, -80(%rsp)
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L4:
	movslq	%eax, %r12
	addl	-20(%rsp), %eax
	movq	-96(%rsp), %rdi
	movl	$0, -12(%rsp)
	movslq	-56(%rsp), %rdx
	movq	-112(%rsp), %r8
	movl	%eax, -72(%rsp)
	cltq
	movslq	-68(%rsp), %rsi
	movq	%rax, (%rsp)
	movslq	-60(%rsp), %rax
	movslq	-64(%rsp), %rcx
	movl	$0, -16(%rsp)
	leaq	(%rdi,%rax,4), %rdi
	subq	%rax, %rdx
	movq	%rdi, -8(%rsp)
	movq	-104(%rsp), %rdi
	movq	%rdx, -40(%rsp)
	addq	%rax, %rdi
	leaq	(%r8,%rdi,4), %rdi
	movq	%rdi, -32(%rsp)
	movq	-88(%rsp), %rdi
	leaq	(%rdi,%rsi), %r11
	leaq	(%rdi,%rcx), %r10
	.p2align 4,,10
	.p2align 3
.L9:
	movl	40(%rbp), %eax
	testl	%eax, %eax
	jle	.L16
	movslq	-12(%rsp), %rdx
	movq	-48(%rsp), %rsi
	vxorps	%xmm7, %xmm7, %xmm7
	xorl	%eax, %eax
	vxorps	%xmm0, %xmm0, %xmm0
	movslq	-16(%rsp), %r9
	vmovaps	%xmm7, %xmm10
	vmovaps	%ymm0, %ymm3
	leaq	(%rsi,%rdx), %r8
	.p2align 4,,10
	.p2align 3
.L5:
	movl	%eax, %edi
	movq	(%rsp), %rdx
	vmovdqa	%ymm11, %ymm4
	vmovdqa	%ymm11, %ymm6
	sarl	$5, %edi
	vmovdqa	%ymm11, %ymm13
	movslq	%edi, %rdi
	leaq	(%r12,%rdi), %rcx
	addq	%rdi, %rdx
	addq	%r9, %rdi
	vmovss	(%r14,%rdi,4), %xmm2
	vmovss	(%rbx,%rdi,4), %xmm5
	leaq	0(,%rdi,4), %rsi
	movl	%eax, %edi
	vmulss	(%r15,%rcx,4), %xmm2, %xmm15
	sarl	%edi
	vmovss	4(%r14,%rsi), %xmm1
	movslq	%edi, %rdi
	movl	4(%rbx,%rsi), %esi
	vfmadd231ss	0(%r13,%rcx,4), %xmm5, %xmm10
	vmovdqu	(%r8,%rdi), %ymm8
	vmulss	(%r15,%rdx,4), %xmm2, %xmm2
	vmulss	4(%r15,%rcx,4), %xmm1, %xmm12
	vfmadd231ss	0(%r13,%rdx,4), %xmm5, %xmm7
	vmulss	4(%r15,%rdx,4), %xmm1, %xmm1
	vpsrlw	$4, %ymm8, %ymm9
	vpand	%ymm14, %ymm8, %ymm8
	{vex} vpdpbusd	(%r11,%rax), %ymm8, %ymm4
	vpand	%ymm9, %ymm14, %ymm9
	vbroadcastss	%xmm15, %ymm15
	{vex} vpdpbusd	(%r10,%rax), %ymm8, %ymm6
	vmovdqa	%ymm11, %ymm8
	{vex} vpdpbusd	32(%r11,%rax), %ymm9, %ymm13
	vbroadcastss	%xmm2, %ymm2
	{vex} vpdpbusd	32(%r10,%rax), %ymm9, %ymm8
	vbroadcastss	%xmm12, %ymm12
	addq	$64, %rax
	vbroadcastss	%xmm1, %ymm1
	vcvtdq2ps	%ymm4, %ymm4
	vfmadd231ps	%ymm15, %ymm4, %ymm3
	vmovd	%esi, %xmm4
	vcvtdq2ps	%ymm6, %ymm6
	vfmadd231ps	%ymm2, %ymm6, %ymm0
	vfmadd231ss	4(%r13,%rcx,4), %xmm4, %xmm10
	vcvtdq2ps	%ymm13, %ymm13
	vfmadd231ss	4(%r13,%rdx,4), %xmm4, %xmm7
	vcvtdq2ps	%ymm8, %ymm8
	vfmadd231ps	%ymm12, %ymm13, %ymm3
	vfmadd231ps	%ymm1, %ymm8, %ymm0
	cmpl	%eax, 40(%rbp)
	jg	.L5
.L8:
	vextractf128	$0x1, %ymm3, %xmm1
	movq	-8(%rsp), %rax
	movq	-40(%rsp), %rcx
	vaddps	%xmm3, %xmm1, %xmm1
	movl	-20(%rsp), %esi
	addl	%esi, -16(%rsp)
	vmovhlps	%xmm1, %xmm1, %xmm2
	vaddps	%xmm2, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm10, %xmm1, %xmm1
	vaddss	(%rax), %xmm1, %xmm1
	vmovss	%xmm1, (%rax)
	vextractf128	$0x1, %ymm0, %xmm1
	vaddps	%xmm0, %xmm1, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	%xmm7, %xmm0, %xmm0
	vaddss	(%rax,%rcx,4), %xmm0, %xmm0
	vmovss	%xmm0, (%rax,%rcx,4)
	addq	$4, %rax
	movl	-24(%rsp), %ecx
	movq	%rax, -8(%rsp)
	addl	%ecx, -12(%rsp)
	cmpq	%rax, -32(%rsp)
	jne	.L9
	movl	-80(%rsp), %edx
	addl	$2, -52(%rsp)
	movl	-72(%rsp), %eax
	addl	%edx, -68(%rsp)
	addl	%edx, -64(%rsp)
	movl	-52(%rsp), %ecx
	addl	$2000, -60(%rsp)
	addl	-20(%rsp), %eax
	addl	$2000, -56(%rsp)
	cmpl	%ecx, -76(%rsp)
	jg	.L4
	vzeroupper
.L13:
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
	.p2align 4,,10
	.p2align 3
.L16:
	.cfi_restore_state
	vxorps	%xmm7, %xmm7, %xmm7
	vxorps	%xmm0, %xmm0, %xmm0
	vmovaps	%xmm7, %xmm10
	vmovaps	%ymm0, %ymm3
	jmp	.L8
	.cfi_endproc
.LFE5518:
	.size	matmul_kernel, .-matmul_kernel
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC1:
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
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
