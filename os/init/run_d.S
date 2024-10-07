
run：     文件格式 elf32-littleriscv


Disassembly of section .text:

00010074 <main>:
   10074:	ff010113          	addi	sp,sp,-16
   10078:	00812623          	sw	s0,12(sp)
   1007c:	01010413          	addi	s0,sp,16
   10080:	00000593          	li	a1,0
   10084:	40058593          	addi	a1,a1,1024
   10088:	01058593          	addi	a1,a1,16
   1008c:	00000617          	auipc	a2,0x0
   10090:	02060613          	addi	a2,a2,32 # 100ac <write_imp>
   10094:	00c5a023          	sw	a2,0(a1)
   10098:	00000793          	li	a5,0
   1009c:	00078513          	mv	a0,a5
   100a0:	00c12403          	lw	s0,12(sp)
   100a4:	01010113          	addi	sp,sp,16
   100a8:	00008067          	ret

000100ac <write_imp>:
   100ac:	00000013          	nop

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347          	.insn	4, 0x3a434347
   4:	2820                	.insn	2, 0x2820
   6:	39386367          	.insn	4, 0x39386367
   a:	6431                	.insn	2, 0x6431
   c:	6438                	.insn	2, 0x6438
   e:	65333263          	.insn	4, 0x65333263
  12:	2029                	.insn	2, 0x2029
  14:	3331                	.insn	2, 0x3331
  16:	322e                	.insn	2, 0x322e
  18:	302e                	.insn	2, 0x302e
	...

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	1b41                	.insn	2, 0x1b41
   2:	0000                	.insn	2, 0x
   4:	7200                	.insn	2, 0x7200
   6:	7369                	.insn	2, 0x7369
   8:	01007663          	bgeu	zero,a6,14 <main-0x10060>
   c:	0011                	.insn	2, 0x0011
   e:	0000                	.insn	2, 0x
  10:	1004                	.insn	2, 0x1004
  12:	7205                	.insn	2, 0x7205
  14:	3376                	.insn	2, 0x3376
  16:	6932                	.insn	2, 0x6932
  18:	7032                	.insn	2, 0x7032
  1a:	0031                	.insn	2, 0x0031
