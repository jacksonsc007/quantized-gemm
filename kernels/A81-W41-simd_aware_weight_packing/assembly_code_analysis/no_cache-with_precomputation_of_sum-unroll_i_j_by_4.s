	.file	"no_cache-with_precomputation_of_sum-unroll_i_j_by_4.c"
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
	pushq	%r14
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	movq	%rdx, %r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$1344, %rsp
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	16(%rbp), %r12
	movl	24(%rbp), %eax
	movq	%rdi, 80(%rsp)
	movq	%rsi, 216(%rsp)
	movq	%rdx, 24(%rsp)
	movq	%r8, 152(%rsp)
	movq	%fs:40, %rdx
	movq	%rdx, 1336(%rsp)
	xorl	%edx, %edx
	movl	32(%rbp), %edx
	testl	%edx, %edx
	jle	.L1
	movq	%rcx, %rbx
	movl	$5, %esi
	movl	$1, %edi
	sarx	%esi, 40(%rbp), %esi
	sarx	%edi, 40(%rbp), %ecx
	movl	%esi, 112(%rsp)
	movl	%esi, %r15d
	movl	%ecx, 68(%rsp)
	testl	%eax, %eax
	jle	.L1
	subl	$1, %eax
	leal	0(,%rsi,4), %edi
	leal	(%rcx,%rcx), %edx
	movl	40(%rbp), %ecx
	shrl	$2, %eax
	addq	$16000, %r14
	movq	%rbx, %r11
	movq	%r12, %r10
	movl	%edi, 100(%rsp)
	leal	(%rsi,%rsi), %edi
	sall	$2, %ecx
	vpxor	%xmm10, %xmm10, %xmm10
	imulq	$4000, %rax, %rax
	movl	%ecx, 96(%rsp)
	movl	40(%rbp), %ecx
	movq	%r9, %rbx
	movq	%r14, 16(%rsp)
	leaq	448(%rsp), %r14
	vmovdqa	.LC0(%rip), %ymm11
	movq	%r14, 136(%rsp)
	leaq	544(%rsp), %r14
	addl	%ecx, %ecx
	movq	%rax, 32(%rsp)
	movslq	%esi, %rax
	xorl	%esi, %esi
	salq	$2, %rax
	movq	%r14, 128(%rsp)
	movq	%rax, 352(%rsp)
	movl	%edi, %eax
	addl	%r15d, %eax
	movl	%edi, 48(%rsp)
	movq	$0, 72(%rsp)
	movl	%eax, 40(%rsp)
	movl	%ecx, 44(%rsp)
	movl	%edi, %ecx
	xorl	%edi, %edi
.L4:
	movslq	%edi, %rax
	movl	$0, 120(%rsp)
	movq	%rax, 184(%rsp)
	movl	112(%rsp), %eax
	movl	$0, 360(%rsp)
	addl	%eax, %edi
	movl	%edi, 52(%rsp)
	movslq	%edi, %rdi
	movq	%rdi, 176(%rsp)
	movslq	%ecx, %rdi
	addl	%eax, %ecx
	movslq	%ecx, %rax
	movl	%ecx, 56(%rsp)
	movslq	%esi, %rcx
	movq	%rax, 160(%rsp)
	movl	68(%rsp), %eax
	movq	%rdi, 168(%rsp)
	addl	%eax, %esi
	movl	%esi, 60(%rsp)
	movslq	%esi, %rsi
	movq	%rsi, 208(%rsp)
	movslq	%edx, %rsi
	addl	%eax, %edx
	movslq	%edx, %rax
	movq	%rsi, 200(%rsp)
	movq	24(%rsp), %rsi
	movq	%rax, 192(%rsp)
	movq	72(%rsp), %rax
	movl	%edx, 64(%rsp)
	leaq	(%rsi,%rax,4), %r12
	movq	16(%rsp), %rsi
	addq	32(%rsp), %rax
	movq	%r12, %r13
	leaq	(%rsi,%rax,4), %rax
	movq	%rax, 88(%rsp)
	movl	48(%rsp), %eax
	movl	%eax, 116(%rsp)
	movl	44(%rsp), %eax
	movl	%eax, 124(%rsp)
	movq	216(%rsp), %rax
	addq	%rcx, %rax
	movq	%rax, 144(%rsp)
