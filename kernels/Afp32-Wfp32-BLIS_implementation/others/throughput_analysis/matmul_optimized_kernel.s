	.file	"matmul_optimized_kernel.c"
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
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	.cfi_offset 14, -24
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	.cfi_offset 3, -48
	movl	%ecx, %ebx
	andq	$-32, %rsp
	movl	24(%rbp), %r11d
	movslq	32(%rbp), %r12
	cmpl	$16, %r8d
	je	.L2
	movl	$16, %edx
	leaq	mask.0(%rip), %rcx
	subl	%r8d, %edx
	movslq	%edx, %rdx
	vpmovsxbd	(%rcx,%rdx), %ymm4
	movl	$24, %edx
	subl	%r8d, %edx
	movslq	%edx, %rdx
	vmovdqa	%ymm4, %ymm15
	vpmovsxbd	(%rcx,%rdx), %ymm3
	vmovdqa	%ymm3, -32(%rsp)
	cmpl	$6, %ebx
	ja	.L37
	leaq	.L5(%rip), %rcx
	movl	%ebx, %edx
	movslq	(%rcx,%rdx,4), %rdx
	addq	%rcx, %rdx
	notrack jmp	*%rdx
	.section	.rodata
	.align 4
	.align 4
.L5:
	.long	.L37-.L5
	.long	.L10-.L5
	.long	.L9-.L5
	.long	.L8-.L5
	.long	.L7-.L5
	.long	.L6-.L5
	.long	.L4-.L5
	.text
	.p2align 4,,10
	.p2align 3
.L2:
	cmpl	$6, %ebx
	ja	.L37
	leaq	.L12(%rip), %rcx
	movl	%ebx, %edx
	movslq	(%rcx,%rdx,4), %rdx
	addq	%rcx, %rdx
	notrack jmp	*%rdx
	.section	.rodata
	.align 4
	.align 4
.L12:
	.long	.L37-.L12
	.long	.L17-.L12
	.long	.L16-.L12
	.long	.L15-.L12
	.long	.L14-.L12
	.long	.L13-.L12
	.long	.L11-.L12
	.text
	.p2align 4,,10
	.p2align 3
.L4:
	movslq	%r12d, %r10
	leal	(%r12,%r12), %r13d
	vmaskmovps	(%rdi), %ymm4, %ymm6
	vmaskmovps	32(%rdi), %ymm3, %ymm5
	leaq	0(,%r10,4), %rcx
	movslq	%r13d, %r14
	addl	%r12d, %r13d
	salq	$4, %r10
	leaq	(%rdi,%rcx), %rdx
	movslq	%r13d, %r13
	vmaskmovps	32(%rdi,%rcx), %ymm3, %ymm7
	vmaskmovps	32(%rdi,%r14,4), %ymm3, %ymm9
	vmaskmovps	(%rdx), %ymm4, %ymm8
	addq	%rcx, %rdx
	vmaskmovps	32(%rdi,%r13,4), %ymm3, %ymm11
	vmaskmovps	32(%rdi,%r10), %ymm3, %ymm13
	vmaskmovps	(%rdx), %ymm4, %ymm10
	addq	%rcx, %rdx
	vmaskmovps	(%rdx), %ymm4, %ymm12
	addq	%rcx, %rdx
	vmaskmovps	(%rdx), %ymm4, %ymm14
	vmaskmovps	(%rdx,%rcx), %ymm4, %ymm4
	leal	(%r12,%r12,4), %edx
	movslq	%edx, %rdx
	vmaskmovps	32(%rdi,%rdx,4), %ymm3, %ymm3
	.p2align 4,,10
	.p2align 3
.L3:
	testl	%r11d, %r11d
	jle	.L18
	movslq	16(%rbp), %r10
	movslq	%r9d, %r9
	movq	%rsi, %rdx
	xorl	%ecx, %ecx
	salq	$2, %r9
	salq	$2, %r10
	.p2align 4,,10
	.p2align 3
