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
	.cfi_offset 15, -24
	movq	%rdx, %r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$1376, %rsp
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	16(%rbp), %r12
	movl	24(%rbp), %eax
	movq	%rdi, 80(%rsp)
	movq	%rsi, 232(%rsp)
	movq	%rdx, 24(%rsp)
	movq	%r8, 160(%rsp)
	movq	%fs:40, %rdx
	movq	%rdx, 1368(%rsp)
	xorl	%edx, %edx
	movl	32(%rbp), %edx
	testl	%edx, %edx
	jle	.L1
	movl	$5, %edx
	sarx	%edx, 40(%rbp), %edi
	movl	$1, %edx
	movl	%edi, 112(%rsp)
	sarx	%edx, 40(%rbp), %esi
	movl	%esi, 68(%rsp)
	testl	%eax, %eax
	jle	.L1
	subl	$1, %eax
	leal	0(,%rdi,4), %edx
	movq	%rcx, %r11
	movq	%r9, %rbx
	shrl	$2, %eax
	movl	%edx, 92(%rsp)
	leal	(%rsi,%rsi), %edx
	movl	40(%rbp), %esi
	imulq	$4000, %rax, %rax
	leal	(%rdi,%rdi), %ecx
	movq	%r12, %r10
	movq	$0, 72(%rsp)
	sall	$2, %esi
	movl	%ecx, 40(%rsp)
	vmovdqa	.LC1(%rip), %ymm6
	movl	%esi, 88(%rsp)
	movl	40(%rbp), %esi
	movq	%rax, 56(%rsp)
	movslq	%edi, %rax
	xorl	%edi, %edi
	salq	$2, %rax
	addl	%esi, %esi
	movq	%rax, 168(%rsp)
	leaq	16000(%r15), %rax
	movq	%rax, 16(%rsp)
	leaq	480(%rsp), %rax
	movq	%rax, 144(%rsp)
	leaq	576(%rsp), %rax
	movq	%rax, 136(%rsp)
	movl	%esi, 36(%rsp)
	xorl	%esi, %esi
.L4:
	movslq	%edi, %rax
	movl	$0, 120(%rsp)
	vpxor	%xmm5, %xmm5, %xmm5
	leaq	832(%rsp), %r9
	movq	%rax, 200(%rsp)
	movl	112(%rsp), %eax
	movl	$0, 448(%rsp)
	addl	%eax, %edi
	movl	%edi, 44(%rsp)
	movslq	%edi, %rdi
	movq	%rdi, 192(%rsp)
	movslq	%ecx, %rdi
	addl	%eax, %ecx
	movslq	%ecx, %rax
	movl	%ecx, 48(%rsp)
	movslq	%esi, %rcx
	movq	%rax, 176(%rsp)
	movl	68(%rsp), %eax
	movq	%rdi, 184(%rsp)
	addl	%eax, %esi
	movslq	%esi, %rdi
	movl	%esi, 52(%rsp)
	movq	%rdi, 224(%rsp)
	movslq	%edx, %rdi
	addl	%eax, %edx
	movslq	%edx, %rax
	movl	%edx, 64(%rsp)
	movq	72(%rsp), %rdx
	movq	%rax, 208(%rsp)
	movq	24(%rsp), %rax
	movq	%rdi, 216(%rsp)
	leaq	(%rax,%rdx,4), %r12
	movq	56(%rsp), %rax
	addq	%rdx, %rax
	movq	16(%rsp), %rdx
	leaq	(%rdx,%rax,4), %rax
	movq	%rax, 96(%rsp)
	movl	40(%rsp), %eax
	movl	%eax, 116(%rsp)
	movl	36(%rsp), %eax
	movl	%eax, 124(%rsp)
	movq	232(%rsp), %rax
	addq	%rcx, %rax
	movq	%rax, 152(%rsp)
	leaq	512(%rsp), %rax
	movq	%rax, 128(%rsp)
	movq	%rbx, %rax
	movq	%r11, %rbx
	movq	%r12, %r11
	movq	%rax, %r12
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
	movslq	120(%rsp), %rax
	movl	112(%rsp), %edx
	movq	%r9, 472(%rsp)
	movslq	124(%rsp), %rcx
	movq	80(%rsp), %r14
	movq	%r11, 104(%rsp)
	movq	%rax, %rdi
	movq	%rax, 272(%rsp)
	movl	%edx, %eax
	addl	%edi, %eax
	cltq
	movq	%rax, 264(%rsp)
	movslq	116(%rsp), %rax
	movq	%rax, %rdi
	movq	%rax, 256(%rsp)
	movl	%edx, %eax
	addl	%edi, %eax
	movslq	448(%rsp), %rdi
	cltq
	movq	%rax, 248(%rsp)
	movq	%rdi, %rax
	addl	40(%rbp), %eax
	leaq	(%r14,%rdi), %r15
	movslq	%eax, %rsi
	movq	%rcx, %rax
	addl	40(%rbp), %eax
	movq	%r14, %rdi
	movslq	%eax, %rdx
	leaq	(%r14,%rsi), %r14
	leaq	(%rdi,%rcx), %r13
	xorl	%eax, %eax
	addq	%rdi, %rdx
	movq	%r13, %r11
	movq	%rdx, 240(%rsp)
	movq	%r14, %rdx
	movq	%r15, %r14
	movq	%rdx, %r15
	.p2align 4,,10
	.p2align 3
