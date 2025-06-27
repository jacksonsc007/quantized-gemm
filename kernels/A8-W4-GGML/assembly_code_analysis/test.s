	.file	"test.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"\033[32m[INFO]\033[m number q blocks in A: %d\n"
	.text
	.p2align 4
	.globl	compute_scaled_sum
	.type	compute_scaled_sum, @function
compute_scaled_sum:
.LFB5518:
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
	leaq	.LC0(%rip), %rsi
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
	jle	.L5
	vxorps	%xmm2, %xmm2, %xmm2
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L3:
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
	jg	.L3
	vzeroupper
.L5:
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
	.size	compute_scaled_sum, .-compute_scaled_sum
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
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
	jle	.L9
	movl	%edx, %r12d
	xorl	%r14d, %r14d
	leaq	.LC1(%rip), %rbp
	xorl	%r13d, %r13d
	.p2align 4,,10
	.p2align 3
.L10:
	testl	%r12d, %r12d
	jle	.L13
	movq	8(%rsp), %rdx
	movslq	%r14d, %rcx
	leal	-1(%r12), %eax
	addq	%rcx, %rax
	leaq	(%rdx,%rcx,4), %r15
	leaq	4(%rdx,%rax,4), %rbx
	.p2align 4,,10
	.p2align 3
.L11:
	vxorpd	%xmm1, %xmm1, %xmm1
	movq	%rbp, %rsi
	movl	$1, %edi
	addq	$4, %r15
	movl	$1, %eax
	vcvtss2sd	-4(%r15), %xmm1, %xmm0
	call	__printf_chk@PLT
	cmpq	%r15, %rbx
	jne	.L11
.L13:
	movl	$10, %edi
	addl	$1, %r13d
	addl	%r12d, %r14d
	call	putchar@PLT
	cmpl	%r13d, 4(%rsp)
	jne	.L10
.L9:
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
	jle	.L22
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
.L18:
	call	rand@PLT
	vxorps	%xmm1, %xmm1, %xmm1
	addq	$4, %rbx
	vcvtsi2ssl	%eax, %xmm1, %xmm0
	vmulss	.LC2(%rip), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rbx)
	cmpq	%rbp, %rbx
	jne	.L18
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L22:
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
	jle	.L31
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
.L27:
	call	rand@PLT
	addq	$1, %rbp
	cltd
	shrl	$24, %edx
	addl	%edx, %eax
	movzbl	%al, %eax
	subl	%edx, %eax
	movb	%al, -1(%rbp)
	cmpq	%rbx, %rbp
	jne	.L27
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L31:
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
	jle	.L40
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
.L36:
	call	rand@PLT
	addq	$1, %rbx
	addl	$-128, %eax
	movb	%al, -1(%rbx)
	cmpq	%rbp, %rbx
	jne	.L36
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L40:
	.cfi_restore 3
	.cfi_restore 6
	ret
	.cfi_endproc
.LFE5522:
	.size	init_rand_int8, .-init_rand_int8
	.p2align 4
	.globl	init_rand_min_max
	.type	init_rand_min_max, @function
init_rand_min_max:
.LFB5523:
	.cfi_startproc
	endbr64
	imull	%edx, %esi
	testl	%esi, %esi
	jle	.L50
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
.L45:
	call	rand@PLT
	addq	$1, %rbx
	cltd
	idivl	%ebp
	addl	%r13d, %edx
	movb	%dl, -1(%rbx)
	cmpq	%rbx, %r12
	jne	.L45
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
.L50:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE5523:
	.size	init_rand_min_max, .-init_rand_min_max
	.p2align 4
	.globl	init_uint_const
	.type	init_uint_const, @function
init_uint_const:
.LFB5524:
	.cfi_startproc
	endbr64
	testl	%edx, %edx
	jle	.L59
	testl	%ecx, %ecx
	jle	.L59
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
.L55:
	movq	%rbp, %rdx
	movl	%r13d, %esi
	addl	$1, %ebx
	call	memset@PLT
	movq	%rax, %rdi
	addq	%rbp, %rdi
	cmpl	%ebx, %r12d
	jne	.L55
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
.L59:
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	ret
	.cfi_endproc
.LFE5524:
	.size	init_uint_const, .-init_uint_const
	.p2align 4
	.globl	init_const
	.type	init_const, @function
init_const:
.LFB5525:
	.cfi_startproc
	endbr64
	movl	%esi, %r9d
	testl	%esi, %esi
	jle	.L77
	movl	%edx, %esi
	testl	%edx, %edx
	jle	.L77
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
.L64:
	movq	%rcx, %rax
	leaq	(%r10,%rcx), %rdx
	cmpl	$6, %r12d
	jbe	.L80
	.p2align 4,,10
	.p2align 3
.L65:
	vmovups	%ymm1, (%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L65
	leaq	(%rcx,%rbx), %r13
	cmpl	%r8d, %esi
	je	.L66
	movl	%r8d, %edx
	movl	%r8d, %eax
.L69:
	movl	%esi, %r14d
	subl	%edx, %r14d
	leal	-1(%r14), %r15d
	cmpl	$2, %r15d
	jbe	.L67
	vmovups	%xmm2, (%rcx,%rdx,4)
	movl	%r14d, %edx
	andl	$-4, %edx
	movl	%edx, %r15d
	addl	%edx, %eax
	leaq	0(%r13,%r15,4), %r13
	cmpl	%edx, %r14d
	je	.L66
.L67:
	leal	1(%rax), %edx
	vmovss	%xmm0, 0(%r13)
	cmpl	%esi, %edx
	jge	.L66
	addl	$2, %eax
	vmovss	%xmm0, 4(%r13)
	cmpl	%esi, %eax
	jge	.L66
	vmovss	%xmm0, 8(%r13)
.L66:
	addl	$1, %edi
	addq	%r11, %rcx
	cmpl	%edi, %r9d
	jne	.L64
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
.L80:
	.cfi_restore_state
	movq	%rcx, %r13
	xorl	%edx, %edx
	xorl	%eax, %eax
	jmp	.L69
.L77:
	.cfi_def_cfa 7, 8
	.cfi_restore 3
	.cfi_restore 6
	.cfi_restore 12
	.cfi_restore 13
	.cfi_restore 14
	.cfi_restore 15
	ret
	.cfi_endproc
.LFE5525:
	.size	init_const, .-init_const
	.section	.rodata.str1.8
	.align 8
.LC10:
	.string	"acc error: % f average error: %f\n"
	.text
	.p2align 4
	.globl	compare_mats
	.type	compare_mats, @function
compare_mats:
.LFB5526:
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
	js	.L91
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
	vmovaps	.LC7(%rip), %xmm6
	vmovdqa	.LC5(%rip), %ymm3
	vmovaps	.LC6(%rip), %ymm4
	salq	$5, %rsi
	subl	%r14d, %r15d
	vxorps	%xmm1, %xmm1, %xmm1
	vmovss	.LC8(%rip), %xmm5
	.p2align 4,,10
	.p2align 3
.L90:
	testl	%r13d, %r13d
	js	.L83
	cmpl	$6, %r13d
	jbe	.L92
	movslq	%ecx, %rax
	addq	%r12, %rax
	leaq	-32(,%rax,4), %rdx
	xorl	%eax, %eax
	leaq	(%r8,%rdx), %rdi
	addq	%r9, %rdx
	.p2align 4,,10
	.p2align 3
.L85:
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
	jne	.L85
	cmpl	%r10d, %r14d
	je	.L83
	movl	%r14d, %ebx
	movl	%r15d, %edx
.L84:
	movl	%r10d, %edi
	subl	%ebx, %edi
	leal	-1(%rdi), %eax
	cmpl	$2, %eax
	jbe	.L88
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
	je	.L83
.L88:
	leal	(%rcx,%rdx), %eax
	cltq
	vmovss	(%r8,%rax,4), %xmm0
	vsubss	(%r9,%rax,4), %xmm0, %xmm0
	leal	-1(%rdx), %eax
	vandps	%xmm5, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	testl	%edx, %edx
	je	.L83
	addl	%ecx, %eax
	cltq
	vmovss	(%r8,%rax,4), %xmm0
	vsubss	(%r9,%rax,4), %xmm0, %xmm0
	leal	-2(%rdx), %eax
	vandps	%xmm5, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	cmpl	$1, %edx
	je	.L83
	addl	%ecx, %eax
	cltq
	vmovss	(%r8,%rax,4), %xmm0
	vsubss	(%r9,%rax,4), %xmm0, %xmm0
	vandps	%xmm5, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
.L83:
	subl	$1, %r11d
	subl	%r10d, %ecx
	cmpl	$-1, %r11d
	jne	.L90
	xorl	%r12d, %r12d
	vcvtss2sd	%xmm1, %xmm1, %xmm2
	vcomisd	.LC9(%rip), %xmm2
	setbe	%r12b
	vzeroupper
.L82:
	movl	-52(%rbp), %eax
	vxorps	%xmm0, %xmm0, %xmm0
	movl	$1, %edi
	leaq	.LC10(%rip), %rsi
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
.L92:
	.cfi_restore_state
	movl	%r13d, %edx
	xorl	%ebx, %ebx
	jmp	.L84
.L91:
	movl	$1, %r12d
	vxorpd	%xmm2, %xmm2, %xmm2
	vxorps	%xmm1, %xmm1, %xmm1
	jmp	.L82
	.cfi_endproc
.LFE5526:
	.size	compare_mats, .-compare_mats
	.p2align 4
	.globl	timer
	.type	timer, @function
timer:
.LFB5527:
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
	jne	.L105
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L105:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5527:
	.size	timer, .-timer
	.p2align 4
	.globl	matmul_kernel
	.type	matmul_kernel, @function
matmul_kernel:
.LFB5528:
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
	jle	.L118
	movl	%r12d, %ebx
	sarl	$5, %ebx
	movl	%ebx, -20(%rsp)
	movl	%r12d, %ebx
	sarl	%ebx
	movl	%ebx, -24(%rsp)
	testl	%eax, %eax
	jle	.L118
	leaq	4(%rdx), %rbx
	subl	$1, %eax
	movl	$0, -48(%rsp)
	vmovdqa	.LC11(%rip), %ymm7
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
.L109:
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
.L114:
	vmovaps	%xmm8, %xmm5
	vxorps	%xmm0, %xmm0, %xmm0
	testl	%r12d, %r12d
	jle	.L113
	movslq	-12(%rsp), %rax
	movq	-40(%rsp), %rsi
	xorl	%edx, %edx
	vmovaps	%xmm8, %xmm5
	movslq	-16(%rsp), %r11
	vxorps	%xmm0, %xmm0, %xmm0
	leaq	(%rsi,%rax), %r9
	.p2align 4,,10
	.p2align 3
.L110:
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
	jg	.L110
.L113:
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
	jne	.L114
	addl	$1, -44(%rsp)
	movl	-20(%rsp), %esi
	addl	%r12d, -52(%rsp)
	movl	-44(%rsp), %eax
	addl	%esi, -56(%rsp)
	addl	$1000, -48(%rsp)
	cmpl	%eax, 24(%rbp)
	jne	.L109
	vzeroupper
.L118:
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
.LFE5528:
	.size	matmul_kernel, .-matmul_kernel
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	805306368
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC5:
	.long	7
	.long	6
	.long	5
	.long	4
	.long	3
	.long	2
	.long	1
	.long	0
	.align 32
.LC6:
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.long	2147483647
	.set	.LC7,.LC6
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC8:
	.long	2147483647
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC9:
	.long	-755914244
	.long	1062232653
	.section	.rodata.cst32
	.align 32
.LC11:
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
