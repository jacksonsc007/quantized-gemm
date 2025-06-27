Iterations:        100
Instructions:      3700
Total Cycles:      827
Total uOps:        3600

Dispatch Width:    6
uOps Per Cycle:    4.35
IPC:               4.47
Block RThroughput: 6.5


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%edx, %eax
 1      8     0.33    *                   vmovdqu	(%rsi,%rdx), %ymm3
 1      1     0.50                        sarl	$5, %eax
 1      1     0.50                        cltq
 1      1     0.50                        vpsignb	%ymm3, %ymm3, %ymm9
 1      1     1.00                        leaq	(%rbx,%rax), %rcx
 1      1     0.20                        addq	%r11, %rax
 1      7     0.33    *                   vmovss	(%rdi,%rcx,4), %xmm8
 1      7     0.33    *                   vmovss	4(%rdi,%rcx,4), %xmm4
 2      11    0.50    *                   vmulss	(%r10,%rax,4), %xmm8, %xmm8
 2      11    0.50    *                   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
 1      0     0.20                        movl	%edx, %eax
 1      1     0.50                        sarl	%eax
 1      1     0.50                        cltq
 1      8     0.33    *                   vmovdqu	(%r9,%rax), %ymm0
 1      3     1.00                        vbroadcastss	%xmm8, %ymm8
 1      1     0.50                        vpsrlw	$4, %ymm0, %ymm2
 1      1     0.33                        vpand	%ymm7, %ymm0, %ymm0
 1      3     1.00                        vbroadcastss	%xmm4, %ymm4
 1      1     0.33                        vpaddb	%ymm0, %ymm6, %ymm0
 1      1     0.50                        vpsignb	%ymm3, %ymm0, %ymm0
 0      1     0.00                        vmovdqa	%ymm5, %ymm3
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
 1      8     0.33    *                   vmovdqu	32(%rsi,%rdx), %ymm9
 1      1     0.33                        vpand	%ymm2, %ymm7, %ymm0
 0      1     0.00                        vmovdqa	%ymm5, %ymm2
 1      1     0.33                        vpaddb	%ymm0, %ymm6, %ymm0
 0      1     0.00                        addq	$64, %rdx
 1      1     0.50                        vpsignb	%ymm9, %ymm0, %ymm0
 1      1     0.50                        vpsignb	%ymm9, %ymm9, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm3
 1      4     0.50                        vfmadd231ps	%ymm8, %ymm3, %ymm1
 1      4     0.50                        vcvtdq2ps	%ymm2, %ymm0
 1      4     0.50                        vfmadd231ps	%ymm4, %ymm0, %ymm1
 1      1     0.20                        cmpl	%edx, %r8d
 1      1     0.50                        jg	.L5


Resources:
[0]   - ADLPPort00
[1]   - ADLPPort01
[2]   - ADLPPort02
[3]   - ADLPPort03
[4]   - ADLPPort04
[5]   - ADLPPort05
[6]   - ADLPPort06
[7]   - ADLPPort07
[8]   - ADLPPort08
[9]   - ADLPPort09
[10]  - ADLPPort10
[11]  - ADLPPort11
[12]  - ADLPPortInvalid


Resource pressure per iteration:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   
7.40   7.43   2.33   2.33    -     7.21   3.69    -      -      -     3.27   2.34    -     

Resource pressure by instruction:
[0]    [1]    [2]    [3]    [4]    [5]    [6]    [7]    [8]    [9]    [10]   [11]   [12]   Instructions:
 -      -      -      -      -     0.01   0.27    -      -      -     0.72    -      -     movl	%edx, %eax
 -      -     0.33   0.29    -      -      -      -      -      -      -     0.38    -     vmovdqu	(%rsi,%rdx), %ymm3
0.01    -      -      -      -      -     0.99    -      -      -      -      -      -     sarl	$5, %eax
 -     0.02    -      -      -     0.98    -      -      -      -      -      -      -     cltq
0.14   0.86    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm3, %ymm9
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     leaq	(%rbx,%rax), %rcx
0.01    -      -      -      -      -     0.13    -      -      -     0.86    -      -     addq	%r11, %rax
 -      -     0.33   0.39    -      -      -      -      -      -      -     0.28    -     vmovss	(%rdi,%rcx,4), %xmm8
 -      -     0.39   0.28    -      -      -      -      -      -      -     0.33    -     vmovss	4(%rdi,%rcx,4), %xmm4
0.49   0.51   0.35   0.37    -      -      -      -      -      -      -     0.28    -     vmulss	(%r10,%rax,4), %xmm8, %xmm8
0.21   0.79   0.31   0.31    -      -      -      -      -      -      -     0.38    -     vmulss	4(%r10,%rax,4), %xmm4, %xmm4
 -      -      -      -      -      -     0.09    -      -      -     0.91    -      -     movl	%edx, %eax
0.01    -      -      -      -      -     0.99    -      -      -      -      -      -     sarl	%eax
 -     0.01    -      -      -     0.99    -      -      -      -      -      -      -     cltq
 -      -     0.37   0.34    -      -      -      -      -      -      -     0.29    -     vmovdqu	(%r9,%rax), %ymm0
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm8, %ymm8
0.53   0.47    -      -      -      -      -      -      -      -      -      -      -     vpsrlw	$4, %ymm0, %ymm2
0.03   0.04    -      -      -     0.93    -      -      -      -      -      -      -     vpand	%ymm7, %ymm0, %ymm0
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm4, %ymm4
0.18   0.02    -      -      -     0.80    -      -      -      -      -      -      -     vpaddb	%ymm0, %ymm6, %ymm0
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm0, %ymm0
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm5, %ymm3
0.48   0.52    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
 -      -     0.25   0.35    -      -      -      -      -      -      -     0.40    -     vmovdqu	32(%rsi,%rdx), %ymm9
