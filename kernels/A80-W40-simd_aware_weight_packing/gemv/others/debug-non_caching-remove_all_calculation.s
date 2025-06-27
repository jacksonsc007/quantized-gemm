	.file	"debug-non_caching-remove_all_calculation.c"
	.text
	.p2align 4
	.type	matmul_naive_no_packing.part.0, @function
matmul_naive_no_packing.part.0:
.LFB5529:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movl	56(%rsp), %eax
	movl	64(%rsp), %ebp
	movq	%rdx, -32(%rsp)
	movl	%r9d, -20(%rsp)
	testl	%r9d, %r9d
	jle	.L15
	testl	%eax, %eax
	jle	.L15
	movq	%rdx, %rbx
	subl	$1, %eax
	movq	%rsi, %r15
	movq	%rcx, %r14
	addq	$4, %rbx
	movq	%rax, -16(%rsp)
	movq	%r8, %r13
	vxorps	%xmm2, %xmm2, %xmm2
	movl	$0, -36(%rsp)
	movl	$0, -24(%rsp)
	movl	$0, -40(%rsp)
	movq	%rbx, -8(%rsp)
.L3:
	movslq	-36(%rsp), %rcx
	movq	-32(%rsp), %rax
	movl	$0, -68(%rsp)
	movslq	-24(%rsp), %rbx
	leaq	(%rax,%rcx,4), %r12
	movq	-16(%rsp), %rax
	movq	%rcx, -64(%rsp)
	addq	%rcx, %rax
	movq	-8(%rsp), %rcx
	leaq	(%rcx,%rax,4), %rax
	movq	%rax, -56(%rsp)
	movl	-40(%rsp), %eax
	leal	(%rax,%rax,4), %eax
	sall	$11, %eax
	cltq
	addq	$32, %rax
	movq	%rax, -48(%rsp)
.L10:
	vxorps	%xmm0, %xmm0, %xmm0
	testl	%ebp, %ebp
	jle	.L9
	movslq	-68(%rsp), %r11
	movq	-48(%rsp), %r8
	xorl	%r9d, %r9d
	vxorps	%xmm0, %xmm0, %xmm0
	movq	%r11, %rax
	sall	$4, %eax
	movslq	%eax, %r10
	subq	-64(%rsp), %r10
	.p2align 4,,10
	.p2align 3
.L6:
	movl	%r9d, %eax
	movq	%r10, %rcx
	sarl	$5, %eax
	subq	%r9, %rcx
	cltq
	leaq	(%rbx,%rax), %rdx
	addq	%r11, %rax
	vmovss	0(%r13,%rax,4), %xmm5
	vmovss	4(%r13,%rax,4), %xmm4
	movl	%r9d, %eax
	sarl	%eax
	vmovss	(%r14,%rdx,4), %xmm7
	vmovss	4(%r14,%rdx,4), %xmm6
	leaq	-32(%r8), %rdx
	cltq
	addq	%rax, %rcx
	addq	%r15, %rcx
	.p2align 4,,10
	.p2align 3
.L5:
	movzbl	(%rcx,%rdx), %eax
	movl	%eax, %esi
	shrb	$4, %al
	andl	$15, %esi
	subl	$8, %eax
	subl	$8, %esi
	movsbl	%al, %eax
	movsbl	%sil, %esi
	vcvtsi2ssl	%esi, %xmm2, %xmm1
	movsbl	(%rdi,%rdx), %esi
	vcvtsi2ssl	%esi, %xmm2, %xmm3
	vmulss	%xmm3, %xmm1, %xmm1
	vmulss	%xmm7, %xmm1, %xmm1
	vfmadd231ss	%xmm1, %xmm5, %xmm0
	vcvtsi2ssl	%eax, %xmm2, %xmm1
	movsbl	32(%rdi,%rdx), %eax
	addq	$1, %rdx
	vcvtsi2ssl	%eax, %xmm2, %xmm3
	vmulss	%xmm3, %xmm1, %xmm1
	vmulss	%xmm6, %xmm1, %xmm1
	vfmadd231ss	%xmm1, %xmm4, %xmm0
	cmpq	%rdx, %r8
	jne	.L5
	addq	$64, %r9
	addq	$64, %r8
	cmpl	%r9d, %ebp
	jg	.L6
.L9:
	vmovss	%xmm0, (%r12)
	addq	$4, %r12
	addl	$320, -68(%rsp)
	cmpq	%r12, -56(%rsp)
	jne	.L10
	addl	$1, -40(%rsp)
	movl	-40(%rsp), %eax
	addl	$320, -24(%rsp)
	addl	$10240, -36(%rsp)
	cmpl	-20(%rsp), %eax
	jne	.L3