.L19:
	vmovups	(%rdx), %ymm1
	vmovups	32(%rdx), %ymm0
	addl	$1, %ecx
	addq	%r10, %rdx
	vbroadcastss	(%rax), %ymm2
	vfmadd231ps	%ymm1, %ymm2, %ymm6
	vfmadd231ps	%ymm0, %ymm2, %ymm5
	vbroadcastss	4(%rax), %ymm2
	vfmadd231ps	%ymm1, %ymm2, %ymm8
	vfmadd231ps	%ymm0, %ymm2, %ymm7
	vbroadcastss	8(%rax), %ymm2
	vfmadd231ps	%ymm1, %ymm2, %ymm10
	vfmadd231ps	%ymm0, %ymm2, %ymm9
	vbroadcastss	12(%rax), %ymm2
	vfmadd231ps	%ymm1, %ymm2, %ymm12
	vfmadd231ps	%ymm0, %ymm2, %ymm11
	vbroadcastss	16(%rax), %ymm2
	vfmadd231ps	%ymm1, %ymm2, %ymm14
	vfmadd231ps	%ymm0, %ymm2, %ymm13
	vbroadcastss	20(%rax), %ymm2
	addq	%r9, %rax
	vfmadd231ps	%ymm1, %ymm2, %ymm4
	vfmadd231ps	%ymm0, %ymm2, %ymm3
	cmpl	%ecx, %r11d
	jne	.L19
.L18:
	cmpl	$16, %r8d
	je	.L20
	cmpl	$6, %ebx
	ja	.L39
	leaq	.L23(%rip), %rdx
	movslq	(%rdx,%rbx,4), %rax
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L23:
	.long	.L39-.L23
	.long	.L28-.L23
	.long	.L27-.L23
	.long	.L26-.L23
	.long	.L25-.L23
	.long	.L24-.L23
	.long	.L22-.L23
	.text
	.p2align 4,,10
	.p2align 3
.L22:
	movslq	%r12d, %rsi
	vmaskmovps	%ymm6, %ymm15, (%rdi)
	vmovdqa	-32(%rsp), %ymm6
	leal	(%r12,%r12), %ecx
	leaq	0(,%rsi,4), %rdx
	movslq	%ecx, %r8
	addl	%r12d, %ecx
	salq	$4, %rsi
	leaq	(%rdi,%rdx), %rax
	vmaskmovps	%ymm5, %ymm6, 32(%rdi)
	movslq	%ecx, %rcx
	vmaskmovps	%ymm8, %ymm15, (%rax)
	addq	%rdx, %rax
	vmaskmovps	%ymm7, %ymm6, 32(%rdi,%rdx)
	vmaskmovps	%ymm10, %ymm15, (%rax)
	addq	%rdx, %rax
	vmaskmovps	%ymm9, %ymm6, 32(%rdi,%r8,4)
	vmaskmovps	%ymm12, %ymm15, (%rax)
	addq	%rdx, %rax
	vmaskmovps	%ymm11, %ymm6, 32(%rdi,%rcx,4)
	vmaskmovps	%ymm14, %ymm15, (%rax)
	vmaskmovps	%ymm13, %ymm6, 32(%rdi,%rsi)
	vmaskmovps	%ymm4, %ymm15, (%rax,%rdx)
	leal	(%r12,%r12,4), %eax
	cltq
	vmaskmovps	%ymm3, %ymm6, 32(%rdi,%rax,4)
