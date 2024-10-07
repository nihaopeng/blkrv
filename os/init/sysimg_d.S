
./init/sysimg：     文件格式 elf32-littleriscv


Disassembly of section .text:

00010094 <_start>:
   10094:	00000517          	auipc	a0,0x0
   10098:	02850513          	addi	a0,a0,40 # 100bc <_gdt_search>
   1009c:	10551073          	csrw	stvec,a0
   100a0:	00000513          	li	a0,0
   100a4:	00000593          	li	a1,0
   100a8:	00000613          	li	a2,0
   100ac:	00000693          	li	a3,0
   100b0:	00000713          	li	a4,0
   100b4:	00000793          	li	a5,0
   100b8:	2a90006f          	j	10b60 <main>

000100bc <_gdt_search>:
   100bc:	00289893          	slli	a7,a7,0x2
   100c0:	00001297          	auipc	t0,0x1
   100c4:	f4028293          	addi	t0,t0,-192 # 11000 <syscall_table>
   100c8:	005888b3          	add	a7,a7,t0
   100cc:	0008a883          	lw	a7,0(a7)
   100d0:	00000293          	li	t0,0
   100d4:	000880e7          	jalr	a7
   100d8:	10200073          	sret

000100dc <abs>:
   100dc:	fe010113          	addi	sp,sp,-32
   100e0:	00812e23          	sw	s0,28(sp)
   100e4:	02010413          	addi	s0,sp,32
   100e8:	fea42623          	sw	a0,-20(s0)
   100ec:	fec42783          	lw	a5,-20(s0)
   100f0:	41f7d713          	srai	a4,a5,0x1f
   100f4:	fec42783          	lw	a5,-20(s0)
   100f8:	00f747b3          	xor	a5,a4,a5
   100fc:	40e787b3          	sub	a5,a5,a4
   10100:	00078513          	mv	a0,a5
   10104:	01c12403          	lw	s0,28(sp)
   10108:	02010113          	addi	sp,sp,32
   1010c:	00008067          	ret

00010110 <mod>:
   10110:	fe010113          	addi	sp,sp,-32
   10114:	00812e23          	sw	s0,28(sp)
   10118:	02010413          	addi	s0,sp,32
   1011c:	fea42623          	sw	a0,-20(s0)
   10120:	feb42423          	sw	a1,-24(s0)
   10124:	0140006f          	j	10138 <mod+0x28>
   10128:	fec42703          	lw	a4,-20(s0)
   1012c:	fe842783          	lw	a5,-24(s0)
   10130:	40f707b3          	sub	a5,a4,a5
   10134:	fef42623          	sw	a5,-20(s0)
   10138:	fec42703          	lw	a4,-20(s0)
   1013c:	fe842783          	lw	a5,-24(s0)
   10140:	fef754e3          	bge	a4,a5,10128 <mod+0x18>
   10144:	fec42783          	lw	a5,-20(s0)
   10148:	00078513          	mv	a0,a5
   1014c:	01c12403          	lw	s0,28(sp)
   10150:	02010113          	addi	sp,sp,32
   10154:	00008067          	ret

00010158 <div>:
   10158:	fc010113          	addi	sp,sp,-64
   1015c:	02812e23          	sw	s0,60(sp)
   10160:	04010413          	addi	s0,sp,64
   10164:	fca42623          	sw	a0,-52(s0)
   10168:	fcb42423          	sw	a1,-56(s0)
   1016c:	fe042623          	sw	zero,-20(s0)
   10170:	fcc42703          	lw	a4,-52(s0)
   10174:	fc842783          	lw	a5,-56(s0)
   10178:	00f747b3          	xor	a5,a4,a5
   1017c:	01f7d793          	srli	a5,a5,0x1f
   10180:	0ff7f793          	zext.b	a5,a5
   10184:	fcf42e23          	sw	a5,-36(s0)
   10188:	fe042423          	sw	zero,-24(s0)
   1018c:	fcc42783          	lw	a5,-52(s0)
   10190:	41f7d793          	srai	a5,a5,0x1f
   10194:	fcc42703          	lw	a4,-52(s0)
   10198:	00e7c733          	xor	a4,a5,a4
   1019c:	40f707b3          	sub	a5,a4,a5
   101a0:	fef42223          	sw	a5,-28(s0)
   101a4:	fc842783          	lw	a5,-56(s0)
   101a8:	41f7d793          	srai	a5,a5,0x1f
   101ac:	fc842703          	lw	a4,-56(s0)
   101b0:	00e7c733          	xor	a4,a5,a4
   101b4:	40f707b3          	sub	a5,a4,a5
   101b8:	fcf42c23          	sw	a5,-40(s0)
   101bc:	00a00793          	li	a5,10
   101c0:	fef42023          	sw	a5,-32(s0)
   101c4:	0540006f          	j	10218 <div+0xc0>
   101c8:	fe042783          	lw	a5,-32(s0)
   101cc:	fe442703          	lw	a4,-28(s0)
   101d0:	40f757b3          	sra	a5,a4,a5
   101d4:	fc842703          	lw	a4,-56(s0)
   101d8:	02e7ca63          	blt	a5,a4,1020c <div+0xb4>
   101dc:	fe042783          	lw	a5,-32(s0)
   101e0:	00100713          	li	a4,1
   101e4:	00f717b3          	sll	a5,a4,a5
   101e8:	fe842703          	lw	a4,-24(s0)
   101ec:	00f707b3          	add	a5,a4,a5
   101f0:	fef42423          	sw	a5,-24(s0)
   101f4:	fe042783          	lw	a5,-32(s0)
   101f8:	fd842703          	lw	a4,-40(s0)
   101fc:	00f717b3          	sll	a5,a4,a5
   10200:	fe442703          	lw	a4,-28(s0)
   10204:	40f707b3          	sub	a5,a4,a5
   10208:	fef42223          	sw	a5,-28(s0)
   1020c:	fe042783          	lw	a5,-32(s0)
   10210:	fff78793          	addi	a5,a5,-1
   10214:	fef42023          	sw	a5,-32(s0)
   10218:	fe042783          	lw	a5,-32(s0)
   1021c:	fa07d6e3          	bgez	a5,101c8 <div+0x70>
   10220:	fdc42783          	lw	a5,-36(s0)
   10224:	00078a63          	beqz	a5,10238 <div+0xe0>
   10228:	fe842783          	lw	a5,-24(s0)
   1022c:	40f007b3          	neg	a5,a5
   10230:	fef42623          	sw	a5,-20(s0)
   10234:	00c0006f          	j	10240 <div+0xe8>
   10238:	fe842783          	lw	a5,-24(s0)
   1023c:	fef42623          	sw	a5,-20(s0)
   10240:	fec42783          	lw	a5,-20(s0)
   10244:	00078513          	mv	a0,a5
   10248:	03c12403          	lw	s0,60(sp)
   1024c:	04010113          	addi	sp,sp,64
   10250:	00008067          	ret