.L15:
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE5529:
	.size	matmul_naive_no_packing.part.0, .-matmul_naive_no_packing.part.0
	.p2align 4
	.type	_mm_malloc.constprop.0, @function
_mm_malloc.constprop.0:
.LFB5530:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rdx
	movl	$64, %esi
	movq	%fs:40, %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rdi
	call	posix_memalign@PLT
	testl	%eax, %eax
	movl	$0, %eax
	cmove	(%rsp), %rax
	movq	8(%rsp), %rdx
	subq	%fs:40, %rdx
	jne	.L23
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L23:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5530:
	.size	_mm_malloc.constprop.0, .-_mm_malloc.constprop.0
	.p2align 4
	.globl	matmul_kernel
	.type	matmul_kernel, @function
matmul_kernel:
.LFB5517:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r13
	movl	16(%rbp), %eax
	movl	24(%rbp), %ecx
	pushq	%r12
	pushq	%rbx
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 3, -40
	testl	%eax, %eax
	jle	.L38
	leal	-1(%rcx), %r10d
	movl	%ecx, %r12d
	subl	$1, %eax
	movq	%rsi, %r11
	shrl	$6, %r10d
	movq	%rdx, %rdi
	sarl	%r12d
	leaq	4(%rdx,%rax,4), %rbx
	movq	number_times_to_access_kernel(%rip), %r9
	xorl	%r8d, %r8d
	xorl	%r13d, %r13d
	addl	$1, %r10d
	vmovss	.LC2(%rip), %xmm3
	vmovdqa	.LC1(%rip), %ymm2
	.p2align 4,,10
	.p2align 3
.L27:
	movslq	%r8d, %rdx
	xorl	%eax, %eax
	leaq	(%r11,%rdx), %rsi
	testl	%ecx, %ecx
	jle	.L31
	.p2align 4,,10
	.p2align 3
.L28:
	movl	%eax, %edx
	sarl	%edx
	movslq	%edx, %rdx
	vmovdqu	(%rsi,%rdx), %ymm0
	vpsrlw	$4, %ymm0, %ymm1
	vpand	%ymm2, %ymm0, %ymm0
	vpand	%ymm1, %ymm2, %ymm0
	addl	$64, %eax
	cmpl	%eax, %ecx
	jg	.L28
	addq	%r10, %r9
	movl	$1, %r13d
.L31:
	vmovss	%xmm3, (%rdi)
	addq	$4, %rdi
	addl	%r12d, %r8d
	cmpq	%rbx, %rdi
	jne	.L27
	testb	%r13b, %r13b
	je	.L37
	movq	%r9, number_times_to_access_kernel(%rip)
	vzeroupper
.L38:
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L37:
	.cfi_restore_state
	vzeroupper
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5517:
	.size	matmul_kernel, .-matmul_kernel
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"debug-non_caching-remove_all_calculation.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC4:
	.string	"K % Q_BLK_SIZE == 0"
	.text
	.p2align 4
	.globl	matmul_naive_no_packing
	.type	matmul_naive_no_packing, @function
matmul_naive_no_packing:
.LFB5518:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	24(%rsp), %eax
	movl	16(%rsp), %r10d
	testb	$31, %al
	jne	.L43
	movl	%eax, 24(%rsp)
	movl	%r10d, 16(%rsp)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	jmp	matmul_naive_no_packing.part.0
.L43:
	.cfi_restore_state
	leaq	__PRETTY_FUNCTION__.0(%rip), %rcx
	movl	$151, %edx
	leaq	.LC3(%rip), %rsi
	leaq	.LC4(%rip), %rdi
	call	__assert_fail@PLT
	.cfi_endproc
.LFE5518:
	.size	matmul_naive_no_packing, .-matmul_naive_no_packing
	.section	.rodata.str1.1
.LC5:
	.string	"%.2f "
	.text
	.p2align 4
	.globl	print_mat
	.type	print_mat, @function
print_mat:
.LFB5519:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, 8(%rsp)
	movl	%esi, 4(%rsp)
	testl	%esi, %esi
	jle	.L45
	movl	%edx, %r12d
	xorl	%r14d, %r14d
	leaq	.LC5(%rip), %rbp
	xorl	%r13d, %r13d
	.p2align 4,,10
	.p2align 3
.L46:
	testl	%r12d, %r12d
	jle	.L49
	movq	8(%rsp), %rdx
	movslq	%r14d, %rcx
	leal	-1(%r12), %eax
	addq	%rcx, %rax
	leaq	(%rdx,%rcx,4), %r15
	leaq	4(%rdx,%rax,4), %rbx
	.p2align 4,,10
	.p2align 3
