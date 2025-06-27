	.file	"good-no_cache-with_precomputation_of_sum-unroll_i_j_by_4-use_loop.c"
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
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$1376, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	16(%rbp), %r13
	movq	%rsi, 192(%rsp)
	movl	32(%rbp), %esi
	movq	%rdi, 48(%rsp)
	movq	%rdx, 8(%rsp)
	movq	%r8, 128(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 1368(%rsp)
	xorl	%eax, %eax
	testl	%esi, %esi
	jle	.L1
	movl	24(%rbp), %edx
	movl	$5, %eax
	movl	$1, %ebx
	sarx	%eax, 40(%rbp), %eax
	sarx	%ebx, 40(%rbp), %ebx
	movl	%eax, 68(%rsp)
	movl	%ebx, 56(%rsp)
	testl	%edx, %edx
	jle	.L1
	leal	0(,%rax,4), %edi
	movq	%rcx, %r11
	leal	(%rax,%rax), %ecx
	movl	40(%rbp), %eax
	movl	%ecx, 20(%rsp)
	leal	(%rbx,%rbx), %edx
	movslq	%edi, %rbx
	xorl	%esi, %esi
	sall	$2, %eax
	vmovdqa	.LC1(%rip), %ymm6
	movl	%edi, 64(%rsp)
	movq	%rbx, %r10
	movl	%eax, 60(%rsp)
	movl	40(%rbp), %eax
	movq	%r9, %rbx
	xorl	%edi, %edi
	movq	$0, 40(%rsp)
	movq	%r13, %r9
	addl	%eax, %eax
	movl	%eax, 16(%rsp)
	leaq	480(%rsp), %rax
	movq	%rax, 112(%rsp)
	leaq	576(%rsp), %rax
	movq	%rax, 104(%rsp)
.L4:
	movslq	%edi, %rax
	vpxor	%xmm5, %xmm5, %xmm5
	leaq	832(%rsp), %r12
	movl	$0, 76(%rsp)
	movq	%rax, 160(%rsp)
	movl	68(%rsp), %eax
	movq	$0, 272(%rsp)
	addl	%eax, %edi
	movl	$0, 84(%rsp)
	movl	%edi, 24(%rsp)
	movslq	%edi, %rdi
	movq	%rdi, 152(%rsp)
	movslq	%ecx, %rdi
	addl	%eax, %ecx
	movq	%rdi, 144(%rsp)
	movl	56(%rsp), %edi
	movslq	%ecx, %rax
	movq	%rax, 136(%rsp)
	movslq	%esi, %rax
	addl	%edi, %esi
	movl	%ecx, 28(%rsp)
	movl	%esi, 32(%rsp)
	movslq	%esi, %rsi
	movq	%rsi, 184(%rsp)
	movslq	%edx, %rsi
	addl	%edi, %edx
	movq	40(%rsp), %rdi
	movl	%edx, 36(%rsp)
	movslq	%edx, %rdx
	movq	%rdx, 168(%rsp)
	movq	8(%rsp), %rdx
	movq	%rsi, 176(%rsp)
	movl	$0, 448(%rsp)
	leaq	(%rdx,%rdi,4), %rdx
	movq	%rdx, 88(%rsp)
	movl	20(%rsp), %edx
	movl	%edx, 72(%rsp)
	movl	16(%rsp), %edx
	movl	%edx, 80(%rsp)
	movq	192(%rsp), %rdx
	addq	%rdx, %rax
	movq	%rax, 120(%rsp)
	leaq	512(%rsp), %rax
	movq	%rax, 96(%rsp)
	movq	%r12, %rax
	movq	%rbx, %r12
	movq	%r11, %rbx
	movq	%r10, %r11
	movq	%rax, %r10
.L10:
	movl	40(%rbp), %eax
	vxorps	%xmm7, %xmm7, %xmm7
	vmovaps	%ymm7, 832(%rsp)
	vmovaps	%ymm7, %ymm4
	vmovaps	%ymm7, 864(%rsp)
	vmovaps	%ymm7, 896(%rsp)
	vmovaps	%ymm7, 928(%rsp)
	vmovaps	%ymm7, 960(%rsp)
	vmovaps	%ymm7, 992(%rsp)
	vmovaps	%ymm7, 1024(%rsp)
	vmovaps	%ymm7, 1056(%rsp)
	vmovaps	%ymm7, 1088(%rsp)
	vmovaps	%ymm7, 1120(%rsp)
	vmovaps	%ymm7, 1152(%rsp)
	vmovaps	%ymm7, 1184(%rsp)
	vmovaps	%ymm7, 1216(%rsp)
	vmovaps	%ymm7, 1248(%rsp)
	vmovaps	%ymm7, 1280(%rsp)
	vmovaps	%ymm7, 1312(%rsp)
	vmovaps	%ymm7, 512(%rsp)
	vmovaps	%ymm7, 544(%rsp)
	testl	%eax, %eax
	jle	.L19
	movslq	76(%rsp), %rax
	movl	68(%rsp), %edx
	movq	%r11, 472(%rsp)
	movslq	80(%rsp), %rcx
	movq	48(%rsp), %r14
	movq	%rax, %rdi
	movq	%rax, 264(%rsp)
	movl	%edx, %eax
	addl	%edi, %eax
	cltq
	movq	%rax, 256(%rsp)
	movslq	72(%rsp), %rax
	movq	%rax, %rdi
	movq	%rax, 216(%rsp)
	movl	%edx, %eax
	xorl	%edx, %edx
	addl	%edi, %eax
	movslq	84(%rsp), %rdi
	cltq
	movq	%rax, 208(%rsp)
	movq	%rdi, %rax
	addl	40(%rbp), %eax
	leaq	(%r14,%rdi), %r15
	movslq	%eax, %rsi
	movq	%rcx, %rax
	addl	40(%rbp), %eax
	movq	%r14, %rdi
	cltq
	leaq	(%rdi,%rcx), %r13
	leaq	(%r14,%rsi), %r14
	addq	%rdi, %rax
	movq	%rax, 200(%rsp)
	movq	%r13, %rax
	movq	%r15, %r13
	movq	%rax, %r15
	.p2align 4,,10
	.p2align 3
.L6:
	movl	%edx, %eax
	movq	264(%rsp), %rdi
	movq	208(%rsp), %r8
	sarl	$5, %eax
	cltq
	leaq	(%rdi,%rax), %rcx
	movq	256(%rsp), %rdi
	addq	%rax, %r8
	vmovss	(%rbx,%r8,4), %xmm0
	vinsertps	$0x10, 4(%rbx,%r8,4), %xmm0, %xmm0
	leaq	(%rax,%rdi), %rsi
	movq	216(%rsp), %rdi
	movq	144(%rsp), %r8
	addq	%rax, %rdi
	vmovss	(%rbx,%rdi,4), %xmm1
	vinsertps	$0x10, 4(%rbx,%rdi,4), %xmm1, %xmm1
	movq	160(%rsp), %rdi
	vmovlhps	%xmm0, %xmm1, %xmm1
	vmovss	(%rbx,%rsi,4), %xmm0
	vinsertps	$0x10, 4(%rbx,%rsi,4), %xmm0, %xmm2
	addq	%rax, %rdi
	vmovss	(%rbx,%rcx,4), %xmm0
	vinsertps	$0x10, 4(%rbx,%rcx,4), %xmm0, %xmm0
	movq	152(%rsp), %rcx
	leaq	0(,%rdi,4), %rsi
	vmovss	(%r12,%rdi,4), %xmm4
	vmovss	4(%r12,%rsi), %xmm7
	movq	%rsi, 336(%rsp)
	vmovlhps	%xmm2, %xmm0, %xmm0
	leaq	(%rcx,%rax), %rsi
	vinsertf128	$0x1, %xmm1, %ymm0, %ymm0
	vmovss	%xmm4, 468(%rsp)
	leaq	0(,%rsi,4), %rcx
	vmovss	(%r12,%rsi,4), %xmm4
	vmovss	%xmm7, 464(%rsp)
	movq	%rcx, 328(%rsp)
	vmovss	4(%r12,%rcx), %xmm7
	leaq	(%r8,%rax), %rcx
	leaq	0(,%rcx,4), %r8
	vmovss	%xmm4, 460(%rsp)
	vmovss	(%r12,%rcx,4), %xmm4
	vmovss	%xmm7, 416(%rsp)
	vmovss	4(%r12,%r8), %xmm7
	movq	%rcx, 288(%rsp)
	vmovaps	%ymm0, 480(%rsp)
	vmovss	%xmm4, 456(%rsp)
	vmovss	%xmm7, 384(%rsp)
	movq	%r8, 320(%rsp)
	movq	136(%rsp), %r8
	movq	120(%rsp), %r11
	addq	%rax, %r8
	vmovss	(%r12,%r8,4), %xmm4
	leaq	0(,%r8,4), %rcx
	vmovss	4(%r12,%rcx), %xmm7
	movq	%rcx, 280(%rsp)
	vmovss	%xmm4, 452(%rsp)
	vmovdqu	0(%r13,%rdx), %ymm4
	movq	200(%rsp), %rcx
	vmovss	%xmm7, 352(%rsp)
	vmovdqu	32(%r13,%rdx), %ymm7
	vmovdqa	%ymm4, 576(%rsp)
	vmovdqu	(%r14,%rdx), %ymm4
	vmovdqa	%ymm7, 608(%rsp)
	vmovdqu	32(%r14,%rdx), %ymm7
	vmovdqa	%ymm4, 640(%rsp)
	vmovdqu	(%r15,%rdx), %ymm4
	vmovdqa	%ymm7, 672(%rsp)
	vmovdqu	32(%r15,%rdx), %ymm7
	vmovdqa	%ymm4, 704(%rsp)
	vmovdqu	(%rcx,%rdx), %ymm4
	vmovdqa	%ymm7, 736(%rsp)
	vmovdqa	%ymm4, 768(%rsp)
	vmovdqu	32(%rcx,%rdx), %ymm4
	movl	%edx, %ecx
	sarl	%ecx
	vmovdqa	%ymm4, 224(%rsp)
	movslq	%ecx, %rcx
	vmovdqa	%ymm4, 800(%rsp)
	vmovdqu	(%r11,%rcx), %ymm1
	movq	184(%rsp), %r11
	addq	192(%rsp), %rcx
	vmovdqu	(%rcx,%r11), %ymm11
	movq	176(%rsp), %r11
	vpsrlw	$4, %ymm1, %ymm0
	vmovdqu	(%rcx,%r11), %ymm10
	movq	168(%rsp), %r11
	vpsrlw	$4, %ymm11, %ymm14
	vpand	%ymm6, %ymm11, %ymm11
	vpand	%ymm14, %ymm6, %ymm14
	vmovdqu	(%rcx,%r11), %ymm9
	movq	288(%rsp), %rcx
	vpsrlw	$4, %ymm10, %ymm13
	vpand	%ymm6, %ymm10, %ymm10
	vmovss	(%r9,%rdi,4), %xmm3
	movq	280(%rsp), %rdi
	vpand	%ymm13, %ymm6, %ymm13
	vinsertps	$0x10, (%r9,%rsi,4), %xmm3, %xmm3
	vmovss	(%r9,%rcx,4), %xmm2
	vpsrlw	$4, %ymm9, %ymm12
	vpand	%ymm6, %ymm9, %ymm9
	vinsertps	$0x10, (%r9,%r8,4), %xmm2, %xmm2
	movq	336(%rsp), %rsi
	vpand	%ymm12, %ymm6, %ymm12
	movq	320(%rsp), %r8
	movq	328(%rsp), %rcx
	vmovlhps	%xmm2, %xmm3, %xmm4
	movq	128(%rsp), %r11
	vmovss	4(%r9,%rsi), %xmm3
	vmovss	4(%r9,%r8), %xmm2
	vinsertps	$0x10, 4(%r9,%rcx), %xmm3, %xmm3
	movq	%r10, %rcx
	vmovaps	%xmm4, 288(%rsp)
	vinsertps	$0x10, 4(%r9,%rdi), %xmm2, %xmm2
	addq	272(%rsp), %rax
	vpand	%ymm0, %ymm6, %ymm4
	leaq	(%r11,%rax,4), %r8
	movq	112(%rsp), %rdi
	movq	104(%rsp), %rsi
	vmovlhps	%xmm2, %xmm3, %xmm7
	movq	96(%rsp), %rax
	movq	472(%rsp), %r11
	vmovaps	%xmm7, 336(%rsp)
	vpand	%ymm6, %ymm1, %ymm7
.L5:
	vmovdqa	%ymm5, %ymm15
	vmovss	(%rdi), %xmm0
	vmovdqa	%ymm5, %ymm2
	addq	$64, %rsi
	{vex} vpdpbusd	-64(%rsi), %ymm7, %ymm15
	vmovss	4(%rdi), %xmm1
	subq	$-128, %rcx
	addq	$8, %rdi
	vmulss	468(%rsp), %xmm0, %xmm3
	addq	$16, %rax
	vmulss	464(%rsp), %xmm1, %xmm8
	{vex} vpdpbusd	-32(%rsi), %ymm4, %ymm2
	vcvtdq2ps	%ymm15, %ymm15
	vbroadcastss	%xmm3, %ymm3
	vfmadd213ps	-128(%rcx), %ymm15, %ymm3
	vbroadcastss	%xmm8, %ymm8
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd132ps	%ymm8, %ymm3, %ymm2
	vbroadcastss	4(%r8), %xmm8
	vmovaps	288(%rsp), %xmm3
	vmovaps	%ymm2, -128(%rcx)
	vbroadcastss	(%r8), %xmm2
	vfmadd213ps	-16(%rax), %xmm3, %xmm2
	addq	%r11, %r8
	vfmadd132ps	336(%rsp), %xmm2, %xmm8
	vmulss	416(%rsp), %xmm1, %xmm2
	vmulss	460(%rsp), %xmm0, %xmm3
	vmovss	%xmm2, 472(%rsp)
	vmovdqa	%ymm5, %ymm2
	{vex} vpdpbusd	-64(%rsi), %ymm11, %ymm2
	vbroadcastss	%xmm3, %ymm3
	vcvtdq2ps	%ymm2, %ymm15
	vmovdqa	%ymm5, %ymm2
	vfmadd213ps	-96(%rcx), %ymm15, %ymm3
	vbroadcastss	472(%rsp), %ymm15
	{vex} vpdpbusd	-32(%rsi), %ymm14, %ymm2
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd132ps	%ymm15, %ymm3, %ymm2
	vmulss	456(%rsp), %xmm0, %xmm3
	vmulss	452(%rsp), %xmm0, %xmm0
	vmovaps	%ymm2, -96(%rcx)
	vmulss	384(%rsp), %xmm1, %xmm2
	vmulss	352(%rsp), %xmm1, %xmm1
	vbroadcastss	%xmm3, %ymm3
	vbroadcastss	%xmm0, %ymm0
	vmovss	%xmm2, 472(%rsp)
	vmovdqa	%ymm5, %ymm2
	{vex} vpdpbusd	-64(%rsi), %ymm10, %ymm2
	vbroadcastss	%xmm1, %ymm1
	vcvtdq2ps	%ymm2, %ymm15
	vmovdqa	%ymm5, %ymm2
	vfmadd213ps	-64(%rcx), %ymm15, %ymm3
	vbroadcastss	472(%rsp), %ymm15
	{vex} vpdpbusd	-32(%rsi), %ymm13, %ymm2
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd132ps	%ymm15, %ymm3, %ymm2
	vmovdqa	%ymm5, %ymm3
	{vex} vpdpbusd	-64(%rsi), %ymm9, %ymm3
	vmovaps	%ymm2, -64(%rcx)
	vmovdqa	%ymm5, %ymm2
	{vex} vpdpbusd	-32(%rsi), %ymm12, %ymm2
	vcvtdq2ps	%ymm3, %ymm3
	vfmadd213ps	-32(%rcx), %ymm3, %ymm0
	vcvtdq2ps	%ymm2, %ymm2
	vfmadd132ps	%ymm1, %ymm0, %ymm2
	vmovaps	%ymm2, -32(%rcx)
	vmovaps	%xmm8, -16(%rax)
	cmpq	%r10, %rsi
	jne	.L5
	movq	%r11, 472(%rsp)
	addq	$64, %rdx
	vmovaps	832(%rsp), %ymm0
	vmovss	512(%rsp), %xmm1
	cmpl	%edx, 40(%rbp)
	jg	.L6
	vmovss	528(%rsp), %xmm12
	vmovss	516(%rsp), %xmm7
	vmovaps	992(%rsp), %ymm13
	vmovss	532(%rsp), %xmm14
	vmovss	%xmm12, 460(%rsp)
	vmovaps	1024(%rsp), %ymm12
	vmovaps	864(%rsp), %ymm2
	vmovss	%xmm7, 472(%rsp)
	vmovss	520(%rsp), %xmm7
	vmovaps	896(%rsp), %ymm3
	vmovaps	%ymm13, 416(%rsp)
	vmovss	536(%rsp), %xmm13
	vmovaps	928(%rsp), %ymm4
	vmovss	%xmm14, 456(%rsp)
	vmovaps	1056(%rsp), %ymm14
	vmovaps	1120(%rsp), %ymm8
	vmovaps	%ymm12, 384(%rsp)
	vmovss	540(%rsp), %xmm12
	vmovaps	1152(%rsp), %ymm9
	vmovss	%xmm7, 468(%rsp)
	vmovss	524(%rsp), %xmm7
	vmovss	%xmm13, 452(%rsp)
	vmovaps	1088(%rsp), %ymm13
	vmovaps	%ymm14, 352(%rsp)
	vmovss	544(%rsp), %xmm14
	vmovss	%xmm12, 336(%rsp)
	vmovss	548(%rsp), %xmm12
	vmovss	%xmm7, 464(%rsp)
	vmovaps	960(%rsp), %ymm7
	vmovss	%xmm14, 328(%rsp)
	vmovss	%xmm12, 320(%rsp)
	vmovaps	%ymm13, 288(%rsp)
	vmovss	552(%rsp), %xmm13
	vmovss	556(%rsp), %xmm14
	vmovss	560(%rsp), %xmm12
	vmovss	572(%rsp), %xmm15
	vmovss	%xmm13, 280(%rsp)
	vmovss	564(%rsp), %xmm13
	vmovaps	1184(%rsp), %ymm10
	vmovss	%xmm14, 224(%rsp)
	vmovss	568(%rsp), %xmm14
	vmovaps	1216(%rsp), %ymm11
	vmovss	%xmm12, 264(%rsp)
	vmovaps	1248(%rsp), %ymm12
	vmovss	%xmm13, 256(%rsp)
	vmovaps	1280(%rsp), %ymm13
	vmovss	%xmm14, 216(%rsp)
	vmovaps	1312(%rsp), %ymm14
	vmovss	%xmm15, 208(%rsp)
.L9:
	vextractf128	$0x1, %ymm0, %xmm15
	movq	88(%rsp), %rax
	vaddps	%xmm0, %xmm15, %xmm0
	addq	$16000, %rax
	vmovhlps	%xmm0, %xmm0, %xmm15
	vaddps	%xmm15, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm15
	vaddss	%xmm15, %xmm0, %xmm0
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-16000(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -16000(%rax)
	vextractf128	$0x1, %ymm2, %xmm0
	vaddps	%xmm2, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	472(%rsp), %xmm0, %xmm0
	vaddss	-15996(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -15996(%rax)
	vextractf128	$0x1, %ymm3, %xmm0
	vaddps	%xmm3, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	468(%rsp), %xmm0, %xmm0
	vaddss	-15992(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -15992(%rax)
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm1
	vmovaps	416(%rsp), %ymm4
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	464(%rsp), %xmm0, %xmm0
	vaddss	-15988(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -15988(%rax)
	vextractf128	$0x1, %ymm7, %xmm0
	vaddps	%xmm7, %xmm0, %xmm1
	vmovaps	384(%rsp), %ymm7
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	460(%rsp), %xmm0, %xmm0
	vaddss	-12000(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -12000(%rax)
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm1
	vmovaps	352(%rsp), %ymm4
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	456(%rsp), %xmm0, %xmm0
	vaddss	-11996(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -11996(%rax)
	vextractf128	$0x1, %ymm7, %xmm0
	vaddps	%xmm7, %xmm0, %xmm1
	vmovaps	288(%rsp), %ymm7
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	452(%rsp), %xmm0, %xmm0
	vaddss	-11992(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -11992(%rax)
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	336(%rsp), %xmm0, %xmm0
	vaddss	-11988(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -11988(%rax)
	vextractf128	$0x1, %ymm7, %xmm0
	vaddps	%xmm7, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	328(%rsp), %xmm0, %xmm0
	vaddss	-8000(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -8000(%rax)
	vextractf128	$0x1, %ymm8, %xmm0
	vaddps	%xmm8, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	320(%rsp), %xmm0, %xmm0
	vaddss	-7996(%rax), %xmm0, %xmm0
	movl	60(%rsp), %edi
	addl	$4, 448(%rsp)
	movl	448(%rsp), %edx
	addl	%edi, 84(%rsp)
	vmovss	%xmm0, -7996(%rax)
	vextractf128	$0x1, %ymm9, %xmm0
	vaddps	%xmm9, %xmm0, %xmm0
	addl	%edi, 80(%rsp)
	movl	64(%rsp), %edi
	addq	%r11, 272(%rsp)
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	280(%rsp), %xmm0, %xmm0
	vaddss	-7992(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -7992(%rax)
	vextractf128	$0x1, %ymm10, %xmm0
	vaddps	%xmm10, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	224(%rsp), %xmm0, %xmm0
	vaddss	-7988(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -7988(%rax)
	vextractf128	$0x1, %ymm11, %xmm0
	vaddps	%xmm11, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	264(%rsp), %xmm0, %xmm0
	vaddss	-4000(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -4000(%rax)
	vextractf128	$0x1, %ymm12, %xmm0
	vaddps	%xmm12, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	256(%rsp), %xmm0, %xmm0
	vaddss	-3996(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -3996(%rax)
	vextractf128	$0x1, %ymm13, %xmm0
	vaddps	%xmm13, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	216(%rsp), %xmm0, %xmm0
	vaddss	-3992(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -3992(%rax)
	vextractf128	$0x1, %ymm14, %xmm0
	vaddps	%xmm14, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	208(%rsp), %xmm0, %xmm0
	vaddss	-3988(%rax), %xmm0, %xmm0
	vmovss	%xmm0, -3988(%rax)
	movq	%rax, 88(%rsp)
	addl	%edi, 76(%rsp)
	addl	%edi, 72(%rsp)
	cmpl	%edx, 24(%rbp)
	jg	.L10
	movl	68(%rsp), %eax
	movl	24(%rsp), %edi
	movq	%r11, %r10
	movq	%rbx, %r11
	movl	32(%rsp), %esi
	addq	$4, 40(%rsp)
	movq	%r12, %rbx
	leal	(%rax,%rax,2), %ecx
	movl	56(%rsp), %eax
	addl	%ecx, %edi
	addl	28(%rsp), %ecx
	leal	(%rax,%rax,2), %edx
	movq	40(%rsp), %rax
	addl	%edx, %esi
	addl	36(%rsp), %edx
	cmpl	%eax, 32(%rbp)
	jg	.L4
	vzeroupper
.L1:
	movq	1368(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L20
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
	movl	$0x00000000, 208(%rsp)
	vmovaps	%ymm7, %ymm14
	vmovaps	%ymm7, %ymm13
	vmovaps	%ymm7, %ymm12
	movl	$0x00000000, 216(%rsp)
	vmovaps	%ymm7, %ymm11
	vmovaps	%ymm7, %ymm10
	vmovaps	%ymm7, %ymm9
	movl	$0x00000000, 256(%rsp)
	vmovaps	%ymm7, %ymm8
	vmovaps	%ymm7, %ymm3
	vmovaps	%ymm7, %ymm2
	movl	$0x00000000, 264(%rsp)
	vxorps	%xmm1, %xmm1, %xmm1
	vmovaps	%ymm7, %ymm0
	movl	$0x00000000, 224(%rsp)
	movl	$0x00000000, 280(%rsp)
	movl	$0x00000000, 320(%rsp)
	movl	$0x00000000, 328(%rsp)
	movl	$0x00000000, 336(%rsp)
	movl	$0x00000000, 452(%rsp)
	movl	$0x00000000, 456(%rsp)
	movl	$0x00000000, 460(%rsp)
	movl	$0x00000000, 464(%rsp)
	movl	$0x00000000, 468(%rsp)
	movl	$0x00000000, 472(%rsp)
	vmovaps	%ymm7, 288(%rsp)
	vmovaps	%ymm7, 352(%rsp)
	vmovaps	%ymm7, 384(%rsp)
	vmovaps	%ymm7, 416(%rsp)
	jmp	.L9
.L20:
	call	__stack_chk_fail@PLT
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
