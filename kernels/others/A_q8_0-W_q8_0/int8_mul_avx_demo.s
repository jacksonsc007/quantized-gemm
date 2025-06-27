	.file	"int8_mul_avx_demo.c"
	.text
	.p2align 4
	.globl	kernel_xx32
	.type	kernel_xx32, @function
kernel_xx32:
.LFB5512:
	.cfi_startproc
	endbr64
	testl	%r9d, %r9d
	jle	.L2
	subl	$1, %r9d
	imull	%r9d, %ecx
	imull	%r9d, %r8d
	movslq	%ecx, %rcx
	movslq	%r8d, %r8
	vpbroadcastb	(%rsi,%rcx), %ymm4
	vmovdqu	(%rdi,%r8), %ymm3
	vpsrlq	$32, %ymm4, %ymm2
	vpsrlq	$32, %ymm3, %ymm0
	vpmuludq	%ymm3, %ymm2, %ymm2
	vpmuludq	%ymm4, %ymm0, %ymm0
	vpmuludq	%ymm4, %ymm3, %ymm1
	vpaddq	%ymm2, %ymm0, %ymm0
	vpsllq	$32, %ymm0, %ymm0
	vpaddq	%ymm0, %ymm1, %ymm0
.L2:
	vmovdqu	%ymm0, (%rdx)
	vzeroupper
	ret
	.cfi_endproc
.LFE5512:
	.size	kernel_xx32, .-kernel_xx32
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