.L47:
	vxorpd	%xmm1, %xmm1, %xmm1
	movq	%rbp, %rsi
	movl	$1, %edi
	addq	$4, %r15
	movl	$1, %eax
	vcvtss2sd	-4(%r15), %xmm1, %xmm0
	call	__printf_chk@PLT
	cmpq	%r15, %rbx
	jne	.L47
.L49:
	movl	$10, %edi
	addl	$1, %r13d
	addl	%r12d, %r14d
	call	putchar@PLT
	cmpl	%r13d, 4(%rsp)
	jne	.L46
.L45:
	addq	$24, %rsp
	.cfi_def_cfa_offset 56
	movl	$10, %edi
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	putchar@PLT
	.cfi_endproc
.LFE5519:
	.size	print_mat, .-print_mat
	.p2align 4
	.globl	init_rand
	.type	init_rand, @function
init_rand:
.LFB5520:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L58
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
.L54:
	call	rand@PLT
	vxorps	%xmm1, %xmm1, %xmm1
	addq	$4, %rbx
	vcvtsi2ssl	%eax, %xmm1, %xmm0
	vmulss	.LC6(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rbx)
	cmpq	%rbp, %rbx
	jne	.L54
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L58:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE5520:
	.size	init_rand, .-init_rand
	.p2align 4
	.globl	init_rand_uint8
	.type	init_rand_uint8, @function
init_rand_uint8:
.LFB5521:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L67
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
.L63:
	call	rand@PLT
	addq	$1, %rbp
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%rbp)
	cmpq	%rbx, %rbp
	jne	.L63
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L67:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE5521:
	.size	init_rand_uint8, .-init_rand_uint8
	.p2align 4
	.globl	init_rand_int8
	.type	init_rand_int8, @function
init_rand_int8:
.LFB5522:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L76
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
.L72:
	call	rand@PLT
	addq	$1, %rbx
	addl	$-128, %eax
	movb	%al, -1(%rbx)
	cmpq	%rbp, %rbx
	jne	.L72
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L76:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE5522:
	.size	init_rand_int8, .-init_rand_int8
	.p2align 4
	.globl	init_int_const
	.type	init_int_const, @function
init_int_const:
.LFB5523:
	.cfi_startproc
	endbr64
	testl	%edx, %edx
	jle	.L85
	testl	%ecx, %ecx
	jle	.L85
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
.L81:
	movq	%rbp, %rdx
	movl	%r13d, %esi
	addl	$1, %ebx
	call	memset@PLT
	movq	%rax, %rdi
	addq	%rbp, %rdi
	cmpl	%ebx, %r12d
	jne	.L81
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
.L85:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE5523:
	.size	init_int_const, .-init_int_const
	.p2align 4
	.globl	init_const
	.type	init_const, @function
init_const:
.LFB5524:
	.cfi_startproc
	endbr64
	movl	%esi, %r9d
	testl	%esi, %esi
	jle	.L103
	movl	%edx, %esi
	testl	%edx, %edx
	jle	.L103
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
.L90:
	movq	%rcx, %rax
	leaq	(%r10,%rcx), %rdx
	cmpl	$6, %r12d
	jbe	.L106
	.p2align 4,,10
	.p2align 3
