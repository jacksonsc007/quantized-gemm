	.file	"peak_flops.cpp"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC7:
	.string	"Peak FLOPS: "
.LC8:
	.string	" ("
.LC9:
	.string	" GFLOPS)\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB7577:
	.cfi_startproc
	endbr64
	leaq	8(%rsp), %r10
	.cfi_def_cfa 10, 0
	andq	$-32, %rsp
	pushq	-8(%r10)
	pushq	%rbp
	movq	%rsp, %rbp
	.cfi_escape 0x10,0x6,0x2,0x76,0
	pushq	%r12
	pushq	%r10
	.cfi_escape 0xf,0x3,0x76,0x70,0x6
	.cfi_escape 0x10,0xc,0x2,0x76,0x78
	subq	$64, %rsp
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	vmovaps	.LC0(%rip), %ymm0
	vmovaps	.LC1(%rip), %ymm2
	vmovaps	.LC2(%rip), %ymm1
	movq	%rax, %r12
	movl	$100000000, %eax
	.p2align 4,,10
	.p2align 3
.L2:
	vfmadd231ps	%ymm1, %ymm2, %ymm0
	subq	$1, %rax
	jne	.L2
	vmovaps	%ymm0, -80(%rbp)
	vzeroupper
	call	_ZNSt6chrono3_V212system_clock3nowEv@PLT
	vmovaps	-80(%rbp), %ymm0
	movl	$12, %edx
	vmovss	.LC4(%rip), %xmm1
	movl	$0x00000000, -20(%rbp)
	vmovss	-20(%rbp), %xmm5
	subq	%r12, %rax
	leaq	_ZSt4cout(%rip), %r12
	vmovaps	%xmm0, %xmm2
	vshufps	$85, %xmm0, %xmm0, %xmm4
	vshufps	$255, %xmm0, %xmm0, %xmm3
	movq	%r12, %rdi
	vfmadd132ss	%xmm1, %xmm0, %xmm2
	leaq	.LC7(%rip), %rsi
	vaddss	%xmm2, %xmm0, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm5, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovaps	%xmm4, %xmm2
	vmovss	-20(%rbp), %xmm5
	vfmadd132ss	%xmm1, %xmm4, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vunpckhps	%xmm0, %xmm0, %xmm4
	vextractf128	$0x1, %ymm0, %xmm0
	vaddss	%xmm5, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovaps	%xmm4, %xmm2
	vmovss	-20(%rbp), %xmm5
	vfmadd132ss	%xmm1, %xmm4, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vaddss	%xmm5, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovaps	%xmm3, %xmm2
	vmovss	-20(%rbp), %xmm4
	vfmadd132ss	%xmm1, %xmm3, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vshufps	$85, %xmm0, %xmm0, %xmm3
	vaddss	%xmm4, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovaps	%xmm0, %xmm2
	vmovss	-20(%rbp), %xmm4
	vfmadd132ss	%xmm1, %xmm0, %xmm2
	vaddss	%xmm2, %xmm0, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm0, %xmm2, %xmm2
	vaddss	%xmm4, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovaps	%xmm3, %xmm2
	vmovss	-20(%rbp), %xmm4
	vfmadd132ss	%xmm1, %xmm3, %xmm2
	vaddss	%xmm2, %xmm3, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm3, %xmm2, %xmm2
	vunpckhps	%xmm0, %xmm0, %xmm3
	vshufps	$255, %xmm0, %xmm0, %xmm0
	vaddss	%xmm4, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovaps	%xmm3, %xmm2
	vmovss	-20(%rbp), %xmm4
	vfmadd132ss	%xmm1, %xmm3, %xmm2
	vfmadd132ss	%xmm0, %xmm0, %xmm1
	vaddss	%xmm2, %xmm3, %xmm2
	vaddss	%xmm0, %xmm1, %xmm1
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm0, %xmm1, %xmm1
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm0, %xmm1, %xmm1
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm0, %xmm1, %xmm1
	vaddss	%xmm3, %xmm2, %xmm2
	vaddss	%xmm0, %xmm1, %xmm0
	vmovsd	.LC6(%rip), %xmm1
	vaddss	%xmm4, %xmm2, %xmm2
	vmovss	%xmm2, -20(%rbp)
	vmovss	-20(%rbp), %xmm2
	vaddss	%xmm2, %xmm0, %xmm0
	vmovss	%xmm0, -20(%rbp)
	vxorps	%xmm0, %xmm0, %xmm0
	vcvtsi2sdq	%rax, %xmm0, %xmm0
	vdivsd	.LC5(%rip), %xmm0, %xmm0
	vdivsd	%xmm0, %xmm1, %xmm6
	vmovsd	%xmm6, -80(%rbp)
	vzeroupper
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	vmovsd	-80(%rbp), %xmm0
	movq	%r12, %rdi
	call	_ZNSo9_M_insertIdEERSoT_@PLT
	movl	$2, %edx
	leaq	.LC8(%rip), %rsi
	movq	%rax, %r12
	movq	%rax, %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	movq	%r12, %rdi
	vmovsd	-80(%rbp), %xmm7
	vdivsd	.LC5(%rip), %xmm7, %xmm0
	call	_ZNSo9_M_insertIdEERSoT_@PLT
	movl	$9, %edx
	leaq	.LC9(%rip), %rsi
	movq	%rax, %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	addq	$64, %rsp
	xorl	%eax, %eax
	popq	%r10
	.cfi_def_cfa 10, 0
	popq	%r12
	popq	%rbp
	leaq	-8(%r10), %rsp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7577:
	.size	main, .-main
	.p2align 4
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB8106:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	_ZStL8__ioinit(%rip), %rbp
	movq	%rbp, %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rdi
	movq	%rbp, %rsi
	popq	%rbp
	.cfi_def_cfa_offset 8
	leaq	__dso_handle(%rip), %rdx
	jmp	__cxa_atexit@PLT
	.cfi_endproc
.LFE8106:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.align 32
.LC1:
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.long	1073741824
	.align 32
.LC2:
	.long	1077936128
	.long	1077936128
	.long	1077936128
	.long	1077936128
	.long	1077936128
	.long	1077936128
	.long	1077936128
	.long	1077936128
	.set	.LC4,.LC1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC5:
	.long	0
	.long	1104006501
	.align 8
.LC6:
	.long	0
	.long	1107810180
	.hidden	__dso_handle
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