.L39:
	vzeroupper
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L25:
	.cfi_restore_state
	movslq	%r12d, %rax
	vmaskmovps	%ymm6, %ymm15, (%rdi)
	vmovdqa	-32(%rsp), %ymm6
	leal	(%r12,%r12), %ecx
	salq	$2, %rax
	movslq	%ecx, %rsi
	leaq	(%rdi,%rax), %rdx
	vmaskmovps	%ymm5, %ymm6, 32(%rdi)
	vmaskmovps	%ymm8, %ymm15, (%rdx)
	addq	%rax, %rdx
	vmaskmovps	%ymm7, %ymm6, 32(%rdi,%rax)
	vmaskmovps	%ymm10, %ymm15, (%rdx)
	vmaskmovps	%ymm9, %ymm6, 32(%rdi,%rsi,4)
	vmaskmovps	%ymm12, %ymm15, (%rdx,%rax)
	leal	(%rcx,%r12), %eax
	cltq
	vmaskmovps	%ymm11, %ymm6, 32(%rdi,%rax,4)
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L26:
	movslq	%r12d, %rax
	vmaskmovps	%ymm6, %ymm15, (%rdi)
	vmovdqa	-32(%rsp), %ymm6
	salq	$2, %rax
	leaq	(%rdi,%rax), %rdx
	vmaskmovps	%ymm5, %ymm6, 32(%rdi)
	vmaskmovps	%ymm8, %ymm15, (%rdx)
	vmaskmovps	%ymm7, %ymm6, 32(%rdi,%rax)
	vmaskmovps	%ymm10, %ymm15, (%rdx,%rax)
	leal	(%r12,%r12), %eax
	cltq
	vmaskmovps	%ymm9, %ymm6, 32(%rdi,%rax,4)
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L24:
	movslq	%r12d, %rsi
	vmaskmovps	%ymm6, %ymm15, (%rdi)
	vmovdqa	-32(%rsp), %ymm6
	leal	(%r12,%r12), %ecx
	leaq	0(,%rsi,4), %rdx
	movslq	%ecx, %r8
	addl	%r12d, %ecx
	leaq	(%rdi,%rdx), %rax
	vmaskmovps	%ymm5, %ymm6, 32(%rdi)
	movslq	%ecx, %rcx
	vmaskmovps	%ymm8, %ymm15, (%rax)
	addq	%rdx, %rax
	vmaskmovps	%ymm7, %ymm6, 32(%rdi,%rdx)
	vmaskmovps	%ymm10, %ymm15, (%rax)
	addq	%rdx, %rax
	vmaskmovps	%ymm9, %ymm6, 32(%rdi,%r8,4)
	vmaskmovps	%ymm12, %ymm15, (%rax)
	vmaskmovps	%ymm11, %ymm6, 32(%rdi,%rcx,4)
	vmaskmovps	%ymm14, %ymm15, (%rax,%rdx)
	movq	%rsi, %rax
	salq	$4, %rax
	vmaskmovps	%ymm13, %ymm6, 32(%rdi,%rax)
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L27:
	vmaskmovps	%ymm6, %ymm15, (%rdi)
	movslq	%r12d, %rax
	vmovdqa	-32(%rsp), %ymm6
	salq	$2, %rax
	vmaskmovps	%ymm5, %ymm6, 32(%rdi)
	vmaskmovps	%ymm8, %ymm15, (%rdi,%rax)
	vmaskmovps	%ymm7, %ymm6, 32(%rdi,%rax)
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L28:
	vmovdqa	-32(%rsp), %ymm7
	vmaskmovps	%ymm6, %ymm15, (%rdi)
	vmaskmovps	%ymm5, %ymm7, 32(%rdi)
	jmp	.L39
	.p2align 4,,10
	.p2align 3
.L10:
	vmaskmovps	(%rdi), %ymm4, %ymm6
	vmaskmovps	32(%rdi), %ymm3, %ymm5
.L41:
	vxorps	%xmm3, %xmm3, %xmm3
	vmovaps	%ymm3, %ymm4
	vmovaps	%ymm3, %ymm13
	vmovaps	%ymm3, %ymm14
	vmovaps	%ymm3, %ymm11
	vmovaps	%ymm3, %ymm12
	vmovaps	%ymm3, %ymm9
	vmovaps	%ymm3, %ymm10
	vmovaps	%ymm3, %ymm7
	vmovaps	%ymm3, %ymm8
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L9:
	movslq	%r12d, %rdx
	vmaskmovps	(%rdi), %ymm4, %ymm6
	vmaskmovps	32(%rdi), %ymm3, %ymm5
	salq	$2, %rdx
	vmaskmovps	(%rdi,%rdx), %ymm4, %ymm8
	vmaskmovps	32(%rdi,%rdx), %ymm3, %ymm7
.L42:
	vxorps	%xmm3, %xmm3, %xmm3
	vmovaps	%ymm3, %ymm4
	vmovaps	%ymm3, %ymm13
	vmovaps	%ymm3, %ymm14
	vmovaps	%ymm3, %ymm11
	vmovaps	%ymm3, %ymm12
	vmovaps	%ymm3, %ymm9
	vmovaps	%ymm3, %ymm10
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L8:
	movslq	%r12d, %rdx
	vmaskmovps	(%rdi), %ymm4, %ymm6
	vmaskmovps	32(%rdi), %ymm3, %ymm5
	salq	$2, %rdx
	leaq	(%rdi,%rdx), %rcx
	vmaskmovps	32(%rdi,%rdx), %ymm3, %ymm7
	vmaskmovps	(%rcx,%rdx), %ymm4, %ymm10
	leal	(%r12,%r12), %edx
	vmaskmovps	(%rcx), %ymm4, %ymm8
	movslq	%edx, %rdx
	vmaskmovps	32(%rdi,%rdx,4), %ymm3, %ymm9