.L10:
	vxorps	%xmm7, %xmm7, %xmm7
	vmovaps	%ymm7, 800(%rsp)
	movl	$0x00000000, 480(%rsp)
	vmovaps	%ymm7, 832(%rsp)
	movl	$0x00000000, 484(%rsp)
	vmovaps	%ymm7, 864(%rsp)
	movl	$0x00000000, 488(%rsp)
	vmovaps	%ymm7, 896(%rsp)
	movl	$0x00000000, 492(%rsp)
	vmovaps	%ymm7, 928(%rsp)
	movl	$0x00000000, 496(%rsp)
	vmovaps	%ymm7, 960(%rsp)
	movl	$0x00000000, 500(%rsp)
	vmovaps	%ymm7, 992(%rsp)
	movl	$0x00000000, 504(%rsp)
	vmovaps	%ymm7, 1024(%rsp)
	movl	$0x00000000, 508(%rsp)
	vmovaps	%ymm7, 1056(%rsp)
	movl	$0x00000000, 512(%rsp)
	vmovaps	%ymm7, 1088(%rsp)
	movl	$0x00000000, 516(%rsp)
	vmovaps	%ymm7, 1120(%rsp)
	movl	$0x00000000, 520(%rsp)
	vmovaps	%ymm7, 1152(%rsp)
	movl	$0x00000000, 524(%rsp)
	vmovaps	%ymm7, 1184(%rsp)
	movl	$0x00000000, 528(%rsp)
	vmovaps	%ymm7, 1216(%rsp)
	movl	$0x00000000, 532(%rsp)
	vmovaps	%ymm7, 1248(%rsp)
	movl	$0x00000000, 536(%rsp)
	vmovaps	%ymm7, 1280(%rsp)
	movl	$0x00000000, 540(%rsp)
	movl	40(%rbp), %eax
	testl	%eax, %eax
	jle	.L9
	movslq	120(%rsp), %rax
	movl	112(%rsp), %esi
	movq	%r13, 104(%rsp)
	movslq	124(%rsp), %rcx
	movq	80(%rsp), %r14
	movq	%rax, %rdi
	movq	%rax, 248(%rsp)
	movl	%esi, %eax
	addl	%edi, %eax
	leaq	(%r14,%rcx), %r15
	cltq
	movq	%rax, 240(%rsp)
	movslq	116(%rsp), %rax
	movq	%rax, %rdi
	movq	%rax, 232(%rsp)
	movl	%esi, %eax
	addl	%edi, %eax
	movslq	360(%rsp), %rdi
	cltq
	movq	%rax, 224(%rsp)
	movq	%rdi, %rax
	addl	40(%rbp), %eax
	leaq	(%r14,%rdi), %r9
	movslq	%eax, %rsi
	movq	%rcx, %rax
	addl	40(%rbp), %eax
	movq	%r9, %r13
	movslq	%eax, %rdx
	leaq	(%r14,%rsi), %r12
	xorl	%eax, %eax
	leaq	(%r14,%rdx), %r14
	movq	%r14, %rsi
	movq	%r12, %r14
	movq	%rsi, %r12
	.p2align 4,,10
	.p2align 3