00010254 <mul>:
   10254:	fc010113          	addi	sp,sp,-64
   10258:	02812e23          	sw	s0,60(sp)
   1025c:	04010413          	addi	s0,sp,64
   10260:	fca42623          	sw	a0,-52(s0)
   10264:	fcb42423          	sw	a1,-56(s0)
   10268:	fe042623          	sw	zero,-20(s0)
   1026c:	fcc42703          	lw	a4,-52(s0)
   10270:	fc842783          	lw	a5,-56(s0)
   10274:	00f747b3          	xor	a5,a4,a5
   10278:	01f7d793          	srli	a5,a5,0x1f
   1027c:	0ff7f793          	zext.b	a5,a5
   10280:	fcf42e23          	sw	a5,-36(s0)
   10284:	fe042423          	sw	zero,-24(s0)
   10288:	fcc42783          	lw	a5,-52(s0)
   1028c:	41f7d793          	srai	a5,a5,0x1f
   10290:	fcc42703          	lw	a4,-52(s0)
   10294:	00e7c733          	xor	a4,a5,a4
   10298:	40f707b3          	sub	a5,a4,a5
   1029c:	fcf42c23          	sw	a5,-40(s0)
   102a0:	fc842783          	lw	a5,-56(s0)
   102a4:	41f7d793          	srai	a5,a5,0x1f
   102a8:	fc842703          	lw	a4,-56(s0)
   102ac:	00e7c733          	xor	a4,a5,a4
   102b0:	40f707b3          	sub	a5,a4,a5
   102b4:	fef42223          	sw	a5,-28(s0)
   102b8:	00a00793          	li	a5,10
   102bc:	fef42023          	sw	a5,-32(s0)
   102c0:	0540006f          	j	10314 <mul+0xc0>
   102c4:	fe042783          	lw	a5,-32(s0)
   102c8:	00100713          	li	a4,1
   102cc:	00f717b3          	sll	a5,a4,a5
   102d0:	fc842703          	lw	a4,-56(s0)
   102d4:	02f74a63          	blt	a4,a5,10308 <mul+0xb4>
   102d8:	fe842703          	lw	a4,-24(s0)
   102dc:	fd842783          	lw	a5,-40(s0)
   102e0:	00f70733          	add	a4,a4,a5
   102e4:	fe042783          	lw	a5,-32(s0)
   102e8:	00f717b3          	sll	a5,a4,a5
   102ec:	fef42423          	sw	a5,-24(s0)
   102f0:	fe042783          	lw	a5,-32(s0)
   102f4:	00100713          	li	a4,1
   102f8:	00f717b3          	sll	a5,a4,a5
   102fc:	fe442703          	lw	a4,-28(s0)
   10300:	40f707b3          	sub	a5,a4,a5
   10304:	fef42223          	sw	a5,-28(s0)
   10308:	fe042783          	lw	a5,-32(s0)
   1030c:	fff78793          	addi	a5,a5,-1
   10310:	fef42023          	sw	a5,-32(s0)
   10314:	fe042783          	lw	a5,-32(s0)
   10318:	fa07d6e3          	bgez	a5,102c4 <mul+0x70>
   1031c:	fdc42783          	lw	a5,-36(s0)
   10320:	00078a63          	beqz	a5,10334 <mul+0xe0>
   10324:	fe842783          	lw	a5,-24(s0)
   10328:	40f007b3          	neg	a5,a5
   1032c:	fef42623          	sw	a5,-20(s0)
   10330:	00c0006f          	j	1033c <mul+0xe8>
   10334:	fe842783          	lw	a5,-24(s0)
   10338:	fef42623          	sw	a5,-20(s0)
   1033c:	fec42783          	lw	a5,-20(s0)
   10340:	00078513          	mv	a0,a5
   10344:	03c12403          	lw	s0,60(sp)
   10348:	04010113          	addi	sp,sp,64
   1034c:	00008067          	ret

00010350 <str_len>:
   10350:	fd010113          	addi	sp,sp,-48
   10354:	02812623          	sw	s0,44(sp)
   10358:	03010413          	addi	s0,sp,48
   1035c:	fca42e23          	sw	a0,-36(s0)
   10360:	fe042623          	sw	zero,-20(s0)
   10364:	0100006f          	j	10374 <str_len+0x24>
   10368:	fec42783          	lw	a5,-20(s0)
   1036c:	00178793          	addi	a5,a5,1
   10370:	fef42623          	sw	a5,-20(s0)
   10374:	fdc42703          	lw	a4,-36(s0)
   10378:	fec42783          	lw	a5,-20(s0)
   1037c:	00f707b3          	add	a5,a4,a5
   10380:	0007c783          	lbu	a5,0(a5)
   10384:	fe0792e3          	bnez	a5,10368 <str_len+0x18>
   10388:	fec42783          	lw	a5,-20(s0)
   1038c:	00078513          	mv	a0,a5
   10390:	02c12403          	lw	s0,44(sp)
   10394:	03010113          	addi	sp,sp,48
   10398:	00008067          	ret

0001039c <str_cmp>:
   1039c:	fd010113          	addi	sp,sp,-48
   103a0:	02112623          	sw	ra,44(sp)
   103a4:	02812423          	sw	s0,40(sp)
   103a8:	02912223          	sw	s1,36(sp)
   103ac:	03010413          	addi	s0,sp,48
   103b0:	fca42e23          	sw	a0,-36(s0)
   103b4:	fcb42c23          	sw	a1,-40(s0)
   103b8:	fdc42503          	lw	a0,-36(s0)
   103bc:	f95ff0ef          	jal	10350 <str_len>
   103c0:	00050493          	mv	s1,a0
   103c4:	fd842503          	lw	a0,-40(s0)
   103c8:	f89ff0ef          	jal	10350 <str_len>
   103cc:	00050793          	mv	a5,a0
   103d0:	02f48c63          	beq	s1,a5,10408 <str_cmp+0x6c>
   103d4:	00000793          	li	a5,0
   103d8:	05c0006f          	j	10434 <str_cmp+0x98>
   103dc:	fdc42703          	lw	a4,-36(s0)
   103e0:	fec42783          	lw	a5,-20(s0)
   103e4:	00f707b3          	add	a5,a4,a5
   103e8:	0007c703          	lbu	a4,0(a5)
   103ec:	fd842683          	lw	a3,-40(s0)
   103f0:	fec42783          	lw	a5,-20(s0)
   103f4:	00f687b3          	add	a5,a3,a5
   103f8:	0007c783          	lbu	a5,0(a5)
   103fc:	00f70663          	beq	a4,a5,10408 <str_cmp+0x6c>
   10400:	00000793          	li	a5,0
   10404:	0300006f          	j	10434 <str_cmp+0x98>
   10408:	fdc42703          	lw	a4,-36(s0)
   1040c:	fec42783          	lw	a5,-20(s0)
   10410:	00f707b3          	add	a5,a4,a5
   10414:	0007c783          	lbu	a5,0(a5)
   10418:	00078c63          	beqz	a5,10430 <str_cmp+0x94>
   1041c:	fd842703          	lw	a4,-40(s0)
   10420:	fec42783          	lw	a5,-20(s0)
   10424:	00f707b3          	add	a5,a4,a5
   10428:	0007c783          	lbu	a5,0(a5)
   1042c:	fa0798e3          	bnez	a5,103dc <str_cmp+0x40>
   10430:	00100793          	li	a5,1
   10434:	00078513          	mv	a0,a5
   10438:	02c12083          	lw	ra,44(sp)
   1043c:	02812403          	lw	s0,40(sp)
   10440:	02412483          	lw	s1,36(sp)
   10444:	03010113          	addi	sp,sp,48
   10448:	00008067          	ret