.L91:
	vmovups	%ymm1, (%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L91
	leaq	(%rcx,%rbx), %r13
	cmpl	%r8d, %esi
	je	.L92
	movl	%r8d, %edx
	movl	%r8d, %eax
.L95:
	movl	%esi, %r14d
	subl	%edx, %r14d
	leal	-1(%r14), %r15d
	cmpl	$2, %r15d
	jbe	.L93
	vmovups	%xmm2, (%rcx,%rdx,4)
	movl	%r14d, %edx
	andl	$-4, %edx
	movl	%edx, %r15d
	addl	%edx, %eax
	leaq	0(%r13,%r15,4), %r13
	cmpl	%edx, %r14d
	je	.L92
.L93:
	leal	1(%rax), %edx
	vmovss	%xmm0, 0(%r13)
	cmpl	%esi, %edx
	jge	.L92
	addl	$2, %eax
	vmovss	%xmm0, 4(%r13)
	cmpl	%esi, %eax
	jge	.L92
	vmovss	%xmm0, 8(%r13)
.L92:
	addl	$1, %edi
	addq	%r11, %rcx
	cmpl	%edi, %r9d
	jne	.L90
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
.L106:
	.cfi_restore_state
	movq	%rcx, %r13
	xorl	%edx, %edx
	xorl	%eax, %eax
	jmp	.L95
.L103:
	.cfi_def_cfa 7, 8
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE5524:
	.size	init_const, .-init_const
	.section	.rodata.str1.8
	.align 8
.LC13:
	.string	"acc error: % f average error: %f\n"
	.text
	.p2align 4
	.globl	compare_mats
	.type	compare_mats, @function
compare_mats:
.LFB5525:
	.cfi_startproc
	endbr64
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	pushq	-8(%r10)
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x58,0x6
	.cfi_escape 0x10,0xf,0x2,0x76,0x78
	.cfi_escape 0x10,0xe,0x2,0x76,0x70
	.cfi_escape 0x10,0xd,0x2,0x76,0x68
	.cfi_escape 0x10,0xc,0x2,0x76,0x60
	movl	%ecx, %r10d
	pushq	%rbx
	subq	$32, %rsp
	.cfi_escape 0x10,0x3,0x2,0x76,0x50
	movl	%edx, -52(%rbp)
	subl	$1, %edx
	js	.L117
	movl	%r10d, %eax
	leal	-1(%rcx), %r13d
	movl	%edx, %ecx
	movq	%rsi, %r9
	shrl	$3, %eax
	movl	%r10d, %r14d
	movl	%r13d, %r15d
	movq	%rdi, %r8
	leal	-1(%rax), %esi
	andl	$-8, %r14d
	movl	%edx, %r11d
	movslq	%r10d, %r12
	imull	%r10d, %ecx
	notq	%rsi
	vmovaps	.LC10(%rip), %xmm6
	vmovdqa	.LC8(%rip), %ymm3
	vmovaps	.LC9(%rip), %ymm4
	salq	$5, %rsi
	subl	%r14d, %r15d
	vxorps	%xmm1, %xmm1, %xmm1
	vmovss	.LC11(%rip), %xmm5
	.p2align 4,,10
	.p2align 3
.L116:
	testl	%r13d, %r13d
	js	.L109
	cmpl	$6, %r13d
	jbe	.L118
	movslq	%ecx, %rax
	addq	%r12, %rax
	leaq	-32(,%rax,4), %rdx
	xorl	%eax, %eax
	leaq	(%r8,%rdx), %rdi
	addq	%r9, %rdx
	.p2align 4,,10
	.p2align 3
.L111:
	vpermd	(%rdx,%rax), %ymm3, %ymm2
	vpermd	(%rdi,%rax), %ymm3, %ymm0
	subq	$32, %rax
	vsubps	%ymm2, %ymm0, %ymm0
	vandps	%ymm4, %ymm0, %ymm0
	vaddss	%xmm0, %xmm1, %xmm1
	vshufps	$85, %xmm0, %xmm0, %xmm7
	vshufps	$255, %xmm0, %xmm0, %xmm2
	vaddss	%xmm1, %xmm7, %xmm7
	vunpckhps	%xmm0, %xmm0, %xmm1
	vextractf128	$0x1, %ymm0, %xmm0
	vaddss	%xmm7, %xmm1, %xmm1
	vaddss	%xmm1, %xmm2, %xmm2
	vshufps	$85, %xmm0, %xmm0, %xmm1
	vaddss	%xmm2, %xmm0, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm2
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm0, %xmm1, %xmm1
	cmpq	%rax, %rsi
	jne	.L111
	cmpl	%r10d, %r14d
	je	.L109
	movl	%r14d, %ebx
	movl	%r15d, %edx
.L110:
	movl	%r10d, %edi
	subl	%ebx, %edi
	leal	-1(%rdi), %eax
	cmpl	$2, %eax
	jbe	.L114
	movslq	%ecx, %rax
	addl	$1, %ebx
	addq	%r12, %rax
	subq	%rbx, %rax
	vpermilps	$27, -12(%r9,%rax,4), %xmm2
	vpermilps	$27, -12(%r8,%rax,4), %xmm0
	movl	%edi, %eax
	andl	$-4, %eax
	vsubps	%xmm2, %xmm0, %xmm0
	subl	%eax, %edx
	vandps	%xmm6, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	vshufps	$85, %xmm0, %xmm0, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm2
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	%xmm0, %xmm1, %xmm1
	cmpl	%eax, %edi
	je	.L109
.L114:
	leal	(%rcx,%rdx), %eax
	cltq
	vmovss	(%r8,%rax,4), %xmm0
	vsubss	(%r9,%rax,4), %xmm0, %xmm0
	leal	-1(%rdx), %eax
	vandps	%xmm5, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	testl	%edx, %edx
	je	.L109
	addl	%ecx, %eax
	cltq
	vmovss	(%r8,%rax,4), %xmm0
	vsubss	(%r9,%rax,4), %xmm0, %xmm0
	leal	-2(%rdx), %eax
	vandps	%xmm5, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	cmpl	$1, %edx
	je	.L109
	addl	%ecx, %eax
	cltq
	vmovss	(%r8,%rax,4), %xmm0
	vsubss	(%r9,%rax,4), %xmm0, %xmm0
	vandps	%xmm5, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
.L109:
	subl	$1, %r11d
	subl	%r10d, %ecx
	cmpl	$-1, %r11d
	jne	.L116
	xorl	%r12d, %r12d
	vcvtss2sd	%xmm1, %xmm1, %xmm2
	vcomisd	.LC12(%rip), %xmm2
	setbe	%r12b
	vzeroupper
.L108:
	movl	-52(%rbp), %eax
	vxorps	%xmm0, %xmm0, %xmm0
	movl	$1, %edi
	leaq	.LC13(%rip), %rsi
	imull	%r10d, %eax
	vcvtsi2ssl	%eax, %xmm0, %xmm0
	movl	$2, %eax
	vdivss	%xmm0, %xmm1, %xmm1
	vmovsd	%xmm2, %xmm2, %xmm0
	vcvtss2sd	%xmm1, %xmm1, %xmm1
	call	__printf_chk@PLT
	addq	$32, %rsp
	movl	%r12d, %eax
	popq	%rbx
	popq	%r10
	.cfi_remember_state
	.cfi_def_cfa 10, 0
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	ret
.L118:
	.cfi_restore_state
	movl	%r13d, %edx
	xorl	%ebx, %ebx
	jmp	.L110
.L117:
	movl	$1, %r12d
	vxorpd	%xmm2, %xmm2, %xmm2
	vxorps	%xmm1, %xmm1, %xmm1
	jmp	.L108
	.cfi_endproc
.LFE5525:
	.size	compare_mats, .-compare_mats
	.p2align 4
	.globl	timer
	.type	timer, @function
timer:
.LFB5526:
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
	jne	.L131
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L131:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5526:
	.size	timer, .-timer
	.p2align 4
	.globl	float_matmul_naive
	.type	float_matmul_naive, @function
float_matmul_naive:
.LFB5527:
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
	jle	.L141
	movl	%r8d, %r10d
	testl	%r8d, %r8d
	jle	.L141
	movl	%r9d, %eax
	testl	%r9d, %r9d
	jle	.L141
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
.L135:
	movq	%r14, %rsi
	movq	%rbp, %rcx
	xorl	%r8d, %r8d
	.p2align 4,,10
	.p2align 3
.L138:
	vmovss	(%rcx), %xmm0
	leaq	(%r11,%rsi), %rax
	movq	%r9, %rdx
	.p2align 4,,10
	.p2align 3
.L136:
	vmovss	(%rdx), %xmm1
	vfmadd231ss	(%rax), %xmm1, %xmm0
	addq	$4, %rax
	addq	%rdi, %rdx
	vmovss	%xmm0, (%rcx)
	cmpq	%rax, %rsi
	jne	.L136
	addl	$1, %r8d
	addq	%rdi, %rcx
	addq	%rbx, %rsi
	cmpl	%r8d, %r10d
	jne	.L138
	addq	$1, %r12
	addq	$4, %r9
	addq	$4, %rbp
	cmpq	%r13, %r12
	jne	.L135
.L141:
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
.LFE5527:
	.size	float_matmul_naive, .-float_matmul_naive
	.section	.rodata.str1.8
	.align 8
.LC16:
	.string	"\n\033[32m[INFO]\033[m MR=%d, NR=%d, MC=%d, NC=%d, KC=%d, M=%d, K=%d, N=%d\n"
	.align 8
.LC20:
	.string	"GFLOPS= %.3f for (%d, %d)x(%d, %d) \n"
	.align 8
.LC21:
	.string	"number_times_to_access_kernel: %lu, Memory bandwidth: %.3f GB/s\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB5528:
	.cfi_startproc
	endbr64
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	movl	$10240, %edi
	pushq	-8(%r10)
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x58,0x6
	.cfi_escape 0x10,0xf,0x2,0x76,0x78
	.cfi_escape 0x10,0xe,0x2,0x76,0x70
	.cfi_escape 0x10,0xd,0x2,0x76,0x68
	.cfi_escape 0x10,0xc,0x2,0x76,0x60
	pushq	%rbx
	addq	$-128, %rsp
	.cfi_escape 0x10,0x3,0x2,0x76,0x50
	movq	%fs:40, %rax
	movq	%rax, -56(%rbp)
	xorl	%eax, %eax
	call	_mm_malloc.constprop.0
	movl	$52428800, %edi
	movq	%rax, %r14
	movq	%rax, -112(%rbp)
	call	_mm_malloc.constprop.0
	movl	$1280, %edi
	movq	%r14, %rbx
	movq	%rax, %r13
	addq	$10240, %rbx
	call	_mm_malloc.constprop.0
	movl	$13107200, %edi
	movq	%rax, -88(%rbp)
	call	_mm_malloc.constprop.0
	movl	$40960, %edi
	movq	%rax, -96(%rbp)
	call	_mm_malloc.constprop.0
	movl	$40960, %edi
	movq	%rax, %r12
	call	_mm_malloc.constprop.0
	movq	%rax, %r15
.L144:
	call	rand@PLT
	addq	$1, %r14
	addl	$-128, %eax
	movb	%al, -1(%r14)
	cmpq	%rbx, %r14
	jne	.L144
	leaq	52428800(%r13), %rbx
	movq	%r13, %r14
.L145:
	call	rand@PLT
	addq	$1, %r14
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%r14)
	cmpq	%rbx, %r14
	jne	.L145
	movq	-88(%rbp), %r14
	leaq	1280(%r14), %rbx
