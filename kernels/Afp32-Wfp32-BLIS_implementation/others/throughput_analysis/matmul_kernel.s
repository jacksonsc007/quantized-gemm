	.file	"matmul_kernel.c"
	.text
	.p2align 4
	.globl	kernel_16x6
	.type	kernel_16x6, @function
kernel_16x6:
.LFB5512:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rax
	movq	%rdx, %rdi
	movslq	%ecx, %rdx
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$448, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movslq	16(%rbp), %r14
	vmovups	(%rdi), %ymm13
	movq	%fs:40, %rcx
	movq	%rcx, 440(%rsp)
	xorl	%ecx, %ecx
	vmovups	32(%rdi), %ymm12
	leaq	0(,%r14,4), %rcx
	leal	(%r14,%r14), %r12d
	movq	%r14, %r13
	vmovaps	%ymm13, 32(%rsp)
	leaq	32(%rdi,%rcx), %rbx
	leaq	(%rdi,%rcx), %r10
	vmovaps	%ymm12, 64(%rsp)
	vmovups	(%rbx), %ymm11
	movq	%rbx, 24(%rsp)
	movslq	%r12d, %rbx
	leaq	(%r10,%rcx), %r11
	leaq	32(%rdi,%rbx,4), %rbx
	addl	%r14d, %r12d
	salq	$4, %r14
	leal	0(%r13,%r13,4), %r13d
	vmovups	(%rbx), %ymm3
	movq	%rbx, 16(%rsp)
	movslq	%r12d, %r12
	leaq	(%r11,%rcx), %rbx
	leaq	32(%rdi,%r12,4), %r15
	leaq	(%rbx,%rcx), %r12
	vmovups	(%rbx), %ymm4
	movslq	%r13d, %r13
	leaq	32(%rdi,%r14), %r14
	vmovups	(%r10), %ymm10
	vmovups	(%r11), %ymm2
	addq	%r12, %rcx
	vmovups	(%r15), %ymm5
	vmovups	(%r12), %ymm6
	leaq	32(%rdi,%r13,4), %r13
	movq	%r15, 8(%rsp)
	vmovups	(%r14), %ymm7
	vmovaps	%ymm10, 96(%rsp)
	vmovaps	%ymm11, 128(%rsp)
	vmovaps	%ymm2, 160(%rsp)
	vmovaps	%ymm3, 192(%rsp)
	vmovaps	%ymm4, 224(%rsp)
	vmovaps	%ymm5, 256(%rsp)
	vmovaps	%ymm6, 288(%rsp)
	vmovaps	%ymm7, 320(%rsp)
	vmovups	(%rcx), %ymm8
	vmovups	0(%r13), %ymm9
	vmovaps	%ymm8, 352(%rsp)
	vmovaps	%ymm9, 384(%rsp)
	testl	%r9d, %r9d
	jle	.L4
	movslq	%r8d, %r8
	salq	$2, %rdx
	xorl	%r15d, %r15d
	salq	$2, %r8
	.p2align 4,,10
	.p2align 3
.L3:
	vbroadcastss	(%rax), %ymm14
	vmovups	(%rsi), %ymm1
	addl	$1, %r15d
	vmovups	32(%rsi), %ymm0
	addq	%r8, %rsi
	vfmadd231ps	%ymm1, %ymm14, %ymm13
	vfmadd231ps	%ymm0, %ymm14, %ymm12
	vmovaps	%ymm13, 32(%rsp)
	vmovaps	%ymm12, 64(%rsp)
	vbroadcastss	4(%rax), %ymm14
	vfmadd231ps	%ymm1, %ymm14, %ymm10
	vfmadd231ps	%ymm0, %ymm14, %ymm11
	vmovaps	%ymm10, 96(%rsp)
	vmovaps	%ymm11, 128(%rsp)
	vbroadcastss	8(%rax), %ymm14
	vfmadd231ps	%ymm1, %ymm14, %ymm2
	vfmadd231ps	%ymm0, %ymm14, %ymm3
	vmovaps	%ymm2, 160(%rsp)
	vmovaps	%ymm3, 192(%rsp)
	vbroadcastss	12(%rax), %ymm14
	vfmadd231ps	%ymm1, %ymm14, %ymm4
	vfmadd231ps	%ymm0, %ymm14, %ymm5
	vmovaps	%ymm4, 224(%rsp)
	vmovaps	%ymm5, 256(%rsp)
	vbroadcastss	16(%rax), %ymm14
	vfmadd231ps	%ymm1, %ymm14, %ymm6
	vfmadd231ps	%ymm0, %ymm14, %ymm7
	vmovaps	%ymm6, 288(%rsp)
	vmovaps	%ymm7, 320(%rsp)
	vbroadcastss	20(%rax), %ymm14
	addq	%rdx, %rax
	vfmadd231ps	%ymm1, %ymm14, %ymm8
	vfmadd231ps	%ymm0, %ymm14, %ymm9
	vmovaps	%ymm8, 352(%rsp)
	vmovaps	%ymm9, 384(%rsp)
	cmpl	%r15d, %r9d
	jne	.L3
.L4:
	vmovaps	32(%rsp), %ymm2
	vmovaps	64(%rsp), %ymm3
	vmovaps	96(%rsp), %ymm4
	movq	24(%rsp), %rax
	vmovaps	128(%rsp), %ymm5
	vmovups	%ymm2, (%rdi)
	vmovaps	160(%rsp), %ymm6
	vmovaps	192(%rsp), %ymm7
	vmovups	%ymm3, 32(%rdi)
	vmovaps	224(%rsp), %ymm2
	vmovaps	256(%rsp), %ymm3
	vmovups	%ymm4, (%r10)
	vmovaps	288(%rsp), %ymm4
	vmovups	%ymm5, (%rax)
	movq	16(%rsp), %rax
	vmovaps	320(%rsp), %ymm5
	vmovups	%ymm6, (%r11)
	vmovaps	352(%rsp), %ymm6
	vmovups	%ymm7, (%rax)
	movq	8(%rsp), %rax
	vmovaps	384(%rsp), %ymm7
	vmovups	%ymm2, (%rbx)
	vmovups	%ymm3, (%rax)
	vmovups	%ymm4, (%r12)
	vmovups	%ymm5, (%r14)
	vmovups	%ymm6, (%rcx)
	vmovups	%ymm7, 0(%r13)
	movq	440(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L11
	vzeroupper
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
	vzeroupper
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5512:
	.size	kernel_16x6, .-kernel_16x6
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