0001044c <str_cpy>:
   1044c:	fd010113          	addi	sp,sp,-48
   10450:	02112623          	sw	ra,44(sp)
   10454:	02812423          	sw	s0,40(sp)
   10458:	03010413          	addi	s0,sp,48
   1045c:	fca42e23          	sw	a0,-36(s0)
   10460:	fcb42c23          	sw	a1,-40(s0)
   10464:	fdc42503          	lw	a0,-36(s0)
   10468:	ee9ff0ef          	jal	10350 <str_len>
   1046c:	fea42423          	sw	a0,-24(s0)
   10470:	fe042623          	sw	zero,-20(s0)
   10474:	0300006f          	j	104a4 <str_cpy+0x58>
   10478:	fec42783          	lw	a5,-20(s0)
   1047c:	fdc42703          	lw	a4,-36(s0)
   10480:	00f70733          	add	a4,a4,a5
   10484:	fec42783          	lw	a5,-20(s0)
   10488:	fd842683          	lw	a3,-40(s0)
   1048c:	00f687b3          	add	a5,a3,a5
   10490:	00074703          	lbu	a4,0(a4)
   10494:	00e78023          	sb	a4,0(a5)
   10498:	fec42783          	lw	a5,-20(s0)
   1049c:	00178793          	addi	a5,a5,1
   104a0:	fef42623          	sw	a5,-20(s0)
   104a4:	fec42783          	lw	a5,-20(s0)
   104a8:	fe842703          	lw	a4,-24(s0)
   104ac:	fce7e6e3          	bltu	a5,a4,10478 <str_cpy+0x2c>
   104b0:	00000013          	nop
   104b4:	00000013          	nop
   104b8:	02c12083          	lw	ra,44(sp)
   104bc:	02812403          	lw	s0,40(sp)
   104c0:	03010113          	addi	sp,sp,48
   104c4:	00008067          	ret

000104c8 <str_cpy_s>:
   104c8:	fd010113          	addi	sp,sp,-48
   104cc:	02812623          	sw	s0,44(sp)
   104d0:	03010413          	addi	s0,sp,48
   104d4:	fca42e23          	sw	a0,-36(s0)
   104d8:	fcb42c23          	sw	a1,-40(s0)
   104dc:	fcc42a23          	sw	a2,-44(s0)
   104e0:	fcd42823          	sw	a3,-48(s0)
   104e4:	fe042623          	sw	zero,-20(s0)
   104e8:	0200006f          	j	10508 <str_cpy_s+0x40>
   104ec:	fec42783          	lw	a5,-20(s0)
   104f0:	fd842703          	lw	a4,-40(s0)
   104f4:	00f707b3          	add	a5,a4,a5
   104f8:	00078023          	sb	zero,0(a5)
   104fc:	fec42783          	lw	a5,-20(s0)
   10500:	00178793          	addi	a5,a5,1
   10504:	fef42623          	sw	a5,-20(s0)
   10508:	fec42703          	lw	a4,-20(s0)
   1050c:	07f00793          	li	a5,127
   10510:	fce7dee3          	bge	a5,a4,104ec <str_cpy_s+0x24>
   10514:	fd442783          	lw	a5,-44(s0)
   10518:	fef42423          	sw	a5,-24(s0)
   1051c:	0300006f          	j	1054c <str_cpy_s+0x84>
   10520:	fe842783          	lw	a5,-24(s0)
   10524:	fdc42703          	lw	a4,-36(s0)
   10528:	00f70733          	add	a4,a4,a5
   1052c:	fe842783          	lw	a5,-24(s0)
   10530:	fd842683          	lw	a3,-40(s0)
   10534:	00f687b3          	add	a5,a3,a5
   10538:	00074703          	lbu	a4,0(a4)
   1053c:	00e78023          	sb	a4,0(a5)
   10540:	fe842783          	lw	a5,-24(s0)
   10544:	00178793          	addi	a5,a5,1
   10548:	fef42423          	sw	a5,-24(s0)
   1054c:	fe842783          	lw	a5,-24(s0)
   10550:	fd042703          	lw	a4,-48(s0)
   10554:	fcf776e3          	bgeu	a4,a5,10520 <str_cpy_s+0x58>
   10558:	00000013          	nop
   1055c:	00000013          	nop
   10560:	02c12403          	lw	s0,44(sp)
   10564:	03010113          	addi	sp,sp,48
   10568:	00008067          	ret