.L43:
	vxorps	%xmm3, %xmm3, %xmm3
	vmovaps	%ymm3, %ymm4
	vmovaps	%ymm3, %ymm13
	vmovaps	%ymm3, %ymm14
	vmovaps	%ymm3, %ymm11
	vmovaps	%ymm3, %ymm12
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L7:
	leal	(%r12,%r12), %edx
	movslq	%r12d, %rcx
	vmaskmovps	32(%rdi), %ymm3, %ymm5
	vmaskmovps	(%rdi), %ymm4, %ymm6
	salq	$2, %rcx
	movslq	%edx, %r13
	addl	%r12d, %edx
	leaq	(%rdi,%rcx), %r10
	movslq	%edx, %rdx
	vmaskmovps	32(%rdi,%rcx), %ymm3, %ymm7
	vmaskmovps	32(%rdi,%r13,4), %ymm3, %ymm9
	vmaskmovps	(%r10), %ymm4, %ymm8
	vmaskmovps	32(%rdi,%rdx,4), %ymm3, %ymm11
	addq	%rcx, %r10
	vxorps	%xmm3, %xmm3, %xmm3
	vmaskmovps	(%r10), %ymm4, %ymm10
	vmaskmovps	(%r10,%rcx), %ymm4, %ymm12
	vmovaps	%ymm3, %ymm13
	vmovaps	%ymm3, %ymm4
	vmovaps	%ymm3, %ymm14
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L6:
	movslq	%r12d, %r13
	leal	(%r12,%r12), %ecx
	vmaskmovps	32(%rdi), %ymm3, %ymm5
	vmaskmovps	(%rdi), %ymm4, %ymm6
	leaq	0(,%r13,4), %r10
	movslq	%ecx, %r14
	addl	%r12d, %ecx
	salq	$4, %r13
	leaq	(%rdi,%r10), %rdx
	movslq	%ecx, %rcx
	vmaskmovps	32(%rdi,%r10), %ymm3, %ymm7
	vmaskmovps	32(%rdi,%r14,4), %ymm3, %ymm9
	vmaskmovps	(%rdx), %ymm4, %ymm8
	addq	%r10, %rdx
	vmaskmovps	32(%rdi,%rcx,4), %ymm3, %ymm11
	vmaskmovps	32(%rdi,%r13), %ymm3, %ymm13
	vmaskmovps	(%rdx), %ymm4, %ymm10
	vxorps	%xmm3, %xmm3, %xmm3
	addq	%r10, %rdx
	vmaskmovps	(%rdx), %ymm4, %ymm12
	vmaskmovps	(%rdx,%r10), %ymm4, %ymm14
	vmovaps	%ymm3, %ymm4
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L20:
	cmpl	$6, %ebx
	ja	.L39
	leaq	.L30(%rip), %rdx
	movslq	(%rdx,%rbx,4), %rax
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L30:
	.long	.L39-.L30
	.long	.L35-.L30
	.long	.L34-.L30
	.long	.L33-.L30
	.long	.L32-.L30
	.long	.L31-.L30
	.long	.L29-.L30
	.text
.L29:
	movslq	%r12d, %rsi
	leal	(%r12,%r12), %edx
	vmovups	%ymm6, (%rdi)
	leaq	0(,%rsi,4), %rcx
	vmovups	%ymm5, 32(%rdi)
	movslq	%edx, %r8
	addl	%r12d, %edx
	leaq	(%rdi,%rcx), %rax
	movslq	%edx, %rdx
	vmovups	%ymm8, (%rax)
	addq	%rcx, %rax
	vmovups	%ymm7, 32(%rdi,%rcx)
	vmovups	%ymm10, (%rax)
	addq	%rcx, %rax
	vmovups	%ymm9, 32(%rdi,%r8,4)
	vmovups	%ymm12, (%rax)
	addq	%rcx, %rax
	vmovups	%ymm11, 32(%rdi,%rdx,4)
	movq	%rsi, %rdx
	salq	$4, %rdx
	vmovups	%ymm14, (%rax)
	vmovups	%ymm13, 32(%rdi,%rdx)
	vmovups	%ymm4, (%rax,%rsi,4)
	leal	(%r12,%r12,4), %eax
	cltq
	vmovups	%ymm3, 32(%rdi,%rax,4)
	jmp	.L39
