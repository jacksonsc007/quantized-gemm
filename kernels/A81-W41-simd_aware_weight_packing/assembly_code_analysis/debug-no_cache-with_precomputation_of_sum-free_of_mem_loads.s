	.file	"debug-no_cache-with_precomputation_of_sum-free_of_mem_loads.c"
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
	pushq	%r13
	movl	32(%rbp), %eax
	movl	40(%rbp), %esi
	pushq	%r12
	pushq	%rbx
	.cfi_offset 13, -24
	.cfi_offset 12, -32
	.cfi_offset 3, -40
	movl	24(%rbp), %ebx
	testl	%ebx, %ebx
	jle	.L11
	testl	%eax, %eax
	jle	.L11
	vmovdqa	.LC1(%rip), %ymm8
	movq	%rdx, %r11
	xorl	%r10d, %r10d
	xorl	%r9d, %r9d
	vmovaps	.LC2(%rip), %ymm7
	vxorps	%xmm9, %xmm9, %xmm9
	leal	-1(%rax), %r13d
	leaq	4(%rdx), %r12
	vmovss	.LC3(%rip), %xmm6
	.p2align 4,,10
	.p2align 3
.L3:
	movslq	%r10d, %rdx
	movl	%r13d, %eax
	vpxor	%xmm5, %xmm5, %xmm5
	addq	%rdx, %rax
	leaq	(%r11,%rdx,4), %rdi
	leaq	(%r12,%rax,4), %r8
	.p2align 4,,10
	.p2align 3
.L7:
	xorl	%edx, %edx
	vmovaps	%xmm9, %xmm2
	vxorps	%xmm10, %xmm10, %xmm10
	testl	%esi, %esi
	jle	.L6
	.p2align 4,,10
	.p2align 3
.L4:
	leal	3(%rdx), %eax
	vmovd	%edx, %xmm4
	vmovdqa	%ymm5, %ymm1
	vmovd	%eax, %xmm0
	vpbroadcastb	%xmm4, %ymm4
	leal	1(%rdx), %ecx
	addl	$64, %edx
	vpbroadcastb	%xmm0, %ymm0
	vaddss	%xmm6, %xmm2, %xmm2
	vpsrlw	$4, %ymm0, %ymm3
	vpand	%ymm0, %ymm8, %ymm0
	{vex} vpdpbusd	%ymm4, %ymm0, %ymm1
	vmovd	%ecx, %xmm4
	vpand	%ymm3, %ymm8, %ymm3
	vpbroadcastb	%xmm4, %ymm4
	vaddss	%xmm6, %xmm2, %xmm2
	vcvtdq2ps	%ymm1, %ymm0
	vmovdqa	%ymm5, %ymm1
	vfmadd132ps	%ymm7, %ymm10, %ymm0
	{vex} vpdpbusd	%ymm4, %ymm3, %ymm1
	vcvtdq2ps	%ymm1, %ymm1
	vfmadd132ps	%ymm7, %ymm0, %ymm1
	vmovaps	%ymm1, %ymm10
	cmpl	%edx, %esi
	jg	.L4
.L6:
	vextractf128	$0x1, %ymm10, %xmm0
	addq	$4, %rdi
	vaddps	%xmm10, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	%xmm2, %xmm0, %xmm0
	vaddss	-4(%rdi), %xmm0, %xmm0
	vmovss	%xmm0, -4(%rdi)
	cmpq	%rdi, %r8
	jne	.L7
	addl	$1, %r9d
	addl	$1000, %r10d
	cmpl	%r9d, %ebx
	jne	.L3
	vzeroupper
.L11:
	popq	%rbx
	popq	%r12
	popq	%r13
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
	.align 32
.LC2:
	.long	980151802
	.long	980151802
	.long	980151802
	.long	980151802
	.long	980151802
	.long	980151802
	.long	980151802
	.long	980151802
	.set	.LC3,.LC2
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
