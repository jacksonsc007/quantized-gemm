Iterations:        100
Instructions:      39200
Total Cycles:      9179
Total uOps:        48600

Dispatch Width:    6
uOps Per Cycle:    5.29
IPC:               4.27
Block RThroughput: 86.0


Cycles with backend pressure increase [ 58.22% ]
Throughput Bottlenecks: 
  Resource Pressure       [ 58.06% ]
  - ADLPPort00  [ 57.99% ]
  - ADLPPort01  [ 58.02% ]
  - ADLPPort02  [ 3.33% ]
  - ADLPPort03  [ 3.33% ]
  - ADLPPort04  [ 1.59% ]
  - ADLPPort05  [ 11.17% ]
  - ADLPPort06  [ 0.01% ]
  - ADLPPort07  [ 1.59% ]
  - ADLPPort08  [ 1.59% ]
  - ADLPPort09  [ 1.59% ]
  - ADLPPort11  [ 3.33% ]
  Data Dependencies:      [ 48.25% ]
  - Register Dependencies [ 48.25% ]
  - Memory Dependencies   [ 0.00% ]

Critical sequence based on the simulation:

              Instruction                                 Dependency Information
 +----< 371.  vcvtdq2ps	%ymm3, %ymm3
 |
 |    < loop carried > 
 |
 |      0.    movl	%edi, %eax
 |      1.    movq	88(%rsp), %r15
 |      2.    addq	$64, %rdx
 |      3.    sarl	$5, %eax
 |      4.    cltq
 |      5.    addq	%rax, %r15
 |      6.    vmovss	4(%rsi,%rax,4), %xmm4
 |      7.    vmovss	(%rsi,%rax,4), %xmm15
 |      8.    vmovss	(%rsi,%r15,4), %xmm13
 |      9.    vmovss	4(%rsi,%r15,4), %xmm5
 |      10.   movq	80(%rsp), %r15
 |      11.   vmovss	%xmm5, 704(%rsp)
 |      12.   vmovss	%xmm13, 708(%rsp)
 |      13.   addq	%rax, %r15
 |      14.   vmovss	(%rsi,%r15,4), %xmm6
 |      15.   vmovss	4(%rsi,%r15,4), %xmm14
 |      16.   movq	72(%rsp), %r15
 |      17.   vmovss	%xmm6, 700(%rsp)
 |      18.   vmovss	%xmm14, 696(%rsp)
 |      19.   addq	%rax, %r15
 |      20.   vmovdqa	%ymm8, %ymm14
 |      21.   vmovss	(%rsi,%r15,4), %xmm7
 |      22.   vmovss	4(%rsi,%r15,4), %xmm11
 +----> 23.   leaq	(%r11,%rax), %r15                 ## RESOURCE interference:  ADLPPort01 [ probability: 48% ]
 +----> 24.   vmovss	(%rcx,%r15,4), %xmm10             ## REGISTER dependency:  %r15
 |      25.   vmovss	4(%rcx,%r15,4), %xmm12
 |      26.   leaq	(%r9,%rax), %r15
 |      27.   vmovss	(%rcx,%r15,4), %xmm2
 |      28.   vmovss	4(%rcx,%r15,4), %xmm1
 |      29.   leaq	(%r10,%rax), %r15
 |      30.   addq	%r8, %rax
 |      31.   vmovss	(%rcx,%rax,4), %xmm9
 |      32.   vmovss	4(%rcx,%rax,4), %xmm5
 |      33.   movl	%edi, %eax
 |      34.   vmovss	%xmm7, 676(%rsp)
 |      35.   vmovss	(%rcx,%r15,4), %xmm3
 |      36.   sarl	%eax
 |      37.   addl	$64, %edi
 |      38.   vmovss	4(%rcx,%r15,4), %xmm0
 |      39.   vmovss	%xmm11, 680(%rsp)
 |      40.   cltq
 |      41.   vmovss	%xmm2, 732(%rsp)
 |      42.   vmovss	%xmm1, 728(%rsp)
 |      43.   vmovss	%xmm3, 724(%rsp)
 |      44.   vmovdqu	(%r14,%rax), %ymm3
 |      45.   vmovss	%xmm0, 720(%rsp)
 |      46.   vmovss	%xmm9, 716(%rsp)
 |      47.   vpsrlw	$4, %ymm3, %ymm7
 |      48.   vmovss	%xmm5, 712(%rsp)
 |      49.   addq	120(%rsp), %rax
 |      50.   movq	112(%rsp), %r15
 |      51.   vpand	.LC0(%rip), %ymm3, %ymm3
 |      52.   vmovss	%xmm10, 692(%rsp)
 |      53.   vpaddb	.LC1(%rip), %ymm3, %ymm3
 |      54.   vmovdqu	(%rax,%r15), %ymm2
 |      55.   movq	104(%rsp), %r15
 |      56.   vmovss	%xmm12, 688(%rsp)
 |      57.   vmovss	%xmm15, 684(%rsp)
 |      58.   vpand	.LC0(%rip), %ymm7, %ymm7
 |      59.   vpaddb	.LC1(%rip), %ymm7, %ymm7
 |      60.   vmovdqu	(%rax,%r15), %ymm1
 |      61.   movq	96(%rsp), %r15
 |      62.   vpsrlw	$4, %ymm2, %ymm6
 |      63.   vpand	.LC0(%rip), %ymm2, %ymm2
 |      64.   vpaddb	.LC1(%rip), %ymm2, %ymm2
 |      65.   vmovdqu	(%rax,%r15), %ymm0
 |      66.   vpand	.LC0(%rip), %ymm6, %ymm6
 |      67.   vpsrlw	$4, %ymm1, %ymm5
 |      68.   vpaddb	.LC1(%rip), %ymm6, %ymm6
 |      69.   vpsrlw	$4, %ymm0, %ymm9
 |      70.   vmovdqa	%ymm9, 128(%rsp)
 +----> 71.   vmulss	%xmm15, %xmm10, %xmm9             ## REGISTER dependency:  %xmm10
 |      72.   vmovdqu	-64(%rdx), %ymm10
 |      73.   vpsignb	%ymm10, %ymm10, %ymm13
 |      74.   vmovd	%xmm9, %r15d
 +----> 75.   vmulss	%xmm12, %xmm4, %xmm9              ## RESOURCE interference:  ADLPPort00 [ probability: 73% ]
 +----> 76.   vpsignb	%ymm10, %ymm3, %ymm12             ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 |      77.   vmovd	%xmm9, %eax
 |      78.   vmovdqa	%ymm8, %ymm9
 +----> 79.   {vex}	vpdpbusd	%ymm12, %ymm13, %ymm9     ## REGISTER dependency:  %ymm12
 |      80.   vcvtdq2ps	%ymm9, %ymm15
 |      81.   vmovdqu	-32(%rdx), %ymm9
 |      82.   vpsignb	%ymm9, %ymm9, %ymm12
 |      83.   vpsignb	%ymm9, %ymm7, %ymm11
 |      84.   {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 |      85.   vmovd	%r15d, %xmm11
 |      86.   vbroadcastss	%xmm11, %ymm11
 |      87.   vfmadd213ps	640(%rsp), %ymm15, %ymm11
 |      88.   vmovd	%eax, %xmm15
 |      89.   vbroadcastss	%xmm15, %ymm15
 |      90.   vcvtdq2ps	%ymm14, %ymm14
 |      91.   vfmadd132ps	%ymm15, %ymm11, %ymm14
 |      92.   vmovss	684(%rsp), %xmm15
 +----> 93.   vmulss	728(%rsp), %xmm4, %xmm11          ## RESOURCE interference:  ADLPPort00 [ probability: 99% ]
 |      94.   vmovaps	%ymm14, 640(%rsp)
 |      95.   vmovaps	%ymm14, 736(%rsp)
 |      96.   vmovd	%xmm11, %eax
 |      97.   vpsignb	%ymm9, %ymm6, %ymm11
 +----> 98.   vmulss	732(%rsp), %xmm15, %xmm14         ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 +----> 99.   vpsignb	%ymm10, %ymm2, %ymm15             ## RESOURCE interference:  ADLPPort00 [ probability: 49% ]
 |      100.  vmovd	%xmm14, %r15d
 |      101.  vmovdqa	%ymm8, %ymm14
 +----> 102.  {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14    ## REGISTER dependency:  %ymm15
 |      103.  vcvtdq2ps	%ymm14, %ymm15
 |      104.  vmovdqa	%ymm8, %ymm14
 |      105.  {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 |      106.  vmovd	%r15d, %xmm11
 |      107.  vbroadcastss	%xmm11, %ymm11
 |      108.  vfmadd213ps	608(%rsp), %ymm15, %ymm11
 |      109.  vmovd	%eax, %xmm15
 |      110.  vbroadcastss	%xmm15, %ymm15
 |      111.  vcvtdq2ps	%ymm14, %ymm14
 |      112.  vfmadd132ps	%ymm15, %ymm11, %ymm14
 |      113.  vmovss	684(%rsp), %xmm15
 |      114.  vmovaps	%ymm14, 608(%rsp)
 |      115.  vmovaps	%ymm14, 768(%rsp)
 +----> 116.  vmulss	724(%rsp), %xmm15, %xmm14         ## RESOURCE interference:  ADLPPort01 [ probability: 50% ]
 |      117.  vmovd	%xmm14, %r15d
 +----> 118.  vmulss	720(%rsp), %xmm4, %xmm14          ## RESOURCE interference:  ADLPPort01 [ probability: 50% ]
 |      119.  vpand	.LC0(%rip), %ymm1, %ymm1
 |      120.  vpaddb	.LC1(%rip), %ymm1, %ymm1
 |      121.  vpand	.LC0(%rip), %ymm5, %ymm5
 |      122.  vpaddb	.LC1(%rip), %ymm5, %ymm5
 |      123.  vpand	.LC0(%rip), %ymm0, %ymm0
 +----> 124.  vpsignb	%ymm10, %ymm1, %ymm15             ## RESOURCE interference:  ADLPPort01 [ probability: 24% ]
 |      125.  vpaddb	.LC1(%rip), %ymm0, %ymm0
 |      126.  vpsignb	%ymm9, %ymm5, %ymm11
 |      127.  vmovd	%xmm14, %eax
 |      128.  vmovdqa	%ymm8, %ymm14
 |      129.  vpsignb	%ymm10, %ymm0, %ymm10
 +----> 130.  {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14    ## REGISTER dependency:  %ymm15
 |      131.  vcvtdq2ps	%ymm14, %ymm15
 |      132.  vmovdqa	%ymm8, %ymm14
 +----> 133.  {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14    ## RESOURCE interference:  ADLPPort01 [ probability: 99% ]
 |      134.  vmovd	%r15d, %xmm11
 |      135.  vbroadcastss	%xmm11, %ymm11
 |      136.  vfmadd213ps	576(%rsp), %ymm15, %ymm11
 |      137.  vmovd	%eax, %xmm15
 |      138.  vbroadcastss	%xmm15, %ymm15
 |      139.  vcvtdq2ps	%ymm14, %ymm14
 |      140.  vfmadd132ps	%ymm15, %ymm11, %ymm14
 |      141.  vmovss	684(%rsp), %xmm15
 +----> 142.  vmulss	716(%rsp), %xmm15, %xmm15         ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 |      143.  vmovaps	%ymm14, 576(%rsp)
 |      144.  vmovaps	%ymm14, 800(%rsp)
 +----> 145.  vmulss	712(%rsp), %xmm4, %xmm14          ## RESOURCE interference:  ADLPPort01 [ probability: 51% ]
 |      146.  vmovdqa	%ymm8, %ymm4
 +----> 147.  {vex}	vpdpbusd	%ymm10, %ymm13, %ymm4     ## RESOURCE interference:  ADLPPort01 [ probability: 98% ]
 |      148.  vmovdqa	%ymm8, %ymm10
 |      149.  vcvtdq2ps	%ymm4, %ymm11
 |      150.  vmovdqa	128(%rsp), %ymm4
 |      151.  vpand	.LC0(%rip), %ymm4, %ymm4
 |      152.  vpaddb	.LC1(%rip), %ymm4, %ymm4
 +----> 153.  vpsignb	%ymm9, %ymm4, %ymm9               ## RESOURCE interference:  ADLPPort01 [ probability: 72% ]
 |      154.  {vex}	vpdpbusd	%ymm9, %ymm12, %ymm10
 |      155.  vbroadcastss	%xmm15, %ymm9
 +----> 156.  vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15     ## RESOURCE interference:  ADLPPort00 [ probability: 48% ]
 |      157.  vmovss	688(%rsp), %xmm12
 |      158.  vfmadd213ps	544(%rsp), %ymm11, %ymm9
 |      159.  vbroadcastss	%xmm14, %ymm11
 +----> 160.  vmulss	704(%rsp), %xmm12, %xmm13         ## RESOURCE interference:  ADLPPort00 [ probability: 24% ]
 |      161.  vmovdqa	%ymm8, %ymm12
 |      162.  vcvtdq2ps	%ymm10, %ymm10
 |      163.  vfmadd132ps	%ymm11, %ymm9, %ymm10
 |      164.  vbroadcastss	%xmm13, %ymm13
 |      165.  vmovaps	%ymm10, 544(%rsp)
 |      166.  vmovaps	%ymm10, 832(%rsp)
 |      167.  vmovss	692(%rsp), %xmm10
 +----> 168.  vmulss	708(%rsp), %xmm10, %xmm9          ## RESOURCE interference:  ADLPPort03 [ probability: 48% ]
 |      169.  vmovdqu	-64(%rdx,%rbx), %ymm10
 |      170.  vpsignb	%ymm10, %ymm10, %ymm11
 +----> 171.  vpsignb	%ymm10, %ymm3, %ymm10             ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 +----> 172.  {vex}	vpdpbusd	%ymm10, %ymm11, %ymm12    ## REGISTER dependency:  %ymm10
 |      173.  vmovdqu	-32(%rdx,%rbx), %ymm10
 |      174.  vbroadcastss	%xmm9, %ymm9
 |      175.  vpsignb	%ymm10, %ymm10, %ymm10
 |      176.  vcvtdq2ps	%ymm12, %ymm14
 |      177.  vmovdqa	%ymm8, %ymm12
 |      178.  vfmadd213ps	512(%rsp), %ymm14, %ymm9
 |      179.  vmovdqa	%ymm8, %ymm14
 |      180.  {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 |      181.  vmovss	704(%rsp), %xmm15
 |      182.  vcvtdq2ps	%ymm12, %ymm12
 |      183.  vfmadd132ps	%ymm13, %ymm9, %ymm12
 |      184.  vmovss	708(%rsp), %xmm13
 +----> 185.  vmulss	732(%rsp), %xmm13, %xmm9          ## RESOURCE interference:  ADLPPort01 [ probability: 24% ]
 |      186.  vmovaps	%ymm12, 512(%rsp)
 |      187.  vmovaps	%ymm12, 864(%rsp)
 |      188.  vbroadcastss	%xmm9, %ymm9
 +----> 189.  vmulss	728(%rsp), %xmm15, %xmm13         ## RESOURCE interference:  ADLPPort00 [ probability: 49% ]
 +----> 190.  vpsignb	-64(%rdx,%rbx), %ymm2, %ymm12     ## RESOURCE interference:  ADLPPort01 [ probability: 50% ]
 +----> 191.  vpsignb	-32(%rdx,%rbx), %ymm6, %ymm15     ## RESOURCE interference:  ADLPPort00 [ probability: 50% ]
 |      192.  {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 |      193.  vmovdqa	%ymm8, %ymm12
 |      194.  {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 |      195.  vmovss	704(%rsp), %xmm15
 |      196.  vbroadcastss	%xmm13, %ymm13
 |      197.  vcvtdq2ps	%ymm14, %ymm14
 |      198.  vfmadd213ps	480(%rsp), %ymm14, %ymm9
 |      199.  vmovdqa	%ymm8, %ymm14
 |      200.  vcvtdq2ps	%ymm12, %ymm12
 |      201.  vfmadd132ps	%ymm13, %ymm9, %ymm12
 |      202.  vmovss	708(%rsp), %xmm13
 +----> 203.  vmulss	724(%rsp), %xmm13, %xmm9          ## RESOURCE interference:  ADLPPort01 [ probability: 51% ]
 +----> 204.  vmulss	720(%rsp), %xmm15, %xmm13         ## RESOURCE interference:  ADLPPort01 [ probability: 73% ]
 +----> 205.  vpsignb	-32(%rdx,%rbx), %ymm5, %ymm15     ## RESOURCE interference:  ADLPPort01 [ probability: 76% ]
 |      206.  vmovaps	%ymm12, 480(%rsp)
 |      207.  vmovaps	%ymm12, 896(%rsp)
 +----> 208.  vpsignb	-64(%rdx,%rbx), %ymm1, %ymm12     ## RESOURCE interference:  ADLPPort01 [ probability: 74% ]
 |      209.  vbroadcastss	%xmm9, %ymm9
 |      210.  {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 |      211.  vmovdqa	%ymm8, %ymm12
 |      212.  vbroadcastss	%xmm13, %ymm13
 |      213.  {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 |      214.  vmovss	704(%rsp), %xmm15
 |      215.  vcvtdq2ps	%ymm14, %ymm14
 |      216.  vfmadd213ps	448(%rsp), %ymm14, %ymm9
 |      217.  vcvtdq2ps	%ymm12, %ymm12
 |      218.  vfmadd132ps	%ymm13, %ymm9, %ymm12
 +----> 219.  vpsignb	-64(%rdx,%rbx), %ymm0, %ymm9      ## RESOURCE interference:  ADLPPort01 [ probability: 50% ]
 |      220.  vmovss	708(%rsp), %xmm13
 +----> 221.  vmulss	716(%rsp), %xmm13, %xmm14         ## RESOURCE interference:  ADLPPort01 [ probability: 73% ]
 +----> 222.  vmulss	712(%rsp), %xmm15, %xmm13         ## RESOURCE interference:  ADLPPort00 [ probability: 51% ]
 |      223.  vmovdqa	%ymm8, %ymm15
 |      224.  {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
 +----> 225.  vpsignb	-32(%rdx,%rbx), %ymm4, %ymm9      ## RESOURCE interference:  ADLPPort01 [ probability: 75% ]
 |      226.  vmovaps	%ymm12, 448(%rsp)
 |      227.  vmovaps	%ymm12, 928(%rsp)
 |      228.  vcvtdq2ps	%ymm15, %ymm12
 |      229.  vmovdqa	%ymm8, %ymm15
 +----> 230.  {vex}	vpdpbusd	%ymm9, %ymm10, %ymm15     ## REGISTER dependency:  %ymm9
 |      231.  vbroadcastss	%xmm14, %ymm9
 |      232.  vbroadcastss	%xmm13, %ymm10
 |      233.  vfmadd213ps	416(%rsp), %ymm12, %ymm9
 |      234.  vmovss	688(%rsp), %xmm12
 |      235.  vmulss	696(%rsp), %xmm12, %xmm13
 |      236.  vmovdqa	%ymm8, %ymm12
 |      237.  vcvtdq2ps	%ymm15, %ymm11
 |      238.  vmovaps	%ymm11, %ymm15
 |      239.  vfmadd132ps	%ymm10, %ymm9, %ymm15
 |      240.  vbroadcastss	%xmm13, %ymm13
 |      241.  vmovss	692(%rsp), %xmm10
 |      242.  vmulss	700(%rsp), %xmm10, %xmm9
 |      243.  vmovaps	%ymm15, 416(%rsp)
 |      244.  vmovaps	%ymm15, 960(%rsp)
 |      245.  vmovdqu	-64(%rdx,%r13), %ymm15
 |      246.  vbroadcastss	%xmm9, %ymm9
 |      247.  vpsignb	%ymm15, %ymm15, %ymm11
 |      248.  vpsignb	%ymm15, %ymm3, %ymm10
 |      249.  vmovdqa	%ymm8, %ymm15
 |      250.  {vex}	vpdpbusd	%ymm10, %ymm11, %ymm15
 |      251.  vcvtdq2ps	%ymm15, %ymm14
 |      252.  vmovdqu	-32(%rdx,%r13), %ymm15
 |      253.  vfmadd213ps	384(%rsp), %ymm14, %ymm9
 |      254.  vpsignb	%ymm15, %ymm15, %ymm10
 +----> 255.  vpsignb	%ymm15, %ymm7, %ymm15             ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 +----> 256.  {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12    ## REGISTER dependency:  %ymm15
 |      257.  vcvtdq2ps	%ymm12, %ymm12
 |      258.  vmovaps	%ymm12, %ymm15
 |      259.  vfmadd132ps	%ymm13, %ymm9, %ymm15
 |      260.  vmovaps	%ymm15, 384(%rsp)
 |      261.  vmovaps	%ymm15, 992(%rsp)
 +----> 262.  vpsignb	-64(%rdx,%r13), %ymm2, %ymm12     ## RESOURCE interference:  ADLPPort01 [ probability: 48% ]
 |      263.  vmovdqa	%ymm8, %ymm15
 |      264.  vmovss	700(%rsp), %xmm13
 |      265.  {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 |      266.  vmovdqa	%ymm8, %ymm12
 |      267.  vmovss	696(%rsp), %xmm14
 +----> 268.  vmulss	732(%rsp), %xmm13, %xmm9          ## RESOURCE interference:  ADLPPort00 [ probability: 72% ]
 +----> 269.  vmulss	728(%rsp), %xmm14, %xmm13         ## RESOURCE interference:  ADLPPort11 [ probability: 72% ]
 |      270.  vcvtdq2ps	%ymm15, %ymm14
 +----> 271.  vpsignb	-32(%rdx,%r13), %ymm6, %ymm15     ## RESOURCE interference:  ADLPPort00 [ probability: 74% ]
 |      272.  vbroadcastss	%xmm9, %ymm9
 |      273.  {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 |      274.  vbroadcastss	%xmm13, %ymm13
 |      275.  vfmadd213ps	352(%rsp), %ymm14, %ymm9
 |      276.  vmovss	696(%rsp), %xmm14
 |      277.  vcvtdq2ps	%ymm12, %ymm12
 |      278.  vmovaps	%ymm12, %ymm15
 +----> 279.  vpsignb	-64(%rdx,%r13), %ymm1, %ymm12     ## RESOURCE interference:  ADLPPort01 [ probability: 74% ]
 |      280.  vfmadd132ps	%ymm13, %ymm9, %ymm15
 |      281.  vmovss	700(%rsp), %xmm13
 +----> 282.  vmulss	724(%rsp), %xmm13, %xmm9          ## RESOURCE interference:  ADLPPort00 [ probability: 74% ]
 +----> 283.  vmulss	720(%rsp), %xmm14, %xmm13         ## RESOURCE interference:  ADLPPort01 [ probability: 74% ]
 |      284.  vmovaps	%ymm15, 352(%rsp)
 |      285.  vmovaps	%ymm15, 1024(%rsp)
 |      286.  vmovdqa	%ymm8, %ymm15
 |      287.  vbroadcastss	%xmm9, %ymm9
 |      288.  {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 |      289.  vmovdqa	%ymm8, %ymm12
 |      290.  vbroadcastss	%xmm13, %ymm13
 |      291.  vcvtdq2ps	%ymm15, %ymm14
 +----> 292.  vpsignb	-32(%rdx,%r13), %ymm5, %ymm15     ## RESOURCE interference:  ADLPPort00 [ probability: 73% ]
 |      293.  vfmadd213ps	320(%rsp), %ymm14, %ymm9
 |      294.  {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 |      295.  vcvtdq2ps	%ymm12, %ymm12
 |      296.  vmovaps	%ymm12, %ymm15
 |      297.  vfmadd132ps	%ymm13, %ymm9, %ymm15
 +----> 298.  vpsignb	-64(%rdx,%r13), %ymm0, %ymm9      ## RESOURCE interference:  ADLPPort11 [ probability: 48% ]
 |      299.  vmovss	700(%rsp), %xmm13
 +----> 300.  vmulss	716(%rsp), %xmm13, %xmm14         ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 |      301.  vmovaps	%ymm15, 320(%rsp)
 |      302.  vmovaps	%ymm15, 1056(%rsp)
 |      303.  vmovss	696(%rsp), %xmm15
 +----> 304.  vmulss	712(%rsp), %xmm15, %xmm13         ## RESOURCE interference:  ADLPPort11 [ probability: 73% ]
 |      305.  vmovdqa	%ymm8, %ymm15
 |      306.  {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
 +----> 307.  vpsignb	-32(%rdx,%r13), %ymm4, %ymm9      ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 |      308.  vmovdqa	%ymm8, %ymm11
 +----> 309.  {vex}	vpdpbusd	%ymm9, %ymm10, %ymm11     ## REGISTER dependency:  %ymm9
 |      310.  vbroadcastss	%xmm14, %ymm9
 |      311.  vbroadcastss	%xmm13, %ymm10
 |      312.  vcvtdq2ps	%ymm15, %ymm12
 |      313.  vfmadd213ps	288(%rsp), %ymm12, %ymm9
 |      314.  vmovss	676(%rsp), %xmm15
 |      315.  vmovss	688(%rsp), %xmm12
 |      316.  vcvtdq2ps	%ymm11, %ymm11
 |      317.  vmulss	680(%rsp), %xmm12, %xmm13
 |      318.  vfmadd132ps	%ymm10, %ymm9, %ymm11
 |      319.  vmovss	692(%rsp), %xmm10
 |      320.  vmulss	%xmm15, %xmm10, %xmm14
 |      321.  vmovdqu	-64(%rdx,%r12), %ymm10
 |      322.  vpsignb	%ymm10, %ymm10, %ymm10
 |      323.  vmovaps	%ymm11, 288(%rsp)
 |      324.  vmovaps	%ymm11, 1088(%rsp)
 |      325.  vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
 |      326.  vmovdqa	%ymm8, %ymm3
 |      327.  vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
 |      328.  {vex}	vpdpbusd	%ymm9, %ymm10, %ymm3
 |      329.  vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
 |      330.  vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
 +----> 331.  vpsignb	-32(%rdx,%r12), %ymm4, %ymm4      ## RESOURCE interference:  ADLPPort01 [ probability: 25% ]
 +----> 332.  vmulss	732(%rsp), %xmm15, %xmm12         ## RESOURCE interference:  ADLPPort00 [ probability: 49% ]
 |      333.  vcvtdq2ps	%ymm3, %ymm11
 |      334.  vmovdqu	-32(%rdx,%r12), %ymm3
 +----> 335.  vpsignb	%ymm3, %ymm3, %ymm9               ## RESOURCE interference:  ADLPPort01 [ probability: 72% ]
 +----> 336.  vpsignb	%ymm3, %ymm7, %ymm7               ## RESOURCE interference:  ADLPPort00 [ probability: 24% ]
 |      337.  vmovdqa	%ymm8, %ymm3
 |      338.  {vex}	vpdpbusd	%ymm7, %ymm9, %ymm3
 |      339.  vcvtdq2ps	%ymm3, %ymm7
 |      340.  vbroadcastss	%xmm14, %ymm3
 |      341.  vfmadd213ps	256(%rsp), %ymm11, %ymm3
 |      342.  vbroadcastss	%xmm13, %ymm11
 |      343.  vmovss	680(%rsp), %xmm13
 |      344.  vfmadd132ps	%ymm11, %ymm3, %ymm7
 |      345.  vmovdqa	%ymm8, %ymm3
 +----> 346.  vmulss	728(%rsp), %xmm13, %xmm11         ## RESOURCE interference:  ADLPPort00 [ probability: 73% ]
 |      347.  vmovaps	%ymm7, 256(%rsp)
 |      348.  vmovaps	%ymm7, 1120(%rsp)
 +----> 349.  vpsignb	-64(%rdx,%r12), %ymm2, %ymm7      ## RESOURCE interference:  ADLPPort01 [ probability: 49% ]
 |      350.  vbroadcastss	%xmm12, %ymm2
 |      351.  {vex}	vpdpbusd	%ymm7, %ymm10, %ymm3
 |      352.  vmovdqa	%ymm8, %ymm7
 +----> 353.  {vex}	vpdpbusd	%ymm6, %ymm9, %ymm7       ## RESOURCE interference:  ADLPPort00 [ probability: 48% ]
 |      354.  vcvtdq2ps	%ymm3, %ymm3
 |      355.  vfmadd213ps	224(%rsp), %ymm2, %ymm3
 |      356.  vbroadcastss	%xmm11, %ymm2
 |      357.  vcvtdq2ps	%ymm7, %ymm6
 +----> 358.  vmulss	724(%rsp), %xmm15, %xmm7          ## RESOURCE interference:  ADLPPort00 [ probability: 50% ]
 |      359.  vfmadd132ps	%ymm2, %ymm3, %ymm6
 +----> 360.  vpsignb	-64(%rdx,%r12), %ymm1, %ymm3      ## RESOURCE interference:  ADLPPort00 [ probability: 49% ]
 |      361.  vmovdqa	%ymm8, %ymm2
 |      362.  {vex}	vpdpbusd	%ymm3, %ymm10, %ymm2
 |      363.  vmovdqa	%ymm8, %ymm3
 |      364.  vbroadcastss	%xmm7, %ymm1
 +----> 365.  {vex}	vpdpbusd	%ymm5, %ymm9, %ymm3       ## RESOURCE interference:  ADLPPort01 [ probability: 73% ]
 |      366.  vmovaps	%ymm6, 224(%rsp)
 |      367.  vmovaps	%ymm6, 1152(%rsp)
 +----> 368.  vmulss	720(%rsp), %xmm13, %xmm6          ## RESOURCE interference:  ADLPPort01 [ probability: 72% ]
 |      369.  vcvtdq2ps	%ymm2, %ymm2
 |      370.  vfmadd213ps	192(%rsp), %ymm1, %ymm2
 |      371.  vcvtdq2ps	%ymm3, %ymm3
 |      372.  vbroadcastss	%xmm6, %ymm1
 |      373.  vfmadd132ps	%ymm1, %ymm2, %ymm3
 |      374.  vmovdqa	%ymm8, %ymm1
 +----> 375.  {vex}	vpdpbusd	%ymm0, %ymm10, %ymm1      ## RESOURCE interference:  ADLPPort01 [ probability: 97% ]
 |      376.  vmovdqa	%ymm8, %ymm0
 +----> 377.  vmulss	716(%rsp), %xmm15, %xmm2          ## RESOURCE interference:  ADLPPort01 [ probability: 96% ]
 +----> 378.  {vex}	vpdpbusd	%ymm4, %ymm9, %ymm0       ## RESOURCE interference:  ADLPPort01 [ probability: 97% ]
 |      379.  vmovaps	%ymm3, 192(%rsp)
 |      380.  vmovaps	%ymm3, 1184(%rsp)
 +----> 381.  vmulss	712(%rsp), %xmm13, %xmm3          ## RESOURCE interference:  ADLPPort01 [ probability: 50% ]
 |      382.  vbroadcastss	%xmm2, %ymm2
 |      383.  vcvtdq2ps	%ymm1, %ymm1
 |      384.  vfmadd213ps	160(%rsp), %ymm2, %ymm1
 |      385.  vcvtdq2ps	%ymm0, %ymm0
 +----> 386.  vbroadcastss	%xmm3, %ymm2              ## REGISTER dependency:  %xmm3
 +----> 387.  vfmadd132ps	%ymm2, %ymm1, %ymm0       ## REGISTER dependency:  %ymm2
 +----> 388.  vmovaps	%ymm0, 160(%rsp)                  ## REGISTER dependency:  %ymm0
 |      389.  vmovaps	%ymm0, 1216(%rsp)
 |      390.  cmpl	%edi, 24(%rbp)
 |      391.  jg	.L4
 |
 |    < loop carried > 
 |
 +----> 11.   vmovss	%xmm5, 704(%rsp)                  ## RESOURCE interference:  ADLPPort08 [ probability: 99% ]


Instruction Info:
[1]: #uOps
[2]: Latency
[3]: RThroughput
[4]: MayLoad
[5]: MayStore
[6]: HasSideEffects (U)

[1]    [2]    [3]    [4]    [5]    [6]    Instructions:
 1      0     0.20                        movl	%edi, %eax
 1      5     0.33    *                   movq	88(%rsp), %r15
 0      1     0.00                        addq	$64, %rdx
 1      1     0.50                        sarl	$5, %eax
 1      1     0.50                        cltq
 1      1     0.20                        addq	%rax, %r15
 1      7     0.33    *                   vmovss	4(%rsi,%rax,4), %xmm4
 1      7     0.33    *                   vmovss	(%rsi,%rax,4), %xmm15
 1      7     0.33    *                   vmovss	(%rsi,%r15,4), %xmm13
 1      7     0.33    *                   vmovss	4(%rsi,%r15,4), %xmm5
 1      5     0.33    *                   movq	80(%rsp), %r15
 2      12    0.50           *            vmovss	%xmm5, 704(%rsp)
 2      12    0.50           *            vmovss	%xmm13, 708(%rsp)
 1      1     0.20                        addq	%rax, %r15
 1      7     0.33    *                   vmovss	(%rsi,%r15,4), %xmm6
 1      7     0.33    *                   vmovss	4(%rsi,%r15,4), %xmm14
 1      5     0.33    *                   movq	72(%rsp), %r15
 2      12    0.50           *            vmovss	%xmm6, 700(%rsp)
 2      12    0.50           *            vmovss	%xmm14, 696(%rsp)
 1      1     0.20                        addq	%rax, %r15
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      7     0.33    *                   vmovss	(%rsi,%r15,4), %xmm7
 1      7     0.33    *                   vmovss	4(%rsi,%r15,4), %xmm11
 1      1     1.00                        leaq	(%r11,%rax), %r15
 1      7     0.33    *                   vmovss	(%rcx,%r15,4), %xmm10
 1      7     0.33    *                   vmovss	4(%rcx,%r15,4), %xmm12
 1      1     1.00                        leaq	(%r9,%rax), %r15
 1      7     0.33    *                   vmovss	(%rcx,%r15,4), %xmm2
 1      7     0.33    *                   vmovss	4(%rcx,%r15,4), %xmm1
 1      1     1.00                        leaq	(%r10,%rax), %r15
 1      1     0.20                        addq	%r8, %rax
 1      7     0.33    *                   vmovss	(%rcx,%rax,4), %xmm9
 1      7     0.33    *                   vmovss	4(%rcx,%rax,4), %xmm5
 1      0     0.20                        movl	%edi, %eax
 2      12    0.50           *            vmovss	%xmm7, 676(%rsp)
 1      7     0.33    *                   vmovss	(%rcx,%r15,4), %xmm3
 1      1     0.50                        sarl	%eax
 1      1     0.20                        addl	$64, %edi
 1      7     0.33    *                   vmovss	4(%rcx,%r15,4), %xmm0
 2      12    0.50           *            vmovss	%xmm11, 680(%rsp)
 1      1     0.50                        cltq
 2      12    0.50           *            vmovss	%xmm2, 732(%rsp)
 2      12    0.50           *            vmovss	%xmm1, 728(%rsp)
 2      12    0.50           *            vmovss	%xmm3, 724(%rsp)
 1      8     0.33    *                   vmovdqu	(%r14,%rax), %ymm3
 2      12    0.50           *            vmovss	%xmm0, 720(%rsp)
 2      12    0.50           *            vmovss	%xmm9, 716(%rsp)
 1      1     0.50                        vpsrlw	$4, %ymm3, %ymm7
 2      12    0.50           *            vmovss	%xmm5, 712(%rsp)
 2      6     0.33    *                   addq	120(%rsp), %rax
 1      5     0.33    *                   movq	112(%rsp), %r15
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm3, %ymm3
 2      12    0.50           *            vmovss	%xmm10, 692(%rsp)
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm3, %ymm3
 1      8     0.33    *                   vmovdqu	(%rax,%r15), %ymm2
 1      5     0.33    *                   movq	104(%rsp), %r15
 2      12    0.50           *            vmovss	%xmm12, 688(%rsp)
 2      12    0.50           *            vmovss	%xmm15, 684(%rsp)
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm7, %ymm7
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm7, %ymm7
 1      8     0.33    *                   vmovdqu	(%rax,%r15), %ymm1
 1      5     0.33    *                   movq	96(%rsp), %r15
 1      1     0.50                        vpsrlw	$4, %ymm2, %ymm6
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm2, %ymm2
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm2, %ymm2
 1      8     0.33    *                   vmovdqu	(%rax,%r15), %ymm0
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm6, %ymm6
 1      1     0.50                        vpsrlw	$4, %ymm1, %ymm5
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm6, %ymm6
 1      1     0.50                        vpsrlw	$4, %ymm0, %ymm9
 2      12    0.50           *            vmovdqa	%ymm9, 128(%rsp)
 1      4     0.50                        vmulss	%xmm15, %xmm10, %xmm9
 1      8     0.33    *                   vmovdqu	-64(%rdx), %ymm10
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm13
 1      3     1.00                        vmovd	%xmm9, %r15d
 1      4     0.50                        vmulss	%xmm12, %xmm4, %xmm9
 1      1     0.50                        vpsignb	%ymm10, %ymm3, %ymm12
 1      3     1.00                        vmovd	%xmm9, %eax
 0      1     0.00                        vmovdqa	%ymm8, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm13, %ymm9
 1      4     0.50                        vcvtdq2ps	%ymm9, %ymm15
 1      8     0.33    *                   vmovdqu	-32(%rdx), %ymm9
 1      1     0.50                        vpsignb	%ymm9, %ymm9, %ymm12
 1      1     0.50                        vpsignb	%ymm9, %ymm7, %ymm11
 1      5     0.50                        {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 1      3     1.00                        vmovd	%r15d, %xmm11
 1      3     1.00                        vbroadcastss	%xmm11, %ymm11
 2      12    0.50    *                   vfmadd213ps	640(%rsp), %ymm15, %ymm11
 1      3     1.00                        vmovd	%eax, %xmm15
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm11, %ymm14
 1      7     0.33    *                   vmovss	684(%rsp), %xmm15
 2      11    0.50    *                   vmulss	728(%rsp), %xmm4, %xmm11
 2      12    0.50           *            vmovaps	%ymm14, 640(%rsp)
 2      12    0.50           *            vmovaps	%ymm14, 736(%rsp)
 1      3     1.00                        vmovd	%xmm11, %eax
 1      1     0.50                        vpsignb	%ymm9, %ymm6, %ymm11
 2      11    0.50    *                   vmulss	732(%rsp), %xmm15, %xmm14
 1      1     0.50                        vpsignb	%ymm10, %ymm2, %ymm15
 1      3     1.00                        vmovd	%xmm14, %r15d
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 1      3     1.00                        vmovd	%r15d, %xmm11
 1      3     1.00                        vbroadcastss	%xmm11, %ymm11
 2      12    0.50    *                   vfmadd213ps	608(%rsp), %ymm15, %ymm11
 1      3     1.00                        vmovd	%eax, %xmm15
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm11, %ymm14
 1      7     0.33    *                   vmovss	684(%rsp), %xmm15
 2      12    0.50           *            vmovaps	%ymm14, 608(%rsp)
 2      12    0.50           *            vmovaps	%ymm14, 768(%rsp)
 2      11    0.50    *                   vmulss	724(%rsp), %xmm15, %xmm14
 1      3     1.00                        vmovd	%xmm14, %r15d
 2      11    0.50    *                   vmulss	720(%rsp), %xmm4, %xmm14
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm1, %ymm1
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm1, %ymm1
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm5, %ymm5
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm5, %ymm5
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm0, %ymm0
 1      1     0.50                        vpsignb	%ymm10, %ymm1, %ymm15
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm0, %ymm0
 1      1     0.50                        vpsignb	%ymm9, %ymm5, %ymm11
 1      3     1.00                        vmovd	%xmm14, %eax
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      1     0.50                        vpsignb	%ymm10, %ymm0, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm13, %ymm14
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm11, %ymm12, %ymm14
 1      3     1.00                        vmovd	%r15d, %xmm11
 1      3     1.00                        vbroadcastss	%xmm11, %ymm11
 2      12    0.50    *                   vfmadd213ps	576(%rsp), %ymm15, %ymm11
 1      3     1.00                        vmovd	%eax, %xmm15
 1      3     1.00                        vbroadcastss	%xmm15, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 1      4     0.50                        vfmadd132ps	%ymm15, %ymm11, %ymm14
 1      7     0.33    *                   vmovss	684(%rsp), %xmm15
 2      11    0.50    *                   vmulss	716(%rsp), %xmm15, %xmm15
 2      12    0.50           *            vmovaps	%ymm14, 576(%rsp)
 2      12    0.50           *            vmovaps	%ymm14, 800(%rsp)
 2      11    0.50    *                   vmulss	712(%rsp), %xmm4, %xmm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm4
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm13, %ymm4
 0      1     0.00                        vmovdqa	%ymm8, %ymm10
 1      4     0.50                        vcvtdq2ps	%ymm4, %ymm11
 1      8     0.33    *                   vmovdqa	128(%rsp), %ymm4
 2      9     0.33    *                   vpand	.LC0(%rip), %ymm4, %ymm4
 2      9     0.33    *                   vpaddb	.LC1(%rip), %ymm4, %ymm4
 1      1     0.50                        vpsignb	%ymm9, %ymm4, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm12, %ymm10
 1      3     1.00                        vbroadcastss	%xmm15, %ymm9
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm7, %ymm15
 1      7     0.33    *                   vmovss	688(%rsp), %xmm12
 2      12    0.50    *                   vfmadd213ps	544(%rsp), %ymm11, %ymm9
 1      3     1.00                        vbroadcastss	%xmm14, %ymm11
 2      11    0.50    *                   vmulss	704(%rsp), %xmm12, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm10, %ymm10
 1      4     0.50                        vfmadd132ps	%ymm11, %ymm9, %ymm10
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 2      12    0.50           *            vmovaps	%ymm10, 544(%rsp)
 2      12    0.50           *            vmovaps	%ymm10, 832(%rsp)
 1      7     0.33    *                   vmovss	692(%rsp), %xmm10
 2      11    0.50    *                   vmulss	708(%rsp), %xmm10, %xmm9
 1      8     0.33    *                   vmovdqu	-64(%rdx,%rbx), %ymm10
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm11
 1      1     0.50                        vpsignb	%ymm10, %ymm3, %ymm10
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm11, %ymm12
 1      8     0.33    *                   vmovdqu	-32(%rdx,%rbx), %ymm10
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm10
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 2      12    0.50    *                   vfmadd213ps	512(%rsp), %ymm14, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      7     0.33    *                   vmovss	704(%rsp), %xmm15
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm12
 1      7     0.33    *                   vmovss	708(%rsp), %xmm13
 2      11    0.50    *                   vmulss	732(%rsp), %xmm13, %xmm9
 2      12    0.50           *            vmovaps	%ymm12, 512(%rsp)
 2      12    0.50           *            vmovaps	%ymm12, 864(%rsp)
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 2      11    0.50    *                   vmulss	728(%rsp), %xmm15, %xmm13
 2      9     0.50    *                   vpsignb	-64(%rdx,%rbx), %ymm2, %ymm12
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm6, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      7     0.33    *                   vmovss	704(%rsp), %xmm15
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 2      12    0.50    *                   vfmadd213ps	480(%rsp), %ymm14, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm14
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm12
 1      7     0.33    *                   vmovss	708(%rsp), %xmm13
 2      11    0.50    *                   vmulss	724(%rsp), %xmm13, %xmm9
 2      11    0.50    *                   vmulss	720(%rsp), %xmm15, %xmm13
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm5, %ymm15
 2      12    0.50           *            vmovaps	%ymm12, 480(%rsp)
 2      12    0.50           *            vmovaps	%ymm12, 896(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%rbx), %ymm1, %ymm12
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm14
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      7     0.33    *                   vmovss	704(%rsp), %xmm15
 1      4     0.50                        vcvtdq2ps	%ymm14, %ymm14
 2      12    0.50    *                   vfmadd213ps	448(%rsp), %ymm14, %ymm9
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm12
 2      9     0.50    *                   vpsignb	-64(%rdx,%rbx), %ymm0, %ymm9
 1      7     0.33    *                   vmovss	708(%rsp), %xmm13
 2      11    0.50    *                   vmulss	716(%rsp), %xmm13, %xmm14
 2      11    0.50    *                   vmulss	712(%rsp), %xmm15, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
 2      9     0.50    *                   vpsignb	-32(%rdx,%rbx), %ymm4, %ymm9
 2      12    0.50           *            vmovaps	%ymm12, 448(%rsp)
 2      12    0.50           *            vmovaps	%ymm12, 928(%rsp)
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm12
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm10, %ymm15
 1      3     1.00                        vbroadcastss	%xmm14, %ymm9
 1      3     1.00                        vbroadcastss	%xmm13, %ymm10
 2      12    0.50    *                   vfmadd213ps	416(%rsp), %ymm12, %ymm9
 1      7     0.33    *                   vmovss	688(%rsp), %xmm12
 2      11    0.50    *                   vmulss	696(%rsp), %xmm12, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm11
 0      1     0.00                        vmovaps	%ymm11, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm10, %ymm9, %ymm15
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      7     0.33    *                   vmovss	692(%rsp), %xmm10
 2      11    0.50    *                   vmulss	700(%rsp), %xmm10, %xmm9
 2      12    0.50           *            vmovaps	%ymm15, 416(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 960(%rsp)
 1      8     0.33    *                   vmovdqu	-64(%rdx,%r13), %ymm15
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      1     0.50                        vpsignb	%ymm15, %ymm15, %ymm11
 1      1     0.50                        vpsignb	%ymm15, %ymm3, %ymm10
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm10, %ymm11, %ymm15
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm14
 1      8     0.33    *                   vmovdqu	-32(%rdx,%r13), %ymm15
 2      12    0.50    *                   vfmadd213ps	384(%rsp), %ymm14, %ymm9
 1      1     0.50                        vpsignb	%ymm15, %ymm15, %ymm10
 1      1     0.50                        vpsignb	%ymm15, %ymm7, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 0      1     0.00                        vmovaps	%ymm12, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm15
 2      12    0.50           *            vmovaps	%ymm15, 384(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 992(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%r13), %ymm2, %ymm12
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      7     0.33    *                   vmovss	700(%rsp), %xmm13
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      7     0.33    *                   vmovss	696(%rsp), %xmm14
 2      11    0.50    *                   vmulss	732(%rsp), %xmm13, %xmm9
 2      11    0.50    *                   vmulss	728(%rsp), %xmm14, %xmm13
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm14
 2      9     0.50    *                   vpsignb	-32(%rdx,%r13), %ymm6, %ymm15
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 2      12    0.50    *                   vfmadd213ps	352(%rsp), %ymm14, %ymm9
 1      7     0.33    *                   vmovss	696(%rsp), %xmm14
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 0      1     0.00                        vmovaps	%ymm12, %ymm15
 2      9     0.50    *                   vpsignb	-64(%rdx,%r13), %ymm1, %ymm12
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm15
 1      7     0.33    *                   vmovss	700(%rsp), %xmm13
 2      11    0.50    *                   vmulss	724(%rsp), %xmm13, %xmm9
 2      11    0.50    *                   vmulss	720(%rsp), %xmm14, %xmm13
 2      12    0.50           *            vmovaps	%ymm15, 352(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 1024(%rsp)
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      3     1.00                        vbroadcastss	%xmm9, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 0      1     0.00                        vmovdqa	%ymm8, %ymm12
 1      3     1.00                        vbroadcastss	%xmm13, %ymm13
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm14
 2      9     0.50    *                   vpsignb	-32(%rdx,%r13), %ymm5, %ymm15
 2      12    0.50    *                   vfmadd213ps	320(%rsp), %ymm14, %ymm9
 1      5     0.50                        {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
 1      4     0.50                        vcvtdq2ps	%ymm12, %ymm12
 0      1     0.00                        vmovaps	%ymm12, %ymm15
 1      4     0.50                        vfmadd132ps	%ymm13, %ymm9, %ymm15
 2      9     0.50    *                   vpsignb	-64(%rdx,%r13), %ymm0, %ymm9
 1      7     0.33    *                   vmovss	700(%rsp), %xmm13
 2      11    0.50    *                   vmulss	716(%rsp), %xmm13, %xmm14
 2      12    0.50           *            vmovaps	%ymm15, 320(%rsp)
 2      12    0.50           *            vmovaps	%ymm15, 1056(%rsp)
 1      7     0.33    *                   vmovss	696(%rsp), %xmm15
 2      11    0.50    *                   vmulss	712(%rsp), %xmm15, %xmm13
 0      1     0.00                        vmovdqa	%ymm8, %ymm15
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
 2      9     0.50    *                   vpsignb	-32(%rdx,%r13), %ymm4, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm11
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm10, %ymm11
 1      3     1.00                        vbroadcastss	%xmm14, %ymm9
 1      3     1.00                        vbroadcastss	%xmm13, %ymm10
 1      4     0.50                        vcvtdq2ps	%ymm15, %ymm12
 2      12    0.50    *                   vfmadd213ps	288(%rsp), %ymm12, %ymm9
 1      7     0.33    *                   vmovss	676(%rsp), %xmm15
 1      7     0.33    *                   vmovss	688(%rsp), %xmm12
 1      4     0.50                        vcvtdq2ps	%ymm11, %ymm11
 2      11    0.50    *                   vmulss	680(%rsp), %xmm12, %xmm13
 1      4     0.50                        vfmadd132ps	%ymm10, %ymm9, %ymm11
 1      7     0.33    *                   vmovss	692(%rsp), %xmm10
 1      4     0.50                        vmulss	%xmm15, %xmm10, %xmm14
 1      8     0.33    *                   vmovdqu	-64(%rdx,%r12), %ymm10
 1      1     0.50                        vpsignb	%ymm10, %ymm10, %ymm10
 2      12    0.50           *            vmovaps	%ymm11, 288(%rsp)
 2      12    0.50           *            vmovaps	%ymm11, 1088(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 2      9     0.50    *                   vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
 1      5     0.50                        {vex}	vpdpbusd	%ymm9, %ymm10, %ymm3
 2      9     0.50    *                   vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
 2      9     0.50    *                   vpsignb	-32(%rdx,%r12), %ymm4, %ymm4
 2      11    0.50    *                   vmulss	732(%rsp), %xmm15, %xmm12
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm11
 1      8     0.33    *                   vmovdqu	-32(%rdx,%r12), %ymm3
 1      1     0.50                        vpsignb	%ymm3, %ymm3, %ymm9
 1      1     0.50                        vpsignb	%ymm3, %ymm7, %ymm7
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 1      5     0.50                        {vex}	vpdpbusd	%ymm7, %ymm9, %ymm3
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm7
 1      3     1.00                        vbroadcastss	%xmm14, %ymm3
 2      12    0.50    *                   vfmadd213ps	256(%rsp), %ymm11, %ymm3
 1      3     1.00                        vbroadcastss	%xmm13, %ymm11
 1      7     0.33    *                   vmovss	680(%rsp), %xmm13
 1      4     0.50                        vfmadd132ps	%ymm11, %ymm3, %ymm7
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 2      11    0.50    *                   vmulss	728(%rsp), %xmm13, %xmm11
 2      12    0.50           *            vmovaps	%ymm7, 256(%rsp)
 2      12    0.50           *            vmovaps	%ymm7, 1120(%rsp)
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm2, %ymm7
 1      3     1.00                        vbroadcastss	%xmm12, %ymm2
 1      5     0.50                        {vex}	vpdpbusd	%ymm7, %ymm10, %ymm3
 0      1     0.00                        vmovdqa	%ymm8, %ymm7
 1      5     0.50                        {vex}	vpdpbusd	%ymm6, %ymm9, %ymm7
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm3
 2      12    0.50    *                   vfmadd213ps	224(%rsp), %ymm2, %ymm3
 1      3     1.00                        vbroadcastss	%xmm11, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm7, %ymm6
 2      11    0.50    *                   vmulss	724(%rsp), %xmm15, %xmm7
 1      4     0.50                        vfmadd132ps	%ymm2, %ymm3, %ymm6
 2      9     0.50    *                   vpsignb	-64(%rdx,%r12), %ymm1, %ymm3
 0      1     0.00                        vmovdqa	%ymm8, %ymm2
 1      5     0.50                        {vex}	vpdpbusd	%ymm3, %ymm10, %ymm2
 0      1     0.00                        vmovdqa	%ymm8, %ymm3
 1      3     1.00                        vbroadcastss	%xmm7, %ymm1
 1      5     0.50                        {vex}	vpdpbusd	%ymm5, %ymm9, %ymm3
 2      12    0.50           *            vmovaps	%ymm6, 224(%rsp)
 2      12    0.50           *            vmovaps	%ymm6, 1152(%rsp)
 2      11    0.50    *                   vmulss	720(%rsp), %xmm13, %xmm6
 1      4     0.50                        vcvtdq2ps	%ymm2, %ymm2
 2      12    0.50    *                   vfmadd213ps	192(%rsp), %ymm1, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm3, %ymm3
 1      3     1.00                        vbroadcastss	%xmm6, %ymm1
 1      4     0.50                        vfmadd132ps	%ymm1, %ymm2, %ymm3
 0      1     0.00                        vmovdqa	%ymm8, %ymm1
 1      5     0.50                        {vex}	vpdpbusd	%ymm0, %ymm10, %ymm1
 0      1     0.00                        vmovdqa	%ymm8, %ymm0
 2      11    0.50    *                   vmulss	716(%rsp), %xmm15, %xmm2
 1      5     0.50                        {vex}	vpdpbusd	%ymm4, %ymm9, %ymm0
 2      12    0.50           *            vmovaps	%ymm3, 192(%rsp)
 2      12    0.50           *            vmovaps	%ymm3, 1184(%rsp)
 2      11    0.50    *                   vmulss	712(%rsp), %xmm13, %xmm3
 1      3     1.00                        vbroadcastss	%xmm2, %ymm2
 1      4     0.50                        vcvtdq2ps	%ymm1, %ymm1
 2      12    0.50    *                   vfmadd213ps	160(%rsp), %ymm2, %ymm1
 1      4     0.50                        vcvtdq2ps	%ymm0, %ymm0
 1      3     1.00                        vbroadcastss	%xmm3, %ymm2
 1      4     0.50                        vfmadd132ps	%ymm2, %ymm1, %ymm0
 2      12    0.50           *            vmovaps	%ymm0, 160(%rsp)
 2      12    0.50           *            vmovaps	%ymm0, 1216(%rsp)
 2      6     0.33    *                   cmpl	%edi, 24(%rbp)
 1      1     0.50                        jg	.L4


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
91.29  91.29  47.00  47.00  24.00  56.43  5.25   24.00  24.00  24.00  4.74   47.00   -     

 -      -     0.26   0.50    -      -      -      -      -      -      -     0.24    -     vmovss	4(%rsi,%rax,4), %xmm4
 -      -     0.73   0.25    -      -      -      -      -      -      -     0.02    -     vmovss	(%rsi,%rax,4), %xmm15
 -      -     0.48   0.50    -      -      -      -      -      -      -     0.02    -     vmovss	(%rsi,%r15,4), %xmm13
 -      -     0.49   0.51    -      -      -      -      -      -      -      -      -     vmovss	4(%rsi,%r15,4), %xmm5
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm5, 704(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm13, 708(%rsp)
 -      -     0.25   0.73    -      -      -      -      -      -      -     0.02    -     vmovss	(%rsi,%r15,4), %xmm6
 -      -     0.49   0.51    -      -      -      -      -      -      -      -      -     vmovss	4(%rsi,%r15,4), %xmm14
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm6, 700(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm14, 696(%rsp)
 -      -     0.27   0.25    -      -      -      -      -      -      -     0.48    -     vmovss	(%rsi,%r15,4), %xmm7
 -      -     0.25   0.49    -      -      -      -      -      -      -     0.26    -     vmovss	4(%rsi,%r15,4), %xmm11
 -      -     0.73   0.01    -      -      -      -      -      -      -     0.26    -     vmovss	(%rcx,%r15,4), %xmm10
 -      -     0.01   0.26    -      -      -      -      -      -      -     0.73    -     vmovss	4(%rcx,%r15,4), %xmm12
 -      -     0.48   0.25    -      -      -      -      -      -      -     0.27    -     vmovss	(%rcx,%r15,4), %xmm2
 -      -     0.25   0.26    -      -      -      -      -      -      -     0.49    -     vmovss	4(%rcx,%r15,4), %xmm1
 -      -     0.50   0.49    -      -      -      -      -      -      -     0.01    -     vmovss	(%rcx,%rax,4), %xmm9
 -      -     0.26   0.74    -      -      -      -      -      -      -      -      -     vmovss	4(%rcx,%rax,4), %xmm5
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm7, 676(%rsp)
 -      -     0.49   0.26    -      -      -      -      -      -      -     0.25    -     vmovss	(%rcx,%r15,4), %xmm3
 -      -     0.26   0.25    -      -      -      -      -      -      -     0.49    -     vmovss	4(%rcx,%r15,4), %xmm0
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm11, 680(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm2, 732(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm1, 728(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm3, 724(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm0, 720(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm9, 716(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm5, 712(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm10, 692(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovss	%xmm12, 688(%rsp)
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovss	%xmm15, 684(%rsp)
 -      -      -     0.01    -      -      -      -      -      -      -     0.99    -     vmovss	684(%rsp), %xmm15
 -      -     0.26   0.72    -      -      -      -      -      -      -     0.02    -     vmovss	684(%rsp), %xmm15
 -      -     0.25   0.02    -      -      -      -      -      -      -     0.73    -     vmovss	684(%rsp), %xmm15
 -      -     0.97   0.02    -      -      -      -      -      -      -     0.01    -     vmovss	688(%rsp), %xmm12
 -      -      -     0.99    -      -      -      -      -      -      -     0.01    -     vmovss	692(%rsp), %xmm10
 -      -     0.02   0.49    -      -      -      -      -      -      -     0.49    -     vmovss	704(%rsp), %xmm15
 -      -     0.49   0.49    -      -      -      -      -      -      -     0.02    -     vmovss	708(%rsp), %xmm13
 -      -     0.26   0.49    -      -      -      -      -      -      -     0.25    -     vmovss	704(%rsp), %xmm15
 -      -     0.49   0.02    -      -      -      -      -      -      -     0.49    -     vmovss	708(%rsp), %xmm13
 -      -     0.72   0.27    -      -      -      -      -      -      -     0.01    -     vmovss	704(%rsp), %xmm15
 -      -     0.26   0.25    -      -      -      -      -      -      -     0.49    -     vmovss	708(%rsp), %xmm13
 -      -     0.01   0.98    -      -      -      -      -      -      -     0.01    -     vmovss	688(%rsp), %xmm12
 -      -     0.25   0.02    -      -      -      -      -      -      -     0.73    -     vmovss	692(%rsp), %xmm10
 -      -     0.73   0.27    -      -      -      -      -      -      -      -      -     vmovss	700(%rsp), %xmm13
 -      -     0.02   0.01    -      -      -      -      -      -      -     0.97    -     vmovss	696(%rsp), %xmm14
 -      -     0.01   0.98    -      -      -      -      -      -      -     0.01    -     vmovss	696(%rsp), %xmm14
 -      -     0.98   0.02    -      -      -      -      -      -      -      -      -     vmovss	700(%rsp), %xmm13
0.25   0.75   0.02   0.26    -      -      -      -      -      -      -     0.72    -     vmulss	724(%rsp), %xmm13, %xmm9
0.74   0.26   0.49   0.24    -      -      -      -      -      -      -     0.27    -     vmulss	720(%rsp), %xmm14, %xmm13
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm15, 352(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm15, 1024(%rsp)
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm9, %ymm9
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm12, %ymm11, %ymm15
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm12
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm13
0.73   0.27    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm14
0.25   0.75   0.01   0.25    -      -      -      -      -      -      -     0.74    -     vpsignb	-32(%rdx,%r13), %ymm5, %ymm15
0.27   0.73   0.01   0.97    -      -      -      -      -      -      -     0.02    -     vfmadd213ps	320(%rsp), %ymm14, %ymm9
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm15, %ymm10, %ymm12
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm12, %ymm12
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovaps	%ymm12, %ymm15
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm13, %ymm9, %ymm15
0.26   0.74   0.50   0.26    -      -      -      -      -      -      -     0.24    -     vpsignb	-64(%rdx,%r13), %ymm0, %ymm9
 -      -     0.49   0.02    -      -      -      -      -      -      -     0.49    -     vmovss	700(%rsp), %xmm13
0.50   0.50   0.26    -      -      -      -      -      -      -      -     0.74    -     vmulss	716(%rsp), %xmm13, %xmm14
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm15, 320(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm15, 1056(%rsp)
 -      -      -     0.50    -      -      -      -      -      -      -     0.50    -     vmovss	696(%rsp), %xmm15
0.51   0.49   0.25   0.25    -      -      -      -      -      -      -     0.50    -     vmulss	712(%rsp), %xmm15, %xmm13
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm15
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm11, %ymm15
0.49   0.51   0.26   0.25    -      -      -      -      -      -      -     0.49    -     vpsignb	-32(%rdx,%r13), %ymm4, %ymm9
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm11
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm10, %ymm11
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm14, %ymm9
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm10
0.75   0.25    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm15, %ymm12
0.25   0.75   0.72   0.03    -      -      -      -      -      -      -     0.25    -     vfmadd213ps	288(%rsp), %ymm12, %ymm9
 -      -     0.02   0.49    -      -      -      -      -      -      -     0.49    -     vmovss	676(%rsp), %xmm15
 -      -     0.97   0.02    -      -      -      -      -      -      -     0.01    -     vmovss	688(%rsp), %xmm12
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm11, %ymm11
0.50   0.50   0.26   0.24    -      -      -      -      -      -      -     0.50    -     vmulss	680(%rsp), %xmm12, %xmm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm10, %ymm9, %ymm11
 -      -     0.02   0.01    -      -      -      -      -      -      -     0.97    -     vmovss	692(%rsp), %xmm10
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     vmulss	%xmm15, %xmm10, %xmm14
 -      -     0.98   0.01    -      -      -      -      -      -      -     0.01    -     vmovdqu	-64(%rdx,%r12), %ymm10
 -     1.00    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm10, %ymm10, %ymm10
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm11, 288(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm11, 1088(%rsp)
 -     1.00   0.72   0.02    -      -      -      -      -      -      -     0.26    -     vpsignb	-64(%rdx,%r12), %ymm3, %ymm9
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
0.03   0.97   0.03   0.25    -      -      -      -      -      -      -     0.72    -     vpsignb	-32(%rdx,%r12), %ymm6, %ymm6
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm9, %ymm10, %ymm3
0.49   0.51   0.49    -      -      -      -      -      -      -      -     0.51    -     vpsignb	-32(%rdx,%r12), %ymm5, %ymm5
 -     1.00   0.03   0.97    -      -      -      -      -      -      -      -      -     vpsignb	-64(%rdx,%r12), %ymm0, %ymm0
0.49   0.51   0.26    -      -      -      -      -      -      -      -     0.74    -     vpsignb	-32(%rdx,%r12), %ymm4, %ymm4
0.27   0.73   0.25   0.49    -      -      -      -      -      -      -     0.26    -     vmulss	732(%rsp), %xmm15, %xmm12
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm11
 -      -     0.73    -      -      -      -      -      -      -      -     0.27    -     vmovdqu	-32(%rdx,%r12), %ymm3
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm3, %ymm9
0.74   0.26    -      -      -      -      -      -      -      -      -      -      -     vpsignb	%ymm3, %ymm7, %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm7, %ymm9, %ymm3
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm14, %ymm3
0.24   0.76   0.49    -      -      -      -      -      -      -      -     0.51    -     vfmadd213ps	256(%rsp), %ymm11, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm13, %ymm11
 -      -     0.24   0.50    -      -      -      -      -      -      -     0.26    -     vmovss	680(%rsp), %xmm13
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm11, %ymm3, %ymm7
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
0.01   0.99   0.96   0.03    -      -      -      -      -      -      -     0.01    -     vmulss	728(%rsp), %xmm13, %xmm11
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm7, 256(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm7, 1120(%rsp)
0.49   0.51   0.01   0.74    -      -      -      -      -      -      -     0.25    -     vpsignb	-64(%rdx,%r12), %ymm2, %ymm7
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm12, %ymm2
0.97   0.03    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm7, %ymm10, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm7
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm6, %ymm9, %ymm7
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm3
0.01   0.99   0.24   0.24    -      -      -      -      -      -      -     0.52    -     vfmadd213ps	224(%rsp), %ymm2, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm11, %ymm2
0.97   0.03    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm7, %ymm6
0.50   0.50   0.72   0.03    -      -      -      -      -      -      -     0.25    -     vmulss	724(%rsp), %xmm15, %xmm7
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm2, %ymm3, %ymm6
0.26   0.74   0.02   0.74    -      -      -      -      -      -      -     0.24    -     vpsignb	-64(%rdx,%r12), %ymm1, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm2
1.00    -      -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm3, %ymm10, %ymm2
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm7, %ymm1
0.26   0.74    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm5, %ymm9, %ymm3
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm6, 224(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm6, 1152(%rsp)
0.01   0.99    -     0.73    -      -      -      -      -      -      -     0.27    -     vmulss	720(%rsp), %xmm13, %xmm6
0.97   0.03    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm2, %ymm2
0.03   0.97    -     0.49    -      -      -      -      -      -      -     0.51    -     vfmadd213ps	192(%rsp), %ymm1, %ymm2
0.51   0.49    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm3, %ymm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm6, %ymm1
0.99   0.01    -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm1, %ymm2, %ymm3
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm1
0.02   0.98    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm0, %ymm10, %ymm1
 -      -      -      -      -      -      -      -      -      -      -      -      -     vmovdqa	%ymm8, %ymm0
0.01   0.99    -     0.02    -      -      -      -      -      -      -     0.98    -     vmulss	716(%rsp), %xmm15, %xmm2
0.25   0.75    -      -      -      -      -      -      -      -      -      -      -     {vex}	vpdpbusd	%ymm4, %ymm9, %ymm0
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm3, 192(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm3, 1184(%rsp)
0.26   0.74   0.04    -      -      -      -      -      -      -      -     0.96    -     vmulss	712(%rsp), %xmm13, %xmm3
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm2, %ymm2
0.50   0.50    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm1, %ymm1
0.99   0.01   0.75    -      -      -      -      -      -      -      -     0.25    -     vfmadd213ps	160(%rsp), %ymm2, %ymm1
0.27   0.73    -      -      -      -      -      -      -      -      -      -      -     vcvtdq2ps	%ymm0, %ymm0
 -      -      -      -      -     1.00    -      -      -      -      -      -      -     vbroadcastss	%xmm3, %ymm2
1.00    -      -      -      -      -      -      -      -      -      -      -      -     vfmadd132ps	%ymm2, %ymm1, %ymm0
 -      -      -      -      -      -      -      -     1.00   1.00    -      -      -     vmovaps	%ymm0, 160(%rsp)
 -      -      -      -     1.00    -      -     1.00    -      -      -      -      -     vmovaps	%ymm0, 1216(%rsp)
 -      -     0.98    -      -      -     0.74    -      -      -     0.26   0.02    -     cmpl	%edi, 24(%rbp)
 -      -      -      -      -      -     1.00    -      -      -      -      -      -     jg	.L4