0.02   0.27    -      -      -     0.71    -      -      -      -      -      -      -     vpand	%ymm2, %ymm7, %ymm0
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm5, %ymm2
0.05   0.16    -      -      -     0.79    -      -      -      -      -      -      -     vpaddb	%ymm0, %ymm6, %ymm0
 -      -      -      -      -      -      -      -      -      -      -      -      -     addq	$64, %rdx
0.37   0.63    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm0, %ymm0
0.42   0.58    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm9, %ymm9, %ymm10
0.66   0.34    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
0.73   0.27    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm3
0.98   0.02    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm8, %ymm3, %ymm1
0.68   0.32    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm2, %ymm0
0.89   0.11    -      -      -      -      -      -      -      -      -      -      -     vfmadd231ps	%ymm4, %ymm0, %ymm1
 -      -      -      -      -      -     0.22    -      -      -     0.78    -      -     cmpl	%edx, %r8d
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L5


Timeline view:
                    0123456789          0123456789          0123456789          0123456789
Index     0123456789          0123456789          0123456789          0123456789          

[0,0]     DER  .    .    .    .    .    .    .    .    .    .    .    .    .    .    .   .   movl	%edx, %eax
[0,1]     DeeeeeeeeER    .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovdqu	(%rsi,%rdx), %ymm3
[0,2]     DeE-------R    .    .    .    .    .    .    .    .    .    .    .    .    .   .   sarl	$5, %eax
[0,3]     D=eE------R    .    .    .    .    .    .    .    .    .    .    .    .    .   .   cltq
[0,4]     D========eER   .    .    .    .    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm3, %ymm9
[0,5]     D==eE------R   .    .    .    .    .    .    .    .    .    .    .    .    .   .   leaq	(%rbx,%rax), %rcx
[0,6]     .D=eE------R   .    .    .    .    .    .    .    .    .    .    .    .    .   .   addq	%r11, %rax
[0,7]     .D==eeeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	(%rdi,%rcx,4), %xmm8
[0,8]     .D==eeeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .    .   .   vmovss	4(%rdi,%rcx,4), %xmm4
[0,9]     .D===eeeeeeeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[0,10]    . D==eeeeeeeeeeeER  .    .    .    .    .    .    .    .    .    .    .    .   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[0,11]    . DE-------------R  .    .    .    .    .    .    .    .    .    .    .    .   .   movl	%edx, %eax
[0,12]    . DeE------------R  .    .    .    .    .    .    .    .    .    .    .    .   .   sarl	%eax
[0,13]    . D=eE-----------R  .    .    .    .    .    .    .    .    .    .    .    .   .   cltq
[0,14]    . D==eeeeeeeeE---R  .    .    .    .    .    .    .    .    .    .    .    .   .   vmovdqu	(%r9,%rax), %ymm0
[0,15]    .  D============eeeER    .    .    .    .    .    .    .    .    .    .    .   .   vbroadcastss	%xmm8, %ymm8
[0,16]    .  D=========eE-----R    .    .    .    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[0,17]    .  D=========eE-----R    .    .    .    .    .    .    .    .    .    .    .   .   vpand	%ymm7, %ymm0, %ymm0
[0,18]    .  D=============eeeER   .    .    .    .    .    .    .    .    .    .    .   .   vbroadcastss	%xmm4, %ymm4
[0,19]    .  D==========eE-----R   .    .    .    .    .    .    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[0,20]    .  D===========eE----R   .    .    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm0, %ymm0
[0,21]    .   DeE--------------R   .    .    .    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm3
[0,22]    .   D===========eeeeeER  .    .    .    .    .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[0,23]    .   D=eeeeeeeeE-------R  .    .    .    .    .    .    .    .    .    .    .   .   vmovdqu	32(%rsi,%rdx), %ymm9
[0,24]    .   D=========eE------R  .    .    .    .    .    .    .    .    .    .    .   .   vpand	%ymm2, %ymm7, %ymm0
[0,25]    .   DeE---------------R  .    .    .    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm2
[0,26]    .   D==========eE-----R  .    .    .    .    .    .    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[0,27]    .   DeE---------------R  .    .    .    .    .    .    .    .    .    .    .   .   addq	$64, %rdx
[0,28]    .   D===========eE----R  .    .    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm0, %ymm0
[0,29]    .   D=========eE------R  .    .    .    .    .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm9, %ymm10
[0,30]    .    D===========eeeeeER .    .    .    .    .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[0,31]    .    D===============eeeeER   .    .    .    .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm3, %ymm3
[0,32]    .    D===================eeeeER    .    .    .    .    .    .    .    .    .   .   vfmadd231ps	%ymm8, %ymm3, %ymm1
[0,33]    .    D================eeeeE---R    .    .    .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm2, %ymm0
[0,34]    .    D=======================eeeeER.    .    .    .    .    .    .    .    .   .   vfmadd231ps	%ymm4, %ymm0, %ymm1
[0,35]    .    DeE--------------------------R.    .    .    .    .    .    .    .    .   .   cmpl	%edx, %r8d
[0,36]    .    .DeE-------------------------R.    .    .    .    .    .    .    .    .   .   jg	.L5
[1,0]     .    .DE--------------------------R.    .    .    .    .    .    .    .    .   .   movl	%edx, %eax
[1,1]     .    .DeeeeeeeeE------------------R.    .    .    .    .    .    .    .    .   .   vmovdqu	(%rsi,%rdx), %ymm3
[1,2]     .    .DeE-------------------------R.    .    .    .    .    .    .    .    .   .   sarl	$5, %eax
[1,3]     .    .D=eE------------------------R.    .    .    .    .    .    .    .    .   .   cltq
[1,4]     .    .D========eE-----------------R.    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm3, %ymm9
[1,5]     .    . D==eE----------------------R.    .    .    .    .    .    .    .    .   .   leaq	(%rbx,%rax), %rcx
[1,6]     .    . D=eE-----------------------R.    .    .    .    .    .    .    .    .   .   addq	%r11, %rax
[1,7]     .    . D===eeeeeeeE---------------R.    .    .    .    .    .    .    .    .   .   vmovss	(%rdi,%rcx,4), %xmm8
[1,8]     .    . D===eeeeeeeE---------------R.    .    .    .    .    .    .    .    .   .   vmovss	4(%rdi,%rcx,4), %xmm4
[1,9]     .    . D====eeeeeeeeeeeE----------R.    .    .    .    .    .    .    .    .   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[1,10]    .    .  D===eeeeeeeeeeeE----------R.    .    .    .    .    .    .    .    .   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[1,11]    .    .  DE------------------------R.    .    .    .    .    .    .    .    .   .   movl	%edx, %eax
[1,12]    .    .  DeE-----------------------R.    .    .    .    .    .    .    .    .   .   sarl	%eax
[1,13]    .    .  D=eE----------------------R.    .    .    .    .    .    .    .    .   .   cltq
[1,14]    .    .  D==eeeeeeeeE--------------R.    .    .    .    .    .    .    .    .   .   vmovdqu	(%r9,%rax), %ymm0
[1,15]    .    .   D=============eeeE-------R.    .    .    .    .    .    .    .    .   .   vbroadcastss	%xmm8, %ymm8
[1,16]    .    .   D=========eE-------------R.    .    .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[1,17]    .    .   D=========eE-------------R.    .    .    .    .    .    .    .    .   .   vpand	%ymm7, %ymm0, %ymm0
[1,18]    .    .   D==============eeeE------R.    .    .    .    .    .    .    .    .   .   vbroadcastss	%xmm4, %ymm4
[1,19]    .    .   D==========eE------------R.    .    .    .    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[1,20]    .    .   D===========eE-----------R.    .    .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm0, %ymm0
[1,21]    .    .    DeE---------------------R.    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm3
[1,22]    .    .    D===========eeeeeE------R.    .    .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[1,23]    .    .    D=eeeeeeeeE-------------R.    .    .    .    .    .    .    .    .   .   vmovdqu	32(%rsi,%rdx), %ymm9
[1,24]    .    .    D=========eE------------R.    .    .    .    .    .    .    .    .   .   vpand	%ymm2, %ymm7, %ymm0
[1,25]    .    .    DeE---------------------R.    .    .    .    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm2
[1,26]    .    .    D==========eE-----------R.    .    .    .    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[1,27]    .    .    DeE---------------------R.    .    .    .    .    .    .    .    .   .   addq	$64, %rdx
[1,28]    .    .    D============eE---------R.    .    .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm0, %ymm0
[1,29]    .    .    D=========eE------------R.    .    .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm9, %ymm10
[1,30]    .    .    .D============eeeeeE----R.    .    .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[1,31]    .    .    .D===============eeeeE--R.    .    .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm3, %ymm3
[1,32]    .    .    .D=====================eeeeER .    .    .    .    .    .    .    .   .   vfmadd231ps	%ymm8, %ymm3, %ymm1
[1,33]    .    .    .D=================eeeeE----R .    .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm2, %ymm0
[1,34]    .    .    .D=========================eeeeER  .    .    .    .    .    .    .   .   vfmadd231ps	%ymm4, %ymm0, %ymm1
[1,35]    .    .    .DeE----------------------------R  .    .    .    .    .    .    .   .   cmpl	%edx, %r8d
[1,36]    .    .    . DeE---------------------------R  .    .    .    .    .    .    .   .   jg	.L5
[2,0]     .    .    . DE----------------------------R  .    .    .    .    .    .    .   .   movl	%edx, %eax
[2,1]     .    .    . DeeeeeeeeE--------------------R  .    .    .    .    .    .    .   .   vmovdqu	(%rsi,%rdx), %ymm3
[2,2]     .    .    . D=eE--------------------------R  .    .    .    .    .    .    .   .   sarl	$5, %eax
[2,3]     .    .    . D=====eE----------------------R  .    .    .    .    .    .    .   .   cltq
[2,4]     .    .    . D==========eE-----------------R  .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm3, %ymm9
[2,5]     .    .    .  D=====eE---------------------R  .    .    .    .    .    .    .   .   leaq	(%rbx,%rax), %rcx
[2,6]     .    .    .  D=====eE---------------------R  .    .    .    .    .    .    .   .   addq	%r11, %rax
[2,7]     .    .    .  D======eeeeeeeE--------------R  .    .    .    .    .    .    .   .   vmovss	(%rdi,%rcx,4), %xmm8
[2,8]     .    .    .  D======eeeeeeeE--------------R  .    .    .    .    .    .    .   .   vmovss	4(%rdi,%rcx,4), %xmm4
[2,9]     .    .    .  D==========eeeeeeeeeeeE------R  .    .    .    .    .    .    .   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[2,10]    .    .    .   D==========eeeeeeeeeeeE-----R  .    .    .    .    .    .    .   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[2,11]    .    .    .   DE--------------------------R  .    .    .    .    .    .    .   .   movl	%edx, %eax
[2,12]    .    .    .   DeE-------------------------R  .    .    .    .    .    .    .   .   sarl	%eax
[2,13]    .    .    .   D===eE----------------------R  .    .    .    .    .    .    .   .   cltq
[2,14]    .    .    .   D====eeeeeeeeE--------------R  .    .    .    .    .    .    .   .   vmovdqu	(%r9,%rax), %ymm0
[2,15]    .    .    .    D===================eeeE---R  .    .    .    .    .    .    .   .   vbroadcastss	%xmm8, %ymm8
[2,16]    .    .    .    D===========eE-------------R  .    .    .    .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[2,17]    .    .    .    D===========eE-------------R  .    .    .    .    .    .    .   .   vpand	%ymm7, %ymm0, %ymm0
[2,18]    .    .    .    D====================eeeE--R  .    .    .    .    .    .    .   .   vbroadcastss	%xmm4, %ymm4
[2,19]    .    .    .    D============eE------------R  .    .    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[2,20]    .    .    .    D==============eE----------R  .    .    .    .    .    .    .   .   vpsignb	%ymm3, %ymm0, %ymm0
[2,21]    .    .    .    .DeE-----------------------R  .    .    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm3
[2,22]    .    .    .    .D==============eeeeeE-----R  .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[2,23]    .    .    .    .DeeeeeeeeE----------------R  .    .    .    .    .    .    .   .   vmovdqu	32(%rsi,%rdx), %ymm9
[2,24]    .    .    .    .D===========eE------------R  .    .    .    .    .    .    .   .   vpand	%ymm2, %ymm7, %ymm0
[2,25]    .    .    .    .DeE-----------------------R  .    .    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm2
[2,26]    .    .    .    .D============eE-----------R  .    .    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[2,27]    .    .    .    .DeE-----------------------R  .    .    .    .    .    .    .   .   addq	$64, %rdx
[2,28]    .    .    .    .D=============eE----------R  .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm0, %ymm0
[2,29]    .    .    .    .D=========eE--------------R  .    .    .    .    .    .    .   .   vpsignb	%ymm9, %ymm9, %ymm10
[2,30]    .    .    .    . D=============eeeeeE-----R  .    .    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[2,31]    .    .    .    . D==================eeeeE-R  .    .    .    .    .    .    .   .   vcvtdq2ps	%ymm3, %ymm3
[2,32]    .    .    .    . D=======================eeeeER   .    .    .    .    .    .   .   vfmadd231ps	%ymm8, %ymm3, %ymm1
[2,33]    .    .    .    . D==================eeeeE-----R   .    .    .    .    .    .   .   vcvtdq2ps	%ymm2, %ymm0
[2,34]    .    .    .    . D===========================eeeeER    .    .    .    .    .   .   vfmadd231ps	%ymm4, %ymm0, %ymm1
[2,35]    .    .    .    . DeE------------------------------R    .    .    .    .    .   .   cmpl	%edx, %r8d
[2,36]    .    .    .    .  D=eE----------------------------R    .    .    .    .    .   .   jg	.L5
[3,0]     .    .    .    .  DE------------------------------R    .    .    .    .    .   .   movl	%edx, %eax
[3,1]     .    .    .    .  DeeeeeeeeE----------------------R    .    .    .    .    .   .   vmovdqu	(%rsi,%rdx), %ymm3
[3,2]     .    .    .    .  D==eE---------------------------R    .    .    .    .    .   .   sarl	$5, %eax
[3,3]     .    .    .    .  D===eE--------------------------R    .    .    .    .    .   .   cltq
[3,4]     .    .    .    .  D=========eE--------------------R    .    .    .    .    .   .   vpsignb	%ymm3, %ymm3, %ymm9
[3,5]     .    .    .    .   D======eE----------------------R    .    .    .    .    .   .   leaq	(%rbx,%rax), %rcx
[3,6]     .    .    .    .   D===eE-------------------------R    .    .    .    .    .   .   addq	%r11, %rax
[3,7]     .    .    .    .   D=======eeeeeeeE---------------R    .    .    .    .    .   .   vmovss	(%rdi,%rcx,4), %xmm8
[3,8]     .    .    .    .   D=======eeeeeeeE---------------R    .    .    .    .    .   .   vmovss	4(%rdi,%rcx,4), %xmm4
[3,9]     .    .    .    .   D============eeeeeeeeeeeE------R    .    .    .    .    .   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[3,10]    .    .    .    .    D===========eeeeeeeeeeeE------R    .    .    .    .    .   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[3,11]    .    .    .    .    DE----------------------------R    .    .    .    .    .   .   movl	%edx, %eax
[3,12]    .    .    .    .    D=eE--------------------------R    .    .    .    .    .   .   sarl	%eax
[3,13]    .    .    .    .    D====eE-----------------------R    .    .    .    .    .   .   cltq
[3,14]    .    .    .    .    D=====eeeeeeeeE---------------R    .    .    .    .    .   .   vmovdqu	(%r9,%rax), %ymm0
[3,15]    .    .    .    .    .D=====================eeeE---R    .    .    .    .    .   .   vbroadcastss	%xmm8, %ymm8
[3,16]    .    .    .    .    .D============eE--------------R    .    .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[3,17]    .    .    .    .    .D============eE--------------R    .    .    .    .    .   .   vpand	%ymm7, %ymm0, %ymm0
[3,18]    .    .    .    .    .D======================eeeE--R    .    .    .    .    .   .   vbroadcastss	%xmm4, %ymm4
[3,19]    .    .    .    .    .D=============eE-------------R    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[3,20]    .    .    .    .    .D===============eE-----------R    .    .    .    .    .   .   vpsignb	%ymm3, %ymm0, %ymm0
[3,21]    .    .    .    .    . DeE-------------------------R    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm3
[3,22]    .    .    .    .    . D===============eeeeeE------R    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[3,23]    .    .    .    .    . DeeeeeeeeE------------------R    .    .    .    .    .   .   vmovdqu	32(%rsi,%rdx), %ymm9
[3,24]    .    .    .    .    . D============eE-------------R    .    .    .    .    .   .   vpand	%ymm2, %ymm7, %ymm0
[3,25]    .    .    .    .    . DeE-------------------------R    .    .    .    .    .   .   vmovdqa	%ymm5, %ymm2
[3,26]    .    .    .    .    . D==============eE-----------R    .    .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[3,27]    .    .    .    .    . DeE-------------------------R    .    .    .    .    .   .   addq	$64, %rdx
[3,28]    .    .    .    .    . D===============eE----------R    .    .    .    .    .   .   vpsignb	%ymm9, %ymm0, %ymm0
[3,29]    .    .    .    .    . D==========eE---------------R    .    .    .    .    .   .   vpsignb	%ymm9, %ymm9, %ymm10
[3,30]    .    .    .    .    .  D===============eeeeeE-----R    .    .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[3,31]    .    .    .    .    .  D===================eeeeE--R    .    .    .    .    .   .   vcvtdq2ps	%ymm3, %ymm3
[3,32]    .    .    .    .    .  D=========================eeeeER.    .    .    .    .   .   vfmadd231ps	%ymm8, %ymm3, %ymm1
[3,33]    .    .    .    .    .  D====================eeeeE-----R.    .    .    .    .   .   vcvtdq2ps	%ymm2, %ymm0
[3,34]    .    .    .    .    .  D=============================eeeeER .    .    .    .   .   vfmadd231ps	%ymm4, %ymm0, %ymm1
[3,35]    .    .    .    .    .  DeE--------------------------------R .    .    .    .   .   cmpl	%edx, %r8d
[3,36]    .    .    .    .    .   DeE-------------------------------R .    .    .    .   .   jg	.L5
[4,0]     .    .    .    .    .   DE--------------------------------R .    .    .    .   .   movl	%edx, %eax
[4,1]     .    .    .    .    .   DeeeeeeeeE------------------------R .    .    .    .   .   vmovdqu	(%rsi,%rdx), %ymm3
[4,2]     .    .    .    .    .   D=eE------------------------------R .    .    .    .   .   sarl	$5, %eax
[4,3]     .    .    .    .    .   D=====eE--------------------------R .    .    .    .   .   cltq
[4,4]     .    .    .    .    .   D=========eE----------------------R .    .    .    .   .   vpsignb	%ymm3, %ymm3, %ymm9
[4,5]     .    .    .    .    .    D=============eE-----------------R .    .    .    .   .   leaq	(%rbx,%rax), %rcx
[4,6]     .    .    .    .    .    D=====eE-------------------------R .    .    .    .   .   addq	%r11, %rax
[4,7]     .    .    .    .    .    D==============eeeeeeeE----------R .    .    .    .   .   vmovss	(%rdi,%rcx,4), %xmm8
[4,8]     .    .    .    .    .    D==============eeeeeeeE----------R .    .    .    .   .   vmovss	4(%rdi,%rcx,4), %xmm4
[4,9]     .    .    .    .    .    D===============eeeeeeeeeeeE-----R .    .    .    .   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[4,10]    .    .    .    .    .    .D===============eeeeeeeeeeeE----R .    .    .    .   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[4,11]    .    .    .    .    .    .DE------------------------------R .    .    .    .   .   movl	%edx, %eax
[4,12]    .    .    .    .    .    .DeE-----------------------------R .    .    .    .   .   sarl	%eax
[4,13]    .    .    .    .    .    .D====eE-------------------------R .    .    .    .   .   cltq
[4,14]    .    .    .    .    .    .D=====eeeeeeeeE-----------------R .    .    .    .   .   vmovdqu	(%r9,%rax), %ymm0
[4,15]    .    .    .    .    .    . D========================eeeE--R .    .    .    .   .   vbroadcastss	%xmm8, %ymm8
[4,16]    .    .    .    .    .    . D============eE----------------R .    .    .    .   .   vpsrlw	$4, %ymm0, %ymm2
[4,17]    .    .    .    .    .    . D============eE----------------R .    .    .    .   .   vpand	%ymm7, %ymm0, %ymm0
[4,18]    .    .    .    .    .    . D=========================eeeE-R .    .    .    .   .   vbroadcastss	%xmm4, %ymm4
[4,19]    .    .    .    .    .    . D=============eE---------------R .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[4,20]    .    .    .    .    .    . D==============eE--------------R .    .    .    .   .   vpsignb	%ymm3, %ymm0, %ymm0
[4,21]    .    .    .    .    .    .  DeE---------------------------R .    .    .    .   .   vmovdqa	%ymm5, %ymm3
[4,22]    .    .    .    .    .    .  D==============eeeeeE---------R .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[4,23]    .    .    .    .    .    .  DeeeeeeeeE--------------------R .    .    .    .   .   vmovdqu	32(%rsi,%rdx), %ymm9
[4,24]    .    .    .    .    .    .  D=============eE--------------R .    .    .    .   .   vpand	%ymm2, %ymm7, %ymm0
[4,25]    .    .    .    .    .    .  DeE---------------------------R .    .    .    .   .   vmovdqa	%ymm5, %ymm2
[4,26]    .    .    .    .    .    .  D===============eE------------R .    .    .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[4,27]    .    .    .    .    .    .  DeE---------------------------R .    .    .    .   .   addq	$64, %rdx
[4,28]    .    .    .    .    .    .  D================eE-----------R .    .    .    .   .   vpsignb	%ymm9, %ymm0, %ymm0
[4,29]    .    .    .    .    .    .  D===========eE----------------R .    .    .    .   .   vpsignb	%ymm9, %ymm9, %ymm10
[4,30]    .    .    .    .    .    .   D================eeeeeE------R .    .    .    .   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[4,31]    .    .    .    .    .    .   D==================eeeeE-----R .    .    .    .   .   vcvtdq2ps	%ymm3, %ymm3
[4,32]    .    .    .    .    .    .   D===========================eeeeER  .    .    .   .   vfmadd231ps	%ymm8, %ymm3, %ymm1
[4,33]    .    .    .    .    .    .   D=====================eeeeE------R  .    .    .   .   vcvtdq2ps	%ymm2, %ymm0
[4,34]    .    .    .    .    .    .   D===============================eeeeER   .    .   .   vfmadd231ps	%ymm4, %ymm0, %ymm1
[4,35]    .    .    .    .    .    .   DeE----------------------------------R   .    .   .   cmpl	%edx, %r8d
[4,36]    .    .    .    .    .    .    D=eE--------------------------------R   .    .   .   jg	.L5
[5,0]     .    .    .    .    .    .    DE----------------------------------R   .    .   .   movl	%edx, %eax
[5,1]     .    .    .    .    .    .    DeeeeeeeeE--------------------------R   .    .   .   vmovdqu	(%rsi,%rdx), %ymm3
[5,2]     .    .    .    .    .    .    D==eE-------------------------------R   .    .   .   sarl	$5, %eax
[5,3]     .    .    .    .    .    .    D=======eE--------------------------R   .    .   .   cltq
[5,4]     .    .    .    .    .    .    D===============eE------------------R   .    .   .   vpsignb	%ymm3, %ymm3, %ymm9
[5,5]     .    .    .    .    .    .    .D===============eE-----------------R   .    .   .   leaq	(%rbx,%rax), %rcx
[5,6]     .    .    .    .    .    .    .D=======eE-------------------------R   .    .   .   addq	%r11, %rax
[5,7]     .    .    .    .    .    .    .D================eeeeeeeE----------R   .    .   .   vmovss	(%rdi,%rcx,4), %xmm8
[5,8]     .    .    .    .    .    .    .D================eeeeeeeE----------R   .    .   .   vmovss	4(%rdi,%rcx,4), %xmm4
[5,9]     .    .    .    .    .    .    .D=================eeeeeeeeeeeE-----R   .    .   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[5,10]    .    .    .    .    .    .    . D=================eeeeeeeeeeeE----R   .    .   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[5,11]    .    .    .    .    .    .    . DE--------------------------------R   .    .   .   movl	%edx, %eax
[5,12]    .    .    .    .    .    .    . D=eE------------------------------R   .    .   .   sarl	%eax
[5,13]    .    .    .    .    .    .    . D======eE-------------------------R   .    .   .   cltq
[5,14]    .    .    .    .    .    .    . D=======eeeeeeeeE-----------------R   .    .   .   vmovdqu	(%r9,%rax), %ymm0
[5,15]    .    .    .    .    .    .    .  D==========================eeeE--R   .    .   .   vbroadcastss	%xmm8, %ymm8
[5,16]    .    .    .    .    .    .    .  D==============eE----------------R   .    .   .   vpsrlw	$4, %ymm0, %ymm2
[5,17]    .    .    .    .    .    .    .  D==============eE----------------R   .    .   .   vpand	%ymm7, %ymm0, %ymm0
[5,18]    .    .    .    .    .    .    .  D===========================eeeE-R   .    .   .   vbroadcastss	%xmm4, %ymm4
[5,19]    .    .    .    .    .    .    .  D===============eE---------------R   .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[5,20]    .    .    .    .    .    .    .  D================eE--------------R   .    .   .   vpsignb	%ymm3, %ymm0, %ymm0
[5,21]    .    .    .    .    .    .    .   DeE-----------------------------R   .    .   .   vmovdqa	%ymm5, %ymm3
[5,22]    .    .    .    .    .    .    .   D================eeeeeE---------R   .    .   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[5,23]    .    .    .    .    .    .    .   DeeeeeeeeE----------------------R   .    .   .   vmovdqu	32(%rsi,%rdx), %ymm9
[5,24]    .    .    .    .    .    .    .   D===============eE--------------R   .    .   .   vpand	%ymm2, %ymm7, %ymm0
[5,25]    .    .    .    .    .    .    .   DeE-----------------------------R   .    .   .   vmovdqa	%ymm5, %ymm2
[5,26]    .    .    .    .    .    .    .   D================eE-------------R   .    .   .   vpaddb	%ymm0, %ymm6, %ymm0
[5,27]    .    .    .    .    .    .    .   DeE-----------------------------R   .    .   .   addq	$64, %rdx
[5,28]    .    .    .    .    .    .    .   D=================eE------------R   .    .   .   vpsignb	%ymm9, %ymm0, %ymm0
[5,29]    .    .    .    .    .    .    .   D============eE-----------------R   .    .   .   vpsignb	%ymm9, %ymm9, %ymm10
[5,30]    .    .    .    .    .    .    .    D=================eeeeeE-------R   .    .   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[5,31]    .    .    .    .    .    .    .    D====================eeeeE-----R   .    .   .   vcvtdq2ps	%ymm3, %ymm3
[5,32]    .    .    .    .    .    .    .    D=============================eeeeER    .   .   vfmadd231ps	%ymm8, %ymm3, %ymm1
[5,33]    .    .    .    .    .    .    .    D======================eeeeE-------R    .   .   vcvtdq2ps	%ymm2, %ymm0
[5,34]    .    .    .    .    .    .    .    D=================================eeeeER.   .   vfmadd231ps	%ymm4, %ymm0, %ymm1
[5,35]    .    .    .    .    .    .    .    DeE------------------------------------R.   .   cmpl	%edx, %r8d
[5,36]    .    .    .    .    .    .    .    .DeE-----------------------------------R.   .   jg	.L5
[6,0]     .    .    .    .    .    .    .    .DE------------------------------------R.   .   movl	%edx, %eax
[6,1]     .    .    .    .    .    .    .    .DeeeeeeeeE----------------------------R.   .   vmovdqu	(%rsi,%rdx), %ymm3
[6,2]     .    .    .    .    .    .    .    .D=eE----------------------------------R.   .   sarl	$5, %eax
[6,3]     .    .    .    .    .    .    .    .D========eE---------------------------R.   .   cltq
[6,4]     .    .    .    .    .    .    .    .D===============eE--------------------R.   .   vpsignb	%ymm3, %ymm3, %ymm9
[6,5]     .    .    .    .    .    .    .    . D================eE------------------R.   .   leaq	(%rbx,%rax), %rcx
[6,6]     .    .    .    .    .    .    .    . D========eE--------------------------R.   .   addq	%r11, %rax
[6,7]     .    .    .    .    .    .    .    . D=================eeeeeeeE-----------R.   .   vmovss	(%rdi,%rcx,4), %xmm8
[6,8]     .    .    .    .    .    .    .    . D=================eeeeeeeE-----------R.   .   vmovss	4(%rdi,%rcx,4), %xmm4
[6,9]     .    .    .    .    .    .    .    . D==================eeeeeeeeeeeE------R.   .   vmulss	(%r10,%rax,4), %xmm8, %xmm8
[6,10]    .    .    .    .    .    .    .    .  D==================eeeeeeeeeeeE-----R.   .   vmulss	4(%r10,%rax,4), %xmm4, %xmm4
[6,11]    .    .    .    .    .    .    .    .  DE----------------------------------R.   .   movl	%edx, %eax
[6,12]    .    .    .    .    .    .    .    .  D=eE--------------------------------R.   .   sarl	%eax
[6,13]    .    .    .    .    .    .    .    .  D=======eE--------------------------R.   .   cltq
[6,14]    .    .    .    .    .    .    .    .  D========eeeeeeeeE------------------R.   .   vmovdqu	(%r9,%rax), %ymm0
[6,15]    .    .    .    .    .    .    .    .   D===========================eeeE---R.   .   vbroadcastss	%xmm8, %ymm8
[6,16]    .    .    .    .    .    .    .    .   D===============eE-----------------R.   .   vpsrlw	$4, %ymm0, %ymm2
[6,17]    .    .    .    .    .    .    .    .   D===============eE-----------------R.   .   vpand	%ymm7, %ymm0, %ymm0
[6,18]    .    .    .    .    .    .    .    .   D============================eeeE--R.   .   vbroadcastss	%xmm4, %ymm4
[6,19]    .    .    .    .    .    .    .    .   D================eE----------------R.   .   vpaddb	%ymm0, %ymm6, %ymm0
[6,20]    .    .    .    .    .    .    .    .   D==================eE--------------R.   .   vpsignb	%ymm3, %ymm0, %ymm0
[6,21]    .    .    .    .    .    .    .    .    DeE-------------------------------R.   .   vmovdqa	%ymm5, %ymm3
[6,22]    .    .    .    .    .    .    .    .    D==================eeeeeE---------R.   .   {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
[6,23]    .    .    .    .    .    .    .    .    DeeeeeeeeE------------------------R.   .   vmovdqu	32(%rsi,%rdx), %ymm9
[6,24]    .    .    .    .    .    .    .    .    D================eE---------------R.   .   vpand	%ymm2, %ymm7, %ymm0
[6,25]    .    .    .    .    .    .    .    .    DeE-------------------------------R.   .   vmovdqa	%ymm5, %ymm2
[6,26]    .    .    .    .    .    .    .    .    D=================eE--------------R.   .   vpaddb	%ymm0, %ymm6, %ymm0
[6,27]    .    .    .    .    .    .    .    .    DeE-------------------------------R.   .   addq	$64, %rdx
[6,28]    .    .    .    .    .    .    .    .    D==================eE-------------R.   .   vpsignb	%ymm9, %ymm0, %ymm0
[6,29]    .    .    .    .    .    .    .    .    D=============eE------------------R.   .   vpsignb	%ymm9, %ymm9, %ymm10
[6,30]    .    .    .    .    .    .    .    .    .D==================eeeeeE--------R.   .   {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
[6,31]    .    .    .    .    .    .    .    .    .D======================eeeeE-----R.   .   vcvtdq2ps	%ymm3, %ymm3
[6,32]    .    .    .    .    .    .    .    .    .D===============================eeeeER.   vfmadd231ps	%ymm8, %ymm3, %ymm1
[6,33]    .    .    .    .    .    .    .    .    .D=======================eeeeE--------R.   vcvtdq2ps	%ymm2, %ymm0
Truncated display due to cycle limit


Average Wait times (based on the timeline view):
[0]: Executions
[1]: Average time spent waiting in a scheduler's queue
[2]: Average time spent waiting in a scheduler's queue while ready
[3]: Average time elapsed from WB until retire stage

      [0]    [1]    [2]    [3]
0.     10    1.0    1.0    30.6      movl	%edx, %eax
1.     10    1.1    1.1    23.3      vmovdqu	(%rsi,%rdx), %ymm3
2.     10    2.0    1.0    29.4      sarl	$5, %eax
3.     10    6.5    3.5    24.9      cltq
4.     10    12.4   3.3    19.1      vpsignb	%ymm3, %ymm3, %ymm9
5.     10    11.8   5.2    18.8      leaq	(%rbx,%rax), %rcx
6.     10    6.5    0.0    24.0      addq	%r11, %rax
7.     10    12.7   0.0    11.9      vmovss	(%rdi,%rcx,4), %xmm8
8.     10    12.7   0.0    11.9      vmovss	4(%rdi,%rcx,4), %xmm4
9.     10    14.6   0.9    6.5       vmulss	(%r10,%rax,4), %xmm8, %xmm8
10.    10    14.1   1.4    6.0       vmulss	4(%r10,%rax,4), %xmm4, %xmm4
11.    10    1.0    1.0    30.1      movl	%edx, %eax
12.    10    1.3    0.3    28.8      sarl	%eax
13.    10    6.8    4.5    23.3      cltq
14.    10    7.8    0.0    15.3      vmovdqu	(%r9,%rax), %ymm0
15.    10    23.6   0.0    3.8       vbroadcastss	%xmm8, %ymm8
16.    10    14.8   0.0    14.6      vpsrlw	$4, %ymm0, %ymm2
17.    10    14.8   0.0    14.6      vpand	%ymm7, %ymm0, %ymm0
18.    10    24.6   0.5    2.9       vbroadcastss	%xmm4, %ymm4
19.    10    15.8   0.0    13.7      vpaddb	%ymm0, %ymm6, %ymm0
20.    10    17.2   0.4    12.3      vpsignb	%ymm3, %ymm0, %ymm0
21.    10    1.0    1.0    27.5      vmovdqa	%ymm5, %ymm3
22.    10    17.2   0.0    7.4       {vex}	vpdpbusd	%ymm0, %ymm9, %ymm3
23.    10    1.2    1.2    20.4      vmovdqu	32(%rsi,%rdx), %ymm9
24.    10    15.3   0.5    13.3      vpand	%ymm2, %ymm7, %ymm0
25.    10    1.0    1.0    27.6      vmovdqa	%ymm5, %ymm2
26.    10    16.5   0.2    12.1      vpaddb	%ymm0, %ymm6, %ymm0
27.    10    1.0    1.0    27.6      addq	$64, %rdx
28.    10    17.7   0.2    10.9      vpsignb	%ymm9, %ymm0, %ymm0
29.    10    13.1   3.9    15.5      vpsignb	%ymm9, %ymm9, %ymm10
30.    10    17.7   0.0    6.0       {vex}	vpdpbusd	%ymm0, %ymm10, %ymm2
31.    10    21.2   0.0    3.8       vcvtdq2ps	%ymm3, %ymm3
32.    10    29.0   0.0    0.0       vfmadd231ps	%ymm8, %ymm3, %ymm1
33.    10    22.7   0.0    6.3       vcvtdq2ps	%ymm2, %ymm0
34.    10    33.0   0.0    0.0       vfmadd231ps	%ymm4, %ymm0, %ymm1
35.    10    1.0    0.0    35.0      cmpl	%edx, %r8d
36.    10    1.2    0.2    33.8      jg	.L5
       10    11.7   0.9    16.6      <total>
