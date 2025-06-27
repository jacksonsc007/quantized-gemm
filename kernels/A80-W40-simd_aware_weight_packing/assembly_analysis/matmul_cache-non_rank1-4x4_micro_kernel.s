	.file	"matmul_cache-non_rank1-4x4_micro_kernel.c"
	.text
	.p2align 4
	.globl	kernel
	.type	kernel, @function
kernel:
.LFB5517:
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
	subq	$552, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%r8, -96(%rsp)
	movl	24(%rbp), %eax
	testl	%eax, %eax
	jle	.L4
	movq	%rsi, %r11
	movl	24(%rbp), %esi
	movslq	24(%rbp), %rbx
	vxorps	%xmm5, %xmm5, %xmm5
	movq	%rdi, %rax
	movq	%r11, %r13
	xorl	%edi, %edi
	vmovaps	%ymm5, -56(%rsp)
	sarl	$5, %esi
	leal	(%rbx,%rbx), %r14d
	movq	%rbx, 456(%rsp)
	vpxor	%xmm11, %xmm11, %xmm11
	movslq	%esi, %r12
	movl	%ebx, %esi
	movslq	%r14d, %r15
	addl	%ebx, %r14d
	sarl	%esi
	movslq	%r14d, %r14
	vmovaps	%ymm5, 328(%rsp)
	movslq	%esi, %rsi
	vmovaps	%ymm5, -24(%rsp)
	movq	%rsi, 472(%rsp)
	addq	%r11, %rsi
	movq	%r12, %r11
	movq	%r15, %r12
	movq	%rsi, 464(%rsp)
	vmovaps	%ymm5, 360(%rsp)
	vmovaps	%ymm5, 392(%rsp)
	vmovaps	%ymm5, 424(%rsp)
	vmovaps	%ymm5, 8(%rsp)
	vmovaps	%ymm5, 40(%rsp)
	vmovaps	%ymm5, 72(%rsp)
	vmovaps	%ymm5, 104(%rsp)
	vmovaps	%ymm5, 136(%rsp)
	vmovaps	%ymm5, 168(%rsp)
	vmovaps	%ymm5, 200(%rsp)
	vmovaps	%ymm5, 232(%rsp)
	vmovaps	%ymm5, 264(%rsp)
	vmovaps	%ymm5, 296(%rsp)
	.p2align 4,,10
	.p2align 3