.L146:
	call	rand@PLT
	vxorps	%xmm6, %xmm6, %xmm6
	addq	$4, %r14
	vcvtsi2ssl	%eax, %xmm6, %xmm0
	vmulss	.LC6(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%r14)
	cmpq	%rbx, %r14
	jne	.L146
	movq	-96(%rbp), %r14
	leaq	13107200(%r14), %rbx
.L147:
	call	rand@PLT
	vxorps	%xmm7, %xmm7, %xmm7
	addq	$4, %r14
	vcvtsi2ssl	%eax, %xmm7, %xmm0
	vmulss	.LC6(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%r14)
	cmpq	%rbx, %r14
	jne	.L147
	movl	$51200, %edx
	movl	$255, %esi
	leaq	blockB_packed(%rip), %rdi
	call	memset@PLT
	leaq	sB_packed(%rip), %rax
	vmovaps	.LC14(%rip), %ymm0
	vmovaps	.LC15(%rip), %xmm1
	leaq	12800(%rax), %rdx
.L148:
	vmovups	%ymm0, (%rax)
	addq	$400, %rax
	vmovups	%ymm0, -368(%rax)
	vmovups	%ymm0, -336(%rax)
	vmovups	%ymm0, -304(%rax)
	vmovups	%ymm0, -272(%rax)
	vmovups	%ymm0, -240(%rax)
	vmovups	%ymm0, -208(%rax)
	vmovups	%ymm0, -176(%rax)
	vmovups	%ymm0, -144(%rax)
	vmovups	%ymm0, -112(%rax)
	vmovups	%ymm0, -80(%rax)
	vmovups	%ymm0, -48(%rax)
	vmovaps	%xmm1, -16(%rax)
	cmpq	%rdx, %rax
	jne	.L148
	movl	$4000000, %edi
	vzeroupper
	call	_mm_malloc.constprop.0
	movl	$4000000, %edi
	movq	%rax, %rbx
	movq	%rax, -120(%rbp)
	call	_mm_malloc.constprop.0
	movl	$4000000, %edi
	movq	%rax, %r14
	call	_mm_malloc.constprop.0
	movq	%rax, -128(%rbp)
	leaq	4000000(%rbx), %rax
	movq	%rax, -104(%rbp)