.L31:
	movslq	%r12d, %rcx
	leal	(%r12,%r12), %edx
	vmovups	%ymm6, (%rdi)
	leaq	0(,%rcx,4), %rsi
	vmovups	%ymm5, 32(%rdi)
	movslq	%edx, %r8
	addl	%r12d, %edx
	leaq	(%rdi,%rsi), %rax
	movslq	%edx, %rdx
	vmovups	%ymm8, (%rax)
	addq	%rsi, %rax
	vmovups	%ymm7, 32(%rdi,%rsi)
	vmovups	%ymm10, (%rax)
	addq	%rsi, %rax
	vmovups	%ymm9, 32(%rdi,%r8,4)
	vmovups	%ymm12, (%rax)
	vmovups	%ymm11, 32(%rdi,%rdx,4)
	vmovups	%ymm14, (%rax,%rcx,4)
	salq	$4, %rcx
	vmovups	%ymm13, 32(%rdi,%rcx)
	jmp	.L39
.L32:
	movslq	%r12d, %rcx
	vmovups	%ymm6, (%rdi)
	leaq	0(,%rcx,4), %rax
	vmovups	%ymm5, 32(%rdi)
	leaq	(%rdi,%rax), %rdx
	vmovups	%ymm8, (%rdx)
	addq	%rax, %rdx
	vmovups	%ymm7, 32(%rdi,%rax)
	leal	(%r12,%r12), %eax
	movslq	%eax, %rsi
	addl	%r12d, %eax
	vmovups	%ymm10, (%rdx)
	cltq
	vmovups	%ymm9, 32(%rdi,%rsi,4)
	vmovups	%ymm12, (%rdx,%rcx,4)
	vmovups	%ymm11, 32(%rdi,%rax,4)
	jmp	.L39
.L33:
	movslq	%r12d, %rdx
	vmovups	%ymm6, (%rdi)
	leaq	0(,%rdx,4), %rcx
	vmovups	%ymm5, 32(%rdi)
	leaq	(%rdi,%rcx), %rax
	vmovups	%ymm8, (%rax)
	vmovups	%ymm7, 32(%rdi,%rcx)
	vmovups	%ymm10, (%rax,%rdx,4)
	leal	(%r12,%r12), %eax
	cltq
	vmovups	%ymm9, 32(%rdi,%rax,4)
	jmp	.L39
.L34:
	vmovups	%ymm6, (%rdi)
	vmovups	%ymm5, 32(%rdi)
	vmovups	%ymm8, (%rdi,%r12,4)
	vmovups	%ymm7, 32(%rdi,%r12,4)
	jmp	.L39
.L35:
	vmovups	%ymm6, (%rdi)
	vmovups	%ymm5, 32(%rdi)
	jmp	.L39
.L16:
	movslq	%r12d, %rdx
	vmovups	(%rdi), %ymm6
	vmovups	32(%rdi), %ymm5
	vmovups	(%rdi,%rdx,4), %ymm8
	vmovups	32(%rdi,%rdx,4), %ymm7
	jmp	.L42
.L15:
	movslq	%r12d, %rcx
	vmovups	(%rdi), %ymm6
	vmovups	32(%rdi), %ymm5
	leaq	0(,%rcx,4), %r10
	leaq	(%rdi,%r10), %rdx
	vmovups	32(%rdi,%r10), %ymm7
	vmovups	(%rdx), %ymm8
	vmovups	(%rdx,%rcx,4), %ymm10
	leal	(%r12,%r12), %edx
	movslq	%edx, %rdx
	vmovups	32(%rdi,%rdx,4), %ymm9
	jmp	.L43
