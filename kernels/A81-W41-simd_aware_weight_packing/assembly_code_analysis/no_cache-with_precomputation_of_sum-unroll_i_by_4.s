	.file	"no_cache-with_precomputation_of_sum-unroll_i_by_4.c"
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
	.cfi_offset 15, -24
	movq	%rcx, %r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$168, %rsp
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, -96(%rsp)
	movq	%rsi, -40(%rsp)
	movl	24(%rbp), %eax
	movl	32(%rbp), %ecx
	movq	%rdx, -104(%rsp)
	subl	$3, %eax
	movl	%eax, -84(%rsp)
	testl	%eax, %eax
	jle	.L13
	movl	$5, %eax
	sarx	%eax, 40(%rbp), %edi
	movl	$1, %eax
	movl	%edi, 16(%rsp)
	sarx	%eax, 40(%rbp), %eax
	movl	%eax, -28(%rsp)
	testl	%ecx, %ecx
	jle	.L13
	movq	%rdx, %rbx
	movl	40(%rbp), %edx
	subl	$1, %ecx
	leal	(%rdi,%rdi), %eax
	movl	%ecx, -108(%rsp)
	addl	%eax, %edi
	leaq	4(%rbx), %rcx
	vpxor	%xmm0, %xmm0, %xmm0
	movl	%edx, -68(%rsp)
	sall	$2, %edx
	movq	%r8, %r12
	movq	%r9, %r13
	movl	%edx, -88(%rsp)
	movl	40(%rbp), %edx
	movl	$3000, -44(%rsp)
	leal	(%rdx,%rdx), %esi
	movl	$2000, -48(%rsp)
	movl	%esi, -64(%rsp)
	addl	%edx, %esi
	xorl	%edx, %edx
	movl	%esi, -60(%rsp)
	movl	$1000, -52(%rsp)
	movl	$0, -56(%rsp)
	movl	$0, -72(%rsp)
	movl	$0, -32(%rsp)
	movq	%rcx, -120(%rsp)
	movl	%edi, -112(%rsp)
	.p2align 4,,10
	.p2align 3
.L4:
	movslq	-44(%rsp), %rsi
	movslq	-56(%rsp), %r11
	movslq	%edx, %rdi
	movl	$0, 88(%rsp)
	movq	-120(%rsp), %rbx
	movslq	-52(%rsp), %r8
	movq	%rdi, 64(%rsp)
	movl	16(%rsp), %edi
	movq	%rsi, (%rsp)
	movq	-104(%rsp), %rsi
	movslq	-48(%rsp), %r9
	movq	%r11, 8(%rsp)
	addl	%edi, %edx
	movl	$0, 20(%rsp)
	leaq	(%rsi,%r11,4), %rsi
	movslq	%edx, %rcx
	movl	%edx, -80(%rsp)
	movslq	-68(%rsp), %rdx
	movq	%rsi, 24(%rsp)
	movl	-108(%rsp), %esi
	movq	%rcx, 56(%rsp)
	movslq	%eax, %rcx
	addl	%edi, %eax
	movslq	-60(%rsp), %rdi
	addq	%r11, %rsi
	movl	%eax, -76(%rsp)
	cltq
	leaq	(%rbx,%rsi,4), %rsi
	movq	%rax, 40(%rsp)
	movslq	-64(%rsp), %rax
	movq	%rsi, -8(%rsp)
	movq	-96(%rsp), %rsi
	movq	%rcx, 48(%rsp)
	movslq	-72(%rsp), %rcx
	leaq	(%rsi,%rax), %rbx
	movq	%r8, %rax
	leaq	(%rsi,%rdi), %r10
	subq	%r11, %rax
	leaq	(%rsi,%rdx), %r14
	addq	%rsi, %rcx
	movq	%r10, 32(%rsp)
	movq	%rax, -24(%rsp)
	movq	%r9, %rax
	movq	%rbx, %rdi
	movq	%rcx, %r10
	subq	%r11, %rax
	movq	%r14, %r11
	movq	%r13, %r14
	movq	%r12, %r13
	movq	%rax, -16(%rsp)
	.p2align 4,,10
	.p2align 3
.L9:
	movl	40(%rbp), %eax
	testl	%eax, %eax
	jle	.L16
	movslq	20(%rsp), %rax
	movq	-40(%rsp), %rdx
	xorl	%r12d, %r12d
	movq	%rdi, %rbx
	movl	$0x00000000, 120(%rsp)
	vxorps	%xmm6, %xmm6, %xmm6
	movq	%rax, 80(%rsp)
	movslq	88(%rsp), %rax
	vmovaps	%ymm6, %ymm14
	vmovaps	%ymm6, %ymm12
	movl	$0x00000000, 124(%rsp)
	vmovaps	%ymm6, %ymm11
	movl	$0x00000000, 128(%rsp)
	addq	%rdx, %rax
	movl	$0x00000000, 132(%rsp)
	movq	%rax, 72(%rsp)
	movq	%r11, %rax
	movq	%r14, %r11
	vmovaps	%ymm6, 136(%rsp)
	movq	%rax, %r14
	.p2align 4,,10
	.p2align 3