.L6:
	movl	%eax, %edx
	movq	248(%rsp), %rsi
	movq	224(%rsp), %r9
	movq	232(%rsp), %rdi
	sarl	$5, %edx
	movslq	%edx, %rdx
	leaq	(%rsi,%rdx), %rcx
	leaq	(%rdx,%r9), %r8
	addq	%rdx, %rdi
	movq	240(%rsp), %rsi
	vmovss	(%r11,%r8,4), %xmm0
	vmovss	(%r11,%rdi,4), %xmm1
	vinsertps	$0x10, 4(%r11,%r8,4), %xmm0, %xmm0
	vinsertps	$0x10, 4(%r11,%rdi,4), %xmm1, %xmm1
	addq	%rdx, %rsi
	movq	176(%rsp), %rdi
	vmovlhps	%xmm0, %xmm1, %xmm1
	vmovss	(%r11,%rsi,4), %xmm0
	vinsertps	$0x10, 4(%r11,%rsi,4), %xmm0, %xmm2
	movq	184(%rsp), %rsi
	vmovss	(%r11,%rcx,4), %xmm0
	vinsertps	$0x10, 4(%r11,%rcx,4), %xmm0, %xmm0
	leaq	(%rsi,%rdx), %r9
	vmovss	(%rbx,%r9,4), %xmm7
	leaq	0(,%r9,4), %rsi
	vmovlhps	%xmm2, %xmm0, %xmm0
	movq	%rsi, 432(%rsp)
	vinsertf128	$0x1, %xmm1, %ymm0, %ymm0
	vmovss	%xmm7, 400(%rsp)
	vmovss	4(%rbx,%rsi), %xmm7
	leaq	(%rdi,%rdx), %rsi
	leaq	0(,%rsi,4), %rdi
	movq	%rsi, 424(%rsp)
	vmovss	%xmm7, 388(%rsp)
	vmovss	(%rbx,%rsi,4), %xmm7
	movq	168(%rsp), %rsi
	movq	%rdi, 416(%rsp)
	vmovss	%xmm7, 384(%rsp)
	vmovss	4(%rbx,%rdi), %xmm7
	leaq	(%rsi,%rdx), %rdi
	leaq	0(,%rdi,4), %rsi
	vmovaps	%ymm0, 448(%rsp)
	vmovss	%xmm7, 380(%rsp)
	vmovss	(%rbx,%rdi,4), %xmm7
	movq	%rsi, 408(%rsp)
	vmovss	%xmm7, 376(%rsp)
	vmovss	4(%rbx,%rsi), %xmm7
	vmovss	%xmm7, 372(%rsp)
	movq	160(%rsp), %rcx
	movq	144(%rsp), %r8
	leaq	(%rcx,%rdx), %rsi
	vmovss	(%rbx,%rsi,4), %xmm7
	leaq	0(,%rsi,4), %rcx
	movq	%rcx, 392(%rsp)
	vmovss	%xmm7, 368(%rsp)
	vmovss	4(%rbx,%rcx), %xmm7
	movl	%eax, %ecx
	sarl	%ecx
	vmovss	%xmm7, 364(%rsp)
	vmovdqu	0(%r13,%rax), %ymm7
	movslq	%ecx, %rcx
	vmovdqu	(%r8,%rcx), %ymm3
	movq	208(%rsp), %r8
	vmovdqa	%ymm7, 544(%rsp)
	vmovdqu	32(%r13,%rax), %ymm7
	addq	216(%rsp), %rcx
	vmovdqu	(%rcx,%r8), %ymm1
	movq	200(%rsp), %r8
	vpsrlw	$4, %ymm3, %ymm2
	vmovdqa	%ymm7, 576(%rsp)
	vmovdqu	(%r14,%rax), %ymm7
	vmovdqu	(%rcx,%r8), %ymm9
	movq	192(%rsp), %r8
	vpsrlw	$4, %ymm1, %ymm0
	vmovdqa	%ymm7, 608(%rsp)
	vmovdqu	32(%r14,%rax), %ymm7
	vmovdqu	(%rcx,%r8), %ymm8
	vpsrlw	$4, %ymm9, %ymm13
	vpand	%ymm11, %ymm9, %ymm9
	vmovdqa	%ymm7, 640(%rsp)
	vmovdqu	(%r15,%rax), %ymm7
	vpand	%ymm13, %ymm11, %ymm13
	vpsrlw	$4, %ymm8, %ymm12
	vpand	%ymm11, %ymm8, %ymm8
	vmovdqa	%ymm7, 672(%rsp)
	vmovdqu	32(%r15,%rax), %ymm7
	vpand	%ymm12, %ymm11, %ymm12
	vmovdqa	%ymm7, 704(%rsp)
	vmovdqu	(%r12,%rax), %ymm7
	vmovdqa	%ymm7, 736(%rsp)
	vmovdqu	32(%r12,%rax), %ymm7
	vmovdqa	%ymm7, 768(%rsp)
	vmovss	(%r10,%r9,4), %xmm7
	vmovss	%xmm7, 444(%rsp)
	movq	432(%rsp), %rcx
	movq	136(%rsp), %r8
	vmovss	4(%r10,%rcx), %xmm7
	movq	424(%rsp), %rcx
	vmovss	%xmm7, 432(%rsp)
	vmovss	(%r10,%rcx,4), %xmm7
	movq	416(%rsp), %rcx
	vmovss	%xmm7, 424(%rsp)
	vmovss	4(%r10,%rcx), %xmm7
	movq	392(%rsp), %rcx
	vmovss	%xmm7, 416(%rsp)
	vmovss	(%r10,%rdi,4), %xmm7
	movq	408(%rsp), %rdi
	vmovss	%xmm7, 440(%rsp)
	vmovss	4(%r10,%rdi), %xmm7
	movq	128(%rsp), %rdi
	vmovss	%xmm7, 408(%rsp)
	vmovss	(%r10,%rsi,4), %xmm7
	movq	152(%rsp), %rsi
	vmovss	%xmm7, 404(%rsp)
	vmovss	4(%r10,%rcx), %xmm7
	leaq	(%rsi,%rdx,4), %r9
	xorl	%esi, %esi
	vmovss	%xmm7, 392(%rsp)
	vpand	%ymm11, %ymm3, %ymm7
	vmovdqa	%ymm7, 320(%rsp)
	vpand	%ymm2, %ymm11, %ymm7
	vmovdqa	%ymm7, 288(%rsp)
	vpand	%ymm11, %ymm1, %ymm7
	vmovdqa	%ymm7, 256(%rsp)
	vpand	%ymm0, %ymm11, %ymm7