.L3:
	movl	%edi, %esi
	addq	$64, %rax
	sarl	$5, %esi
	movslq	%esi, %rsi
	leaq	(%rsi,%r11), %r9
	vmovss	(%rcx,%rsi,4), %xmm12
	vmovss	(%rdx,%rsi,4), %xmm13
	leaq	0(,%rsi,4), %r15
	leaq	(%r11,%r9), %r8
	leaq	0(,%r9,4), %r10
	vmovss	(%rdx,%r9,4), %xmm15
	vmovss	(%rcx,%r9,4), %xmm3
	movq	%r10, 520(%rsp)
	movq	520(%rsp), %rsi
	vmovss	4(%rdx,%r10), %xmm14
	leaq	0(,%r8,4), %r10
	vmovss	(%rdx,%r8,4), %xmm5
	vmovss	%xmm15, 548(%rsp)
	movq	%r10, 512(%rsp)
	vmovss	4(%rcx,%rsi), %xmm2
	movq	512(%rsp), %rsi
	vmovss	4(%rdx,%r10), %xmm6
	leaq	(%r11,%r8), %r10
	vmovss	%xmm5, 540(%rsp)
	leaq	0(,%r10,4), %rbx
	vmovss	(%rcx,%r8,4), %xmm0
	vmovss	(%rdx,%r10,4), %xmm7
	vmovss	%xmm14, 544(%rsp)
	vmovss	4(%rcx,%rsi), %xmm8
	movl	%edi, %esi
	vmovss	4(%rdx,%rbx), %xmm1
	vmovss	%xmm6, 536(%rsp)
	vmovss	(%rcx,%r10,4), %xmm5
	sarl	%esi
	vmovdqa	%ymm11, %ymm14
	addl	$64, %edi
	vmovss	4(%rcx,%rbx), %xmm6
	movslq	%esi, %rsi
	vmovss	4(%rdx,%r15), %xmm4
	vmovss	%xmm7, 496(%rsp)
	vmovss	%xmm1, 492(%rsp)
	vmovss	4(%rcx,%r15), %xmm9
	vmovss	%xmm2, 520(%rsp)
	vmovss	%xmm0, 528(%rsp)
	vmovss	%xmm8, 512(%rsp)
	vmovss	%xmm5, 508(%rsp)
	vmovss	%xmm6, 504(%rsp)
	vmovss	%xmm3, 532(%rsp)
	vmovdqu	0(%r13,%rsi), %ymm3
	movq	472(%rsp), %rbx
	vmovss	%xmm13, 500(%rsp)
	vmulss	%xmm13, %xmm12, %xmm13
	addq	%rbx, %rsi
	vpsrlw	$4, %ymm3, %ymm7
	vmovss	%xmm9, 484(%rsp)
	vpand	.LC0(%rip), %ymm3, %ymm3
	vmovdqu	0(%r13,%rsi), %ymm2
	addq	%rbx, %rsi
	movq	464(%rsp), %rbx
	vmovss	%xmm12, 488(%rsp)
	vpaddb	.LC1(%rip), %ymm3, %ymm3
	vmovdqu	0(%r13,%rsi), %ymm1
	vmovdqu	(%rbx,%rsi), %ymm0
	vpand	.LC0(%rip), %ymm7, %ymm7
	vpsrlw	$4, %ymm2, %ymm6
	vmovd	%xmm13, %r8d
	vpaddb	.LC1(%rip), %ymm7, %ymm7
	vpand	.LC0(%rip), %ymm2, %ymm2
	vmulss	%xmm9, %xmm4, %xmm13
	vmovdqu	-64(%rax), %ymm9
	vpsrlw	$4, %ymm0, %ymm8
	vpaddb	.LC1(%rip), %ymm2, %ymm2
	vmovdqa	%ymm8, -88(%rsp)
	vmovdqa	%ymm11, %ymm8
	vpsrlw	$4, %ymm1, %ymm5
	vpand	.LC0(%rip), %ymm6, %ymm6
	vpsignb	%ymm9, %ymm3, %ymm12
	vpaddb	.LC1(%rip), %ymm6, %ymm6
	vpand	.LC0(%rip), %ymm1, %ymm1
	vpaddb	.LC1(%rip), %ymm1, %ymm1
	vmovd	%xmm13, %esi
	vpsignb	%ymm9, %ymm9, %ymm13
	{vex} vpdpbusd	%ymm12, %ymm13, %ymm8
	vcvtdq2ps	%ymm8, %ymm15
	vmovdqu	-32(%rax), %ymm8
	vpsignb	%ymm8, %ymm8, %ymm12
	vpsignb	%ymm8, %ymm7, %ymm10
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	296(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vmulss	520(%rsp), %xmm4, %xmm15
	vmovaps	%ymm14, 296(%rsp)
	vmovss	500(%rsp), %xmm14
	vmulss	532(%rsp), %xmm14, %xmm10
	vmovdqa	%ymm11, %ymm14
	vmovd	%xmm15, %esi
	vpsignb	%ymm9, %ymm2, %ymm15
	vmovd	%xmm10, %r8d
	vmovdqa	%ymm11, %ymm10
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm10
	vcvtdq2ps	%ymm10, %ymm15
	vpsignb	%ymm8, %ymm6, %ymm10
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	264(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vmovdqa	%ymm11, %ymm10
	vmovaps	%ymm14, 264(%rsp)
	vmovss	500(%rsp), %xmm14
	vmulss	528(%rsp), %xmm14, %xmm15
	vmovdqa	%ymm11, %ymm14
	vmovd	%xmm15, %r8d
	vmulss	512(%rsp), %xmm4, %xmm15
	vpand	.LC0(%rip), %ymm5, %ymm5
	vpaddb	.LC1(%rip), %ymm5, %ymm5
	vpand	.LC0(%rip), %ymm0, %ymm0
	vpaddb	.LC1(%rip), %ymm0, %ymm0
	movq	456(%rsp), %rbx
	vmovd	%xmm15, %esi
	vpsignb	%ymm9, %ymm1, %ymm15
	vpsignb	%ymm9, %ymm0, %ymm9
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm10
	vcvtdq2ps	%ymm10, %ymm15
	vpsignb	%ymm8, %ymm5, %ymm10
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	232(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vmovaps	%ymm14, 232(%rsp)
	vmovss	500(%rsp), %xmm14
	vmulss	508(%rsp), %xmm14, %xmm15
	vmulss	504(%rsp), %xmm4, %xmm14
	vmovdqa	%ymm11, %ymm4
	{vex} vpdpbusd	%ymm9, %ymm13, %ymm4
	vmovdqa	%ymm11, %ymm13
	vcvtdq2ps	%ymm4, %ymm10
	vmovdqa	-88(%rsp), %ymm4
	vpand	.LC0(%rip), %ymm4, %ymm4
	vpaddb	.LC1(%rip), %ymm4, %ymm4
	vpsignb	%ymm8, %ymm4, %ymm8
	{vex} vpdpbusd	%ymm8, %ymm12, %ymm13
	vbroadcastss	%xmm15, %ymm8
	vfmadd213ps	200(%rsp), %ymm10, %ymm8
	vbroadcastss	%xmm14, %ymm10
	vmovss	488(%rsp), %xmm12
	vmovdqa	%ymm11, %ymm14
	vcvtdq2ps	%ymm13, %ymm9
	vmovaps	%ymm9, %ymm13
	vmovss	484(%rsp), %xmm9
	vfmadd132ps	%ymm10, %ymm8, %ymm13
	vmovdqa	%ymm11, %ymm8
	vmovaps	%ymm13, 200(%rsp)
	vmulss	548(%rsp), %xmm12, %xmm13
	vmovd	%xmm13, %r8d
	vmulss	544(%rsp), %xmm9, %xmm13
	vmovdqu	-64(%rax,%rbx), %ymm9
	vpsignb	%ymm9, %ymm3, %ymm12
	vmovd	%xmm13, %esi
	vpsignb	%ymm9, %ymm9, %ymm13
	{vex} vpdpbusd	%ymm12, %ymm13, %ymm8
	vcvtdq2ps	%ymm8, %ymm15
	vmovdqu	-32(%rax,%rbx), %ymm8
	vpsignb	%ymm8, %ymm8, %ymm12
	vpsignb	%ymm8, %ymm7, %ymm10
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	168(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vpsignb	%ymm8, %ymm6, %ymm10
	vmovss	548(%rsp), %xmm15
	vmovaps	%ymm14, 168(%rsp)
	vmulss	532(%rsp), %xmm15, %xmm14
	vpsignb	%ymm9, %ymm2, %ymm15
	vmovd	%xmm14, %r8d
	vmovss	544(%rsp), %xmm14
	vmulss	520(%rsp), %xmm14, %xmm14
	vmovd	%xmm14, %esi
	vmovdqa	%ymm11, %ymm14
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm14
	vcvtdq2ps	%ymm14, %ymm15
	vmovdqa	%ymm11, %ymm14
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	136(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vpsignb	%ymm8, %ymm5, %ymm10
	vmovss	548(%rsp), %xmm15
	vpsignb	%ymm8, %ymm4, %ymm8
	vmovaps	%ymm14, 136(%rsp)
	vmulss	528(%rsp), %xmm15, %xmm14
	vpsignb	%ymm9, %ymm1, %ymm15
	vpsignb	%ymm9, %ymm0, %ymm9
	vmovd	%xmm14, %r8d
	vmovss	544(%rsp), %xmm14
	vmulss	512(%rsp), %xmm14, %xmm14
	vmovd	%xmm14, %esi
	vmovdqa	%ymm11, %ymm14
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm14
	vcvtdq2ps	%ymm14, %ymm15
	vmovdqa	%ymm11, %ymm14
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	104(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vmovdqa	%ymm11, %ymm10
	vmovss	548(%rsp), %xmm15
	{vex} vpdpbusd	%ymm9, %ymm13, %ymm10
	vmovdqa	%ymm11, %ymm13
	vmulss	508(%rsp), %xmm15, %xmm15
	{vex} vpdpbusd	%ymm8, %ymm12, %ymm13
	vmovss	488(%rsp), %xmm12
	vmovaps	%ymm14, 104(%rsp)
	vmovss	544(%rsp), %xmm14
	vmulss	504(%rsp), %xmm14, %xmm14
	vbroadcastss	%xmm15, %ymm8
	vcvtdq2ps	%ymm10, %ymm10
	vfmadd213ps	72(%rsp), %ymm10, %ymm8
	vcvtdq2ps	%ymm13, %ymm9
	vmovaps	%ymm9, %ymm13
	vmovss	484(%rsp), %xmm9
	vbroadcastss	%xmm14, %ymm10
	vmovdqa	%ymm11, %ymm14
	vfmadd132ps	%ymm10, %ymm8, %ymm13
	vmulss	536(%rsp), %xmm9, %xmm8
	vmovdqu	-64(%rax,%r12), %ymm9
	vmovaps	%ymm13, 72(%rsp)
	vmulss	540(%rsp), %xmm12, %xmm13
	vpsignb	%ymm9, %ymm3, %ymm12
	vmovd	%xmm8, %esi
	vmovdqa	%ymm11, %ymm8
	vmovd	%xmm13, %r8d
	vpsignb	%ymm9, %ymm9, %ymm13
	{vex} vpdpbusd	%ymm12, %ymm13, %ymm8
	vcvtdq2ps	%ymm8, %ymm15
	vmovdqu	-32(%rax,%r12), %ymm8
	vpsignb	%ymm8, %ymm8, %ymm12
	vpsignb	%ymm8, %ymm7, %ymm10
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	40(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vpsignb	%ymm8, %ymm6, %ymm10
	vmovss	540(%rsp), %xmm15
	vmovaps	%ymm14, 40(%rsp)
	vmulss	532(%rsp), %xmm15, %xmm14
	vmovd	%xmm14, %r8d
	vmovss	536(%rsp), %xmm14
	vmulss	520(%rsp), %xmm14, %xmm15
	vmovdqa	%ymm11, %ymm14
	vmovd	%xmm15, %esi
	vpsignb	%ymm9, %ymm2, %ymm15
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm14
	vcvtdq2ps	%ymm14, %ymm15
	vmovdqa	%ymm11, %ymm14
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	8(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vpsignb	%ymm8, %ymm5, %ymm10
	vmovss	540(%rsp), %xmm15
	vpsignb	%ymm8, %ymm4, %ymm8
	vmovaps	%ymm14, 8(%rsp)
	vmulss	528(%rsp), %xmm15, %xmm14
	vmovd	%xmm14, %r8d
	vmovss	536(%rsp), %xmm14
	vmulss	512(%rsp), %xmm14, %xmm15
	vmovdqa	%ymm11, %ymm14
	vmovd	%xmm15, %esi
	vpsignb	%ymm9, %ymm1, %ymm15
	vpsignb	%ymm9, %ymm0, %ymm9
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm14
	vcvtdq2ps	%ymm14, %ymm15
	vmovdqa	%ymm11, %ymm14
	{vex} vpdpbusd	%ymm10, %ymm12, %ymm14
	vmovd	%r8d, %xmm10
	vbroadcastss	%xmm10, %ymm10
	vfmadd213ps	-24(%rsp), %ymm15, %ymm10
	vmovd	%esi, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm10, %ymm14
	vmovdqa	%ymm11, %ymm10
	vmovss	540(%rsp), %xmm15
	{vex} vpdpbusd	%ymm9, %ymm13, %ymm10
	vmovdqa	%ymm11, %ymm13
	vmulss	508(%rsp), %xmm15, %xmm15
	{vex} vpdpbusd	%ymm8, %ymm12, %ymm13
	vmovaps	%ymm14, -24(%rsp)
	vmovss	536(%rsp), %xmm14
	vmulss	504(%rsp), %xmm14, %xmm14
	vmovss	488(%rsp), %xmm12
	vmulss	496(%rsp), %xmm12, %xmm12
	vbroadcastss	%xmm15, %ymm8
	vcvtdq2ps	%ymm10, %ymm10
	vfmadd213ps	-56(%rsp), %ymm10, %ymm8
	vcvtdq2ps	%ymm13, %ymm9
	vmovaps	%ymm9, %ymm13
	vmovss	484(%rsp), %xmm9
	vmulss	492(%rsp), %xmm9, %xmm15
	vbroadcastss	%xmm14, %ymm10
	vmovdqu	-64(%rax,%r14), %ymm9
	vmovss	492(%rsp), %xmm14
	vmovd	%xmm12, %esi
	vfmadd132ps	%ymm10, %ymm8, %ymm13
	vpsignb	%ymm9, %ymm9, %ymm12
	vpsignb	%ymm9, %ymm3, %ymm8
	vmovdqa	%ymm11, %ymm3
	vpsignb	%ymm9, %ymm0, %ymm0
	vmulss	520(%rsp), %xmm14, %xmm14
	vmovaps	%ymm13, -56(%rsp)
	vmovdqa	%ymm11, %ymm13
	{vex} vpdpbusd	%ymm8, %ymm12, %ymm13
	vmovdqu	-32(%rax,%r14), %ymm8
	vpsignb	%ymm8, %ymm8, %ymm10
	vpsignb	%ymm8, %ymm7, %ymm7
	vpsignb	%ymm8, %ymm5, %ymm5
	{vex} vpdpbusd	%ymm7, %ymm10, %ymm3
	vpsignb	%ymm8, %ymm4, %ymm4
	vcvtdq2ps	%ymm13, %ymm13
	vcvtdq2ps	%ymm3, %ymm7
	vmovd	%esi, %xmm3
	vbroadcastss	%xmm3, %ymm3
	vfmadd213ps	424(%rsp), %ymm13, %ymm3
	vbroadcastss	%xmm15, %ymm13
	vmovss	496(%rsp), %xmm15
	vmulss	532(%rsp), %xmm15, %xmm15
	vfmadd132ps	%ymm13, %ymm3, %ymm7
	vpsignb	%ymm9, %ymm2, %ymm3
	vpsignb	%ymm8, %ymm6, %ymm13
	vbroadcastss	%xmm15, %ymm2
	vbroadcastss	%xmm14, %ymm6
	vmovss	496(%rsp), %xmm15
	vmovss	492(%rsp), %xmm14
	vmovaps	%ymm7, 424(%rsp)
	vmovdqa	%ymm11, %ymm7
	{vex} vpdpbusd	%ymm3, %ymm12, %ymm7
	vmovdqa	%ymm11, %ymm3
	{vex} vpdpbusd	%ymm13, %ymm10, %ymm3
	vmulss	528(%rsp), %xmm15, %xmm13
	vcvtdq2ps	%ymm7, %ymm7
	vfmadd213ps	392(%rsp), %ymm7, %ymm2
	vmulss	512(%rsp), %xmm14, %xmm7
	vcvtdq2ps	%ymm3, %ymm3
	vfmadd132ps	%ymm6, %ymm2, %ymm3
	vpsignb	%ymm9, %ymm1, %ymm2
	vmovdqa	%ymm11, %ymm6
	{vex} vpdpbusd	%ymm2, %ymm12, %ymm6
	vmovdqa	%ymm11, %ymm2
	vbroadcastss	%xmm13, %ymm1
	{vex} vpdpbusd	%ymm5, %ymm10, %ymm2
	vmovaps	%ymm3, 392(%rsp)
	vcvtdq2ps	%ymm6, %ymm3
	vfmadd213ps	360(%rsp), %ymm3, %ymm1
	vbroadcastss	%xmm7, %ymm3
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd132ps	%ymm3, %ymm1, %ymm2
	vmovdqa	%ymm11, %ymm1
	{vex} vpdpbusd	%ymm0, %ymm12, %ymm1
	vmovdqa	%ymm11, %ymm0
	{vex} vpdpbusd	%ymm4, %ymm10, %ymm0
	vmulss	504(%rsp), %xmm14, %xmm3
	vmovaps	%ymm2, 360(%rsp)
	vmulss	508(%rsp), %xmm15, %xmm2
	vcvtdq2ps	%ymm1, %ymm1
	vcvtdq2ps	%ymm0, %ymm0
	vbroadcastss	%xmm2, %ymm2
	vfmadd213ps	328(%rsp), %ymm2, %ymm1
	vbroadcastss	%xmm3, %ymm2
	vfmadd132ps	%ymm2, %ymm1, %ymm0
	vmovaps	%ymm0, 328(%rsp)
	cmpl	%edi, 24(%rbp)
	jg	.L3
	vmovaps	%ymm0, %ymm7
.L2:
	vmovaps	296(%rsp), %ymm5
	movq	-96(%rsp), %rax
	vmovaps	232(%rsp), %ymm4
	vmovaps	-56(%rsp), %ymm6
	vextractf128	$0x1, %ymm5, %xmm1
	vaddps	%xmm5, %xmm1, %xmm0
	vextractf128	$0x1, %ymm4, %xmm3
	vmovaps	264(%rsp), %ymm5
	vextractf128	$0x1, %ymm5, %xmm2
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm1
	vmovshdup	%xmm1, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	vaddps	%xmm5, %xmm2, %xmm0
	vmovaps	200(%rsp), %ymm5
	vmovhlps	%xmm0, %xmm0, %xmm2
	vaddps	%xmm2, %xmm0, %xmm2
	vmovshdup	%xmm2, %xmm0
	vaddss	%xmm0, %xmm2, %xmm2
	vaddps	%xmm4, %xmm3, %xmm0
	vextractf128	$0x1, %ymm5, %xmm4
	vmovhlps	%xmm0, %xmm0, %xmm3
	vaddps	%xmm3, %xmm0, %xmm3
	vmovshdup	%xmm3, %xmm0
	vaddss	%xmm0, %xmm3, %xmm3
	vaddps	%xmm5, %xmm4, %xmm0
	vmovaps	136(%rsp), %ymm5
	vmovhlps	%xmm0, %xmm0, %xmm4
	vaddps	%xmm4, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm4
	vaddss	%xmm4, %xmm0, %xmm4
	vunpcklps	%xmm2, %xmm1, %xmm0
	vunpcklps	%xmm4, %xmm3, %xmm3
	vmovaps	168(%rsp), %ymm4
	vmovlhps	%xmm3, %xmm0, %xmm0
	vaddps	(%rax), %xmm0, %xmm0
	vextractf128	$0x1, %ymm5, %xmm3
	vextractf128	$0x1, %ymm4, %xmm1
	vaddps	%xmm5, %xmm3, %xmm3
	vmovaps	72(%rsp), %ymm5
	vaddps	%xmm4, %xmm1, %xmm1
	vmovaps	104(%rsp), %ymm4
	vmovups	%xmm0, (%rax)
	vextractf128	$0x1, %ymm4, %xmm2
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm4, %xmm2, %xmm2
	vextractf128	$0x1, %ymm5, %xmm4
	vaddps	%xmm0, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	vmovhlps	%xmm3, %xmm3, %xmm0
	vaddps	%xmm0, %xmm3, %xmm3
	vmovshdup	%xmm3, %xmm0
	vaddss	%xmm0, %xmm3, %xmm3
	vmovhlps	%xmm2, %xmm2, %xmm0
	vaddps	%xmm0, %xmm2, %xmm2
	vmovshdup	%xmm2, %xmm0
	vaddss	%xmm0, %xmm2, %xmm2
	vaddps	%xmm5, %xmm4, %xmm0
	vmovaps	8(%rsp), %ymm5
	vmovhlps	%xmm0, %xmm0, %xmm4
	vaddps	%xmm4, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm4
	vaddss	%xmm4, %xmm0, %xmm0
	vmovaps	40(%rsp), %ymm4
	vunpcklps	%xmm0, %xmm2, %xmm2
	vunpcklps	%xmm3, %xmm1, %xmm0
	vextractf128	$0x1, %ymm4, %xmm1
	vaddps	%xmm4, %xmm1, %xmm1
	vmovlhps	%xmm2, %xmm0, %xmm0
	vextractf128	$0x1, %ymm5, %xmm3
	vmovaps	-24(%rsp), %ymm4
	vaddps	4000(%rax), %xmm0, %xmm0
	vaddps	%xmm5, %xmm3, %xmm3
	vmovaps	424(%rsp), %ymm5
	vextractf128	$0x1, %ymm4, %xmm2
	vmovups	%xmm0, 4000(%rax)
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	vmovhlps	%xmm3, %xmm3, %xmm0
	vaddps	%xmm0, %xmm3, %xmm3
	vmovshdup	%xmm3, %xmm0
	vaddss	%xmm0, %xmm3, %xmm3
	vaddps	%xmm4, %xmm2, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm2
	vaddps	%xmm2, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm2
	vaddss	%xmm2, %xmm0, %xmm4
	vextractf128	$0x1, %ymm6, %xmm2
	vaddps	%xmm6, %xmm2, %xmm0
	vmovaps	360(%rsp), %ymm6
	vmovhlps	%xmm0, %xmm0, %xmm2
	vaddps	%xmm2, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm2
	vaddss	%xmm2, %xmm0, %xmm0
	vunpcklps	%xmm0, %xmm4, %xmm4
	vunpcklps	%xmm3, %xmm1, %xmm0
	vextractf128	$0x1, %ymm5, %xmm1
	vaddps	%xmm5, %xmm1, %xmm1
	vmovlhps	%xmm4, %xmm0, %xmm0
	vaddps	8000(%rax), %xmm0, %xmm0
	vmovaps	392(%rsp), %ymm4
	vextractf128	$0x1, %ymm7, %xmm3
	vextractf128	$0x1, %ymm4, %xmm2
	vmovups	%xmm0, 8000(%rax)
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm1
	vmovshdup	%xmm1, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	vaddps	%xmm4, %xmm2, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm2
	vaddps	%xmm2, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm2
	vaddss	%xmm2, %xmm0, %xmm4
	vextractf128	$0x1, %ymm6, %xmm2
	vaddps	%xmm6, %xmm2, %xmm0
	vunpcklps	%xmm4, %xmm1, %xmm1
	vmovhlps	%xmm0, %xmm0, %xmm2
	vaddps	%xmm2, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm2
	vaddss	%xmm2, %xmm0, %xmm2
	vaddps	%xmm7, %xmm3, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm3
	vaddps	%xmm3, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm3
	vaddss	%xmm3, %xmm0, %xmm3
	vunpcklps	%xmm3, %xmm2, %xmm0
	vmovlhps	%xmm0, %xmm1, %xmm0
	vaddps	12000(%rax), %xmm0, %xmm0
	vmovups	%xmm0, 12000(%rax)
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
	.p2align 4,,10
	.p2align 3
.L4:
	.cfi_restore_state
	vxorps	%xmm7, %xmm7, %xmm7
	vmovaps	%ymm7, 328(%rsp)
	vmovaps	%ymm7, 360(%rsp)
	vmovaps	%ymm7, 392(%rsp)
	vmovaps	%ymm7, 424(%rsp)
	vmovaps	%ymm7, -56(%rsp)
	vmovaps	%ymm7, -24(%rsp)
	vmovaps	%ymm7, 8(%rsp)
	vmovaps	%ymm7, 40(%rsp)
	vmovaps	%ymm7, 72(%rsp)
	vmovaps	%ymm7, 104(%rsp)
	vmovaps	%ymm7, 136(%rsp)
	vmovaps	%ymm7, 168(%rsp)
	vmovaps	%ymm7, 200(%rsp)
	vmovaps	%ymm7, 232(%rsp)
	vmovaps	%ymm7, 264(%rsp)
	vmovaps	%ymm7, 296(%rsp)
	jmp	.L2
	.cfi_endproc
.LFE5517:
	.size	kernel, .-kernel
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.quad	1085102592571150095
	.align 32
.LC1:
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
	.byte	-8
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
