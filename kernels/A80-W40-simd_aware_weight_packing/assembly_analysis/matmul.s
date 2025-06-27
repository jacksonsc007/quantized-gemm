	.file	"matmul.c"
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
	subq	$1280, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rsi, 120(%rsp)
	movl	16(%rbp), %esi
	movq	%rdi, 16(%rsp)
	movl	%r9d, 48(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 1272(%rsp)
	xorl	%eax, %eax
	testl	%esi, %esi
	jle	.L1
	movl	24(%rbp), %edi
	movq	%rdx, %rsi
	movq	%r8, %r14
	vpxor	%xmm8, %xmm8, %xmm8
	movl	$0, 56(%rsp)
	movl	$0, 672(%rsp)
	sarl	$5, %edi
	movslq	%edi, %rax
	movl	%edi, 28(%rsp)
	movq	%rax, 88(%rsp)
	leal	(%rdi,%rdi), %eax
	movslq	%eax, %rbx
	movl	%eax, 60(%rsp)
	addl	%edi, %eax
	cltq
	movq	%rbx, 80(%rsp)
	movslq	24(%rbp), %rbx
	movq	%rax, 72(%rsp)
	movl	24(%rbp), %eax
	movq	$0, 64(%rsp)
	addl	%eax, %eax
	movslq	%eax, %r13
	addl	24(%rbp), %eax
	movslq	%eax, %r12
	leal	0(,%rdi,4), %eax
	movl	%eax, 36(%rsp)
	movl	24(%rbp), %eax
	sarl	%eax
	leal	0(,%rax,4), %edi
	movl	%eax, 24(%rsp)
	addl	%eax, %eax
	movl	%edi, 32(%rsp)
	movl	%eax, 52(%rsp)
.L3:
	movl	24(%rbp), %eax
	vxorps	%xmm4, %xmm4, %xmm4
	vmovaps	%ymm4, 736(%rsp)
	vmovaps	%ymm4, 768(%rsp)
	vmovaps	%ymm4, 800(%rsp)
	vmovaps	%ymm4, 832(%rsp)
	vmovaps	%ymm4, 864(%rsp)
	vmovaps	%ymm4, 896(%rsp)
	vmovaps	%ymm4, 928(%rsp)
	vmovaps	%ymm4, 960(%rsp)
	vmovaps	%ymm4, 992(%rsp)
	vmovaps	%ymm4, 1024(%rsp)
	vmovaps	%ymm4, 1056(%rsp)
	vmovaps	%ymm4, 1088(%rsp)
	vmovaps	%ymm4, 1120(%rsp)
	vmovaps	%ymm4, 1152(%rsp)
	vmovaps	%ymm4, 1184(%rsp)
	vmovaps	%ymm4, 1216(%rsp)
	testl	%eax, %eax
	jle	.L9
	movl	28(%rsp), %eax
	movslq	60(%rsp), %r10
	vxorps	%xmm4, %xmm4, %xmm4
	movq	%r14, 40(%rsp)
	movslq	56(%rsp), %r15
	movl	24(%rsp), %edi
	vmovaps	%ymm4, 160(%rsp)
	movl	%eax, %r9d
	addl	%r10d, %eax
	movslq	672(%rsp), %r11
	vmovaps	%ymm4, 192(%rsp)
	vmovaps	%ymm4, 224(%rsp)
	movslq	%eax, %r8
	movq	%r15, %rax
	vmovaps	%ymm4, 256(%rsp)
	addl	%edi, %eax
	addl	%r11d, %r9d
	vmovaps	%ymm4, 288(%rsp)
	cltq
	movslq	%r9d, %r9
	movq	%rax, 112(%rsp)
	movslq	52(%rsp), %rax
	vmovaps	%ymm4, 320(%rsp)
	movq	%rax, %rdx
	movq	%rax, 104(%rsp)
	movl	%edi, %eax
	xorl	%edi, %edi
	vmovaps	%ymm4, 352(%rsp)
	addl	%edx, %eax
	movq	16(%rsp), %rdx
	vmovaps	%ymm4, 384(%rsp)
	cltq
	movq	%rax, 96(%rsp)
	movq	%r15, %rax
	movq	120(%rsp), %r15
	vmovaps	%ymm4, 416(%rsp)
	vmovaps	%ymm4, 448(%rsp)
	addq	%r15, %rax
	vmovaps	%ymm4, 480(%rsp)
	movq	%rax, %r14
	vmovaps	%ymm4, 512(%rsp)
	vmovaps	%ymm4, 544(%rsp)
	vmovaps	%ymm4, 576(%rsp)
	vmovaps	%ymm4, 608(%rsp)
	vmovaps	%ymm4, 640(%rsp)
	.p2align 4,,10
	.p2align 3
.L4:
	movl	%edi, %eax
	movq	88(%rsp), %r15
	addq	$64, %rdx
	sarl	$5, %eax
	cltq
	addq	%rax, %r15
	vmovss	4(%rsi,%rax,4), %xmm4
	vmovss	(%rsi,%rax,4), %xmm15
	vmovss	(%rsi,%r15,4), %xmm13
	vmovss	4(%rsi,%r15,4), %xmm5
	movq	80(%rsp), %r15
	vmovss	%xmm5, 704(%rsp)
	vmovss	%xmm13, 708(%rsp)
	addq	%rax, %r15
	vmovss	(%rsi,%r15,4), %xmm6
	vmovss	4(%rsi,%r15,4), %xmm14
	movq	72(%rsp), %r15
	vmovss	%xmm6, 700(%rsp)
	vmovss	%xmm14, 696(%rsp)
	addq	%rax, %r15
	vmovdqa	%ymm8, %ymm14
	vmovss	(%rsi,%r15,4), %xmm7
	vmovss	4(%rsi,%r15,4), %xmm11
	leaq	(%r11,%rax), %r15
	vmovss	(%rcx,%r15,4), %xmm10
	vmovss	4(%rcx,%r15,4), %xmm12
	leaq	(%r9,%rax), %r15
	vmovss	(%rcx,%r15,4), %xmm2
	vmovss	4(%rcx,%r15,4), %xmm1
	leaq	(%r10,%rax), %r15
	addq	%r8, %rax
	vmovss	(%rcx,%rax,4), %xmm9
	vmovss	4(%rcx,%rax,4), %xmm5
	movl	%edi, %eax
	vmovss	%xmm7, 676(%rsp)
	vmovss	(%rcx,%r15,4), %xmm3
	sarl	%eax
	addl	$64, %edi
	vmovss	4(%rcx,%r15,4), %xmm0
	vmovss	%xmm11, 680(%rsp)
	cltq
	vmovss	%xmm2, 732(%rsp)
	vmovss	%xmm1, 728(%rsp)
	vmovss	%xmm3, 724(%rsp)
	vmovdqu	(%r14,%rax), %ymm3
	vmovss	%xmm0, 720(%rsp)
	vmovss	%xmm9, 716(%rsp)
	vpsrlw	$4, %ymm3, %ymm7
	vmovss	%xmm5, 712(%rsp)
	addq	120(%rsp), %rax
	movq	112(%rsp), %r15
	vpand	.LC0(%rip), %ymm3, %ymm3
	vmovss	%xmm10, 692(%rsp)
	vpaddb	.LC1(%rip), %ymm3, %ymm3
	vmovdqu	(%rax,%r15), %ymm2
	movq	104(%rsp), %r15
	vmovss	%xmm12, 688(%rsp)
	vmovss	%xmm15, 684(%rsp)
	vpand	.LC0(%rip), %ymm7, %ymm7
	vpaddb	.LC1(%rip), %ymm7, %ymm7
	vmovdqu	(%rax,%r15), %ymm1
	movq	96(%rsp), %r15
	vpsrlw	$4, %ymm2, %ymm6
	vpand	.LC0(%rip), %ymm2, %ymm2
	vpaddb	.LC1(%rip), %ymm2, %ymm2
	vmovdqu	(%rax,%r15), %ymm0
	vpand	.LC0(%rip), %ymm6, %ymm6
	vpsrlw	$4, %ymm1, %ymm5
	vpaddb	.LC1(%rip), %ymm6, %ymm6
	vpsrlw	$4, %ymm0, %ymm9
	vmovdqa	%ymm9, 128(%rsp)
	vmulss	%xmm15, %xmm10, %xmm9
	vmovdqu	-64(%rdx), %ymm10
	vpsignb	%ymm10, %ymm10, %ymm13
	vmovd	%xmm9, %r15d
	vmulss	%xmm12, %xmm4, %xmm9
	vpsignb	%ymm10, %ymm3, %ymm12
	vmovd	%xmm9, %eax
	vmovdqa	%ymm8, %ymm9
	{vex} vpdpbusd	%ymm12, %ymm13, %ymm9
	vcvtdq2ps	%ymm9, %ymm15
	vmovdqu	-32(%rdx), %ymm9
	vpsignb	%ymm9, %ymm9, %ymm12
	vpsignb	%ymm9, %ymm7, %ymm11
	{vex} vpdpbusd	%ymm11, %ymm12, %ymm14
	vmovd	%r15d, %xmm11
	vbroadcastss	%xmm11, %ymm11
	vfmadd213ps	640(%rsp), %ymm15, %ymm11
	vmovd	%eax, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm11, %ymm14
	vmovss	684(%rsp), %xmm15
	vmulss	728(%rsp), %xmm4, %xmm11
	vmovaps	%ymm14, 640(%rsp)
	vmovaps	%ymm14, 736(%rsp)
	vmovd	%xmm11, %eax
	vpsignb	%ymm9, %ymm6, %ymm11
	vmulss	732(%rsp), %xmm15, %xmm14
	vpsignb	%ymm10, %ymm2, %ymm15
	vmovd	%xmm14, %r15d
	vmovdqa	%ymm8, %ymm14
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm14
	vcvtdq2ps	%ymm14, %ymm15
	vmovdqa	%ymm8, %ymm14
	{vex} vpdpbusd	%ymm11, %ymm12, %ymm14
	vmovd	%r15d, %xmm11
	vbroadcastss	%xmm11, %ymm11
	vfmadd213ps	608(%rsp), %ymm15, %ymm11
	vmovd	%eax, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm11, %ymm14
	vmovss	684(%rsp), %xmm15
	vmovaps	%ymm14, 608(%rsp)
	vmovaps	%ymm14, 768(%rsp)
	vmulss	724(%rsp), %xmm15, %xmm14
	vmovd	%xmm14, %r15d
	vmulss	720(%rsp), %xmm4, %xmm14
	vpand	.LC0(%rip), %ymm1, %ymm1
	vpaddb	.LC1(%rip), %ymm1, %ymm1
	vpand	.LC0(%rip), %ymm5, %ymm5
	vpaddb	.LC1(%rip), %ymm5, %ymm5
	vpand	.LC0(%rip), %ymm0, %ymm0
	vpsignb	%ymm10, %ymm1, %ymm15
	vpaddb	.LC1(%rip), %ymm0, %ymm0
	vpsignb	%ymm9, %ymm5, %ymm11
	vmovd	%xmm14, %eax
	vmovdqa	%ymm8, %ymm14
	vpsignb	%ymm10, %ymm0, %ymm10
	{vex} vpdpbusd	%ymm15, %ymm13, %ymm14
	vcvtdq2ps	%ymm14, %ymm15
	vmovdqa	%ymm8, %ymm14
	{vex} vpdpbusd	%ymm11, %ymm12, %ymm14
	vmovd	%r15d, %xmm11
	vbroadcastss	%xmm11, %ymm11
	vfmadd213ps	576(%rsp), %ymm15, %ymm11
	vmovd	%eax, %xmm15
	vbroadcastss	%xmm15, %ymm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd132ps	%ymm15, %ymm11, %ymm14
	vmovss	684(%rsp), %xmm15
	vmulss	716(%rsp), %xmm15, %xmm15
	vmovaps	%ymm14, 576(%rsp)
	vmovaps	%ymm14, 800(%rsp)
	vmulss	712(%rsp), %xmm4, %xmm14
	vmovdqa	%ymm8, %ymm4
	{vex} vpdpbusd	%ymm10, %ymm13, %ymm4
	vmovdqa	%ymm8, %ymm10
	vcvtdq2ps	%ymm4, %ymm11
	vmovdqa	128(%rsp), %ymm4
	vpand	.LC0(%rip), %ymm4, %ymm4
	vpaddb	.LC1(%rip), %ymm4, %ymm4
	vpsignb	%ymm9, %ymm4, %ymm9
	{vex} vpdpbusd	%ymm9, %ymm12, %ymm10
	vbroadcastss	%xmm15, %ymm9
	vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15
	vmovss	688(%rsp), %xmm12
	vfmadd213ps	544(%rsp), %ymm11, %ymm9
	vbroadcastss	%xmm14, %ymm11
	vmulss	704(%rsp), %xmm12, %xmm13
	vmovdqa	%ymm8, %ymm12
	vcvtdq2ps	%ymm10, %ymm10
	vfmadd132ps	%ymm11, %ymm9, %ymm10
	vbroadcastss	%xmm13, %ymm13
	vmovaps	%ymm10, 544(%rsp)
	vmovaps	%ymm10, 832(%rsp)
	vmovss	692(%rsp), %xmm10
	vmulss	708(%rsp), %xmm10, %xmm9
	vmovdqu	-64(%rdx,%rbx), %ymm10
	vpsignb	%ymm10, %ymm10, %ymm11
	vpsignb	%ymm10, %ymm3, %ymm10
	{vex} vpdpbusd	%ymm10, %ymm11, %ymm12
	vmovdqu	-32(%rdx,%rbx), %ymm10
	vbroadcastss	%xmm9, %ymm9
	vpsignb	%ymm10, %ymm10, %ymm10
	vcvtdq2ps	%ymm12, %ymm14
	vmovdqa	%ymm8, %ymm12
	vfmadd213ps	512(%rsp), %ymm14, %ymm9
	vmovdqa	%ymm8, %ymm14
	{vex} vpdpbusd	%ymm15, %ymm10, %ymm12
	vmovss	704(%rsp), %xmm15
	vcvtdq2ps	%ymm12, %ymm12
	vfmadd132ps	%ymm13, %ymm9, %ymm12
	vmovss	708(%rsp), %xmm13
	vmulss	732(%rsp), %xmm13, %xmm9
	vmovaps	%ymm12, 512(%rsp)
	vmovaps	%ymm12, 864(%rsp)
	vbroadcastss	%xmm9, %ymm9
	vmulss	728(%rsp), %xmm15, %xmm13
	vpsignb	-64(%rdx,%rbx), %ymm2, %ymm12
	vpsignb	-32(%rdx,%rbx), %ymm6, %ymm15
	{vex} vpdpbusd	%ymm12, %ymm11, %ymm14
	vmovdqa	%ymm8, %ymm12
	{vex} vpdpbusd	%ymm15, %ymm10, %ymm12
	vmovss	704(%rsp), %xmm15
	vbroadcastss	%xmm13, %ymm13
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd213ps	480(%rsp), %ymm14, %ymm9
	vmovdqa	%ymm8, %ymm14
	vcvtdq2ps	%ymm12, %ymm12
	vfmadd132ps	%ymm13, %ymm9, %ymm12
	vmovss	708(%rsp), %xmm13
	vmulss	724(%rsp), %xmm13, %xmm9
	vmulss	720(%rsp), %xmm15, %xmm13
	vpsignb	-32(%rdx,%rbx), %ymm5, %ymm15
	vmovaps	%ymm12, 480(%rsp)
	vmovaps	%ymm12, 896(%rsp)
	vpsignb	-64(%rdx,%rbx), %ymm1, %ymm12
	vbroadcastss	%xmm9, %ymm9
	{vex} vpdpbusd	%ymm12, %ymm11, %ymm14
	vmovdqa	%ymm8, %ymm12
	vbroadcastss	%xmm13, %ymm13
	{vex} vpdpbusd	%ymm15, %ymm10, %ymm12
	vmovss	704(%rsp), %xmm15
	vcvtdq2ps	%ymm14, %ymm14
	vfmadd213ps	448(%rsp), %ymm14, %ymm9
	vcvtdq2ps	%ymm12, %ymm12
	vfmadd132ps	%ymm13, %ymm9, %ymm12
	vpsignb	-64(%rdx,%rbx), %ymm0, %ymm9
	vmovss	708(%rsp), %xmm13
	vmulss	716(%rsp), %xmm13, %xmm14
	vmulss	712(%rsp), %xmm15, %xmm13
	vmovdqa	%ymm8, %ymm15
	{vex} vpdpbusd	%ymm9, %ymm11, %ymm15
	vpsignb	-32(%rdx,%rbx), %ymm4, %ymm9
	vmovaps	%ymm12, 448(%rsp)
	vmovaps	%ymm12, 928(%rsp)
	vcvtdq2ps	%ymm15, %ymm12
	vmovdqa	%ymm8, %ymm15
	{vex} vpdpbusd	%ymm9, %ymm10, %ymm15
	vbroadcastss	%xmm14, %ymm9
	vbroadcastss	%xmm13, %ymm10
	vfmadd213ps	416(%rsp), %ymm12, %ymm9
	vmovss	688(%rsp), %xmm12
	vmulss	696(%rsp), %xmm12, %xmm13
	vmovdqa	%ymm8, %ymm12
	vcvtdq2ps	%ymm15, %ymm11
	vmovaps	%ymm11, %ymm15
	vfmadd132ps	%ymm10, %ymm9, %ymm15
	vbroadcastss	%xmm13, %ymm13
	vmovss	692(%rsp), %xmm10
	vmulss	700(%rsp), %xmm10, %xmm9
	vmovaps	%ymm15, 416(%rsp)
	vmovaps	%ymm15, 960(%rsp)
	vmovdqu	-64(%rdx,%r13), %ymm15
	vbroadcastss	%xmm9, %ymm9
	vpsignb	%ymm15, %ymm15, %ymm11
	vpsignb	%ymm15, %ymm3, %ymm10
	vmovdqa	%ymm8, %ymm15
	{vex} vpdpbusd	%ymm10, %ymm11, %ymm15
	vcvtdq2ps	%ymm15, %ymm14
	vmovdqu	-32(%rdx,%r13), %ymm15
	vfmadd213ps	384(%rsp), %ymm14, %ymm9
	vpsignb	%ymm15, %ymm15, %ymm10
	vpsignb	%ymm15, %ymm7, %ymm15
	{vex} vpdpbusd	%ymm15, %ymm10, %ymm12
	vcvtdq2ps	%ymm12, %ymm12
	vmovaps	%ymm12, %ymm15
	vfmadd132ps	%ymm13, %ymm9, %ymm15
	vmovaps	%ymm15, 384(%rsp)
	vmovaps	%ymm15, 992(%rsp)
	vpsignb	-64(%rdx,%r13), %ymm2, %ymm12
	vmovdqa	%ymm8, %ymm15
	vmovss	700(%rsp), %xmm13
	{vex} vpdpbusd	%ymm12, %ymm11, %ymm15
	vmovdqa	%ymm8, %ymm12
	vmovss	696(%rsp), %xmm14
	vmulss	732(%rsp), %xmm13, %xmm9
	vmulss	728(%rsp), %xmm14, %xmm13
	vcvtdq2ps	%ymm15, %ymm14
	vpsignb	-32(%rdx,%r13), %ymm6, %ymm15
	vbroadcastss	%xmm9, %ymm9
	{vex} vpdpbusd	%ymm15, %ymm10, %ymm12
	vbroadcastss	%xmm13, %ymm13
	vfmadd213ps	352(%rsp), %ymm14, %ymm9
	vmovss	696(%rsp), %xmm14
	vcvtdq2ps	%ymm12, %ymm12
	vmovaps	%ymm12, %ymm15
	vpsignb	-64(%rdx,%r13), %ymm1, %ymm12
	vfmadd132ps	%ymm13, %ymm9, %ymm15
	vmovss	700(%rsp), %xmm13
	vmulss	724(%rsp), %xmm13, %xmm9
	vmulss	720(%rsp), %xmm14, %xmm13
	vmovaps	%ymm15, 352(%rsp)
	vmovaps	%ymm15, 1024(%rsp)
	vmovdqa	%ymm8, %ymm15
	vbroadcastss	%xmm9, %ymm9
	{vex} vpdpbusd	%ymm12, %ymm11, %ymm15
	vmovdqa	%ymm8, %ymm12
	vbroadcastss	%xmm13, %ymm13
	vcvtdq2ps	%ymm15, %ymm14
	vpsignb	-32(%rdx,%r13), %ymm5, %ymm15
	vfmadd213ps	320(%rsp), %ymm14, %ymm9
	{vex} vpdpbusd	%ymm15, %ymm10, %ymm12
	vcvtdq2ps	%ymm12, %ymm12
	vmovaps	%ymm12, %ymm15
	vfmadd132ps	%ymm13, %ymm9, %ymm15
	vpsignb	-64(%rdx,%r13), %ymm0, %ymm9
	vmovss	700(%rsp), %xmm13
	vmulss	716(%rsp), %xmm13, %xmm14
	vmovaps	%ymm15, 320(%rsp)
	vmovaps	%ymm15, 1056(%rsp)
	vmovss	696(%rsp), %xmm15
	vmulss	712(%rsp), %xmm15, %xmm13
	vmovdqa	%ymm8, %ymm15
	{vex} vpdpbusd	%ymm9, %ymm11, %ymm15
	vpsignb	-32(%rdx,%r13), %ymm4, %ymm9
	vmovdqa	%ymm8, %ymm11
	{vex} vpdpbusd	%ymm9, %ymm10, %ymm11
	vbroadcastss	%xmm14, %ymm9
	vbroadcastss	%xmm13, %ymm10
	vcvtdq2ps	%ymm15, %ymm12
	vfmadd213ps	288(%rsp), %ymm12, %ymm9
	vmovss	676(%rsp), %xmm15
	vmovss	688(%rsp), %xmm12
	vcvtdq2ps	%ymm11, %ymm11
	vmulss	680(%rsp), %xmm12, %xmm13
	vfmadd132ps	%ymm10, %ymm9, %ymm11
	vmovss	692(%rsp), %xmm10
	vmulss	%xmm15, %xmm10, %xmm14
	vmovdqu	-64(%rdx,%r12), %ymm10
	vpsignb	%ymm10, %ymm10, %ymm10
	vmovaps	%ymm11, 288(%rsp)
	vmovaps	%ymm11, 1088(%rsp)
	vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
	vmovdqa	%ymm8, %ymm3
	vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
	{vex} vpdpbusd	%ymm9, %ymm10, %ymm3
	vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
	vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
	vpsignb	-32(%rdx,%r12), %ymm4, %ymm4
	vmulss	732(%rsp), %xmm15, %xmm12
	vcvtdq2ps	%ymm3, %ymm11
	vmovdqu	-32(%rdx,%r12), %ymm3
	vpsignb	%ymm3, %ymm3, %ymm9
	vpsignb	%ymm3, %ymm7, %ymm7
	vmovdqa	%ymm8, %ymm3
	{vex} vpdpbusd	%ymm7, %ymm9, %ymm3
	vcvtdq2ps	%ymm3, %ymm7
	vbroadcastss	%xmm14, %ymm3
	vfmadd213ps	256(%rsp), %ymm11, %ymm3
	vbroadcastss	%xmm13, %ymm11
	vmovss	680(%rsp), %xmm13
	vfmadd132ps	%ymm11, %ymm3, %ymm7
	vmovdqa	%ymm8, %ymm3
	vmulss	728(%rsp), %xmm13, %xmm11
	vmovaps	%ymm7, 256(%rsp)
	vmovaps	%ymm7, 1120(%rsp)
	vpsignb	-64(%rdx,%r12), %ymm2, %ymm7
	vbroadcastss	%xmm12, %ymm2
	{vex} vpdpbusd	%ymm7, %ymm10, %ymm3
	vmovdqa	%ymm8, %ymm7
	{vex} vpdpbusd	%ymm6, %ymm9, %ymm7
	vcvtdq2ps	%ymm3, %ymm3
	vfmadd213ps	224(%rsp), %ymm2, %ymm3
	vbroadcastss	%xmm11, %ymm2
	vcvtdq2ps	%ymm7, %ymm6
	vmulss	724(%rsp), %xmm15, %xmm7
	vfmadd132ps	%ymm2, %ymm3, %ymm6
	vpsignb	-64(%rdx,%r12), %ymm1, %ymm3
	vmovdqa	%ymm8, %ymm2
	{vex} vpdpbusd	%ymm3, %ymm10, %ymm2
	vmovdqa	%ymm8, %ymm3
	vbroadcastss	%xmm7, %ymm1
	{vex} vpdpbusd	%ymm5, %ymm9, %ymm3
	vmovaps	%ymm6, 224(%rsp)
	vmovaps	%ymm6, 1152(%rsp)
	vmulss	720(%rsp), %xmm13, %xmm6
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd213ps	192(%rsp), %ymm1, %ymm2
	vcvtdq2ps	%ymm3, %ymm3
	vbroadcastss	%xmm6, %ymm1
	vfmadd132ps	%ymm1, %ymm2, %ymm3
	vmovdqa	%ymm8, %ymm1
	{vex} vpdpbusd	%ymm0, %ymm10, %ymm1
	vmovdqa	%ymm8, %ymm0
	vmulss	716(%rsp), %xmm15, %xmm2
	{vex} vpdpbusd	%ymm4, %ymm9, %ymm0
	vmovaps	%ymm3, 192(%rsp)
	vmovaps	%ymm3, 1184(%rsp)
	vmulss	712(%rsp), %xmm13, %xmm3
	vbroadcastss	%xmm2, %ymm2
	vcvtdq2ps	%ymm1, %ymm1
	vfmadd213ps	160(%rsp), %ymm2, %ymm1
	vcvtdq2ps	%ymm0, %ymm0
	vbroadcastss	%xmm3, %ymm2
	vfmadd132ps	%ymm2, %ymm1, %ymm0
	vmovaps	%ymm0, 160(%rsp)
	vmovaps	%ymm0, 1216(%rsp)
	cmpl	%edi, 24(%rbp)
	jg	.L4
	movq	40(%rsp), %r14
.L9:
	movl	48(%rsp), %edx
	testl	%edx, %edx
	jle	.L7
	vmovaps	736(%rsp), %ymm4
	movq	64(%rsp), %r10
	vmovaps	768(%rsp), %ymm5
	movl	48(%rsp), %r11d
	vextractf128	$0x1, %ymm4, %xmm0
	leal	1(%r10), %eax
	vaddps	%xmm0, %xmm4, %xmm1
	salq	$2, %rax
	vmovaps	800(%rsp), %ymm4
	leaq	(%r14,%rax), %rdx
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%r14,%r10,4), %xmm0, %xmm0
	vmovss	%xmm0, (%r14,%r10,4)
	vextractf128	$0x1, %ymm5, %xmm0
	vaddps	%xmm0, %xmm5, %xmm1
	vmovaps	832(%rsp), %ymm5
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	vextractf128	$0x1, %ymm4, %xmm0
	leal	2(%r10), %edx
	vaddps	%xmm0, %xmm4, %xmm1
	leaq	0(,%rdx,4), %rdi
	leaq	(%r14,%rdi), %rdx
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	vextractf128	$0x1, %ymm5, %xmm0
	leal	3(%r10), %edx
	vaddps	%xmm0, %xmm5, %xmm1
	leaq	0(,%rdx,4), %r8
	leaq	(%r14,%r8), %rdx
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	cmpl	$1, %r11d
	je	.L7
	vmovaps	864(%rsp), %ymm4
	leaq	4000(%r14,%rax), %rdx
	vmovaps	896(%rsp), %ymm5
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm0, %xmm4, %xmm1
	vmovaps	928(%rsp), %ymm4
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	4000(%r14,%r10,4), %xmm0, %xmm0
	vmovss	%xmm0, 4000(%r14,%r10,4)
	vextractf128	$0x1, %ymm5, %xmm0
	vaddps	%xmm0, %xmm5, %xmm1
	vmovaps	960(%rsp), %ymm5
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	vextractf128	$0x1, %ymm4, %xmm0
	leaq	4000(%r14,%rdi), %rdx
	vaddps	%xmm0, %xmm4, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	vextractf128	$0x1, %ymm5, %xmm0
	leaq	4000(%r14,%r8), %rdx
	vaddps	%xmm0, %xmm5, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	cmpl	$2, %r11d
	je	.L7
	vmovaps	992(%rsp), %ymm4
	leaq	8000(%r14,%rax), %rdx
	vmovaps	1024(%rsp), %ymm5
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm0, %xmm4, %xmm1
	vmovaps	1056(%rsp), %ymm4
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	8000(%r14,%r10,4), %xmm0, %xmm0
	vmovss	%xmm0, 8000(%r14,%r10,4)
	vextractf128	$0x1, %ymm5, %xmm0
	vaddps	%xmm0, %xmm5, %xmm1
	vmovaps	1088(%rsp), %ymm5
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	vextractf128	$0x1, %ymm4, %xmm0
	leaq	8000(%r14,%rdi), %rdx
	vaddps	%xmm0, %xmm4, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	vextractf128	$0x1, %ymm5, %xmm0
	leaq	8000(%r14,%r8), %rdx
	vaddps	%xmm0, %xmm5, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rdx), %xmm0, %xmm0
	vmovss	%xmm0, (%rdx)
	cmpl	$3, %r11d
	je	.L7
	vmovaps	1120(%rsp), %ymm4
	leaq	12000(%r14,%rax), %rax
	vmovaps	1152(%rsp), %ymm5
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm0
	vmovaps	1184(%rsp), %ymm4
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	12000(%r14,%r10,4), %xmm0, %xmm0
	vmovss	%xmm0, 12000(%r14,%r10,4)
	vextractf128	$0x1, %ymm5, %xmm0
	vaddps	%xmm5, %xmm0, %xmm0
	vmovaps	1216(%rsp), %ymm5
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rax), %xmm0, %xmm0
	vmovss	%xmm0, (%rax)
	vextractf128	$0x1, %ymm4, %xmm0
	leaq	12000(%r14,%rdi), %rax
	vaddps	%xmm4, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rax), %xmm0, %xmm0
	vmovss	%xmm0, (%rax)
	vextractf128	$0x1, %ymm5, %xmm0
	leaq	12000(%r14,%r8), %rax
	vaddps	%xmm5, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	(%rax), %xmm0, %xmm0
	vmovss	%xmm0, (%rax)
.L7:
	movl	36(%rsp), %edx
	addq	$4, 64(%rsp)
	addl	%edx, 672(%rsp)
	movq	64(%rsp), %rax
	addl	%edx, 60(%rsp)
	movl	32(%rsp), %edx
	addl	%edx, 56(%rsp)
	addl	%edx, 52(%rsp)
	cmpl	%eax, 16(%rbp)
	jg	.L3
	vzeroupper
.L1:
	movq	1272(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L25
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
.L25:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
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