.L5:
	vmovdqa	%ymm10, %ymm3
	vmovss	(%r8), %xmm1
	movslq	%esi, %rdx
	vmovdqa	%ymm10, %ymm2
	movq	%rdx, %rcx
	vmovss	4(%r8), %xmm0
	salq	$4, %rdx
	addl	$1, %esi
	salq	$7, %rcx
	addq	$8, %r8
	vmovdqa	320(%rsp), %ymm6
	addq	$64, %rdi
	vmulss	400(%rsp), %xmm1, %xmm5
	{vex} vpdpbusd	-64(%rdi), %ymm6, %ymm3
	vmovdqa	288(%rsp), %ymm6
	vmulss	388(%rsp), %xmm0, %xmm4
	vmulss	384(%rsp), %xmm1, %xmm14
	{vex} vpdpbusd	-32(%rdi), %ymm6, %ymm2
	vmovaps	800(%rsp,%rcx), %ymm6
	vbroadcastss	%xmm5, %ymm5
	vbroadcastss	%xmm4, %ymm4
	vcvtdq2ps	%ymm3, %ymm3
	vfmadd132ps	%ymm5, %ymm6, %ymm3
	vmovdqa	%ymm10, %ymm5
	vbroadcastss	%xmm14, %ymm14
	vmulss	380(%rsp), %xmm0, %xmm6
	vcvtdq2ps	%ymm2, %ymm2
	vmovaps	%ymm3, 800(%rsp,%rcx)
	vmovaps	800(%rsp,%rcx), %ymm3
	vbroadcastss	%xmm6, %ymm6
	vfmadd132ps	%ymm4, %ymm3, %ymm2
	vmovss	(%r9), %xmm3
	vmovaps	%ymm2, 800(%rsp,%rcx)
	vmovss	4(%r9), %xmm2
	vmovss	480(%rsp,%rdx), %xmm4
	vfmadd231ss	444(%rsp), %xmm3, %xmm4
	vmovss	%xmm4, 480(%rsp,%rdx)
	vmovss	480(%rsp,%rdx), %xmm4
	vfmadd231ss	432(%rsp), %xmm2, %xmm4
	vmovss	%xmm4, 480(%rsp,%rdx)
	vmovdqa	256(%rsp), %ymm4
	vmovaps	832(%rsp,%rcx), %ymm15
	{vex} vpdpbusd	-64(%rdi), %ymm4, %ymm5
	vmovdqa	%ymm10, %ymm4
	{vex} vpdpbusd	-32(%rdi), %ymm7, %ymm4
	vcvtdq2ps	%ymm5, %ymm5
	vfmadd132ps	%ymm14, %ymm15, %ymm5
	vcvtdq2ps	%ymm4, %ymm4
	vmovaps	%ymm5, 832(%rsp,%rcx)
	vmovaps	832(%rsp,%rcx), %ymm5
	vfmadd132ps	%ymm6, %ymm5, %ymm4
	vmovdqa	%ymm10, %ymm5
	vmovaps	%ymm4, 832(%rsp,%rcx)
	vmovss	484(%rsp,%rdx), %xmm4
	vfmadd231ss	424(%rsp), %xmm3, %xmm4
	vmovss	%xmm4, 484(%rsp,%rdx)
	vmovss	484(%rsp,%rdx), %xmm4
	vfmadd231ss	416(%rsp), %xmm2, %xmm4
	{vex} vpdpbusd	-64(%rdi), %ymm9, %ymm5
	addq	352(%rsp), %r9
	vmulss	376(%rsp), %xmm1, %xmm14
	vmulss	372(%rsp), %xmm0, %xmm6
	vmulss	368(%rsp), %xmm1, %xmm1
	vmovss	%xmm4, 484(%rsp,%rdx)
	vmovdqa	%ymm10, %ymm4
	vmovaps	864(%rsp,%rcx), %ymm15
	{vex} vpdpbusd	-32(%rdi), %ymm13, %ymm4
	vmulss	364(%rsp), %xmm0, %xmm0
	vbroadcastss	%xmm14, %ymm14
	vcvtdq2ps	%ymm5, %ymm5
	vfmadd132ps	%ymm14, %ymm15, %ymm5
	vbroadcastss	%xmm6, %ymm6
	vbroadcastss	%xmm1, %ymm1
	vbroadcastss	%xmm0, %ymm0
	vcvtdq2ps	%ymm4, %ymm4
	vmovaps	%ymm5, 864(%rsp,%rcx)
	vmovaps	864(%rsp,%rcx), %ymm5
	vfmadd132ps	%ymm6, %ymm5, %ymm4
	vmovdqa	%ymm10, %ymm5
	{vex} vpdpbusd	-64(%rdi), %ymm8, %ymm5
	vmovaps	%ymm4, 864(%rsp,%rcx)
	vmovss	488(%rsp,%rdx), %xmm4
	vfmadd231ss	440(%rsp), %xmm3, %xmm4
	vcvtdq2ps	%ymm5, %ymm5
	vmovss	%xmm4, 488(%rsp,%rdx)
	vmovss	488(%rsp,%rdx), %xmm4
	vfmadd231ss	408(%rsp), %xmm2, %xmm4
	vmovss	%xmm4, 488(%rsp,%rdx)
	vmovdqa	%ymm10, %ymm4
	vmovaps	896(%rsp,%rcx), %ymm6
	{vex} vpdpbusd	-32(%rdi), %ymm12, %ymm4
	vfmadd132ps	%ymm1, %ymm6, %ymm5
	vmovaps	%ymm5, 896(%rsp,%rcx)
	vcvtdq2ps	%ymm4, %ymm4
	vmovaps	896(%rsp,%rcx), %ymm1
	vfmadd132ps	%ymm0, %ymm1, %ymm4
	vmovaps	%ymm4, 896(%rsp,%rcx)
	vmovss	492(%rsp,%rdx), %xmm0
	vfmadd132ss	404(%rsp), %xmm0, %xmm3
	vmovss	%xmm3, 492(%rsp,%rdx)
	vmovss	492(%rsp,%rdx), %xmm0
	vfmadd132ss	392(%rsp), %xmm0, %xmm2
	vmovss	%xmm2, 492(%rsp,%rdx)
	cmpl	$4, %esi
	jne	.L5
	addq	$64, %rax
	cmpl	%eax, 40(%rbp)
	jg	.L6
	movq	104(%rsp), %r13