0001056c <split>:
   1056c:	fd010113          	addi	sp,sp,-48
   10570:	02112623          	sw	ra,44(sp)
   10574:	02812423          	sw	s0,40(sp)
   10578:	03010413          	addi	s0,sp,48
   1057c:	fca42e23          	sw	a0,-36(s0)
   10580:	00058793          	mv	a5,a1
   10584:	fcc42a23          	sw	a2,-44(s0)
   10588:	fcf40da3          	sb	a5,-37(s0)
   1058c:	fdc42503          	lw	a0,-36(s0)
   10590:	dc1ff0ef          	jal	10350 <str_len>
   10594:	fea42223          	sw	a0,-28(s0)
   10598:	fe042623          	sw	zero,-20(s0)
   1059c:	fe042423          	sw	zero,-24(s0)
   105a0:	0480006f          	j	105e8 <split+0x7c>
   105a4:	fe842783          	lw	a5,-24(s0)
   105a8:	fdc42703          	lw	a4,-36(s0)
   105ac:	00f707b3          	add	a5,a4,a5
   105b0:	0007c783          	lbu	a5,0(a5)
   105b4:	fdb44703          	lbu	a4,-37(s0)
   105b8:	02f71263          	bne	a4,a5,105dc <split+0x70>
   105bc:	fec42783          	lw	a5,-20(s0)
   105c0:	00178713          	addi	a4,a5,1
   105c4:	fee42623          	sw	a4,-20(s0)
   105c8:	00279793          	slli	a5,a5,0x2
   105cc:	fd442703          	lw	a4,-44(s0)
   105d0:	00f707b3          	add	a5,a4,a5
   105d4:	fe842703          	lw	a4,-24(s0)
   105d8:	00e7a023          	sw	a4,0(a5)
   105dc:	fe842783          	lw	a5,-24(s0)
   105e0:	00178793          	addi	a5,a5,1
   105e4:	fef42423          	sw	a5,-24(s0)
   105e8:	fe842783          	lw	a5,-24(s0)
   105ec:	fe442703          	lw	a4,-28(s0)
   105f0:	fae7eae3          	bltu	a5,a4,105a4 <split+0x38>
   105f4:	00000013          	nop
   105f8:	00000013          	nop
   105fc:	02c12083          	lw	ra,44(sp)
   10600:	02812403          	lw	s0,40(sp)
   10604:	03010113          	addi	sp,sp,48
   10608:	00008067          	ret

0001060c <itoa>:
   1060c:	fa010113          	addi	sp,sp,-96
   10610:	04112e23          	sw	ra,92(sp)
   10614:	04812c23          	sw	s0,88(sp)
   10618:	06010413          	addi	s0,sp,96
   1061c:	faa42623          	sw	a0,-84(s0)
   10620:	fab42423          	sw	a1,-88(s0)
   10624:	fac42223          	sw	a2,-92(s0)
   10628:	fe042623          	sw	zero,-20(s0)
   1062c:	fac42783          	lw	a5,-84(s0)
   10630:	fef42423          	sw	a5,-24(s0)
   10634:	fa042e23          	sw	zero,-68(s0)
   10638:	fc042023          	sw	zero,-64(s0)
   1063c:	fc042223          	sw	zero,-60(s0)
   10640:	fc042423          	sw	zero,-56(s0)
   10644:	fc042623          	sw	zero,-52(s0)
   10648:	fc042823          	sw	zero,-48(s0)
   1064c:	fc042a23          	sw	zero,-44(s0)
   10650:	fc042c23          	sw	zero,-40(s0)
   10654:	fc040e23          	sb	zero,-36(s0)
   10658:	fac42783          	lw	a5,-84(s0)
   1065c:	0207d463          	bgez	a5,10684 <itoa+0x78>
   10660:	fac42783          	lw	a5,-84(s0)
   10664:	40f007b3          	neg	a5,a5
   10668:	faf42623          	sw	a5,-84(s0)
   1066c:	fac42783          	lw	a5,-84(s0)
   10670:	fff7c793          	not	a5,a5
   10674:	fef42423          	sw	a5,-24(s0)
   10678:	fe842783          	lw	a5,-24(s0)
   1067c:	00178793          	addi	a5,a5,1
   10680:	fef42423          	sw	a5,-24(s0)
   10684:	fac42783          	lw	a5,-84(s0)
   10688:	0a079063          	bnez	a5,10728 <itoa+0x11c>
   1068c:	fec42783          	lw	a5,-20(s0)
   10690:	ff078793          	addi	a5,a5,-16
   10694:	008787b3          	add	a5,a5,s0
   10698:	03000713          	li	a4,48
   1069c:	fce78623          	sb	a4,-52(a5)
   106a0:	fec42783          	lw	a5,-20(s0)
   106a4:	00178793          	addi	a5,a5,1
   106a8:	fef42623          	sw	a5,-20(s0)
   106ac:	07c0006f          	j	10728 <itoa+0x11c>
   106b0:	fe842783          	lw	a5,-24(s0)
   106b4:	fa442583          	lw	a1,-92(s0)
   106b8:	00078513          	mv	a0,a5
   106bc:	a55ff0ef          	jal	10110 <mod>
   106c0:	fea42023          	sw	a0,-32(s0)
   106c4:	fe042703          	lw	a4,-32(s0)
   106c8:	00900793          	li	a5,9
   106cc:	00e7dc63          	bge	a5,a4,106e4 <itoa+0xd8>
   106d0:	fe042783          	lw	a5,-32(s0)
   106d4:	0ff7f793          	zext.b	a5,a5
   106d8:	05778793          	addi	a5,a5,87
   106dc:	0ff7f793          	zext.b	a5,a5
   106e0:	0140006f          	j	106f4 <itoa+0xe8>
   106e4:	fe042783          	lw	a5,-32(s0)
   106e8:	0ff7f793          	zext.b	a5,a5
   106ec:	03078793          	addi	a5,a5,48
   106f0:	0ff7f793          	zext.b	a5,a5
   106f4:	fec42703          	lw	a4,-20(s0)
   106f8:	ff070713          	addi	a4,a4,-16
   106fc:	00870733          	add	a4,a4,s0
   10700:	fcf70623          	sb	a5,-52(a4)
   10704:	fe842783          	lw	a5,-24(s0)
   10708:	fa442583          	lw	a1,-92(s0)
   1070c:	00078513          	mv	a0,a5
   10710:	a49ff0ef          	jal	10158 <div>
   10714:	00050793          	mv	a5,a0
   10718:	fef42423          	sw	a5,-24(s0)
   1071c:	fec42783          	lw	a5,-20(s0)
   10720:	00178793          	addi	a5,a5,1
   10724:	fef42623          	sw	a5,-20(s0)
   10728:	fe842783          	lw	a5,-24(s0)
   1072c:	f80792e3          	bnez	a5,106b0 <itoa+0xa4>
   10730:	fec42783          	lw	a5,-20(s0)
   10734:	fff78793          	addi	a5,a5,-1
   10738:	fef42623          	sw	a5,-20(s0)
   1073c:	fe042223          	sw	zero,-28(s0)
   10740:	fec42783          	lw	a5,-20(s0)
   10744:	00178793          	addi	a5,a5,1
   10748:	fa842703          	lw	a4,-88(s0)
   1074c:	00f707b3          	add	a5,a4,a5
   10750:	00078023          	sb	zero,0(a5)
   10754:	03c0006f          	j	10790 <itoa+0x184>
   10758:	fec42783          	lw	a5,-20(s0)
   1075c:	fa842703          	lw	a4,-88(s0)
   10760:	00f707b3          	add	a5,a4,a5
   10764:	fe442703          	lw	a4,-28(s0)
   10768:	ff070713          	addi	a4,a4,-16
   1076c:	00870733          	add	a4,a4,s0
   10770:	fcc74703          	lbu	a4,-52(a4)
   10774:	00e78023          	sb	a4,0(a5)
   10778:	fe442783          	lw	a5,-28(s0)
   1077c:	00178793          	addi	a5,a5,1
   10780:	fef42223          	sw	a5,-28(s0)
   10784:	fec42783          	lw	a5,-20(s0)
   10788:	fff78793          	addi	a5,a5,-1
   1078c:	fef42623          	sw	a5,-20(s0)
   10790:	fec42783          	lw	a5,-20(s0)
   10794:	fc07d2e3          	bgez	a5,10758 <itoa+0x14c>
   10798:	00000013          	nop
   1079c:	00000013          	nop
   107a0:	05c12083          	lw	ra,92(sp)
   107a4:	05812403          	lw	s0,88(sp)
   107a8:	06010113          	addi	sp,sp,96
   107ac:	00008067          	ret

