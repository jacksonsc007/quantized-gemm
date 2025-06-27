	.file	"tmp.c"
	.text
	.p2align 4
	.globl	kernel_2x16
	.type	kernel_2x16, @function
kernel_2x16:
.LFB5512:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movq	%r8, %rbx
	movl	24(%rbp), %r8d
	vmovups	(%rbx), %ymm4
	vmovups	32(%rbx), %ymm3
	vmovups	3200(%rbx), %ymm2
	vmovups	3232(%rbx), %ymm1
	testl	%r8d, %r8d
	jle	.L2
	movq	%rcx, %r11
	movq	%rdx, %r10
	movl	$64, %ecx
	xorl	%r9d, %r9d
	.p2align 4,,10
	.p2align 3
.L4:
	movl	%r9d, %eax
	sarl	$5, %eax
	movl	%eax, %edx
	addl	%eax, %eax
	sall	$4, %edx
	cltq
	movslq	%edx, %rdx
	vbroadcastss	(%r10,%rax,4), %ymm7
	vbroadcastss	4(%r10,%rax,4), %ymm0
	vmovups	(%r11,%rdx,4), %ymm6
	vmovups	32(%r11,%rdx,4), %ymm5
	leaq	-64(%rcx), %rdx
	vmulps	%ymm7, %ymm6, %ymm8
	vmulps	%ymm7, %ymm5, %ymm7
	vmulps	%ymm0, %ymm6, %ymm6
	vmulps	%ymm0, %ymm5, %ymm5
	.p2align 4,,10
	.p2align 3
.L3:
	movsbw	(%rdi,%rdx), %ax
	vpmovsxbw	(%rsi,%rdx,8), %ymm10
	vmovd	%eax, %xmm0
	movsbw	1(%rdi,%rdx), %ax
	addq	$2, %rdx
	vpbroadcastw	%xmm0, %ymm0
	vpmullw	%ymm10, %ymm0, %ymm0
	vpmovsxwd	%xmm0, %ymm9
	vextracti128	$0x1, %ymm0, %xmm0
	vpmovsxwd	%xmm0, %ymm0
	vcvtdq2ps	%ymm9, %ymm9
	vfmadd231ps	%ymm8, %ymm9, %ymm4
	vcvtdq2ps	%ymm0, %ymm0
	vfmadd231ps	%ymm7, %ymm0, %ymm3
	vmovd	%eax, %xmm0
	vpbroadcastw	%xmm0, %ymm0
	vpmullw	%ymm10, %ymm0, %ymm0
	vpmovsxwd	%xmm0, %ymm9
	vextracti128	$0x1, %ymm0, %xmm0
	vpmovsxwd	%xmm0, %ymm0
	vcvtdq2ps	%ymm9, %ymm9
	vfmadd231ps	%ymm6, %ymm9, %ymm2
	vcvtdq2ps	%ymm0, %ymm0
	vfmadd231ps	%ymm5, %ymm0, %ymm1
	cmpq	%rdx, %rcx
	jne	.L3
	addl	$32, %r9d
	addq	$64, %rcx
	cmpl	%r9d, %r8d
	jg	.L4
.L2:
	vmovups	%ymm4, (%rbx)
	vmovups	%ymm3, 32(%rbx)
	vmovups	%ymm2, 3200(%rbx)
	vmovups	%ymm1, 3232(%rbx)
	vzeroupper
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5512:
	.size	kernel_2x16, .-kernel_2x16
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