.L6:
	movl	%eax, %edx
	movq	272(%rsp), %rdi
	movq	248(%rsp), %r9
	sarl	$5, %edx
	movslq	%edx, %rdx
	leaq	(%rdi,%rdx), %rcx
	movq	264(%rsp), %rdi
	leaq	(%rdx,%r9), %r8
	movq	184(%rsp), %r9
	vmovss	(%rbx,%r8,4), %xmm0
	vinsertps	$0x10, 4(%rbx,%r8,4), %xmm0, %xmm0
	leaq	(%rdx,%rdi), %rsi
	movq	256(%rsp), %rdi
	addq	%rdx, %r9
	leaq	0(,%r9,4), %r8
	addq	%rdx, %rdi
	movq	%r8, 320(%rsp)
	vmovss	(%rbx,%rdi,4), %xmm1
	vinsertps	$0x10, 4(%rbx,%rdi,4), %xmm1, %xmm1
	movq	200(%rsp), %rdi
	vmovlhps	%xmm0, %xmm1, %xmm1
	vmovss	(%rbx,%rsi,4), %xmm0
	vinsertps	$0x10, 4(%rbx,%rsi,4), %xmm0, %xmm2
	addq	%rdx, %rdi
	vmovss	(%rbx,%rcx,4), %xmm0
	vinsertps	$0x10, 4(%rbx,%rcx,4), %xmm0, %xmm0
	movq	192(%rsp), %rcx
	leaq	0(,%rdi,4), %rsi
	vmovss	(%r12,%rdi,4), %xmm4
	vmovss	4(%r12,%rsi), %xmm7
	movq	%rsi, 336(%rsp)
	vmovlhps	%xmm2, %xmm0, %xmm0
	leaq	(%rcx,%rdx), %rsi
	vinsertf128	$0x1, %xmm1, %ymm0, %ymm0
	vmovss	%xmm4, 468(%rsp)
	leaq	0(,%rsi,4), %rcx
	vmovss	(%r12,%rsi,4), %xmm4
	vmovss	%xmm7, 464(%rsp)
	vmovss	4(%r12,%rcx), %xmm7
	movq	%rcx, 328(%rsp)
	movq	176(%rsp), %rcx
	vmovss	%xmm4, 460(%rsp)
	vmovss	(%r12,%r9,4), %xmm4
	vmovss	%xmm7, 416(%rsp)
	vmovss	4(%r12,%r8), %xmm7
	vmovaps	%ymm0, 480(%rsp)
	leaq	(%rcx,%rdx), %r8
	vmovss	%xmm7, 384(%rsp)
	leaq	0(,%r8,4), %rcx
	vmovss	%xmm4, 456(%rsp)
	vmovss	(%r12,%r8,4), %xmm4
	vmovss	4(%r12,%rcx), %xmm7
	movq	%rcx, 280(%rsp)
	movq	240(%rsp), %rcx
	vmovss	%xmm4, 452(%rsp)
	vmovdqu	(%r14,%rax), %ymm4
	movq	152(%rsp), %r13
	vmovss	%xmm7, 352(%rsp)
	vmovdqu	32(%r14,%rax), %ymm7
	vmovdqa	%ymm4, 576(%rsp)
	vmovdqu	(%r15,%rax), %ymm4
	vmovdqa	%ymm7, 608(%rsp)
	vmovdqu	32(%r15,%rax), %ymm7
	vmovdqa	%ymm4, 640(%rsp)
	vmovdqu	(%r11,%rax), %ymm4
	vmovdqa	%ymm7, 672(%rsp)
	vmovdqu	32(%r11,%rax), %ymm7
	vmovdqa	%ymm4, 704(%rsp)
	vmovdqu	(%rcx,%rax), %ymm4
	vmovdqa	%ymm7, 736(%rsp)
	vmovdqa	%ymm4, 768(%rsp)
	vmovdqu	32(%rcx,%rax), %ymm4
	movl	%eax, %ecx
	sarl	%ecx
	vmovdqa	%ymm4, 288(%rsp)
	movslq	%ecx, %rcx
	vmovdqa	%ymm4, 800(%rsp)
	vmovdqu	0(%r13,%rcx), %ymm1
	movq	224(%rsp), %r13
	addq	232(%rsp), %rcx
	vmovdqu	(%rcx,%r13), %ymm11
	movq	216(%rsp), %r13
	vpsrlw	$4, %ymm1, %ymm0
	vmovdqu	(%rcx,%r13), %ymm10
	movq	208(%rsp), %r13
	vpsrlw	$4, %ymm11, %ymm14
	vpand	%ymm6, %ymm11, %ymm11
	vpand	%ymm14, %ymm6, %ymm14
	vmovdqu	(%rcx,%r13), %ymm9
	vmovss	(%r10,%r9,4), %xmm2
	vpsrlw	$4, %ymm10, %ymm13
	vpand	%ymm6, %ymm10, %ymm10
	vinsertps	$0x10, (%r10,%r8,4), %xmm2, %xmm2
	vmovss	(%r10,%rdi,4), %xmm3
	vpand	%ymm13, %ymm6, %ymm13
	vinsertps	$0x10, (%r10,%rsi,4), %xmm3, %xmm3
	movq	320(%rsp), %r8
	vpsrlw	$4, %ymm9, %ymm12
	vpand	%ymm6, %ymm9, %ymm9
	movq	336(%rsp), %rsi
	movq	280(%rsp), %rdi
	vpand	%ymm12, %ymm6, %ymm12
	movq	328(%rsp), %rcx
	vmovlhps	%xmm2, %xmm3, %xmm4
	movq	160(%rsp), %r9
	vmovss	4(%r10,%r8), %xmm2
	vmovss	4(%r10,%rsi), %xmm3
	vmovaps	%xmm4, 288(%rsp)
	vpand	%ymm0, %ymm6, %ymm4
	vinsertps	$0x10, 4(%r10,%rdi), %xmm2, %xmm2
	vinsertps	$0x10, 4(%r10,%rcx), %xmm3, %xmm3
	leaq	(%r9,%rdx,4), %r8
	movq	%rax, %r9
	movq	472(%rsp), %r13
	movq	144(%rsp), %rdi
	vmovlhps	%xmm2, %xmm3, %xmm7
	movq	136(%rsp), %rsi
	movq	128(%rsp), %rdx
	movq	168(%rsp), %rax
	movq	%r13, %rcx
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
	addq	$16, %rdx
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
	vfmadd213ps	-16(%rdx), %xmm3, %xmm2
	addq	%rax, %r8
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
	vmovaps	%xmm8, -16(%rdx)
	cmpq	%r13, %rsi
	jne	.L5
	movq	%r9, %rax
	movq	%r13, 472(%rsp)
	vmovaps	832(%rsp), %ymm0
	vmovss	512(%rsp), %xmm1
	addq	$64, %rax
	cmpl	%eax, 40(%rbp)
	jg	.L6
	vmovss	528(%rsp), %xmm12
	movq	104(%rsp), %r11
	movq	%r13, %r9
	vmovss	516(%rsp), %xmm7
	vmovaps	992(%rsp), %ymm13
	vmovss	532(%rsp), %xmm14
	vmovss	%xmm12, 460(%rsp)
	vmovaps	1024(%rsp), %ymm12
	vmovss	%xmm7, 472(%rsp)
	vmovss	520(%rsp), %xmm7
	vmovaps	864(%rsp), %ymm2
	vmovaps	%ymm13, 416(%rsp)
	vmovss	536(%rsp), %xmm13
	vmovaps	896(%rsp), %ymm3
	vmovss	%xmm14, 456(%rsp)
	vmovaps	1056(%rsp), %ymm14
	vmovaps	928(%rsp), %ymm4
	vmovaps	%ymm12, 384(%rsp)
	vmovss	540(%rsp), %xmm12
	vmovaps	1120(%rsp), %ymm8
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
	vmovaps	%ymm13, 288(%rsp)
	vmovss	%xmm14, 328(%rsp)
	vmovss	%xmm12, 320(%rsp)
	vmovaps	1152(%rsp), %ymm9
	vmovss	552(%rsp), %xmm13
	vmovss	556(%rsp), %xmm14
	vmovss	560(%rsp), %xmm12
	vmovss	%xmm13, 280(%rsp)
	vmovss	564(%rsp), %xmm13
	vmovss	572(%rsp), %xmm15
	vmovss	%xmm14, 272(%rsp)
	vmovss	568(%rsp), %xmm14
	vmovaps	1184(%rsp), %ymm10
	vmovss	%xmm12, 264(%rsp)
	vmovaps	1216(%rsp), %ymm11
	vmovaps	1248(%rsp), %ymm12
	vmovss	%xmm13, 256(%rsp)
	vmovaps	1280(%rsp), %ymm13
	vmovss	%xmm14, 248(%rsp)
	vmovaps	1312(%rsp), %ymm14
	vmovss	%xmm15, 240(%rsp)