.L5:
	movl	%r12d, %eax
	movq	64(%rsp), %rcx
	movq	16(%rbp), %r9
	vmovdqa	%ymm0, %ymm9
	sarl	$5, %eax
	movq	56(%rsp), %rsi
	movq	40(%rsp), %rdx
	vmovdqa	%ymm0, %ymm15
	cltq
	leaq	(%rcx,%rax), %r8
	movq	48(%rsp), %rcx
	leaq	(%rax,%rsi), %rdi
	addq	%rax, %rdx
	leaq	(%rax,%rcx), %rsi
	addq	80(%rsp), %rax
	leaq	0(,%rax,4), %rcx
	vmovss	(%r9,%rax,4), %xmm13
	vmovss	(%r11,%rax,4), %xmm2
	vmovss	4(%r11,%rcx), %xmm1
	vmulss	4(%r15,%r8,4), %xmm1, %xmm4
	vmovss	4(%r9,%rcx), %xmm10
	vmulss	(%r15,%r8,4), %xmm2, %xmm6
	movl	%r12d, %r9d
	vmulss	(%r15,%rdi,4), %xmm2, %xmm7
	movq	72(%rsp), %rax
	sarl	%r9d
	vmulss	(%r15,%rsi,4), %xmm2, %xmm3
	movslq	%r9d, %r9
	vmulss	(%r15,%rdx,4), %xmm2, %xmm2
	vmulss	4(%r15,%rdi,4), %xmm1, %xmm5
	vmovss	%xmm4, 112(%rsp)
	vmulss	4(%r15,%rsi,4), %xmm1, %xmm8
	vmovss	%xmm6, 116(%rsp)
	vmovdqa	%ymm0, %ymm6
	vmulss	4(%r15,%rdx,4), %xmm1, %xmm4
	vmovdqu	(%rax,%r9), %ymm1
	vmovss	%xmm7, 108(%rsp)
	vmovdqa	%ymm0, %ymm7
	vmovss	%xmm3, 100(%rsp)
	vmovdqa	%ymm0, %ymm3
	vmovss	%xmm2, 92(%rsp)
	vpsrlw	$4, %ymm1, %ymm2
	vpand	.LC1(%rip), %ymm1, %ymm1
	vpand	.LC1(%rip), %ymm2, %ymm2
	vmovss	%xmm5, 104(%rsp)
	vmovdqa	%ymm0, %ymm5
	{vex} vpdpbusd	(%r14,%r12), %ymm1, %ymm5
	vmovss	%xmm8, 96(%rsp)
	{vex} vpdpbusd	(%r10,%r12), %ymm1, %ymm3
	{vex} vpdpbusd	32(%r10,%r12), %ymm2, %ymm9
	{vex} vpdpbusd	32(%r14,%r12), %ymm2, %ymm7
	movq	32(%rsp), %rax
	{vex} vpdpbusd	(%rbx,%r12), %ymm1, %ymm6
	{vex} vpdpbusd	(%rax,%r12), %ymm1, %ymm15
	vcvtdq2ps	%ymm5, %ymm8
	vmovdqa	%ymm0, %ymm5
	{vex} vpdpbusd	32(%rbx,%r12), %ymm2, %ymm5
	vcvtdq2ps	%ymm3, %ymm3
	vcvtdq2ps	%ymm9, %ymm9
	vcvtdq2ps	%ymm7, %ymm7
	vcvtdq2ps	%ymm6, %ymm6
	vcvtdq2ps	%ymm15, %ymm1
	vmovdqa	%ymm0, %ymm15
	{vex} vpdpbusd	32(%rax,%r12), %ymm2, %ymm15
	addq	$64, %r12
	vcvtdq2ps	%ymm5, %ymm5
	vcvtdq2ps	%ymm15, %ymm2
	vbroadcastss	116(%rsp), %ymm15
	vfmadd132ps	%ymm15, %ymm11, %ymm3
	vbroadcastss	112(%rsp), %ymm11
	vfmadd132ps	%ymm9, %ymm3, %ymm11
	vbroadcastss	108(%rsp), %ymm3
	vfmadd132ps	%ymm8, %ymm12, %ymm3
	vbroadcastss	104(%rsp), %ymm8
	vfmadd132ps	%ymm8, %ymm3, %ymm7
	vbroadcastss	100(%rsp), %ymm3
	vfmadd132ps	%ymm6, %ymm14, %ymm3
	vbroadcastss	96(%rsp), %ymm6
	vmovaps	%ymm7, %ymm12
	vfmadd132ps	%ymm6, %ymm3, %ymm5
	vbroadcastss	92(%rsp), %ymm3
	vfmadd213ps	136(%rsp), %ymm3, %ymm1
	vbroadcastss	%xmm4, %ymm3
	vmovss	124(%rsp), %xmm4
	vfmadd231ss	0(%r13,%rsi,4), %xmm13, %xmm4
	vfmadd231ss	4(%r13,%rsi,4), %xmm10, %xmm4
	vfmadd132ps	%ymm3, %ymm1, %ymm2
	vmovaps	%ymm5, %ymm14
	vmovss	132(%rsp), %xmm1
	vfmadd231ss	0(%r13,%r8,4), %xmm13, %xmm1
	vfmadd231ss	4(%r13,%r8,4), %xmm10, %xmm1
	vmovss	%xmm4, 124(%rsp)
	vmovss	120(%rsp), %xmm4
	vmovaps	%ymm2, 136(%rsp)
	vmovss	%xmm1, 132(%rsp)
	vmovss	128(%rsp), %xmm1
	vfmadd231ss	0(%r13,%rdi,4), %xmm13, %xmm1
	vfmadd132ss	0(%r13,%rdx,4), %xmm4, %xmm13
	vfmadd231ss	4(%r13,%rdi,4), %xmm10, %xmm1
	vfmadd132ss	4(%r13,%rdx,4), %xmm13, %xmm10
	vmovss	%xmm1, 128(%rsp)
	vmovss	%xmm10, 120(%rsp)
	cmpl	%r12d, 40(%rbp)
	jg	.L5
	movq	%r14, %rax
	vmovaps	%ymm11, %ymm4
	movq	%r11, %r14
	movq	%rbx, %rdi
	vmovaps	%ymm7, %ymm6
	movq	%rax, %r11