.L9:
	vmovaps	800(%rsp), %ymm1
	addq	$16000, %r13
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	480(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-16000(%r13), %xmm0, %xmm0
	vmovaps	832(%rsp), %ymm1
	vmovss	%xmm0, -16000(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	484(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-15996(%r13), %xmm0, %xmm0
	vmovaps	864(%rsp), %ymm1
	vmovss	%xmm0, -15996(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	488(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-15992(%r13), %xmm0, %xmm0
	vmovaps	896(%rsp), %ymm1
	vmovss	%xmm0, -15992(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	492(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-15988(%r13), %xmm0, %xmm0
	vmovaps	928(%rsp), %ymm1
	vmovss	%xmm0, -15988(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	496(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-12000(%r13), %xmm0, %xmm0
	vmovaps	960(%rsp), %ymm1
	vmovss	%xmm0, -12000(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	500(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-11996(%r13), %xmm0, %xmm0
	vmovaps	992(%rsp), %ymm1
	vmovss	%xmm0, -11996(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	504(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-11992(%r13), %xmm0, %xmm0
	vmovaps	1024(%rsp), %ymm1
	vmovss	%xmm0, -11992(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	508(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-11988(%r13), %xmm0, %xmm0
	vmovss	%xmm0, -11988(%r13)
	vmovaps	1056(%rsp), %ymm1
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	512(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-8000(%r13), %xmm0, %xmm0
	vmovaps	1088(%rsp), %ymm1
	vmovss	%xmm0, -8000(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	516(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-7996(%r13), %xmm0, %xmm0
	vmovaps	1120(%rsp), %ymm1
	vmovss	%xmm0, -7996(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	520(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-7992(%r13), %xmm0, %xmm0
	vmovaps	1152(%rsp), %ymm1
	vmovss	%xmm0, -7992(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	524(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-7988(%r13), %xmm0, %xmm0
	vmovaps	1184(%rsp), %ymm1
	vmovss	%xmm0, -7988(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	528(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-4000(%r13), %xmm0, %xmm0
	vmovaps	1216(%rsp), %ymm1
	vmovss	%xmm0, -4000(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	532(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-3996(%r13), %xmm0, %xmm0
	vmovaps	1248(%rsp), %ymm1
	vmovss	%xmm0, -3996(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	536(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-3992(%r13), %xmm0, %xmm0
	vmovaps	1280(%rsp), %ymm1
	vmovss	%xmm0, -3992(%r13)
	vextractf128	$0x1, %ymm1, %xmm0
	vaddps	%xmm1, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vmovss	540(%rsp), %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-3988(%r13), %xmm0, %xmm0
	vmovss	%xmm0, -3988(%r13)
	movl	96(%rsp), %esi
	addl	%esi, 360(%rsp)
	addl	%esi, 124(%rsp)
	movl	100(%rsp), %esi
	addl	%esi, 120(%rsp)
	addl	%esi, 116(%rsp)
	cmpq	88(%rsp), %r13
	jne	.L10
	movl	40(%rsp), %eax
	movl	52(%rsp), %edi
	movl	56(%rsp), %ecx
	movl	60(%rsp), %esi
	addl	%eax, %edi
	addq	$4, 72(%rsp)
	addl	%eax, %ecx
	movl	68(%rsp), %eax
	leal	(%rax,%rax,2), %edx
	movq	72(%rsp), %rax
	addl	%edx, %esi
	addl	64(%rsp), %edx
	cmpl	%eax, 32(%rbp)
	jg	.L4
	vzeroupper
.L1:
	movq	1336(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L19
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
.L19:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE5518:
	.size	matmul_kernel, .-matmul_kernel
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
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