000107b0 <out>:
   107b0:	fd010113          	addi	sp,sp,-48
   107b4:	02112623          	sw	ra,44(sp)
   107b8:	02812423          	sw	s0,40(sp)
   107bc:	03010413          	addi	s0,sp,48
   107c0:	fca42e23          	sw	a0,-36(s0)
   107c4:	fdc42503          	lw	a0,-36(s0)
   107c8:	b89ff0ef          	jal	10350 <str_len>
   107cc:	fea42623          	sw	a0,-20(s0)
   107d0:	00000013          	nop
   107d4:	02c12083          	lw	ra,44(sp)
   107d8:	02812403          	lw	s0,40(sp)
   107dc:	03010113          	addi	sp,sp,48
   107e0:	00008067          	ret

000107e4 <memset_i>:
   107e4:	fd010113          	addi	sp,sp,-48
   107e8:	02812623          	sw	s0,44(sp)
   107ec:	03010413          	addi	s0,sp,48
   107f0:	fca42e23          	sw	a0,-36(s0)
   107f4:	00058793          	mv	a5,a1
   107f8:	fcf40da3          	sb	a5,-37(s0)
   107fc:	fdc42783          	lw	a5,-36(s0)
   10800:	fef42623          	sw	a5,-20(s0)
   10804:	fec42783          	lw	a5,-20(s0)
   10808:	fdb44703          	lbu	a4,-37(s0)
   1080c:	00e78023          	sb	a4,0(a5)
   10810:	00000793          	li	a5,0
   10814:	00078513          	mv	a0,a5
   10818:	02c12403          	lw	s0,44(sp)
   1081c:	03010113          	addi	sp,sp,48
   10820:	00008067          	ret

00010824 <vprint>:
   10824:	fd010113          	addi	sp,sp,-48
   10828:	02812623          	sw	s0,44(sp)
   1082c:	03010413          	addi	s0,sp,48
   10830:	fca42e23          	sw	a0,-36(s0)
   10834:	fcb42c23          	sw	a1,-40(s0)
   10838:	fdc42783          	lw	a5,-36(s0)
   1083c:	fd842703          	lw	a4,-40(s0)
   10840:	00600893          	li	a7,6
   10844:	00078513          	mv	a0,a5
   10848:	00070593          	mv	a1,a4
   1084c:	00000073          	ecall
   10850:	00050793          	mv	a5,a0
   10854:	fef42623          	sw	a5,-20(s0)
   10858:	fec42783          	lw	a5,-20(s0)
   1085c:	00078513          	mv	a0,a5
   10860:	02c12403          	lw	s0,44(sp)
   10864:	03010113          	addi	sp,sp,48
   10868:	00008067          	ret

0001086c <vprint_i>:
   1086c:	fd010113          	addi	sp,sp,-48
   10870:	02112623          	sw	ra,44(sp)
   10874:	02812423          	sw	s0,40(sp)
   10878:	03010413          	addi	s0,sp,48
   1087c:	fca42e23          	sw	a0,-36(s0)
   10880:	fcb42c23          	sw	a1,-40(s0)
   10884:	fe042623          	sw	zero,-20(s0)
   10888:	04c0006f          	j	108d4 <vprint_i+0x68>
   1088c:	fec42703          	lw	a4,-20(s0)
   10890:	00001797          	auipc	a5,0x1
   10894:	b7078793          	addi	a5,a5,-1168 # 11400 <out_cache>
   10898:	00f707b3          	add	a5,a4,a5
   1089c:	0007c783          	lbu	a5,0(a5)
   108a0:	fef403a3          	sb	a5,-25(s0)
   108a4:	fec42703          	lw	a4,-20(s0)
   108a8:	300007b7          	lui	a5,0x30000
   108ac:	00478793          	addi	a5,a5,4 # 30000004 <__BSS_END__+0x2feeec04>
   108b0:	00f707b3          	add	a5,a4,a5
   108b4:	fef42023          	sw	a5,-32(s0)
   108b8:	fe744783          	lbu	a5,-25(s0)
   108bc:	00078593          	mv	a1,a5
   108c0:	fe042503          	lw	a0,-32(s0)
   108c4:	f21ff0ef          	jal	107e4 <memset_i>
   108c8:	fec42783          	lw	a5,-20(s0)
   108cc:	00178793          	addi	a5,a5,1
   108d0:	fef42623          	sw	a5,-20(s0)
   108d4:	fec42783          	lw	a5,-20(s0)
   108d8:	fd842703          	lw	a4,-40(s0)
   108dc:	fae7e8e3          	bltu	a5,a4,1088c <vprint_i+0x20>
   108e0:	300007b7          	lui	a5,0x30000
   108e4:	00378793          	addi	a5,a5,3 # 30000003 <__BSS_END__+0x2feeec03>
   108e8:	fef42423          	sw	a5,-24(s0)
   108ec:	00100593          	li	a1,1
   108f0:	fe842503          	lw	a0,-24(s0)
   108f4:	ef1ff0ef          	jal	107e4 <memset_i>
   108f8:	00000793          	li	a5,0
   108fc:	00078513          	mv	a0,a5
   10900:	02c12083          	lw	ra,44(sp)
   10904:	02812403          	lw	s0,40(sp)
   10908:	03010113          	addi	sp,sp,48
   1090c:	00008067          	ret

