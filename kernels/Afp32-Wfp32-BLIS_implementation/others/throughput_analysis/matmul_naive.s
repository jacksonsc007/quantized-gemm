	.file	"matmul_naive.c"
	.text
	.p2align 4
	.globl	matmul_naive
	.type	matmul_naive, @function
matmul_naive:
.LFB5512:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	movslq	%ecx, %r13
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	testl	%r13d, %r13d
	jle	.L10
	movl	%r8d, %r10d
	testl	%r8d, %r8d
	jle	.L10
	movl	%r9d, %eax
	testl	%r9d, %r9d
	jle	.L10
	movslq	%eax, %rbx
	subl	$1, %eax
	movq	$-4, %r11
	movq	%rdi, %r9
	salq	$2, %rax
	movq	%rdx, %rbp
	salq	$2, %rbx
	xorl	%r12d, %r12d
	leaq	0(,%r13,4), %rdi
	leaq	4(%rsi,%rax), %r14
	subq	%rax, %r11
	.p2align 4,,10
	.p2align 3
.L4:
	movq	%r14, %rsi
	movq	%rbp, %rcx
	xorl	%r8d, %r8d
	.p2align 4,,10
	.p2align 3
.L7:
	vmovss	(%rcx), %xmm0
	leaq	(%r11,%rsi), %rax
	movq	%r9, %rdx
	.p2align 4,,10
	.p2align 3
.L5:
	vmovss	(%rdx), %xmm1
	vfmadd231ss	(%rax), %xmm1, %xmm0
	addq	$4, %rax
	addq	%rdi, %rdx
	vmovss	%xmm0, (%rcx)
	cmpq	%rax, %rsi
	jne	.L5
	addl	$1, %r8d
	addq	%rdi, %rcx
	addq	%rbx, %rsi
	cmpl	%r8d, %r10d
	jne	.L7
	addq	$1, %r12
	addq	$4, %r9
	addq	$4, %rbp
	cmpq	%r13, %r12
	jne	.L4
.L10:
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE5512:
	.size	matmul_naive, .-matmul_naive
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