.L8:
	vextractf128	$0x1, %ymm4, %xmm1
	movq	24(%rsp), %rbx
	movq	-24(%rsp), %rax
	vaddps	%xmm4, %xmm1, %xmm1
	movl	16(%rsp), %esi
	movl	-28(%rsp), %ecx
	addl	%esi, 20(%rsp)
	addl	%ecx, 88(%rsp)
	vmovhlps	%xmm1, %xmm1, %xmm2
	vaddps	%xmm2, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	132(%rsp), %xmm1, %xmm1
	vaddss	(%rbx), %xmm1, %xmm1
	vmovss	%xmm1, (%rbx)
	vextractf128	$0x1, %ymm6, %xmm1
	vaddps	%xmm6, %xmm1, %xmm1
	vmovaps	136(%rsp), %ymm6
	vmovhlps	%xmm1, %xmm1, %xmm2
	vaddps	%xmm2, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	128(%rsp), %xmm1, %xmm1
	vaddss	(%rbx,%rax,4), %xmm1, %xmm1
	vmovss	%xmm1, (%rbx,%rax,4)
	vextractf128	$0x1, %ymm14, %xmm1
	movq	-16(%rsp), %rax
	vaddps	%xmm14, %xmm1, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm2
	vaddps	%xmm2, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	124(%rsp), %xmm1, %xmm1
	vaddss	(%rbx,%rax,4), %xmm1, %xmm1
	vmovss	%xmm1, (%rbx,%rax,4)
	vextractf128	$0x1, %ymm6, %xmm1
	movq	(%rsp), %rax
	subq	8(%rsp), %rax
	vaddps	%xmm6, %xmm1, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm2
	vaddps	%xmm2, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm2
	vaddss	%xmm2, %xmm1, %xmm1
	vaddss	120(%rsp), %xmm1, %xmm1
	vaddss	(%rbx,%rax,4), %xmm1, %xmm1
	vmovss	%xmm1, (%rbx,%rax,4)
	addq	$4, %rbx
	movq	%rbx, 24(%rsp)
	cmpq	%rbx, -8(%rsp)
	jne	.L9
	movl	-112(%rsp), %edi
	movl	-80(%rsp), %edx
	movq	%r13, %r12
	movq	%r14, %r13
	movl	-76(%rsp), %eax
	addl	$4, -32(%rsp)
	addl	%edi, %edx
	addl	$4000, -56(%rsp)
	movl	-32(%rsp), %ecx
	addl	%edi, %eax
	addl	$4000, -52(%rsp)
	movl	-88(%rsp), %edi
	addl	$4000, -48(%rsp)
	addl	%edi, -72(%rsp)
	addl	%edi, -68(%rsp)
	addl	%edi, -64(%rsp)
	addl	%edi, -60(%rsp)
	addl	$4000, -44(%rsp)
	cmpl	%ecx, -84(%rsp)
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
	movl	$0x00000000, 120(%rsp)
	movl	$0x00000000, 124(%rsp)
	vmovaps	%ymm7, %ymm14
	vmovaps	%ymm7, %ymm4
	vmovaps	%ymm7, %ymm6
	movl	$0x00000000, 128(%rsp)
	movl	$0x00000000, 132(%rsp)
	vmovaps	%ymm7, 136(%rsp)
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