00010910 <print>:
   10910:	fa010113          	addi	sp,sp,-96
   10914:	02112e23          	sw	ra,60(sp)
   10918:	02812c23          	sw	s0,56(sp)
   1091c:	04010413          	addi	s0,sp,64
   10920:	fca42623          	sw	a0,-52(s0)
   10924:	00b42223          	sw	a1,4(s0)
   10928:	00c42423          	sw	a2,8(s0)
   1092c:	00d42623          	sw	a3,12(s0)
   10930:	00e42823          	sw	a4,16(s0)
   10934:	00f42a23          	sw	a5,20(s0)
   10938:	01042c23          	sw	a6,24(s0)
   1093c:	01142e23          	sw	a7,28(s0)
   10940:	fcc42503          	lw	a0,-52(s0)
   10944:	a0dff0ef          	jal	10350 <str_len>
   10948:	00050793          	mv	a5,a0
   1094c:	fcf42e23          	sw	a5,-36(s0)
   10950:	00100793          	li	a5,1
   10954:	fef42623          	sw	a5,-20(s0)
   10958:	fc042c23          	sw	zero,-40(s0)
   1095c:	fe042423          	sw	zero,-24(s0)
   10960:	fe042223          	sw	zero,-28(s0)
   10964:	16c0006f          	j	10ad0 <print+0x1c0>
   10968:	fe442783          	lw	a5,-28(s0)
   1096c:	fcc42703          	lw	a4,-52(s0)
   10970:	00f707b3          	add	a5,a4,a5
   10974:	0007c703          	lbu	a4,0(a5)
   10978:	02500793          	li	a5,37
   1097c:	10f71e63          	bne	a4,a5,10a98 <print+0x188>
   10980:	fe442783          	lw	a5,-28(s0)
   10984:	00178793          	addi	a5,a5,1
   10988:	fcc42703          	lw	a4,-52(s0)
   1098c:	00f707b3          	add	a5,a4,a5
   10990:	0007c783          	lbu	a5,0(a5)
   10994:	06300713          	li	a4,99
   10998:	00e78863          	beq	a5,a4,109a8 <print+0x98>
   1099c:	07300713          	li	a4,115
   109a0:	04e78c63          	beq	a5,a4,109f8 <print+0xe8>
   109a4:	0e40006f          	j	10a88 <print+0x178>
   109a8:	fec42783          	lw	a5,-20(s0)
   109ac:	00279793          	slli	a5,a5,0x2
   109b0:	fcf42a23          	sw	a5,-44(s0)
   109b4:	fd442783          	lw	a5,-44(s0)
   109b8:	00f405b3          	add	a1,s0,a5
   109bc:	0005a783          	lw	a5,0(a1)
   109c0:	fcf42c23          	sw	a5,-40(s0)
   109c4:	fec42783          	lw	a5,-20(s0)
   109c8:	00178793          	addi	a5,a5,1
   109cc:	fef42623          	sw	a5,-20(s0)
   109d0:	fe842783          	lw	a5,-24(s0)
   109d4:	00178713          	addi	a4,a5,1
   109d8:	fee42423          	sw	a4,-24(s0)
   109dc:	fd842703          	lw	a4,-40(s0)
   109e0:	0ff77713          	zext.b	a4,a4
   109e4:	00001697          	auipc	a3,0x1
   109e8:	a1c68693          	addi	a3,a3,-1508 # 11400 <out_cache>
   109ec:	00f687b3          	add	a5,a3,a5
   109f0:	00e78023          	sb	a4,0(a5)
   109f4:	0940006f          	j	10a88 <print+0x178>
   109f8:	fec42783          	lw	a5,-20(s0)
   109fc:	00279793          	slli	a5,a5,0x2
   10a00:	fcf42a23          	sw	a5,-44(s0)
   10a04:	fd442783          	lw	a5,-44(s0)
   10a08:	00f405b3          	add	a1,s0,a5
   10a0c:	0005a783          	lw	a5,0(a1)
   10a10:	fcf42c23          	sw	a5,-40(s0)
   10a14:	fec42783          	lw	a5,-20(s0)
   10a18:	00178793          	addi	a5,a5,1
   10a1c:	fef42623          	sw	a5,-20(s0)
   10a20:	fd842783          	lw	a5,-40(s0)
   10a24:	00078513          	mv	a0,a5
   10a28:	929ff0ef          	jal	10350 <str_len>
   10a2c:	00050793          	mv	a5,a0
   10a30:	fcf42823          	sw	a5,-48(s0)
   10a34:	fe042023          	sw	zero,-32(s0)
   10a38:	0400006f          	j	10a78 <print+0x168>
   10a3c:	fe042703          	lw	a4,-32(s0)
   10a40:	fd842783          	lw	a5,-40(s0)
   10a44:	00f707b3          	add	a5,a4,a5
   10a48:	00078693          	mv	a3,a5
   10a4c:	fe842783          	lw	a5,-24(s0)
   10a50:	00178713          	addi	a4,a5,1
   10a54:	fee42423          	sw	a4,-24(s0)
   10a58:	0006c703          	lbu	a4,0(a3)
   10a5c:	00001697          	auipc	a3,0x1
   10a60:	9a468693          	addi	a3,a3,-1628 # 11400 <out_cache>
   10a64:	00f687b3          	add	a5,a3,a5
   10a68:	00e78023          	sb	a4,0(a5)
   10a6c:	fe042783          	lw	a5,-32(s0)
   10a70:	00178793          	addi	a5,a5,1
   10a74:	fef42023          	sw	a5,-32(s0)
   10a78:	fe042703          	lw	a4,-32(s0)
   10a7c:	fd042783          	lw	a5,-48(s0)
   10a80:	faf74ee3          	blt	a4,a5,10a3c <print+0x12c>
   10a84:	00000013          	nop
   10a88:	fe442783          	lw	a5,-28(s0)
   10a8c:	00178793          	addi	a5,a5,1
   10a90:	fef42223          	sw	a5,-28(s0)
   10a94:	0300006f          	j	10ac4 <print+0x1b4>
   10a98:	fe442783          	lw	a5,-28(s0)
   10a9c:	fcc42703          	lw	a4,-52(s0)
   10aa0:	00f70733          	add	a4,a4,a5
   10aa4:	fe842783          	lw	a5,-24(s0)
   10aa8:	00178693          	addi	a3,a5,1
   10aac:	fed42423          	sw	a3,-24(s0)
   10ab0:	00074703          	lbu	a4,0(a4)
   10ab4:	00001697          	auipc	a3,0x1
   10ab8:	94c68693          	addi	a3,a3,-1716 # 11400 <out_cache>
   10abc:	00f687b3          	add	a5,a3,a5
   10ac0:	00e78023          	sb	a4,0(a5)
   10ac4:	fe442783          	lw	a5,-28(s0)
   10ac8:	00178793          	addi	a5,a5,1
   10acc:	fef42223          	sw	a5,-28(s0)
   10ad0:	fe442703          	lw	a4,-28(s0)
   10ad4:	fdc42783          	lw	a5,-36(s0)
   10ad8:	e8f748e3          	blt	a4,a5,10968 <print+0x58>
   10adc:	00001717          	auipc	a4,0x1
   10ae0:	92470713          	addi	a4,a4,-1756 # 11400 <out_cache>
   10ae4:	fe842783          	lw	a5,-24(s0)
   10ae8:	00f707b3          	add	a5,a4,a5
   10aec:	00078023          	sb	zero,0(a5)
   10af0:	fe842583          	lw	a1,-24(s0)
   10af4:	00001517          	auipc	a0,0x1
   10af8:	90c50513          	addi	a0,a0,-1780 # 11400 <out_cache>
   10afc:	d29ff0ef          	jal	10824 <vprint>
   10b00:	00000013          	nop
   10b04:	00078513          	mv	a0,a5
   10b08:	03c12083          	lw	ra,60(sp)
   10b0c:	03812403          	lw	s0,56(sp)
   10b10:	06010113          	addi	sp,sp,96
   10b14:	00008067          	ret