.L149:
	call	rand@PLT
	vxorps	%xmm6, %xmm6, %xmm6
	addq	$4, %rbx
	vcvtsi2ssl	%eax, %xmm6, %xmm0
	vmulss	.LC6(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rbx)
	cmpq	-104(%rbp), %rbx
	jne	.L149
	leaq	4000000(%r14), %rax
	movq	%r14, %rbx
	movq	%rax, -136(%rbp)
.L150:
	call	rand@PLT
	vxorps	%xmm7, %xmm7, %xmm7
	addq	$4, %rbx
	vcvtsi2ssl	%eax, %xmm7, %xmm0
	vmulss	.LC6(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rbx)
	cmpq	-136(%rbp), %rbx
	jne	.L150
	pushq	$10240
	movq	-96(%rbp), %r8
	movq	%r15, %rdx
	movq	%r13, %rsi
	pushq	$10240
	movq	-88(%rbp), %rcx
	movl	$1, %r9d
	leaq	-80(%rbp), %rbx
	movq	-112(%rbp), %rdi
	call	matmul_naive_no_packing.part.0
	pushq	$10240
	movl	$100, %r9d
	movl	$1, %r8d
	pushq	$10240
	movl	$4, %ecx
	xorl	%eax, %eax
	movl	$1, %edx
	pushq	$1
	leaq	.LC16(%rip), %rsi
	movl	$1, %edi
	pushq	$1024
	call	__printf_chk@PLT
	addq	$48, %rsp
	movl	$5, -136(%rbp)
