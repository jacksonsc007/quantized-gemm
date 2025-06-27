	.file	"no_cache-with_precomputation_of_sum.c"
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
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rsi, -40(%rsp)
	movl	24(%rbp), %esi
	movq	16(%rbp), %r14
	movq	%rdi, -64(%rsp)
	movl	32(%rbp), %eax
	movl	40(%rbp), %r12d
	movq	%rdx, -72(%rsp)
	testl	%esi, %esi
	jle	.L13
	movl	%r12d, %ebx
	sarl	$5, %ebx
	movl	%ebx, -20(%rsp)
	movl	%r12d, %ebx
	sarl	%ebx
	movl	%ebx, -24(%rsp)
	testl	%eax, %eax
	jle	.L13
	leaq	4(%rdx), %rbx
	subl	$1, %eax
	movl	$0, -48(%rsp)
	vmovdqa	.LC1(%rip), %ymm7
	movq	%rbx, -88(%rsp)
	movq	%rcx, %r10
	movq	%r8, %r15
	movq	%r9, %r13
	movl	$0, -52(%rsp)
	vxorps	%xmm8, %xmm8, %xmm8
	vpxor	%xmm6, %xmm6, %xmm6
	movl	$0, -56(%rsp)
	movl	$0, -44(%rsp)
	movq	%rax, -80(%rsp)
	.p2align 4,,10
	.p2align 3
.L4:
	movslq	-48(%rsp), %rdx
	movq	-72(%rsp), %rsi
	movl	$0, -12(%rsp)
	movslq	-52(%rsp), %rax
	movl	$0, -16(%rsp)
	leaq	(%rsi,%rdx,4), %rsi
	addq	-80(%rsp), %rdx
	movslq	-56(%rsp), %rbx
	movq	%rsi, -8(%rsp)
	movq	-88(%rsp), %rsi
	leaq	(%rsi,%rdx,4), %rsi
	movq	%rsi, -32(%rsp)
	movq	-64(%rsp), %rsi
	leaq	(%rsi,%rax), %r8
	.p2align 4,,10
	.p2align 3
.L9:
	vmovaps	%xmm8, %xmm5
	vxorps	%xmm0, %xmm0, %xmm0
	testl	%r12d, %r12d
	jle	.L8
	movslq	-12(%rsp), %rax
	movq	-40(%rsp), %rsi
	xorl	%edx, %edx
	vmovaps	%xmm8, %xmm5
	movslq	-16(%rsp), %r11
	vxorps	%xmm0, %xmm0, %xmm0
	leaq	(%rsi,%rax), %r9
	.p2align 4,,10
	.p2align 3
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
.L8:
	vextractf128	$0x1, %ymm0, %xmm1
	movq	-8(%rsp), %rax
	movl	-20(%rsp), %ecx
	vaddps	%xmm0, %xmm1, %xmm0
	addl	%ecx, -16(%rsp)
	movl	-24(%rsp), %ecx
	addq	$4, %rax
	addl	%ecx, -12(%rsp)
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	%xmm5, %xmm0, %xmm0
	vaddss	-4(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rax)
	movq	%rax, -8(%rsp)
	cmpq	%rax, -32(%rsp)
	jne	.L9
	addl	$1, -44(%rsp)
	movl	-20(%rsp), %esi
	addl	%r12d, -52(%rsp)
	movl	-44(%rsp), %eax
	addl	%esi, -56(%rsp)
	addl	$1000, -48(%rsp)
	cmpl	%eax, 24(%rbp)
	jne	.L4
	vzeroupper
.L13:
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