00010b18 <init_std>:
   10b18:	fe010113          	addi	sp,sp,-32
   10b1c:	00812e23          	sw	s0,28(sp)
   10b20:	02010413          	addi	s0,sp,32
   10b24:	00000797          	auipc	a5,0x0
   10b28:	4f478793          	addi	a5,a5,1268 # 11018 <syscall_table+0x18>
   10b2c:	fef42623          	sw	a5,-20(s0)
   10b30:	00000797          	auipc	a5,0x0
   10b34:	d3c78793          	addi	a5,a5,-708 # 1086c <vprint_i>
   10b38:	fef42423          	sw	a5,-24(s0)
   10b3c:	fec42783          	lw	a5,-20(s0)
   10b40:	fe842703          	lw	a4,-24(s0)
   10b44:	00f005b3          	add	a1,zero,a5
   10b48:	00e00633          	add	a2,zero,a4
   10b4c:	00c5a023          	sw	a2,0(a1)
   10b50:	00000013          	nop
   10b54:	01c12403          	lw	s0,28(sp)
   10b58:	02010113          	addi	sp,sp,32
   10b5c:	00008067          	ret

00010b60 <main>:
   10b60:	ff010113          	addi	sp,sp,-16
   10b64:	00112623          	sw	ra,12(sp)
   10b68:	00812423          	sw	s0,8(sp)
   10b6c:	01010413          	addi	s0,sp,16
   10b70:	fa9ff0ef          	jal	10b18 <init_std>
   10b74:	06400813          	li	a6,100
   10b78:	06c00793          	li	a5,108
   10b7c:	07200713          	li	a4,114
   10b80:	06f00693          	li	a3,111
   10b84:	07700613          	li	a2,119
   10b88:	06f00593          	li	a1,111
   10b8c:	00000517          	auipc	a0,0x0
   10b90:	09850513          	addi	a0,a0,152 # 10c24 <main+0xc4>
   10b94:	d7dff0ef          	jal	10910 <print>
   10b98:	00000597          	auipc	a1,0x0
   10b9c:	0a058593          	addi	a1,a1,160 # 10c38 <main+0xd8>
   10ba0:	00000517          	auipc	a0,0x0
   10ba4:	0a850513          	addi	a0,a0,168 # 10c48 <main+0xe8>
   10ba8:	d69ff0ef          	jal	10910 <print>
   10bac:	00000517          	auipc	a0,0x0
   10bb0:	0a050513          	addi	a0,a0,160 # 10c4c <main+0xec>
   10bb4:	d5dff0ef          	jal	10910 <print>
   10bb8:	00000517          	auipc	a0,0x0
   10bbc:	0b050513          	addi	a0,a0,176 # 10c68 <main+0x108>
   10bc0:	d51ff0ef          	jal	10910 <print>
   10bc4:	00000517          	auipc	a0,0x0
   10bc8:	0c050513          	addi	a0,a0,192 # 10c84 <main+0x124>
   10bcc:	d45ff0ef          	jal	10910 <print>
   10bd0:	00000517          	auipc	a0,0x0
   10bd4:	0d050513          	addi	a0,a0,208 # 10ca0 <main+0x140>
   10bd8:	d39ff0ef          	jal	10910 <print>
   10bdc:	00000517          	auipc	a0,0x0
   10be0:	0e050513          	addi	a0,a0,224 # 10cbc <main+0x15c>
   10be4:	d2dff0ef          	jal	10910 <print>
   10be8:	00000517          	auipc	a0,0x0
   10bec:	0ec50513          	addi	a0,a0,236 # 10cd4 <main+0x174>
   10bf0:	d21ff0ef          	jal	10910 <print>
   10bf4:	00000517          	auipc	a0,0x0
   10bf8:	0f850513          	addi	a0,a0,248 # 10cec <main+0x18c>
   10bfc:	d15ff0ef          	jal	10910 <print>
   10c00:	00000517          	auipc	a0,0x0
   10c04:	10450513          	addi	a0,a0,260 # 10d04 <main+0x1a4>
   10c08:	d09ff0ef          	jal	10910 <print>
   10c0c:	00000793          	li	a5,0
   10c10:	00078513          	mv	a0,a5
   10c14:	00c12083          	lw	ra,12(sp)
   10c18:	00812403          	lw	s0,8(sp)
   10c1c:	01010113          	addi	sp,sp,16
   10c20:	00008067          	ret

Disassembly of section .rodata:

00010c24 <__DATA_BEGIN__-0x3dc>:
   10c24:	6568                	.insn	2, 0x6568
   10c26:	6c6c                	.insn	2, 0x6c6c
   10c28:	6325                	.insn	2, 0x6325
   10c2a:	252c                	.insn	2, 0x252c
   10c2c:	25632563          	.insn	4, 0x25632563
   10c30:	25632563          	.insn	4, 0x25632563
   10c34:	00000a63          	beqz	zero,10c48 <main+0xe8>
   10c38:	6568                	.insn	2, 0x6568
   10c3a:	6c6c                	.insn	2, 0x6c6c
   10c3c:	6f772c6f          	jal	s8,83b32 <__global_pointer$+0x72332>
   10c40:	6c72                	.insn	2, 0x6c72
   10c42:	3264                	.insn	2, 0x3264
   10c44:	0000                	.insn	2, 0x
   10c46:	0000                	.insn	2, 0x
   10c48:	7325                	.insn	2, 0x7325
   10c4a:	000a                	.insn	2, 0x000a
   10c4c:	7272                	.insn	2, 0x7272
   10c4e:	7272                	.insn	2, 0x7272
   10c50:	7272                	.insn	2, 0x7272
   10c52:	2020                	.insn	2, 0x2020
   10c54:	2020                	.insn	2, 0x2020
   10c56:	2020                	.insn	2, 0x2020
   10c58:	7676                	.insn	2, 0x7676
   10c5a:	2020                	.insn	2, 0x2020
   10c5c:	2020                	.insn	2, 0x2020
   10c5e:	2020                	.insn	2, 0x2020
   10c60:	2020                	.insn	2, 0x2020
   10c62:	2020                	.insn	2, 0x2020
   10c64:	7676                	.insn	2, 0x7676
   10c66:	000a                	.insn	2, 0x000a
   10c68:	7272                	.insn	2, 0x7272
   10c6a:	2020                	.insn	2, 0x2020
   10c6c:	2020                	.insn	2, 0x2020
   10c6e:	7272                	.insn	2, 0x7272
   10c70:	2020                	.insn	2, 0x2020
   10c72:	2020                	.insn	2, 0x2020
   10c74:	7620                	.insn	2, 0x7620
   10c76:	2076                	.insn	2, 0x2076
   10c78:	2020                	.insn	2, 0x2020
   10c7a:	2020                	.insn	2, 0x2020
   10c7c:	2020                	.insn	2, 0x2020
   10c7e:	7620                	.insn	2, 0x7620
   10c80:	0a76                	.insn	2, 0x0a76
   10c82:	0000                	.insn	2, 0x
   10c84:	7272                	.insn	2, 0x7272
   10c86:	2020                	.insn	2, 0x2020
   10c88:	2020                	.insn	2, 0x2020
   10c8a:	7272                	.insn	2, 0x7272
   10c8c:	2020                	.insn	2, 0x2020
   10c8e:	2020                	.insn	2, 0x2020
   10c90:	2020                	.insn	2, 0x2020
   10c92:	7676                	.insn	2, 0x7676
   10c94:	2020                	.insn	2, 0x2020
   10c96:	2020                	.insn	2, 0x2020
   10c98:	2020                	.insn	2, 0x2020
   10c9a:	7676                	.insn	2, 0x7676
   10c9c:	000a                	.insn	2, 0x000a
   10c9e:	0000                	.insn	2, 0x
   10ca0:	7272                	.insn	2, 0x7272
   10ca2:	7272                	.insn	2, 0x7272
   10ca4:	7272                	.insn	2, 0x7272
   10ca6:	2020                	.insn	2, 0x2020
   10ca8:	2020                	.insn	2, 0x2020
   10caa:	2020                	.insn	2, 0x2020
   10cac:	2020                	.insn	2, 0x2020
   10cae:	7620                	.insn	2, 0x7620
   10cb0:	2076                	.insn	2, 0x2076
   10cb2:	2020                	.insn	2, 0x2020
   10cb4:	7620                	.insn	2, 0x7620
   10cb6:	0a76                	.insn	2, 0x0a76
   10cb8:	0000                	.insn	2, 0x
   10cba:	0000                	.insn	2, 0x
   10cbc:	7272                	.insn	2, 0x7272
   10cbe:	7272                	.insn	2, 0x7272
   10cc0:	2020                	.insn	2, 0x2020
   10cc2:	2020                	.insn	2, 0x2020
   10cc4:	2020                	.insn	2, 0x2020
   10cc6:	2020                	.insn	2, 0x2020
   10cc8:	2020                	.insn	2, 0x2020
   10cca:	2020                	.insn	2, 0x2020
   10ccc:	7676                	.insn	2, 0x7676
   10cce:	2020                	.insn	2, 0x2020
   10cd0:	7676                	.insn	2, 0x7676
   10cd2:	000a                	.insn	2, 0x000a
   10cd4:	7272                	.insn	2, 0x7272
   10cd6:	2020                	.insn	2, 0x2020
   10cd8:	7272                	.insn	2, 0x7272
   10cda:	2020                	.insn	2, 0x2020
   10cdc:	2020                	.insn	2, 0x2020
   10cde:	2020                	.insn	2, 0x2020
   10ce0:	2020                	.insn	2, 0x2020
   10ce2:	2020                	.insn	2, 0x2020
   10ce4:	7620                	.insn	2, 0x7620
   10ce6:	7676                	.insn	2, 0x7676
   10ce8:	0a76                	.insn	2, 0x0a76
   10cea:	0000                	.insn	2, 0x
   10cec:	7272                	.insn	2, 0x7272
   10cee:	2020                	.insn	2, 0x2020
   10cf0:	2020                	.insn	2, 0x2020
   10cf2:	7272                	.insn	2, 0x7272
   10cf4:	2020                	.insn	2, 0x2020
   10cf6:	2020                	.insn	2, 0x2020
   10cf8:	2020                	.insn	2, 0x2020
   10cfa:	2020                	.insn	2, 0x2020
   10cfc:	2020                	.insn	2, 0x2020
   10cfe:	7676                	.insn	2, 0x7676
   10d00:	0a0a                	.insn	2, 0x0a0a
   10d02:	0000                	.insn	2, 0x
   10d04:	6c697473          	.insn	4, 0x6c697473
   10d08:	206c                	.insn	2, 0x206c
   10d0a:	6e69                	.insn	2, 0x6e69
   10d0c:	6220                	.insn	2, 0x6220
   10d0e:	6975                	.insn	2, 0x6975
   10d10:	646c                	.insn	2, 0x646c
   10d12:	6e69                	.insn	2, 0x6e69
   10d14:	2e2e2e67          	.insn	4, 0x2e2e2e67
   10d18:	000a                	.insn	2, 0x000a

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
   0:	2841                	.insn	2, 0x2841
   2:	0000                	.insn	2, 0x
   4:	7200                	.insn	2, 0x7200
   6:	7369                	.insn	2, 0x7369
   8:	01007663          	bgeu	zero,a6,14 <_start-0x10080>
   c:	001e                	.insn	2, 0x001e
   e:	0000                	.insn	2, 0x
  10:	1004                	.insn	2, 0x1004
  12:	7205                	.insn	2, 0x7205
  14:	3376                	.insn	2, 0x3376
  16:	6932                	.insn	2, 0x6932
  18:	7032                	.insn	2, 0x7032
  1a:	5f31                	.insn	2, 0x5f31
  1c:	697a                	.insn	2, 0x697a
  1e:	32727363          	bgeu	tp,t2,344 <_start-0xfd50>
  22:	3070                	.insn	2, 0x3070
  24:	0800                	.insn	2, 0x0800
  26:	0a01                	.insn	2, 0x0a01
  28:	Address 0x28 is out of bounds.