.L151:
	movq	-120(%rbp), %r9
	movq	-104(%rbp), %rsi
	xorl	%r10d, %r10d
	movq	-128(%rbp), %r11
	.p2align 4,,10
	.p2align 3
.L165:
	movq	%r14, %r8
	movq	%r11, %rcx
	xorl	%edi, %edi
	.p2align 4,,10
	.p2align 3
.L155:
	vmovss	(%rcx), %xmm0
	movq	%r8, %rdx
	movq	%r9, %rax
	.p2align 4,,10
	.p2align 3
.L152:
	vmovss	(%rax), %xmm5
	vfmadd231ss	(%rdx), %xmm5, %xmm0
	addq	$4000, %rax
	addq	$4, %rdx
	vmovss	%xmm0, (%rcx)
	cmpq	%rsi, %rax
	jne	.L152
	addl	$1000, %edi
	addq	$4000, %rcx
	addq	$4000, %r8
	cmpl	$1000000, %edi
	jne	.L155
	addq	$1, %r10
	addq	$4, %r9
	leaq	4(%rax), %rsi
	addq	$4, %r11
	cmpq	$1000, %r10
	jne	.L165
	movl	$40960, %edx
	xorl	%esi, %esi
	movq	%r12, %rdi
	call	memset@PLT
	movq	%rbx, %rsi
	movl	$4, %edi
	call	clock_gettime@PLT
	movq	-80(%rbp), %r8
	movq	-72(%rbp), %r9
	movq	%r12, %rdi
	vmovdqa	.LC1(%rip), %ymm2
	xorl	%esi, %esi
.L156:
	xorl	%eax, %eax
	leaq	0(%r13,%rsi), %rcx
	.p2align 4,,10
	.p2align 3
.L157:
	movl	%eax, %edx
	sarl	%edx
	movslq	%edx, %rdx
	vmovdqu	(%rcx,%rdx), %ymm0
	vpsrlw	$4, %ymm0, %ymm1
	vpand	%ymm2, %ymm0, %ymm0
	vpand	%ymm1, %ymm2, %ymm0
	addl	$64, %eax
	cmpl	$10240, %eax
	jne	.L157
	addq	$5120, %rsi
	movl	$0x3faa3d71, (%rdi)
	addq	$4, %rdi
	cmpq	$52428800, %rsi
	jne	.L156
	movq	%rbx, %rsi
	movl	$4, %edi
	movq	%r8, -152(%rbp)
	movq	%r9, -144(%rbp)
	movq	$1638400, number_times_to_access_kernel(%rip)
	vzeroupper
	call	clock_gettime@PLT
	movq	-152(%rbp), %r8
	imulq	$1000000000, -80(%rbp), %rax
	movq	-144(%rbp), %r9
	imulq	$1000000000, %r8, %r8
	subq	%r9, %rax
	addq	-72(%rbp), %rax
	subq	%r8, %rax
	js	.L159
	vxorpd	%xmm6, %xmm6, %xmm6
	vcvtsi2sdq	%rax, %xmm6, %xmm3
.L160:
	movl	$1, %edx
	movl	$10240, %ecx
	movl	$1, %edi
	vmovsd	.LC18(%rip), %xmm6
	vmulsd	.LC17(%rip), %xmm3, %xmm3
	movl	$10240, %r9d
	movl	$10240, %r8d
	movl	$1, %eax
	leaq	.LC20(%rip), %rsi
	vdivsd	%xmm3, %xmm6, %xmm0
	vmovsd	%xmm3, -144(%rbp)
	vdivsd	.LC19(%rip), %xmm0, %xmm0
	call	__printf_chk@PLT
	movq	number_times_to_access_kernel(%rip), %rdx
	vmovsd	-144(%rbp), %xmm3
	leaq	0(,%rdx,8), %rax
	subq	%rdx, %rax
	salq	$4, %rax
	js	.L161
	vxorpd	%xmm7, %xmm7, %xmm7
	vcvtsi2sdq	%rax, %xmm7, %xmm0
.L162:
	vdivsd	%xmm3, %xmm0, %xmm0
	movl	$1, %edi
	movl	$1, %eax
	leaq	.LC21(%rip), %rsi
	vdivsd	.LC19(%rip), %xmm0, %xmm0
	call	__printf_chk@PLT
	movl	$40928, %eax
	vxorps	%xmm3, %xmm3, %xmm3
	.p2align 4,,10
	.p2align 3