.L17:
	vmovups	(%rdi), %ymm6
	vmovups	32(%rdi), %ymm5
	jmp	.L41
.L14:
	movslq	%r12d, %r10
	vxorps	%xmm3, %xmm3, %xmm3
	vmovups	(%rdi), %ymm6
	vmovups	32(%rdi), %ymm5
	leaq	0(,%r10,4), %rdx
	vmovaps	%ymm3, %ymm4
	vmovaps	%ymm3, %ymm13
	leaq	(%rdi,%rdx), %rcx
	vmovups	32(%rdi,%rdx), %ymm7
	vmovaps	%ymm3, %ymm14
	vmovups	(%rcx), %ymm8
	addq	%rdx, %rcx
	leal	(%r12,%r12), %edx
	movslq	%edx, %r13
	addl	%r12d, %edx
	vmovups	(%rcx), %ymm10
	vmovups	(%rcx,%r10,4), %ymm12
	movslq	%edx, %rdx
	vmovups	32(%rdi,%r13,4), %ymm9
	vmovups	32(%rdi,%rdx,4), %ymm11
	jmp	.L3
.L13:
	movslq	%r12d, %r10
	leal	(%r12,%r12), %ecx
	vxorps	%xmm3, %xmm3, %xmm3
	vmovups	(%rdi), %ymm6
	leaq	0(,%r10,4), %r13
	movslq	%ecx, %r14
	addl	%r12d, %ecx
	vmovups	32(%rdi), %ymm5
	leaq	(%rdi,%r13), %rdx
	movslq	%ecx, %rcx
	vmovaps	%ymm3, %ymm4
	vmovups	32(%rdi,%r13), %ymm7
	vmovups	(%rdx), %ymm8
	addq	%r13, %rdx
	vmovups	32(%rdi,%r14,4), %ymm9
	vmovups	(%rdx), %ymm10
	addq	%r13, %rdx
	vmovups	32(%rdi,%rcx,4), %ymm11
	vmovups	(%rdx,%r10,4), %ymm14
	salq	$4, %r10
	vmovups	(%rdx), %ymm12
	vmovups	32(%rdi,%r10), %ymm13
	jmp	.L3
.L11:
	movslq	%r12d, %r10
	leal	(%r12,%r12), %ecx
	vmovups	(%rdi), %ymm6
	vmovups	32(%rdi), %ymm5
	leaq	0(,%r10,4), %r13
	movslq	%ecx, %r14
	addl	%r12d, %ecx
	leaq	(%rdi,%r13), %rdx
	movslq	%ecx, %rcx
	vmovups	32(%rdi,%r13), %ymm7
	vmovups	32(%rdi,%r14,4), %ymm9
	vmovups	(%rdx), %ymm8
	addq	%r13, %rdx
	vmovups	32(%rdi,%rcx,4), %ymm11
	movq	%r10, %rcx
	vmovups	(%rdx), %ymm10
	addq	%r13, %rdx
	salq	$4, %rcx
	vmovups	(%rdx), %ymm12
	addq	%r13, %rdx
	vmovups	32(%rdi,%rcx), %ymm13
	vmovups	(%rdx), %ymm14
	vmovups	(%rdx,%r10,4), %ymm4
	leal	(%r12,%r12,4), %edx
	movslq	%edx, %rdx
	vmovups	32(%rdi,%rdx,4), %ymm3
	jmp	.L3
.L37:
	vxorps	%xmm3, %xmm3, %xmm3
	vmovaps	%ymm3, %ymm4
	vmovaps	%ymm3, %ymm13
	vmovaps	%ymm3, %ymm14
	vmovaps	%ymm3, %ymm11
	vmovaps	%ymm3, %ymm12
	vmovaps	%ymm3, %ymm9
	vmovaps	%ymm3, %ymm10
	vmovaps	%ymm3, %ymm7
	vmovaps	%ymm3, %ymm8
	vmovaps	%ymm3, %ymm5
	vmovaps	%ymm3, %ymm6
	jmp	.L3
	.cfi_endproc
.LFE5512:
	.size	kernel_16x6, .-kernel_16x6
	.data
	.align 64
	.type	mask.0, @object
	.size	mask.0, 32
mask.0:
	.string	"\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377\377"
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
	.string	""
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