.L9:
	vextractf128	$0x1, %ymm0, %xmm15
	addq	$16000, %r11
	vaddps	%xmm0, %xmm15, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm15
	vaddps	%xmm15, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm15
	vaddss	%xmm15, %xmm0, %xmm0
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	-16000(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -16000(%r11)
	vextractf128	$0x1, %ymm2, %xmm0
	vaddps	%xmm2, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	472(%rsp), %xmm0, %xmm0
	vaddss	-15996(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -15996(%r11)
	vextractf128	$0x1, %ymm3, %xmm0
	vaddps	%xmm3, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	468(%rsp), %xmm0, %xmm0
	vaddss	-15992(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -15992(%r11)
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm1
	vmovaps	416(%rsp), %ymm4
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	464(%rsp), %xmm0, %xmm0
	vaddss	-15988(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -15988(%r11)
	vextractf128	$0x1, %ymm7, %xmm0
	vaddps	%xmm7, %xmm0, %xmm1
	vmovaps	384(%rsp), %ymm7
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	460(%rsp), %xmm0, %xmm0
	vaddss	-12000(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -12000(%r11)
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm1
	vmovaps	352(%rsp), %ymm4
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	456(%rsp), %xmm0, %xmm0
	vaddss	-11996(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -11996(%r11)
	vextractf128	$0x1, %ymm7, %xmm0
	vaddps	%xmm7, %xmm0, %xmm1
	vmovaps	288(%rsp), %ymm7
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	452(%rsp), %xmm0, %xmm0
	vaddss	-11992(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -11992(%r11)
	vextractf128	$0x1, %ymm4, %xmm0
	vaddps	%xmm4, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	336(%rsp), %xmm0, %xmm0
	vaddss	-11988(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -11988(%r11)
	vextractf128	$0x1, %ymm7, %xmm0
	vaddps	%xmm7, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	328(%rsp), %xmm0, %xmm0
	vaddss	-8000(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -8000(%r11)
	vextractf128	$0x1, %ymm8, %xmm0
	vaddps	%xmm8, %xmm0, %xmm1
	vmovhlps	%xmm1, %xmm1, %xmm0
	vaddps	%xmm0, %xmm1, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	320(%rsp), %xmm0, %xmm0
	vaddss	-7996(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -7996(%r11)
	vextractf128	$0x1, %ymm9, %xmm0
	movl	88(%rsp), %edx
	vaddps	%xmm9, %xmm0, %xmm0
	addl	%edx, 448(%rsp)
	addl	%edx, 124(%rsp)
	movl	92(%rsp), %edx
	addl	%edx, 120(%rsp)
	vmovhlps	%xmm0, %xmm0, %xmm1
	addl	%edx, 116(%rsp)
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	280(%rsp), %xmm0, %xmm0
	vaddss	-7992(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -7992(%r11)
	vextractf128	$0x1, %ymm10, %xmm0
	vaddps	%xmm10, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	272(%rsp), %xmm0, %xmm0
	vaddss	-7988(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -7988(%r11)
	vextractf128	$0x1, %ymm11, %xmm0
	vaddps	%xmm11, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	264(%rsp), %xmm0, %xmm0
	vaddss	-4000(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -4000(%r11)
	vextractf128	$0x1, %ymm12, %xmm0
	vaddps	%xmm12, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	256(%rsp), %xmm0, %xmm0
	vaddss	-3996(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -3996(%r11)
	vextractf128	$0x1, %ymm13, %xmm0
	vaddps	%xmm13, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	248(%rsp), %xmm0, %xmm0
	vaddss	-3992(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -3992(%r11)
	vextractf128	$0x1, %ymm14, %xmm0
	vaddps	%xmm14, %xmm0, %xmm0
	vmovhlps	%xmm0, %xmm0, %xmm1
	vaddps	%xmm1, %xmm0, %xmm0
	vmovshdup	%xmm0, %xmm1
	vaddss	%xmm1, %xmm0, %xmm0
	vaddss	240(%rsp), %xmm0, %xmm0
	vaddss	-3988(%r11), %xmm0, %xmm0
	vmovss	%xmm0, -3988(%r11)
	cmpq	%r11, 96(%rsp)
	jne	.L10
	movl	112(%rsp), %eax
	movl	44(%rsp), %edi
	movq	%rbx, %r11
	movq	%r12, %rbx
	movl	52(%rsp), %esi
	addq	$4, 72(%rsp)
	leal	(%rax,%rax,2), %ecx
	movl	68(%rsp), %eax
	addl	%ecx, %edi
	addl	48(%rsp), %ecx
	leal	(%rax,%rax,2), %edx
	movq	72(%rsp), %rax
	addl	%edx, %esi
	addl	64(%rsp), %edx
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
	movl	$0x00000000, 240(%rsp)
	vmovaps	%ymm7, %ymm14
	vmovaps	%ymm7, %ymm13
	vmovaps	%ymm7, %ymm12
	movl	$0x00000000, 248(%rsp)
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
	movl	$0x00000000, 272(%rsp)
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