.L163:
	vmovdqa	.LC8(%rip), %ymm6
	vpermd	(%r15,%rax), %ymm6, %ymm1
	vpermd	(%r12,%rax), %ymm6, %ymm0
	subq	$32, %rax
	vsubps	%ymm1, %ymm0, %ymm0
	vandps	.LC9(%rip), %ymm0, %ymm0
	vaddss	%xmm3, %xmm0, %xmm3
	vshufps	$85, %xmm0, %xmm0, %xmm1
	vshufps	$255, %xmm0, %xmm0, %xmm4
	vaddss	%xmm3, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm3
	vextractf128	$0x1, %ymm0, %xmm0
	vaddss	%xmm3, %xmm1, %xmm1
	vaddss	%xmm4, %xmm1, %xmm1
	vaddss	%xmm1, %xmm0, %xmm3
	vshufps	$85, %xmm0, %xmm0, %xmm1
	vaddss	%xmm3, %xmm1, %xmm1
	vunpckhps	%xmm0, %xmm0, %xmm3
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm1, %xmm3, %xmm3
	vaddss	%xmm0, %xmm3, %xmm3
	cmpq	$-32, %rax
	jne	.L163
	leaq	.LC13(%rip), %rsi
	movl	$1, %edi
	vcvtss2sd	%xmm3, %xmm3, %xmm0
	movl	$2, %eax
	vdivss	.LC22(%rip), %xmm3, %xmm1
	vcvtss2sd	%xmm1, %xmm1, %xmm1
	vzeroupper
	call	__printf_chk@PLT
	movl	$10, %edi
	call	putchar@PLT
	subl	$1, -136(%rbp)
	jne	.L151
	movq	-112(%rbp), %rdi
	call	free@PLT
	movq	%r13, %rdi
	call	free@PLT
	movq	%r12, %rdi
	call	free@PLT
	movq	%r15, %rdi
	call	free@PLT
	movq	-88(%rbp), %rdi
	call	free@PLT
	movq	-96(%rbp), %rdi
	call	free@PLT
	movq	-120(%rbp), %rdi
	call	free@PLT
	movq	%r14, %rdi
	call	free@PLT
	movq	-128(%rbp), %rdi
	call	free@PLT
	movq	-56(%rbp), %rax
	subq	%fs:40, %rax
	jne	.L178
	leaq	-48(%rbp), %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%r10
	.cfi_remember_state
	.cfi_def_cfa 10, 0
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	ret
.L161:
	.cfi_restore_state
	shrq	%rax
	vxorpd	%xmm6, %xmm6, %xmm6
	vcvtsi2sdq	%rax, %xmm6, %xmm1
	vaddsd	%xmm1, %xmm1, %xmm0
	jmp	.L162
.L159:
	movq	%rax, %rdx
	andl	$1, %eax
	vxorpd	%xmm7, %xmm7, %xmm7
	shrq	%rdx
	orq	%rax, %rdx
	vcvtsi2sdq	%rdx, %xmm7, %xmm3
	vaddsd	%xmm3, %xmm3, %xmm3
	jmp	.L160
.L178:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5528:
	.size	main, .-main
	.section	.rodata
	.align 16
	.type	__PRETTY_FUNCTION__.0, @object
	.size	__PRETTY_FUNCTION__.0, 24
__PRETTY_FUNCTION__.0:
	.string	"matmul_naive_no_packing"
	.local	number_times_to_access_kernel
	.comm	number_times_to_access_kernel,8,8
	.local	sB_packed
	.comm	sB_packed,12800,64
	.local	blockB_packed
	.comm	blockB_packed,51200,64
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC1:
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	1068121457
	.align 4
.LC6:
	.long	805306368
	.section	.rodata.cst32
	.align 32
.LC8:
	.long	7
	.long	6
	.long	5
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.align 32
.LC9:
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.set	.LC10,.LC9
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC11:
	.long	2147483647
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC12:
	.long	-755914244
	.long	1062232653
	.section	.rodata.cst32
	.align 32
.LC14:
	.long	-1082130432
	.long	-1082130432
	.long	-1082130432
	.long	-1082130432
	.long	-1082130432
	.long	-1082130432
	.long	-1082130432
	.long	-1082130432
	.set	.LC15,.LC14
	.section	.rodata.cst8
	.align 8
.LC17:
	.long	-400107883
	.long	1041313291
	.align 8
.LC18:
	.long	0
	.long	1101594624
	.align 8
.LC19:
	.long	0
	.long	1104006501
	.section	.rodata.cst4
	.align 4
.LC22:
	.long	1176502272
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
