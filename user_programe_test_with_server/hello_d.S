
hello:     file format elf32-littleriscv


Disassembly of section .text:

000100b4 <exit>:
   100b4:	fe010113          	addi	sp,sp,-32
   100b8:	00112e23          	sw	ra,28(sp)
   100bc:	00812c23          	sw	s0,24(sp)
   100c0:	02010413          	addi	s0,sp,32
   100c4:	00900893          	li	a7,9
   100c8:	00000073          	ecall
   100cc:	00050793          	mv	a5,a0
   100d0:	fef42623          	sw	a5,-20(s0)
   100d4:	fec42783          	lw	a5,-20(s0)
   100d8:	00078513          	mv	a0,a5
   100dc:	01c12083          	lw	ra,28(sp)
   100e0:	01812403          	lw	s0,24(sp)
   100e4:	02010113          	addi	sp,sp,32
   100e8:	00008067          	ret

000100ec <vgetch>:
   100ec:	fd010113          	addi	sp,sp,-48
   100f0:	02112623          	sw	ra,44(sp)
   100f4:	02812423          	sw	s0,40(sp)
   100f8:	03010413          	addi	s0,sp,48
   100fc:	fca42e23          	sw	a0,-36(s0)
   10100:	fdc42783          	lw	a5,-36(s0)
   10104:	00600893          	li	a7,6
   10108:	00078513          	mv	a0,a5
   1010c:	00000073          	ecall
   10110:	00050793          	mv	a5,a0
   10114:	fef42623          	sw	a5,-20(s0)
   10118:	fec42783          	lw	a5,-20(s0)
   1011c:	00078513          	mv	a0,a5
   10120:	02c12083          	lw	ra,44(sp)
   10124:	02812403          	lw	s0,40(sp)
   10128:	03010113          	addi	sp,sp,48
   1012c:	00008067          	ret

00010130 <kbhit>:
   10130:	fd010113          	addi	sp,sp,-48
   10134:	02112623          	sw	ra,44(sp)
   10138:	02812423          	sw	s0,40(sp)
   1013c:	03010413          	addi	s0,sp,48
   10140:	fca42e23          	sw	a0,-36(s0)
   10144:	fdc42783          	lw	a5,-36(s0)
   10148:	00800893          	li	a7,8
   1014c:	00078513          	mv	a0,a5
   10150:	00000073          	ecall
   10154:	00050793          	mv	a5,a0
   10158:	fef42623          	sw	a5,-20(s0)
   1015c:	fec42783          	lw	a5,-20(s0)
   10160:	00078513          	mv	a0,a5
   10164:	02c12083          	lw	ra,44(sp)
   10168:	02812403          	lw	s0,40(sp)
   1016c:	03010113          	addi	sp,sp,48
   10170:	00008067          	ret

00010174 <powoff>:
   10174:	fe010113          	addi	sp,sp,-32
   10178:	00112e23          	sw	ra,28(sp)
   1017c:	00812c23          	sw	s0,24(sp)
   10180:	02010413          	addi	s0,sp,32
   10184:	00700893          	li	a7,7
   10188:	00000073          	ecall
   1018c:	00050793          	mv	a5,a0
   10190:	fef42623          	sw	a5,-20(s0)
   10194:	fec42783          	lw	a5,-20(s0)
   10198:	00078513          	mv	a0,a5
   1019c:	01c12083          	lw	ra,28(sp)
   101a0:	01812403          	lw	s0,24(sp)
   101a4:	02010113          	addi	sp,sp,32
   101a8:	00008067          	ret

000101ac <vprint>:
   101ac:	fd010113          	addi	sp,sp,-48
   101b0:	02112623          	sw	ra,44(sp)
   101b4:	02812423          	sw	s0,40(sp)
   101b8:	03010413          	addi	s0,sp,48
   101bc:	fca42e23          	sw	a0,-36(s0)
   101c0:	fcb42c23          	sw	a1,-40(s0)
   101c4:	fdc42783          	lw	a5,-36(s0)
   101c8:	fd842703          	lw	a4,-40(s0)
   101cc:	00500893          	li	a7,5
   101d0:	00078513          	mv	a0,a5
   101d4:	00070593          	mv	a1,a4
   101d8:	00000073          	ecall
   101dc:	00050793          	mv	a5,a0
   101e0:	fef42623          	sw	a5,-20(s0)
   101e4:	fec42783          	lw	a5,-20(s0)
   101e8:	00078513          	mv	a0,a5
   101ec:	02c12083          	lw	ra,44(sp)
   101f0:	02812403          	lw	s0,40(sp)
   101f4:	03010113          	addi	sp,sp,48
   101f8:	00008067          	ret

000101fc <read>:
   101fc:	fd010113          	addi	sp,sp,-48
   10200:	02112623          	sw	ra,44(sp)
   10204:	02812423          	sw	s0,40(sp)
   10208:	03010413          	addi	s0,sp,48
   1020c:	fca42e23          	sw	a0,-36(s0)
   10210:	fcb42c23          	sw	a1,-40(s0)
   10214:	fcc42a23          	sw	a2,-44(s0)
   10218:	fcd42823          	sw	a3,-48(s0)
   1021c:	fdc42783          	lw	a5,-36(s0)
   10220:	fd842703          	lw	a4,-40(s0)
   10224:	fd442683          	lw	a3,-44(s0)
   10228:	fd042603          	lw	a2,-48(s0)
   1022c:	00300893          	li	a7,3
   10230:	00078293          	mv	t0,a5
   10234:	00070313          	mv	t1,a4
   10238:	00068393          	mv	t2,a3
   1023c:	00060e13          	mv	t3,a2
   10240:	00028513          	mv	a0,t0
   10244:	00030593          	mv	a1,t1
   10248:	00038613          	mv	a2,t2
   1024c:	000e0693          	mv	a3,t3
   10250:	00000073          	ecall
   10254:	00050793          	mv	a5,a0
   10258:	fef42623          	sw	a5,-20(s0)
   1025c:	fec42783          	lw	a5,-20(s0)
   10260:	00078513          	mv	a0,a5
   10264:	02c12083          	lw	ra,44(sp)
   10268:	02812403          	lw	s0,40(sp)
   1026c:	03010113          	addi	sp,sp,48
   10270:	00008067          	ret

00010274 <write>:
   10274:	fd010113          	addi	sp,sp,-48
   10278:	02112623          	sw	ra,44(sp)
   1027c:	02812423          	sw	s0,40(sp)
   10280:	03010413          	addi	s0,sp,48
   10284:	fca42e23          	sw	a0,-36(s0)
   10288:	fcb42c23          	sw	a1,-40(s0)
   1028c:	fcc42a23          	sw	a2,-44(s0)
   10290:	fcd42823          	sw	a3,-48(s0)
   10294:	fdc42783          	lw	a5,-36(s0)
   10298:	fd842703          	lw	a4,-40(s0)
   1029c:	fd442683          	lw	a3,-44(s0)
   102a0:	fd042603          	lw	a2,-48(s0)
   102a4:	00200893          	li	a7,2
   102a8:	00078293          	mv	t0,a5
   102ac:	00070313          	mv	t1,a4
   102b0:	00068393          	mv	t2,a3
   102b4:	00060e13          	mv	t3,a2
   102b8:	00028513          	mv	a0,t0
   102bc:	00030593          	mv	a1,t1
   102c0:	00038613          	mv	a2,t2
   102c4:	000e0693          	mv	a3,t3
   102c8:	00000073          	ecall
   102cc:	00050793          	mv	a5,a0
   102d0:	fef42623          	sw	a5,-20(s0)
   102d4:	fec42783          	lw	a5,-20(s0)
   102d8:	00078513          	mv	a0,a5
   102dc:	02c12083          	lw	ra,44(sp)
   102e0:	02812403          	lw	s0,40(sp)
   102e4:	03010113          	addi	sp,sp,48
   102e8:	00008067          	ret

000102ec <create>:
   102ec:	fd010113          	addi	sp,sp,-48
   102f0:	02112623          	sw	ra,44(sp)
   102f4:	02812423          	sw	s0,40(sp)
   102f8:	03010413          	addi	s0,sp,48
   102fc:	fca42e23          	sw	a0,-36(s0)
   10300:	00058793          	mv	a5,a1
   10304:	fcc42a23          	sw	a2,-44(s0)
   10308:	fcd42823          	sw	a3,-48(s0)
   1030c:	fcf40da3          	sb	a5,-37(s0)
   10310:	fdc42783          	lw	a5,-36(s0)
   10314:	fdb44703          	lbu	a4,-37(s0)
   10318:	00070613          	mv	a2,a4
   1031c:	fd442703          	lw	a4,-44(s0)
   10320:	fd042683          	lw	a3,-48(s0)
   10324:	00400893          	li	a7,4
   10328:	00078293          	mv	t0,a5
   1032c:	00060313          	mv	t1,a2
   10330:	00070393          	mv	t2,a4
   10334:	00068e13          	mv	t3,a3
   10338:	00028513          	mv	a0,t0
   1033c:	00030593          	mv	a1,t1
   10340:	00038613          	mv	a2,t2
   10344:	000e0693          	mv	a3,t3
   10348:	00000073          	ecall
   1034c:	00050793          	mv	a5,a0
   10350:	fef42623          	sw	a5,-20(s0)
   10354:	fec42783          	lw	a5,-20(s0)
   10358:	00078513          	mv	a0,a5
   1035c:	02c12083          	lw	ra,44(sp)
   10360:	02812403          	lw	s0,40(sp)
   10364:	03010113          	addi	sp,sp,48
   10368:	00008067          	ret

0001036c <open>:
   1036c:	fd010113          	addi	sp,sp,-48
   10370:	02112623          	sw	ra,44(sp)
   10374:	02812423          	sw	s0,40(sp)
   10378:	03010413          	addi	s0,sp,48
   1037c:	fca42e23          	sw	a0,-36(s0)
   10380:	fcb42c23          	sw	a1,-40(s0)
   10384:	fcc42a23          	sw	a2,-44(s0)
   10388:	fdc42783          	lw	a5,-36(s0)
   1038c:	fd842703          	lw	a4,-40(s0)
   10390:	fd442683          	lw	a3,-44(s0)
   10394:	00000893          	li	a7,0
   10398:	00078513          	mv	a0,a5
   1039c:	00070593          	mv	a1,a4
   103a0:	00068613          	mv	a2,a3
   103a4:	00000073          	ecall
   103a8:	00050793          	mv	a5,a0
   103ac:	fef42623          	sw	a5,-20(s0)
   103b0:	fec42783          	lw	a5,-20(s0)
   103b4:	00078513          	mv	a0,a5
   103b8:	02c12083          	lw	ra,44(sp)
   103bc:	02812403          	lw	s0,40(sp)
   103c0:	03010113          	addi	sp,sp,48
   103c4:	00008067          	ret

000103c8 <send>:
   103c8:	fd010113          	addi	sp,sp,-48
   103cc:	02112623          	sw	ra,44(sp)
   103d0:	02812423          	sw	s0,40(sp)
   103d4:	03010413          	addi	s0,sp,48
   103d8:	fca42e23          	sw	a0,-36(s0)
   103dc:	fcb42c23          	sw	a1,-40(s0)
   103e0:	fcc42a23          	sw	a2,-44(s0)
   103e4:	fdc42783          	lw	a5,-36(s0)
   103e8:	fd842703          	lw	a4,-40(s0)
   103ec:	fd442683          	lw	a3,-44(s0)
   103f0:	00a00893          	li	a7,10
   103f4:	00078513          	mv	a0,a5
   103f8:	00070593          	mv	a1,a4
   103fc:	00068613          	mv	a2,a3
   10400:	00000073          	ecall
   10404:	00050793          	mv	a5,a0
   10408:	fef42623          	sw	a5,-20(s0)
   1040c:	fec42783          	lw	a5,-20(s0)
   10410:	00078513          	mv	a0,a5
   10414:	02c12083          	lw	ra,44(sp)
   10418:	02812403          	lw	s0,40(sp)
   1041c:	03010113          	addi	sp,sp,48
   10420:	00008067          	ret

00010424 <recv>:
   10424:	fd010113          	addi	sp,sp,-48
   10428:	02112623          	sw	ra,44(sp)
   1042c:	02812423          	sw	s0,40(sp)
   10430:	03010413          	addi	s0,sp,48
   10434:	fca42e23          	sw	a0,-36(s0)
   10438:	fcb42c23          	sw	a1,-40(s0)
   1043c:	fcc42a23          	sw	a2,-44(s0)
   10440:	fcd42823          	sw	a3,-48(s0)
   10444:	fdc42783          	lw	a5,-36(s0)
   10448:	fd842703          	lw	a4,-40(s0)
   1044c:	fd442683          	lw	a3,-44(s0)
   10450:	fd042603          	lw	a2,-48(s0)
   10454:	00b00893          	li	a7,11
   10458:	00078293          	mv	t0,a5
   1045c:	00070313          	mv	t1,a4
   10460:	00068393          	mv	t2,a3
   10464:	00060e13          	mv	t3,a2
   10468:	00028513          	mv	a0,t0
   1046c:	00030593          	mv	a1,t1
   10470:	00038613          	mv	a2,t2
   10474:	000e0693          	mv	a3,t3
   10478:	00000073          	ecall
   1047c:	00050793          	mv	a5,a0
   10480:	fef42623          	sw	a5,-20(s0)
   10484:	fec42783          	lw	a5,-20(s0)
   10488:	00078513          	mv	a0,a5
   1048c:	02c12083          	lw	ra,44(sp)
   10490:	02812403          	lw	s0,40(sp)
   10494:	03010113          	addi	sp,sp,48
   10498:	00008067          	ret

0001049c <print>:
   1049c:	f7010113          	addi	sp,sp,-144
   104a0:	06112623          	sw	ra,108(sp)
   104a4:	06812423          	sw	s0,104(sp)
   104a8:	07010413          	addi	s0,sp,112
   104ac:	f8a42e23          	sw	a0,-100(s0)
   104b0:	00b42223          	sw	a1,4(s0)
   104b4:	00c42423          	sw	a2,8(s0)
   104b8:	00d42623          	sw	a3,12(s0)
   104bc:	00e42823          	sw	a4,16(s0)
   104c0:	00f42a23          	sw	a5,20(s0)
   104c4:	01042c23          	sw	a6,24(s0)
   104c8:	01142e23          	sw	a7,28(s0)
   104cc:	f9c42503          	lw	a0,-100(s0)
   104d0:	554000ef          	jal	10a24 <str_len>
   104d4:	00050793          	mv	a5,a0
   104d8:	fcf42c23          	sw	a5,-40(s0)
   104dc:	00100793          	li	a5,1
   104e0:	fef42623          	sw	a5,-20(s0)
   104e4:	fc042a23          	sw	zero,-44(s0)
   104e8:	fe042423          	sw	zero,-24(s0)
   104ec:	fe042223          	sw	zero,-28(s0)
   104f0:	2280006f          	j	10718 <print+0x27c>
   104f4:	fe442783          	lw	a5,-28(s0)
   104f8:	f9c42703          	lw	a4,-100(s0)
   104fc:	00f707b3          	add	a5,a4,a5
   10500:	0007c703          	lbu	a4,0(a5)
   10504:	02500793          	li	a5,37
   10508:	1cf71c63          	bne	a4,a5,106e0 <print+0x244>
   1050c:	fc042823          	sw	zero,-48(s0)
   10510:	fc042623          	sw	zero,-52(s0)
   10514:	fe442783          	lw	a5,-28(s0)
   10518:	00178793          	addi	a5,a5,1
   1051c:	f9c42703          	lw	a4,-100(s0)
   10520:	00f707b3          	add	a5,a4,a5
   10524:	0007c783          	lbu	a5,0(a5)
   10528:	07300713          	li	a4,115
   1052c:	06e78863          	beq	a5,a4,1059c <print+0x100>
   10530:	07300713          	li	a4,115
   10534:	18f74c63          	blt	a4,a5,106cc <print+0x230>
   10538:	06300713          	li	a4,99
   1053c:	00e78863          	beq	a5,a4,1054c <print+0xb0>
   10540:	06400713          	li	a4,100
   10544:	0ee78463          	beq	a5,a4,1062c <print+0x190>
   10548:	1840006f          	j	106cc <print+0x230>
   1054c:	fec42783          	lw	a5,-20(s0)
   10550:	00279793          	slli	a5,a5,0x2
   10554:	fcf42823          	sw	a5,-48(s0)
   10558:	fd042783          	lw	a5,-48(s0)
   1055c:	00f405b3          	add	a1,s0,a5
   10560:	0005a783          	lw	a5,0(a1)
   10564:	fcf42a23          	sw	a5,-44(s0)
   10568:	fec42783          	lw	a5,-20(s0)
   1056c:	00178793          	addi	a5,a5,1
   10570:	fef42623          	sw	a5,-20(s0)
   10574:	fe842783          	lw	a5,-24(s0)
   10578:	00178713          	addi	a4,a5,1
   1057c:	fee42423          	sw	a4,-24(s0)
   10580:	fd442703          	lw	a4,-44(s0)
   10584:	0ff77713          	zext.b	a4,a4
   10588:	00002697          	auipc	a3,0x2
   1058c:	2e068693          	addi	a3,a3,736 # 12868 <out_cache>
   10590:	00f687b3          	add	a5,a3,a5
   10594:	00e78023          	sb	a4,0(a5)
   10598:	1380006f          	j	106d0 <print+0x234>
   1059c:	fec42783          	lw	a5,-20(s0)
   105a0:	00279793          	slli	a5,a5,0x2
   105a4:	fcf42823          	sw	a5,-48(s0)
   105a8:	fd042783          	lw	a5,-48(s0)
   105ac:	00f405b3          	add	a1,s0,a5
   105b0:	0005a783          	lw	a5,0(a1)
   105b4:	fcf42a23          	sw	a5,-44(s0)
   105b8:	fec42783          	lw	a5,-20(s0)
   105bc:	00178793          	addi	a5,a5,1
   105c0:	fef42623          	sw	a5,-20(s0)
   105c4:	fd442783          	lw	a5,-44(s0)
   105c8:	00078513          	mv	a0,a5
   105cc:	458000ef          	jal	10a24 <str_len>
   105d0:	00050793          	mv	a5,a0
   105d4:	fcf42623          	sw	a5,-52(s0)
   105d8:	fe042023          	sw	zero,-32(s0)
   105dc:	0400006f          	j	1061c <print+0x180>
   105e0:	fe042703          	lw	a4,-32(s0)
   105e4:	fd442783          	lw	a5,-44(s0)
   105e8:	00f707b3          	add	a5,a4,a5
   105ec:	00078693          	mv	a3,a5
   105f0:	fe842783          	lw	a5,-24(s0)
   105f4:	00178713          	addi	a4,a5,1
   105f8:	fee42423          	sw	a4,-24(s0)
   105fc:	0006c703          	lbu	a4,0(a3)
   10600:	00002697          	auipc	a3,0x2
   10604:	26868693          	addi	a3,a3,616 # 12868 <out_cache>
   10608:	00f687b3          	add	a5,a3,a5
   1060c:	00e78023          	sb	a4,0(a5)
   10610:	fe042783          	lw	a5,-32(s0)
   10614:	00178793          	addi	a5,a5,1
   10618:	fef42023          	sw	a5,-32(s0)
   1061c:	fe042703          	lw	a4,-32(s0)
   10620:	fcc42783          	lw	a5,-52(s0)
   10624:	faf74ee3          	blt	a4,a5,105e0 <print+0x144>
   10628:	0a80006f          	j	106d0 <print+0x234>
   1062c:	fec42783          	lw	a5,-20(s0)
   10630:	00279793          	slli	a5,a5,0x2
   10634:	fcf42823          	sw	a5,-48(s0)
   10638:	fd042783          	lw	a5,-48(s0)
   1063c:	00f405b3          	add	a1,s0,a5
   10640:	0005a783          	lw	a5,0(a1)
   10644:	fcf42a23          	sw	a5,-44(s0)
   10648:	fec42783          	lw	a5,-20(s0)
   1064c:	00178793          	addi	a5,a5,1
   10650:	fef42623          	sw	a5,-20(s0)
   10654:	fd442783          	lw	a5,-44(s0)
   10658:	fa840713          	addi	a4,s0,-88
   1065c:	00070593          	mv	a1,a4
   10660:	00078513          	mv	a0,a5
   10664:	66c000ef          	jal	10cd0 <itoa>
   10668:	fa840793          	addi	a5,s0,-88
   1066c:	00078513          	mv	a0,a5
   10670:	3b4000ef          	jal	10a24 <str_len>
   10674:	00050793          	mv	a5,a0
   10678:	fcf42623          	sw	a5,-52(s0)
   1067c:	fc042e23          	sw	zero,-36(s0)
   10680:	03c0006f          	j	106bc <print+0x220>
   10684:	fe842783          	lw	a5,-24(s0)
   10688:	00178713          	addi	a4,a5,1
   1068c:	fee42423          	sw	a4,-24(s0)
   10690:	fdc42703          	lw	a4,-36(s0)
   10694:	ff070713          	addi	a4,a4,-16
   10698:	00870733          	add	a4,a4,s0
   1069c:	fb874703          	lbu	a4,-72(a4)
   106a0:	00002697          	auipc	a3,0x2
   106a4:	1c868693          	addi	a3,a3,456 # 12868 <out_cache>
   106a8:	00f687b3          	add	a5,a3,a5
   106ac:	00e78023          	sb	a4,0(a5)
   106b0:	fdc42783          	lw	a5,-36(s0)
   106b4:	00178793          	addi	a5,a5,1
   106b8:	fcf42e23          	sw	a5,-36(s0)
   106bc:	fdc42703          	lw	a4,-36(s0)
   106c0:	fcc42783          	lw	a5,-52(s0)
   106c4:	fcf740e3          	blt	a4,a5,10684 <print+0x1e8>
   106c8:	0080006f          	j	106d0 <print+0x234>
   106cc:	00000013          	nop
   106d0:	fe442783          	lw	a5,-28(s0)
   106d4:	00178793          	addi	a5,a5,1
   106d8:	fef42223          	sw	a5,-28(s0)
   106dc:	0300006f          	j	1070c <print+0x270>
   106e0:	fe442783          	lw	a5,-28(s0)
   106e4:	f9c42703          	lw	a4,-100(s0)
   106e8:	00f70733          	add	a4,a4,a5
   106ec:	fe842783          	lw	a5,-24(s0)
   106f0:	00178693          	addi	a3,a5,1
   106f4:	fed42423          	sw	a3,-24(s0)
   106f8:	00074703          	lbu	a4,0(a4)
   106fc:	00002697          	auipc	a3,0x2
   10700:	16c68693          	addi	a3,a3,364 # 12868 <out_cache>
   10704:	00f687b3          	add	a5,a3,a5
   10708:	00e78023          	sb	a4,0(a5)
   1070c:	fe442783          	lw	a5,-28(s0)
   10710:	00178793          	addi	a5,a5,1
   10714:	fef42223          	sw	a5,-28(s0)
   10718:	fe442703          	lw	a4,-28(s0)
   1071c:	fd842783          	lw	a5,-40(s0)
   10720:	dcf74ae3          	blt	a4,a5,104f4 <print+0x58>
   10724:	00002717          	auipc	a4,0x2
   10728:	14470713          	addi	a4,a4,324 # 12868 <out_cache>
   1072c:	fe842783          	lw	a5,-24(s0)
   10730:	00f707b3          	add	a5,a4,a5
   10734:	00078023          	sb	zero,0(a5)
   10738:	fe842583          	lw	a1,-24(s0)
   1073c:	00002517          	auipc	a0,0x2
   10740:	12c50513          	addi	a0,a0,300 # 12868 <out_cache>
   10744:	a69ff0ef          	jal	101ac <vprint>
   10748:	00000793          	li	a5,0
   1074c:	00078513          	mv	a0,a5
   10750:	06c12083          	lw	ra,108(sp)
   10754:	06812403          	lw	s0,104(sp)
   10758:	09010113          	addi	sp,sp,144
   1075c:	00008067          	ret

00010760 <input>:
   10760:	f5010113          	addi	sp,sp,-176
   10764:	08112623          	sw	ra,140(sp)
   10768:	08812423          	sw	s0,136(sp)
   1076c:	09010413          	addi	s0,sp,144
   10770:	f6a42e23          	sw	a0,-132(s0)
   10774:	00b42223          	sw	a1,4(s0)
   10778:	00c42423          	sw	a2,8(s0)
   1077c:	00d42623          	sw	a3,12(s0)
   10780:	00e42823          	sw	a4,16(s0)
   10784:	00f42a23          	sw	a5,20(s0)
   10788:	01042c23          	sw	a6,24(s0)
   1078c:	01142e23          	sw	a7,28(s0)
   10790:	f7c42503          	lw	a0,-132(s0)
   10794:	290000ef          	jal	10a24 <str_len>
   10798:	00050793          	mv	a5,a0
   1079c:	fef42023          	sw	a5,-32(s0)
   107a0:	00100793          	li	a5,1
   107a4:	fef42623          	sw	a5,-20(s0)
   107a8:	fc042e23          	sw	zero,-36(s0)
   107ac:	fe042423          	sw	zero,-24(s0)
   107b0:	2180006f          	j	109c8 <input+0x268>
   107b4:	fe842783          	lw	a5,-24(s0)
   107b8:	f7c42703          	lw	a4,-132(s0)
   107bc:	00f707b3          	add	a5,a4,a5
   107c0:	0007c703          	lbu	a4,0(a5)
   107c4:	02500793          	li	a5,37
   107c8:	1ef71a63          	bne	a4,a5,109bc <input+0x25c>
   107cc:	fe042223          	sw	zero,-28(s0)
   107d0:	fc042c23          	sw	zero,-40(s0)
   107d4:	fc042a23          	sw	zero,-44(s0)
   107d8:	fc0409a3          	sb	zero,-45(s0)
   107dc:	fd340793          	addi	a5,s0,-45
   107e0:	00078513          	mv	a0,a5
   107e4:	909ff0ef          	jal	100ec <vgetch>
   107e8:	fd344703          	lbu	a4,-45(s0)
   107ec:	00a00793          	li	a5,10
   107f0:	00f70863          	beq	a4,a5,10800 <input+0xa0>
   107f4:	fd344703          	lbu	a4,-45(s0)
   107f8:	02000793          	li	a5,32
   107fc:	04f71c63          	bne	a4,a5,10854 <input+0xf4>
   10800:	fd344783          	lbu	a5,-45(s0)
   10804:	00078593          	mv	a1,a5
   10808:	00001517          	auipc	a0,0x1
   1080c:	ed850513          	addi	a0,a0,-296 # 116e0 <_start+0xc>
   10810:	c8dff0ef          	jal	1049c <print>
   10814:	fe442783          	lw	a5,-28(s0)
   10818:	08078463          	beqz	a5,108a0 <input+0x140>
   1081c:	fe842783          	lw	a5,-24(s0)
   10820:	00178793          	addi	a5,a5,1
   10824:	f7c42703          	lw	a4,-132(s0)
   10828:	00f707b3          	add	a5,a4,a5
   1082c:	0007c783          	lbu	a5,0(a5)
   10830:	07300713          	li	a4,115
   10834:	0ae78a63          	beq	a5,a4,108e8 <input+0x188>
   10838:	07300713          	li	a4,115
   1083c:	16f74863          	blt	a4,a5,109ac <input+0x24c>
   10840:	06300713          	li	a4,99
   10844:	06e78663          	beq	a5,a4,108b0 <input+0x150>
   10848:	06400713          	li	a4,100
   1084c:	0ee78863          	beq	a5,a4,1093c <input+0x1dc>
   10850:	15c0006f          	j	109ac <input+0x24c>
   10854:	fd344703          	lbu	a4,-45(s0)
   10858:	01f00793          	li	a5,31
   1085c:	04e7f663          	bgeu	a5,a4,108a8 <input+0x148>
   10860:	fd344703          	lbu	a4,-45(s0)
   10864:	07e00793          	li	a5,126
   10868:	04e7e063          	bltu	a5,a4,108a8 <input+0x148>
   1086c:	fd344783          	lbu	a5,-45(s0)
   10870:	00078593          	mv	a1,a5
   10874:	00001517          	auipc	a0,0x1
   10878:	e6c50513          	addi	a0,a0,-404 # 116e0 <_start+0xc>
   1087c:	c21ff0ef          	jal	1049c <print>
   10880:	fe442783          	lw	a5,-28(s0)
   10884:	00178713          	addi	a4,a5,1
   10888:	fee42223          	sw	a4,-28(s0)
   1088c:	fd344703          	lbu	a4,-45(s0)
   10890:	ff078793          	addi	a5,a5,-16
   10894:	008787b3          	add	a5,a5,s0
   10898:	f8e78a23          	sb	a4,-108(a5)
   1089c:	f3dff06f          	j	107d8 <input+0x78>
   108a0:	00000013          	nop
   108a4:	f35ff06f          	j	107d8 <input+0x78>
   108a8:	00000013          	nop
   108ac:	f2dff06f          	j	107d8 <input+0x78>
   108b0:	fec42783          	lw	a5,-20(s0)
   108b4:	00279793          	slli	a5,a5,0x2
   108b8:	fcf42c23          	sw	a5,-40(s0)
   108bc:	fd842783          	lw	a5,-40(s0)
   108c0:	00f405b3          	add	a1,s0,a5
   108c4:	0005a783          	lw	a5,0(a1)
   108c8:	fcf42e23          	sw	a5,-36(s0)
   108cc:	fec42783          	lw	a5,-20(s0)
   108d0:	00178793          	addi	a5,a5,1
   108d4:	fef42623          	sw	a5,-20(s0)
   108d8:	fdc42783          	lw	a5,-36(s0)
   108dc:	f8444703          	lbu	a4,-124(s0)
   108e0:	00e78023          	sb	a4,0(a5)
   108e4:	0cc0006f          	j	109b0 <input+0x250>
   108e8:	fec42783          	lw	a5,-20(s0)
   108ec:	00279793          	slli	a5,a5,0x2
   108f0:	fcf42c23          	sw	a5,-40(s0)
   108f4:	fd842783          	lw	a5,-40(s0)
   108f8:	00f405b3          	add	a1,s0,a5
   108fc:	0005a783          	lw	a5,0(a1)
   10900:	fcf42e23          	sw	a5,-36(s0)
   10904:	fec42783          	lw	a5,-20(s0)
   10908:	00178793          	addi	a5,a5,1
   1090c:	fef42623          	sw	a5,-20(s0)
   10910:	fdc42703          	lw	a4,-36(s0)
   10914:	fe442783          	lw	a5,-28(s0)
   10918:	fff78793          	addi	a5,a5,-1
   1091c:	fef42223          	sw	a5,-28(s0)
   10920:	fe442683          	lw	a3,-28(s0)
   10924:	f8440793          	addi	a5,s0,-124
   10928:	00000613          	li	a2,0
   1092c:	00070593          	mv	a1,a4
   10930:	00078513          	mv	a0,a5
   10934:	280000ef          	jal	10bb4 <str_cpy_s>
   10938:	0780006f          	j	109b0 <input+0x250>
   1093c:	fec42783          	lw	a5,-20(s0)
   10940:	00279793          	slli	a5,a5,0x2
   10944:	fcf42c23          	sw	a5,-40(s0)
   10948:	fd842783          	lw	a5,-40(s0)
   1094c:	00f405b3          	add	a1,s0,a5
   10950:	0005a783          	lw	a5,0(a1)
   10954:	fcf42e23          	sw	a5,-36(s0)
   10958:	fec42783          	lw	a5,-20(s0)
   1095c:	00178793          	addi	a5,a5,1
   10960:	fef42623          	sw	a5,-20(s0)
   10964:	fe442783          	lw	a5,-28(s0)
   10968:	fff78793          	addi	a5,a5,-1
   1096c:	fef42223          	sw	a5,-28(s0)
   10970:	fe442683          	lw	a3,-28(s0)
   10974:	fc440713          	addi	a4,s0,-60
   10978:	f8440793          	addi	a5,s0,-124
   1097c:	00000613          	li	a2,0
   10980:	00070593          	mv	a1,a4
   10984:	00078513          	mv	a0,a5
   10988:	22c000ef          	jal	10bb4 <str_cpy_s>
   1098c:	fc440793          	addi	a5,s0,-60
   10990:	00078513          	mv	a0,a5
   10994:	4d4000ef          	jal	10e68 <atoi>
   10998:	fca42a23          	sw	a0,-44(s0)
   1099c:	fdc42783          	lw	a5,-36(s0)
   109a0:	fd442703          	lw	a4,-44(s0)
   109a4:	00e7a023          	sw	a4,0(a5)
   109a8:	0080006f          	j	109b0 <input+0x250>
   109ac:	00000013          	nop
   109b0:	fe842783          	lw	a5,-24(s0)
   109b4:	00178793          	addi	a5,a5,1
   109b8:	fef42423          	sw	a5,-24(s0)
   109bc:	fe842783          	lw	a5,-24(s0)
   109c0:	00178793          	addi	a5,a5,1
   109c4:	fef42423          	sw	a5,-24(s0)
   109c8:	fe842703          	lw	a4,-24(s0)
   109cc:	fe042783          	lw	a5,-32(s0)
   109d0:	def742e3          	blt	a4,a5,107b4 <input+0x54>
   109d4:	00000793          	li	a5,0
   109d8:	00078513          	mv	a0,a5
   109dc:	08c12083          	lw	ra,140(sp)
   109e0:	08812403          	lw	s0,136(sp)
   109e4:	0b010113          	addi	sp,sp,176
   109e8:	00008067          	ret

000109ec <shutdown>:
   109ec:	ff010113          	addi	sp,sp,-16
   109f0:	00112623          	sw	ra,12(sp)
   109f4:	00812423          	sw	s0,8(sp)
   109f8:	01010413          	addi	s0,sp,16
   109fc:	00001517          	auipc	a0,0x1
   10a00:	ce850513          	addi	a0,a0,-792 # 116e4 <_start+0x10>
   10a04:	a99ff0ef          	jal	1049c <print>
   10a08:	f6cff0ef          	jal	10174 <powoff>
   10a0c:	00000013          	nop
   10a10:	00078513          	mv	a0,a5
   10a14:	00c12083          	lw	ra,12(sp)
   10a18:	00812403          	lw	s0,8(sp)
   10a1c:	01010113          	addi	sp,sp,16
   10a20:	00008067          	ret

00010a24 <str_len>:
   10a24:	fd010113          	addi	sp,sp,-48
   10a28:	02112623          	sw	ra,44(sp)
   10a2c:	02812423          	sw	s0,40(sp)
   10a30:	03010413          	addi	s0,sp,48
   10a34:	fca42e23          	sw	a0,-36(s0)
   10a38:	fe042623          	sw	zero,-20(s0)
   10a3c:	0100006f          	j	10a4c <str_len+0x28>
   10a40:	fec42783          	lw	a5,-20(s0)
   10a44:	00178793          	addi	a5,a5,1
   10a48:	fef42623          	sw	a5,-20(s0)
   10a4c:	fdc42703          	lw	a4,-36(s0)
   10a50:	fec42783          	lw	a5,-20(s0)
   10a54:	00f707b3          	add	a5,a4,a5
   10a58:	0007c783          	lbu	a5,0(a5)
   10a5c:	fe0792e3          	bnez	a5,10a40 <str_len+0x1c>
   10a60:	fec42783          	lw	a5,-20(s0)
   10a64:	00078513          	mv	a0,a5
   10a68:	02c12083          	lw	ra,44(sp)
   10a6c:	02812403          	lw	s0,40(sp)
   10a70:	03010113          	addi	sp,sp,48
   10a74:	00008067          	ret

00010a78 <str_cmp>:
   10a78:	fd010113          	addi	sp,sp,-48
   10a7c:	02112623          	sw	ra,44(sp)
   10a80:	02812423          	sw	s0,40(sp)
   10a84:	02912223          	sw	s1,36(sp)
   10a88:	03010413          	addi	s0,sp,48
   10a8c:	fca42e23          	sw	a0,-36(s0)
   10a90:	fcb42c23          	sw	a1,-40(s0)
   10a94:	fe042623          	sw	zero,-20(s0)
   10a98:	fdc42503          	lw	a0,-36(s0)
   10a9c:	f89ff0ef          	jal	10a24 <str_len>
   10aa0:	00050493          	mv	s1,a0
   10aa4:	fd842503          	lw	a0,-40(s0)
   10aa8:	f7dff0ef          	jal	10a24 <str_len>
   10aac:	00050793          	mv	a5,a0
   10ab0:	04f48263          	beq	s1,a5,10af4 <str_cmp+0x7c>
   10ab4:	00000793          	li	a5,0
   10ab8:	0680006f          	j	10b20 <str_cmp+0xa8>
   10abc:	fdc42703          	lw	a4,-36(s0)
   10ac0:	fec42783          	lw	a5,-20(s0)
   10ac4:	00f707b3          	add	a5,a4,a5
   10ac8:	0007c703          	lbu	a4,0(a5)
   10acc:	fd842683          	lw	a3,-40(s0)
   10ad0:	fec42783          	lw	a5,-20(s0)
   10ad4:	00f687b3          	add	a5,a3,a5
   10ad8:	0007c783          	lbu	a5,0(a5)
   10adc:	00f70663          	beq	a4,a5,10ae8 <str_cmp+0x70>
   10ae0:	00000793          	li	a5,0
   10ae4:	03c0006f          	j	10b20 <str_cmp+0xa8>
   10ae8:	fec42783          	lw	a5,-20(s0)
   10aec:	00178793          	addi	a5,a5,1
   10af0:	fef42623          	sw	a5,-20(s0)
   10af4:	fdc42703          	lw	a4,-36(s0)
   10af8:	fec42783          	lw	a5,-20(s0)
   10afc:	00f707b3          	add	a5,a4,a5
   10b00:	0007c783          	lbu	a5,0(a5)
   10b04:	00078c63          	beqz	a5,10b1c <str_cmp+0xa4>
   10b08:	fd842703          	lw	a4,-40(s0)
   10b0c:	fec42783          	lw	a5,-20(s0)
   10b10:	00f707b3          	add	a5,a4,a5
   10b14:	0007c783          	lbu	a5,0(a5)
   10b18:	fa0792e3          	bnez	a5,10abc <str_cmp+0x44>
   10b1c:	00100793          	li	a5,1
   10b20:	00078513          	mv	a0,a5
   10b24:	02c12083          	lw	ra,44(sp)
   10b28:	02812403          	lw	s0,40(sp)
   10b2c:	02412483          	lw	s1,36(sp)
   10b30:	03010113          	addi	sp,sp,48
   10b34:	00008067          	ret

00010b38 <str_cpy>:
   10b38:	fd010113          	addi	sp,sp,-48
   10b3c:	02112623          	sw	ra,44(sp)
   10b40:	02812423          	sw	s0,40(sp)
   10b44:	03010413          	addi	s0,sp,48
   10b48:	fca42e23          	sw	a0,-36(s0)
   10b4c:	fcb42c23          	sw	a1,-40(s0)
   10b50:	fdc42503          	lw	a0,-36(s0)
   10b54:	ed1ff0ef          	jal	10a24 <str_len>
   10b58:	fea42423          	sw	a0,-24(s0)
   10b5c:	fe042623          	sw	zero,-20(s0)
   10b60:	0300006f          	j	10b90 <str_cpy+0x58>
   10b64:	fdc42703          	lw	a4,-36(s0)
   10b68:	fec42783          	lw	a5,-20(s0)
   10b6c:	00f70733          	add	a4,a4,a5
   10b70:	fd842683          	lw	a3,-40(s0)
   10b74:	fec42783          	lw	a5,-20(s0)
   10b78:	00f687b3          	add	a5,a3,a5
   10b7c:	00074703          	lbu	a4,0(a4)
   10b80:	00e78023          	sb	a4,0(a5)
   10b84:	fec42783          	lw	a5,-20(s0)
   10b88:	00178793          	addi	a5,a5,1
   10b8c:	fef42623          	sw	a5,-20(s0)
   10b90:	fec42703          	lw	a4,-20(s0)
   10b94:	fe842783          	lw	a5,-24(s0)
   10b98:	fcf766e3          	bltu	a4,a5,10b64 <str_cpy+0x2c>
   10b9c:	00000013          	nop
   10ba0:	00000013          	nop
   10ba4:	02c12083          	lw	ra,44(sp)
   10ba8:	02812403          	lw	s0,40(sp)
   10bac:	03010113          	addi	sp,sp,48
   10bb0:	00008067          	ret

00010bb4 <str_cpy_s>:
   10bb4:	fd010113          	addi	sp,sp,-48
   10bb8:	02112623          	sw	ra,44(sp)
   10bbc:	02812423          	sw	s0,40(sp)
   10bc0:	03010413          	addi	s0,sp,48
   10bc4:	fca42e23          	sw	a0,-36(s0)
   10bc8:	fcb42c23          	sw	a1,-40(s0)
   10bcc:	fcc42a23          	sw	a2,-44(s0)
   10bd0:	fcd42823          	sw	a3,-48(s0)
   10bd4:	fd442783          	lw	a5,-44(s0)
   10bd8:	fef42623          	sw	a5,-20(s0)
   10bdc:	0300006f          	j	10c0c <str_cpy_s+0x58>
   10be0:	fdc42703          	lw	a4,-36(s0)
   10be4:	fec42783          	lw	a5,-20(s0)
   10be8:	00f70733          	add	a4,a4,a5
   10bec:	fd842683          	lw	a3,-40(s0)
   10bf0:	fec42783          	lw	a5,-20(s0)
   10bf4:	00f687b3          	add	a5,a3,a5
   10bf8:	00074703          	lbu	a4,0(a4)
   10bfc:	00e78023          	sb	a4,0(a5)
   10c00:	fec42783          	lw	a5,-20(s0)
   10c04:	00178793          	addi	a5,a5,1
   10c08:	fef42623          	sw	a5,-20(s0)
   10c0c:	fec42703          	lw	a4,-20(s0)
   10c10:	fd042783          	lw	a5,-48(s0)
   10c14:	fce7f6e3          	bgeu	a5,a4,10be0 <str_cpy_s+0x2c>
   10c18:	00000013          	nop
   10c1c:	00000013          	nop
   10c20:	02c12083          	lw	ra,44(sp)
   10c24:	02812403          	lw	s0,40(sp)
   10c28:	03010113          	addi	sp,sp,48
   10c2c:	00008067          	ret

00010c30 <split>:
   10c30:	fd010113          	addi	sp,sp,-48
   10c34:	02112623          	sw	ra,44(sp)
   10c38:	02812423          	sw	s0,40(sp)
   10c3c:	03010413          	addi	s0,sp,48
   10c40:	fca42e23          	sw	a0,-36(s0)
   10c44:	00058793          	mv	a5,a1
   10c48:	fcc42a23          	sw	a2,-44(s0)
   10c4c:	fcf40da3          	sb	a5,-37(s0)
   10c50:	fdc42503          	lw	a0,-36(s0)
   10c54:	dd1ff0ef          	jal	10a24 <str_len>
   10c58:	fea42223          	sw	a0,-28(s0)
   10c5c:	fe042623          	sw	zero,-20(s0)
   10c60:	fe042423          	sw	zero,-24(s0)
   10c64:	0480006f          	j	10cac <split+0x7c>
   10c68:	fdc42703          	lw	a4,-36(s0)
   10c6c:	fe842783          	lw	a5,-24(s0)
   10c70:	00f707b3          	add	a5,a4,a5
   10c74:	0007c783          	lbu	a5,0(a5)
   10c78:	fdb44703          	lbu	a4,-37(s0)
   10c7c:	02f71263          	bne	a4,a5,10ca0 <split+0x70>
   10c80:	fec42783          	lw	a5,-20(s0)
   10c84:	00178713          	addi	a4,a5,1
   10c88:	fee42623          	sw	a4,-20(s0)
   10c8c:	00279793          	slli	a5,a5,0x2
   10c90:	fd442703          	lw	a4,-44(s0)
   10c94:	00f707b3          	add	a5,a4,a5
   10c98:	fe842703          	lw	a4,-24(s0)
   10c9c:	00e7a023          	sw	a4,0(a5)
   10ca0:	fe842783          	lw	a5,-24(s0)
   10ca4:	00178793          	addi	a5,a5,1
   10ca8:	fef42423          	sw	a5,-24(s0)
   10cac:	fe842703          	lw	a4,-24(s0)
   10cb0:	fe442783          	lw	a5,-28(s0)
   10cb4:	faf76ae3          	bltu	a4,a5,10c68 <split+0x38>
   10cb8:	00000013          	nop
   10cbc:	00000013          	nop
   10cc0:	02c12083          	lw	ra,44(sp)
   10cc4:	02812403          	lw	s0,40(sp)
   10cc8:	03010113          	addi	sp,sp,48
   10ccc:	00008067          	ret

00010cd0 <itoa>:
   10cd0:	fc010113          	addi	sp,sp,-64
   10cd4:	02112e23          	sw	ra,60(sp)
   10cd8:	02812c23          	sw	s0,56(sp)
   10cdc:	04010413          	addi	s0,sp,64
   10ce0:	fca42623          	sw	a0,-52(s0)
   10ce4:	fcb42423          	sw	a1,-56(s0)
   10ce8:	fcc42783          	lw	a5,-52(s0)
   10cec:	02079063          	bnez	a5,10d0c <itoa+0x3c>
   10cf0:	fc842783          	lw	a5,-56(s0)
   10cf4:	03000713          	li	a4,48
   10cf8:	00e78023          	sb	a4,0(a5)
   10cfc:	fc842783          	lw	a5,-56(s0)
   10d00:	00178793          	addi	a5,a5,1
   10d04:	00078023          	sb	zero,0(a5)
   10d08:	1500006f          	j	10e58 <itoa+0x188>
   10d0c:	fcc42783          	lw	a5,-52(s0)
   10d10:	01f7d793          	srli	a5,a5,0x1f
   10d14:	0ff7f793          	zext.b	a5,a5
   10d18:	fef42023          	sw	a5,-32(s0)
   10d1c:	fe042783          	lw	a5,-32(s0)
   10d20:	00078863          	beqz	a5,10d30 <itoa+0x60>
   10d24:	fcc42783          	lw	a5,-52(s0)
   10d28:	40f007b3          	neg	a5,a5
   10d2c:	0080006f          	j	10d34 <itoa+0x64>
   10d30:	fcc42783          	lw	a5,-52(s0)
   10d34:	fef42623          	sw	a5,-20(s0)
   10d38:	fe042423          	sw	zero,-24(s0)
   10d3c:	0500006f          	j	10d8c <itoa+0xbc>
   10d40:	00a00593          	li	a1,10
   10d44:	fec42503          	lw	a0,-20(s0)
   10d48:	310000ef          	jal	11058 <mod>
   10d4c:	fca42c23          	sw	a0,-40(s0)
   10d50:	fd842783          	lw	a5,-40(s0)
   10d54:	0ff7f713          	zext.b	a4,a5
   10d58:	fe842783          	lw	a5,-24(s0)
   10d5c:	00178693          	addi	a3,a5,1
   10d60:	fed42423          	sw	a3,-24(s0)
   10d64:	00078693          	mv	a3,a5
   10d68:	fc842783          	lw	a5,-56(s0)
   10d6c:	00d787b3          	add	a5,a5,a3
   10d70:	03070713          	addi	a4,a4,48
   10d74:	0ff77713          	zext.b	a4,a4
   10d78:	00e78023          	sb	a4,0(a5)
   10d7c:	00a00593          	li	a1,10
   10d80:	fec42503          	lw	a0,-20(s0)
   10d84:	44c000ef          	jal	111d0 <div>
   10d88:	fea42623          	sw	a0,-20(s0)
   10d8c:	fec42783          	lw	a5,-20(s0)
   10d90:	fa0798e3          	bnez	a5,10d40 <itoa+0x70>
   10d94:	fe042783          	lw	a5,-32(s0)
   10d98:	02078263          	beqz	a5,10dbc <itoa+0xec>
   10d9c:	fe842783          	lw	a5,-24(s0)
   10da0:	00178713          	addi	a4,a5,1
   10da4:	fee42423          	sw	a4,-24(s0)
   10da8:	00078713          	mv	a4,a5
   10dac:	fc842783          	lw	a5,-56(s0)
   10db0:	00e787b3          	add	a5,a5,a4
   10db4:	02d00713          	li	a4,45
   10db8:	00e78023          	sb	a4,0(a5)
   10dbc:	fe842783          	lw	a5,-24(s0)
   10dc0:	fc842703          	lw	a4,-56(s0)
   10dc4:	00f707b3          	add	a5,a4,a5
   10dc8:	00078023          	sb	zero,0(a5)
   10dcc:	fe042223          	sw	zero,-28(s0)
   10dd0:	0700006f          	j	10e40 <itoa+0x170>
   10dd4:	fe442783          	lw	a5,-28(s0)
   10dd8:	fc842703          	lw	a4,-56(s0)
   10ddc:	00f707b3          	add	a5,a4,a5
   10de0:	0007c783          	lbu	a5,0(a5)
   10de4:	fcf40fa3          	sb	a5,-33(s0)
   10de8:	fe842703          	lw	a4,-24(s0)
   10dec:	fe442783          	lw	a5,-28(s0)
   10df0:	40f707b3          	sub	a5,a4,a5
   10df4:	fff78793          	addi	a5,a5,-1
   10df8:	fc842703          	lw	a4,-56(s0)
   10dfc:	00f70733          	add	a4,a4,a5
   10e00:	fe442783          	lw	a5,-28(s0)
   10e04:	fc842683          	lw	a3,-56(s0)
   10e08:	00f687b3          	add	a5,a3,a5
   10e0c:	00074703          	lbu	a4,0(a4)
   10e10:	00e78023          	sb	a4,0(a5)
   10e14:	fe842703          	lw	a4,-24(s0)
   10e18:	fe442783          	lw	a5,-28(s0)
   10e1c:	40f707b3          	sub	a5,a4,a5
   10e20:	fff78793          	addi	a5,a5,-1
   10e24:	fc842703          	lw	a4,-56(s0)
   10e28:	00f707b3          	add	a5,a4,a5
   10e2c:	fdf44703          	lbu	a4,-33(s0)
   10e30:	00e78023          	sb	a4,0(a5)
   10e34:	fe442783          	lw	a5,-28(s0)
   10e38:	00178793          	addi	a5,a5,1
   10e3c:	fef42223          	sw	a5,-28(s0)
   10e40:	00200593          	li	a1,2
   10e44:	fe842503          	lw	a0,-24(s0)
   10e48:	388000ef          	jal	111d0 <div>
   10e4c:	00050713          	mv	a4,a0
   10e50:	fe442783          	lw	a5,-28(s0)
   10e54:	f8e7c0e3          	blt	a5,a4,10dd4 <itoa+0x104>
   10e58:	03c12083          	lw	ra,60(sp)
   10e5c:	03812403          	lw	s0,56(sp)
   10e60:	04010113          	addi	sp,sp,64
   10e64:	00008067          	ret

00010e68 <atoi>:
   10e68:	fd010113          	addi	sp,sp,-48
   10e6c:	02112623          	sw	ra,44(sp)
   10e70:	02812423          	sw	s0,40(sp)
   10e74:	03010413          	addi	s0,sp,48
   10e78:	fca42e23          	sw	a0,-36(s0)
   10e7c:	fe042623          	sw	zero,-20(s0)
   10e80:	00100793          	li	a5,1
   10e84:	fef42423          	sw	a5,-24(s0)
   10e88:	0100006f          	j	10e98 <atoi+0x30>
   10e8c:	fdc42783          	lw	a5,-36(s0)
   10e90:	00178793          	addi	a5,a5,1
   10e94:	fcf42e23          	sw	a5,-36(s0)
   10e98:	fdc42783          	lw	a5,-36(s0)
   10e9c:	0007c703          	lbu	a4,0(a5)
   10ea0:	02000793          	li	a5,32
   10ea4:	fef704e3          	beq	a4,a5,10e8c <atoi+0x24>
   10ea8:	fdc42783          	lw	a5,-36(s0)
   10eac:	0007c703          	lbu	a4,0(a5)
   10eb0:	02b00793          	li	a5,43
   10eb4:	00f70a63          	beq	a4,a5,10ec8 <atoi+0x60>
   10eb8:	fdc42783          	lw	a5,-36(s0)
   10ebc:	0007c703          	lbu	a4,0(a5)
   10ec0:	02d00793          	li	a5,45
   10ec4:	06f71263          	bne	a4,a5,10f28 <atoi+0xc0>
   10ec8:	fdc42783          	lw	a5,-36(s0)
   10ecc:	0007c703          	lbu	a4,0(a5)
   10ed0:	02d00793          	li	a5,45
   10ed4:	00f71663          	bne	a4,a5,10ee0 <atoi+0x78>
   10ed8:	fff00793          	li	a5,-1
   10edc:	0080006f          	j	10ee4 <atoi+0x7c>
   10ee0:	00100793          	li	a5,1
   10ee4:	fef42423          	sw	a5,-24(s0)
   10ee8:	fdc42783          	lw	a5,-36(s0)
   10eec:	00178793          	addi	a5,a5,1
   10ef0:	fcf42e23          	sw	a5,-36(s0)
   10ef4:	0340006f          	j	10f28 <atoi+0xc0>
   10ef8:	00a00593          	li	a1,10
   10efc:	fec42503          	lw	a0,-20(s0)
   10f00:	3ec000ef          	jal	112ec <mul>
   10f04:	00050713          	mv	a4,a0
   10f08:	fdc42783          	lw	a5,-36(s0)
   10f0c:	0007c783          	lbu	a5,0(a5)
   10f10:	fd078793          	addi	a5,a5,-48
   10f14:	00f707b3          	add	a5,a4,a5
   10f18:	fef42623          	sw	a5,-20(s0)
   10f1c:	fdc42783          	lw	a5,-36(s0)
   10f20:	00178793          	addi	a5,a5,1
   10f24:	fcf42e23          	sw	a5,-36(s0)
   10f28:	fdc42783          	lw	a5,-36(s0)
   10f2c:	0007c703          	lbu	a4,0(a5)
   10f30:	02f00793          	li	a5,47
   10f34:	00e7fa63          	bgeu	a5,a4,10f48 <atoi+0xe0>
   10f38:	fdc42783          	lw	a5,-36(s0)
   10f3c:	0007c703          	lbu	a4,0(a5)
   10f40:	03900793          	li	a5,57
   10f44:	fae7fae3          	bgeu	a5,a4,10ef8 <atoi+0x90>
   10f48:	fec42703          	lw	a4,-20(s0)
   10f4c:	800007b7          	lui	a5,0x80000
   10f50:	00f71663          	bne	a4,a5,10f5c <atoi+0xf4>
   10f54:	fec42783          	lw	a5,-20(s0)
   10f58:	0140006f          	j	10f6c <atoi+0x104>
   10f5c:	fec42583          	lw	a1,-20(s0)
   10f60:	fe842503          	lw	a0,-24(s0)
   10f64:	388000ef          	jal	112ec <mul>
   10f68:	00050793          	mv	a5,a0
   10f6c:	00078513          	mv	a0,a5
   10f70:	02c12083          	lw	ra,44(sp)
   10f74:	02812403          	lw	s0,40(sp)
   10f78:	03010113          	addi	sp,sp,48
   10f7c:	00008067          	ret

00010f80 <uint32_to_char>:
   10f80:	fe010113          	addi	sp,sp,-32
   10f84:	00112e23          	sw	ra,28(sp)
   10f88:	00812c23          	sw	s0,24(sp)
   10f8c:	02010413          	addi	s0,sp,32
   10f90:	fea42623          	sw	a0,-20(s0)
   10f94:	feb42423          	sw	a1,-24(s0)
   10f98:	fec42783          	lw	a5,-20(s0)
   10f9c:	0ff7f713          	zext.b	a4,a5
   10fa0:	fe842783          	lw	a5,-24(s0)
   10fa4:	00e78023          	sb	a4,0(a5) # 80000000 <__BSS_END__+0x7feed798>
   10fa8:	fec42783          	lw	a5,-20(s0)
   10fac:	0087d713          	srli	a4,a5,0x8
   10fb0:	fe842783          	lw	a5,-24(s0)
   10fb4:	00178793          	addi	a5,a5,1
   10fb8:	0ff77713          	zext.b	a4,a4
   10fbc:	00e78023          	sb	a4,0(a5)
   10fc0:	fec42783          	lw	a5,-20(s0)
   10fc4:	0107d713          	srli	a4,a5,0x10
   10fc8:	fe842783          	lw	a5,-24(s0)
   10fcc:	00278793          	addi	a5,a5,2
   10fd0:	0ff77713          	zext.b	a4,a4
   10fd4:	00e78023          	sb	a4,0(a5)
   10fd8:	fec42783          	lw	a5,-20(s0)
   10fdc:	0187d713          	srli	a4,a5,0x18
   10fe0:	fe842783          	lw	a5,-24(s0)
   10fe4:	00378793          	addi	a5,a5,3
   10fe8:	0ff77713          	zext.b	a4,a4
   10fec:	00e78023          	sb	a4,0(a5)
   10ff0:	00000013          	nop
   10ff4:	01c12083          	lw	ra,28(sp)
   10ff8:	01812403          	lw	s0,24(sp)
   10ffc:	02010113          	addi	sp,sp,32
   11000:	00008067          	ret

00011004 <abs>:
   11004:	fe010113          	addi	sp,sp,-32
   11008:	00112e23          	sw	ra,28(sp)
   1100c:	00812c23          	sw	s0,24(sp)
   11010:	02010413          	addi	s0,sp,32
   11014:	fea42623          	sw	a0,-20(s0)
   11018:	fec42703          	lw	a4,-20(s0)
   1101c:	800007b7          	lui	a5,0x80000
   11020:	fff78793          	addi	a5,a5,-1 # 7fffffff <__BSS_END__+0x7feed797>
   11024:	00f71663          	bne	a4,a5,11030 <abs+0x2c>
   11028:	fec42783          	lw	a5,-20(s0)
   1102c:	0180006f          	j	11044 <abs+0x40>
   11030:	fec42783          	lw	a5,-20(s0)
   11034:	41f7d713          	srai	a4,a5,0x1f
   11038:	fec42783          	lw	a5,-20(s0)
   1103c:	00f747b3          	xor	a5,a4,a5
   11040:	40e787b3          	sub	a5,a5,a4
   11044:	00078513          	mv	a0,a5
   11048:	01c12083          	lw	ra,28(sp)
   1104c:	01812403          	lw	s0,24(sp)
   11050:	02010113          	addi	sp,sp,32
   11054:	00008067          	ret

00011058 <mod>:
   11058:	fc010113          	addi	sp,sp,-64
   1105c:	02112e23          	sw	ra,60(sp)
   11060:	02812c23          	sw	s0,56(sp)
   11064:	04010413          	addi	s0,sp,64
   11068:	fca42623          	sw	a0,-52(s0)
   1106c:	fcb42423          	sw	a1,-56(s0)
   11070:	fcc42783          	lw	a5,-52(s0)
   11074:	00078663          	beqz	a5,11080 <mod+0x28>
   11078:	fc842783          	lw	a5,-56(s0)
   1107c:	00079663          	bnez	a5,11088 <mod+0x30>
   11080:	00000793          	li	a5,0
   11084:	1380006f          	j	111bc <mod+0x164>
   11088:	fc842703          	lw	a4,-56(s0)
   1108c:	00100793          	li	a5,1
   11090:	00f71663          	bne	a4,a5,1109c <mod+0x44>
   11094:	00000793          	li	a5,0
   11098:	1240006f          	j	111bc <mod+0x164>
   1109c:	fcc42783          	lw	a5,-52(s0)
   110a0:	fef42623          	sw	a5,-20(s0)
   110a4:	fc842783          	lw	a5,-56(s0)
   110a8:	fcf42a23          	sw	a5,-44(s0)
   110ac:	fe042423          	sw	zero,-24(s0)
   110b0:	fcc42783          	lw	a5,-52(s0)
   110b4:	0ef05a63          	blez	a5,111a8 <mod+0x150>
   110b8:	0700006f          	j	11128 <mod+0xd0>
   110bc:	fd442783          	lw	a5,-44(s0)
   110c0:	fef42223          	sw	a5,-28(s0)
   110c4:	00100793          	li	a5,1
   110c8:	fef42023          	sw	a5,-32(s0)
   110cc:	01c0006f          	j	110e8 <mod+0x90>
   110d0:	fe442783          	lw	a5,-28(s0)
   110d4:	00179793          	slli	a5,a5,0x1
   110d8:	fef42223          	sw	a5,-28(s0)
   110dc:	fe042783          	lw	a5,-32(s0)
   110e0:	00179793          	slli	a5,a5,0x1
   110e4:	fef42023          	sw	a5,-32(s0)
   110e8:	fe442783          	lw	a5,-28(s0)
   110ec:	00179793          	slli	a5,a5,0x1
   110f0:	fec42703          	lw	a4,-20(s0)
   110f4:	00f74a63          	blt	a4,a5,11108 <mod+0xb0>
   110f8:	fe442783          	lw	a5,-28(s0)
   110fc:	00179713          	slli	a4,a5,0x1
   11100:	400007b7          	lui	a5,0x40000
   11104:	fce7d6e3          	bge	a5,a4,110d0 <mod+0x78>
   11108:	fec42703          	lw	a4,-20(s0)
   1110c:	fe442783          	lw	a5,-28(s0)
   11110:	40f707b3          	sub	a5,a4,a5
   11114:	fef42623          	sw	a5,-20(s0)
   11118:	fe842703          	lw	a4,-24(s0)
   1111c:	fe042783          	lw	a5,-32(s0)
   11120:	00f707b3          	add	a5,a4,a5
   11124:	fef42423          	sw	a5,-24(s0)
   11128:	fec42703          	lw	a4,-20(s0)
   1112c:	fd442783          	lw	a5,-44(s0)
   11130:	f8f756e3          	bge	a4,a5,110bc <mod+0x64>
   11134:	0840006f          	j	111b8 <mod+0x160>
   11138:	fd442783          	lw	a5,-44(s0)
   1113c:	40f007b3          	neg	a5,a5
   11140:	fcf42e23          	sw	a5,-36(s0)
   11144:	00100793          	li	a5,1
   11148:	fcf42c23          	sw	a5,-40(s0)
   1114c:	01c0006f          	j	11168 <mod+0x110>
   11150:	fdc42783          	lw	a5,-36(s0)
   11154:	00179793          	slli	a5,a5,0x1
   11158:	fcf42e23          	sw	a5,-36(s0)
   1115c:	fd842783          	lw	a5,-40(s0)
   11160:	00179793          	slli	a5,a5,0x1
   11164:	fcf42c23          	sw	a5,-40(s0)
   11168:	fdc42783          	lw	a5,-36(s0)
   1116c:	00179793          	slli	a5,a5,0x1
   11170:	fec42703          	lw	a4,-20(s0)
   11174:	00e7ca63          	blt	a5,a4,11188 <mod+0x130>
   11178:	fdc42783          	lw	a5,-36(s0)
   1117c:	00179713          	slli	a4,a5,0x1
   11180:	c00007b7          	lui	a5,0xc0000
   11184:	fcf756e3          	bge	a4,a5,11150 <mod+0xf8>
   11188:	fec42703          	lw	a4,-20(s0)
   1118c:	fdc42783          	lw	a5,-36(s0)
   11190:	40f707b3          	sub	a5,a4,a5
   11194:	fef42623          	sw	a5,-20(s0)
   11198:	fe842703          	lw	a4,-24(s0)
   1119c:	fd842783          	lw	a5,-40(s0)
   111a0:	40f707b3          	sub	a5,a4,a5
   111a4:	fef42423          	sw	a5,-24(s0)
   111a8:	fd442783          	lw	a5,-44(s0)
   111ac:	40f007b3          	neg	a5,a5
   111b0:	fec42703          	lw	a4,-20(s0)
   111b4:	f8e7d2e3          	bge	a5,a4,11138 <mod+0xe0>
   111b8:	fec42783          	lw	a5,-20(s0)
   111bc:	00078513          	mv	a0,a5
   111c0:	03c12083          	lw	ra,60(sp)
   111c4:	03812403          	lw	s0,56(sp)
   111c8:	04010113          	addi	sp,sp,64
   111cc:	00008067          	ret

000111d0 <div>:
   111d0:	fd010113          	addi	sp,sp,-48
   111d4:	02112623          	sw	ra,44(sp)
   111d8:	02812423          	sw	s0,40(sp)
   111dc:	03010413          	addi	s0,sp,48
   111e0:	fca42e23          	sw	a0,-36(s0)
   111e4:	fcb42c23          	sw	a1,-40(s0)
   111e8:	fdc42783          	lw	a5,-36(s0)
   111ec:	00079663          	bnez	a5,111f8 <div+0x28>
   111f0:	00000793          	li	a5,0
   111f4:	0e40006f          	j	112d8 <div+0x108>
   111f8:	fdc42703          	lw	a4,-36(s0)
   111fc:	fd842783          	lw	a5,-40(s0)
   11200:	00f747b3          	xor	a5,a4,a5
   11204:	01f7d793          	srli	a5,a5,0x1f
   11208:	0ff7f793          	zext.b	a5,a5
   1120c:	fef42023          	sw	a5,-32(s0)
   11210:	fe042623          	sw	zero,-20(s0)
   11214:	fdc42783          	lw	a5,-36(s0)
   11218:	41f7d793          	srai	a5,a5,0x1f
   1121c:	fdc42703          	lw	a4,-36(s0)
   11220:	00f74733          	xor	a4,a4,a5
   11224:	40f707b3          	sub	a5,a4,a5
   11228:	fcf42e23          	sw	a5,-36(s0)
   1122c:	fd842783          	lw	a5,-40(s0)
   11230:	41f7d793          	srai	a5,a5,0x1f
   11234:	fd842703          	lw	a4,-40(s0)
   11238:	00f74733          	xor	a4,a4,a5
   1123c:	40f707b3          	sub	a5,a4,a5
   11240:	fcf42c23          	sw	a5,-40(s0)
   11244:	0700006f          	j	112b4 <div+0xe4>
   11248:	fd842783          	lw	a5,-40(s0)
   1124c:	fef42423          	sw	a5,-24(s0)
   11250:	00100793          	li	a5,1
   11254:	fef42223          	sw	a5,-28(s0)
   11258:	01c0006f          	j	11274 <div+0xa4>
   1125c:	fe842783          	lw	a5,-24(s0)
   11260:	00179793          	slli	a5,a5,0x1
   11264:	fef42423          	sw	a5,-24(s0)
   11268:	fe442783          	lw	a5,-28(s0)
   1126c:	00179793          	slli	a5,a5,0x1
   11270:	fef42223          	sw	a5,-28(s0)
   11274:	fe842783          	lw	a5,-24(s0)
   11278:	00179793          	slli	a5,a5,0x1
   1127c:	fdc42703          	lw	a4,-36(s0)
   11280:	00f74a63          	blt	a4,a5,11294 <div+0xc4>
   11284:	fe842783          	lw	a5,-24(s0)
   11288:	00179713          	slli	a4,a5,0x1
   1128c:	400007b7          	lui	a5,0x40000
   11290:	fce7d6e3          	bge	a5,a4,1125c <div+0x8c>
   11294:	fdc42703          	lw	a4,-36(s0)
   11298:	fe842783          	lw	a5,-24(s0)
   1129c:	40f707b3          	sub	a5,a4,a5
   112a0:	fcf42e23          	sw	a5,-36(s0)
   112a4:	fec42703          	lw	a4,-20(s0)
   112a8:	fe442783          	lw	a5,-28(s0)
   112ac:	00f707b3          	add	a5,a4,a5
   112b0:	fef42623          	sw	a5,-20(s0)
   112b4:	fdc42703          	lw	a4,-36(s0)
   112b8:	fd842783          	lw	a5,-40(s0)
   112bc:	f8f756e3          	bge	a4,a5,11248 <div+0x78>
   112c0:	fe042783          	lw	a5,-32(s0)
   112c4:	00078863          	beqz	a5,112d4 <div+0x104>
   112c8:	fec42783          	lw	a5,-20(s0)
   112cc:	40f007b3          	neg	a5,a5
   112d0:	fef42623          	sw	a5,-20(s0)
   112d4:	fec42783          	lw	a5,-20(s0)
   112d8:	00078513          	mv	a0,a5
   112dc:	02c12083          	lw	ra,44(sp)
   112e0:	02812403          	lw	s0,40(sp)
   112e4:	03010113          	addi	sp,sp,48
   112e8:	00008067          	ret

000112ec <mul>:
   112ec:	fd010113          	addi	sp,sp,-48
   112f0:	02112623          	sw	ra,44(sp)
   112f4:	02812423          	sw	s0,40(sp)
   112f8:	03010413          	addi	s0,sp,48
   112fc:	fca42e23          	sw	a0,-36(s0)
   11300:	fcb42c23          	sw	a1,-40(s0)
   11304:	fe042623          	sw	zero,-20(s0)
   11308:	00100793          	li	a5,1
   1130c:	fef42423          	sw	a5,-24(s0)
   11310:	fdc42783          	lw	a5,-36(s0)
   11314:	0007de63          	bgez	a5,11330 <mul+0x44>
   11318:	fe842783          	lw	a5,-24(s0)
   1131c:	40f007b3          	neg	a5,a5
   11320:	fef42423          	sw	a5,-24(s0)
   11324:	fdc42783          	lw	a5,-36(s0)
   11328:	40f007b3          	neg	a5,a5
   1132c:	fcf42e23          	sw	a5,-36(s0)
   11330:	fd842783          	lw	a5,-40(s0)
   11334:	0407da63          	bgez	a5,11388 <mul+0x9c>
   11338:	fe842783          	lw	a5,-24(s0)
   1133c:	40f007b3          	neg	a5,a5
   11340:	fef42423          	sw	a5,-24(s0)
   11344:	fd842783          	lw	a5,-40(s0)
   11348:	40f007b3          	neg	a5,a5
   1134c:	fcf42c23          	sw	a5,-40(s0)
   11350:	0380006f          	j	11388 <mul+0x9c>
   11354:	fd842783          	lw	a5,-40(s0)
   11358:	0017f793          	andi	a5,a5,1
   1135c:	00078a63          	beqz	a5,11370 <mul+0x84>
   11360:	fec42703          	lw	a4,-20(s0)
   11364:	fdc42783          	lw	a5,-36(s0)
   11368:	00f707b3          	add	a5,a4,a5
   1136c:	fef42623          	sw	a5,-20(s0)
   11370:	fdc42783          	lw	a5,-36(s0)
   11374:	00179793          	slli	a5,a5,0x1
   11378:	fcf42e23          	sw	a5,-36(s0)
   1137c:	fd842783          	lw	a5,-40(s0)
   11380:	4017d793          	srai	a5,a5,0x1
   11384:	fcf42c23          	sw	a5,-40(s0)
   11388:	fd842783          	lw	a5,-40(s0)
   1138c:	fcf044e3          	bgtz	a5,11354 <mul+0x68>
   11390:	fe842703          	lw	a4,-24(s0)
   11394:	00100793          	li	a5,1
   11398:	00f70863          	beq	a4,a5,113a8 <mul+0xbc>
   1139c:	fec42783          	lw	a5,-20(s0)
   113a0:	40f007b3          	neg	a5,a5
   113a4:	0080006f          	j	113ac <mul+0xc0>
   113a8:	fec42783          	lw	a5,-20(s0)
   113ac:	00078513          	mv	a0,a5
   113b0:	02c12083          	lw	ra,44(sp)
   113b4:	02812403          	lw	s0,40(sp)
   113b8:	03010113          	addi	sp,sp,48
   113bc:	00008067          	ret

000113c0 <main>:
   113c0:	b7010113          	addi	sp,sp,-1168
   113c4:	48112623          	sw	ra,1164(sp)
   113c8:	48812423          	sw	s0,1160(sp)
   113cc:	48912223          	sw	s1,1156(sp)
   113d0:	49010413          	addi	s0,sp,1168
   113d4:	b6a42e23          	sw	a0,-1156(s0)
   113d8:	b6b42c23          	sw	a1,-1160(s0)
   113dc:	b7842783          	lw	a5,-1160(s0)
   113e0:	00478793          	addi	a5,a5,4 # 40000004 <__BSS_END__+0x3feed79c>
   113e4:	0007a783          	lw	a5,0(a5)
   113e8:	00078593          	mv	a1,a5
   113ec:	00000517          	auipc	a0,0x0
   113f0:	31050513          	addi	a0,a0,784 # 116fc <_start+0x28>
   113f4:	8a8ff0ef          	jal	1049c <print>
   113f8:	00000517          	auipc	a0,0x0
   113fc:	30850513          	addi	a0,a0,776 # 11700 <_start+0x2c>
   11400:	89cff0ef          	jal	1049c <print>
   11404:	fdc40793          	addi	a5,s0,-36
   11408:	00078593          	mv	a1,a5
   1140c:	00000517          	auipc	a0,0x0
   11410:	32c50513          	addi	a0,a0,812 # 11738 <_start+0x64>
   11414:	b4cff0ef          	jal	10760 <input>
   11418:	fdc40793          	addi	a5,s0,-36
   1141c:	00078593          	mv	a1,a5
   11420:	00000517          	auipc	a0,0x0
   11424:	31c50513          	addi	a0,a0,796 # 1173c <_start+0x68>
   11428:	e50ff0ef          	jal	10a78 <str_cmp>
   1142c:	00050793          	mv	a5,a0
   11430:	00078a63          	beqz	a5,11444 <main+0x84>
   11434:	00000517          	auipc	a0,0x0
   11438:	30c50513          	addi	a0,a0,780 # 11740 <_start+0x6c>
   1143c:	860ff0ef          	jal	1049c <print>
   11440:	03c0006f          	j	1147c <main+0xbc>
   11444:	fdc40793          	addi	a5,s0,-36
   11448:	00078593          	mv	a1,a5
   1144c:	00000517          	auipc	a0,0x0
   11450:	2fc50513          	addi	a0,a0,764 # 11748 <_start+0x74>
   11454:	e24ff0ef          	jal	10a78 <str_cmp>
   11458:	00050793          	mv	a5,a0
   1145c:	00078a63          	beqz	a5,11470 <main+0xb0>
   11460:	00000517          	auipc	a0,0x0
   11464:	2ec50513          	addi	a0,a0,748 # 1174c <_start+0x78>
   11468:	834ff0ef          	jal	1049c <print>
   1146c:	0100006f          	j	1147c <main+0xbc>
   11470:	00000517          	auipc	a0,0x0
   11474:	2f450513          	addi	a0,a0,756 # 11764 <_start+0x90>
   11478:	824ff0ef          	jal	1049c <print>
   1147c:	00000517          	auipc	a0,0x0
   11480:	2f850513          	addi	a0,a0,760 # 11774 <_start+0xa0>
   11484:	818ff0ef          	jal	1049c <print>
   11488:	fc042c23          	sw	zero,-40(s0)
   1148c:	fd840793          	addi	a5,s0,-40
   11490:	00078513          	mv	a0,a5
   11494:	c9dfe0ef          	jal	10130 <kbhit>
   11498:	fd842783          	lw	a5,-40(s0)
   1149c:	fe0788e3          	beqz	a5,1148c <main+0xcc>
   114a0:	fd740793          	addi	a5,s0,-41
   114a4:	00078513          	mv	a0,a5
   114a8:	c45fe0ef          	jal	100ec <vgetch>
   114ac:	fd744703          	lbu	a4,-41(s0)
   114b0:	07100793          	li	a5,113
   114b4:	00f70463          	beq	a4,a5,114bc <main+0xfc>
   114b8:	fd5ff06f          	j	1148c <main+0xcc>
   114bc:	00000013          	nop
   114c0:	00000517          	auipc	a0,0x0
   114c4:	2d850513          	addi	a0,a0,728 # 11798 <_start+0xc4>
   114c8:	fd5fe0ef          	jal	1049c <print>
   114cc:	fc042823          	sw	zero,-48(s0)
   114d0:	fc042623          	sw	zero,-52(s0)
   114d4:	fcc40713          	addi	a4,s0,-52
   114d8:	fd040793          	addi	a5,s0,-48
   114dc:	00070693          	mv	a3,a4
   114e0:	00078613          	mv	a2,a5
   114e4:	06600593          	li	a1,102
   114e8:	00000517          	auipc	a0,0x0
   114ec:	2c050513          	addi	a0,a0,704 # 117a8 <_start+0xd4>
   114f0:	dfdfe0ef          	jal	102ec <create>
   114f4:	00000517          	auipc	a0,0x0
   114f8:	2d050513          	addi	a0,a0,720 # 117c4 <_start+0xf0>
   114fc:	fa1fe0ef          	jal	1049c <print>
   11500:	fcc40713          	addi	a4,s0,-52
   11504:	fd040793          	addi	a5,s0,-48
   11508:	00070613          	mv	a2,a4
   1150c:	00078593          	mv	a1,a5
   11510:	00000517          	auipc	a0,0x0
   11514:	29850513          	addi	a0,a0,664 # 117a8 <_start+0xd4>
   11518:	e55fe0ef          	jal	1036c <open>
   1151c:	00000517          	auipc	a0,0x0
   11520:	2b850513          	addi	a0,a0,696 # 117d4 <_start+0x100>
   11524:	f79fe0ef          	jal	1049c <print>
   11528:	6c6c67b7          	lui	a5,0x6c6c6
   1152c:	56878793          	addi	a5,a5,1384 # 6c6c6568 <__BSS_END__+0x6c5b3d00>
   11530:	fcf42023          	sw	a5,-64(s0)
   11534:	6f7727b7          	lui	a5,0x6f772
   11538:	06f78793          	addi	a5,a5,111 # 6f77206f <__BSS_END__+0x6f65f807>
   1153c:	fcf42223          	sw	a5,-60(s0)
   11540:	006477b7          	lui	a5,0x647
   11544:	c7278793          	addi	a5,a5,-910 # 646c72 <__BSS_END__+0x53440a>
   11548:	fcf42423          	sw	a5,-56(s0)
   1154c:	fd042483          	lw	s1,-48(s0)
   11550:	fc040793          	addi	a5,s0,-64
   11554:	00078513          	mv	a0,a5
   11558:	cccff0ef          	jal	10a24 <str_len>
   1155c:	00050713          	mv	a4,a0
   11560:	fc040793          	addi	a5,s0,-64
   11564:	00070693          	mv	a3,a4
   11568:	00000613          	li	a2,0
   1156c:	00078593          	mv	a1,a5
   11570:	00048513          	mv	a0,s1
   11574:	d01fe0ef          	jal	10274 <write>
   11578:	00000517          	auipc	a0,0x0
   1157c:	26c50513          	addi	a0,a0,620 # 117e4 <_start+0x110>
   11580:	f1dfe0ef          	jal	1049c <print>
   11584:	fd042783          	lw	a5,-48(s0)
   11588:	fac40713          	addi	a4,s0,-84
   1158c:	01400693          	li	a3,20
   11590:	00000613          	li	a2,0
   11594:	00070593          	mv	a1,a4
   11598:	00078513          	mv	a0,a5
   1159c:	c61fe0ef          	jal	101fc <read>
   115a0:	fac40793          	addi	a5,s0,-84
   115a4:	00078593          	mv	a1,a5
   115a8:	00000517          	auipc	a0,0x0
   115ac:	15450513          	addi	a0,a0,340 # 116fc <_start+0x28>
   115b0:	eedfe0ef          	jal	1049c <print>
   115b4:	00000517          	auipc	a0,0x0
   115b8:	24050513          	addi	a0,a0,576 # 117f4 <_start+0x120>
   115bc:	ee1fe0ef          	jal	1049c <print>
   115c0:	00000797          	auipc	a5,0x0
   115c4:	28878793          	addi	a5,a5,648 # 11848 <_start+0x174>
   115c8:	0007a503          	lw	a0,0(a5)
   115cc:	0047a583          	lw	a1,4(a5)
   115d0:	0087a603          	lw	a2,8(a5)
   115d4:	00c7a683          	lw	a3,12(a5)
   115d8:	0107a703          	lw	a4,16(a5)
   115dc:	0147a783          	lw	a5,20(a5)
   115e0:	f8a42a23          	sw	a0,-108(s0)
   115e4:	f8b42c23          	sw	a1,-104(s0)
   115e8:	f8c42e23          	sw	a2,-100(s0)
   115ec:	fad42023          	sw	a3,-96(s0)
   115f0:	fae42223          	sw	a4,-92(s0)
   115f4:	faf42423          	sw	a5,-88(s0)
   115f8:	646e67b7          	lui	a5,0x646e6
   115fc:	57378793          	addi	a5,a5,1395 # 646e6573 <__BSS_END__+0x645d3d0b>
   11600:	f8f42423          	sw	a5,-120(s0)
   11604:	736577b7          	lui	a5,0x73657
   11608:	42078793          	addi	a5,a5,1056 # 73657420 <__BSS_END__+0x73544bb8>
   1160c:	f8f42623          	sw	a5,-116(s0)
   11610:	07400793          	li	a5,116
   11614:	f8f41823          	sh	a5,-112(s0)
   11618:	f8840793          	addi	a5,s0,-120
   1161c:	00078513          	mv	a0,a5
   11620:	c04ff0ef          	jal	10a24 <str_len>
   11624:	00050693          	mv	a3,a0
   11628:	f8840713          	addi	a4,s0,-120
   1162c:	f9440793          	addi	a5,s0,-108
   11630:	00068613          	mv	a2,a3
   11634:	00070593          	mv	a1,a4
   11638:	00078513          	mv	a0,a5
   1163c:	d8dfe0ef          	jal	103c8 <send>
   11640:	fcc40713          	addi	a4,s0,-52
   11644:	fd040793          	addi	a5,s0,-48
   11648:	00070613          	mv	a2,a4
   1164c:	00078593          	mv	a1,a5
   11650:	00000517          	auipc	a0,0x0
   11654:	1b450513          	addi	a0,a0,436 # 11804 <_start+0x130>
   11658:	d15fe0ef          	jal	1036c <open>
   1165c:	fcc40693          	addi	a3,s0,-52
   11660:	b8840713          	addi	a4,s0,-1144
   11664:	f9440793          	addi	a5,s0,-108
   11668:	40000613          	li	a2,1024
   1166c:	00070593          	mv	a1,a4
   11670:	00078513          	mv	a0,a5
   11674:	db1fe0ef          	jal	10424 <recv>
   11678:	fcc42703          	lw	a4,-52(s0)
   1167c:	fff00793          	li	a5,-1
   11680:	00f70e63          	beq	a4,a5,1169c <main+0x2dc>
   11684:	b8840793          	addi	a5,s0,-1144
   11688:	00078593          	mv	a1,a5
   1168c:	00000517          	auipc	a0,0x0
   11690:	18850513          	addi	a0,a0,392 # 11814 <_start+0x140>
   11694:	e09fe0ef          	jal	1049c <print>
   11698:	fc5ff06f          	j	1165c <main+0x29c>
   1169c:	00000013          	nop
   116a0:	00000517          	auipc	a0,0x0
   116a4:	17c50513          	addi	a0,a0,380 # 1181c <_start+0x148>
   116a8:	df5fe0ef          	jal	1049c <print>
   116ac:	00000517          	auipc	a0,0x0
   116b0:	18850513          	addi	a0,a0,392 # 11834 <_start+0x160>
   116b4:	de9fe0ef          	jal	1049c <print>
   116b8:	00000793          	li	a5,0
   116bc:	00078513          	mv	a0,a5
   116c0:	48c12083          	lw	ra,1164(sp)
   116c4:	48812403          	lw	s0,1160(sp)
   116c8:	48412483          	lw	s1,1156(sp)
   116cc:	49010113          	addi	sp,sp,1168
   116d0:	00008067          	ret

000116d4 <_start>:
   116d4:	cedff0ef          	jal	113c0 <main>
   116d8:	00900893          	li	a7,9
   116dc:	00000073          	ecall

Disassembly of section .rodata:

000116e0 <__DATA_BEGIN__-0x1180>:
   116e0:	6325                	.insn	2, 0x6325
   116e2:	0000                	.insn	2, 0x
   116e4:	2d0a                	.insn	2, 0x2d0a
   116e6:	2d2d                	.insn	2, 0x2d2d
   116e8:	7021                	.insn	2, 0x7021
   116ea:	666f776f          	jal	a4,108d50 <__global_pointer$+0xf5cf0>
   116ee:	2066                	.insn	2, 0x2066
   116f0:	6f6e                	.insn	2, 0x6f6e
   116f2:	2d2d2177          	.insn	4, 0x2d2d2177
   116f6:	0a2d                	.insn	2, 0x0a2d
   116f8:	0000                	.insn	2, 0x
   116fa:	0000                	.insn	2, 0x
   116fc:	7325                	.insn	2, 0x7325
   116fe:	000a                	.insn	2, 0x000a
   11700:	2d0a                	.insn	2, 0x2d0a
   11702:	2d2d                	.insn	2, 0x2d2d
   11704:	682d                	.insn	2, 0x682d
   11706:	6c65                	.insn	2, 0x6c65
   11708:	6f6c                	.insn	2, 0x6f6c
   1170a:	2d2d                	.insn	2, 0x2d2d
   1170c:	2d2d                	.insn	2, 0x2d2d
   1170e:	740a                	.insn	2, 0x740a
   11710:	6968                	.insn	2, 0x6968
   11712:	73692073          	.insn	4, 0x73692073
   11716:	7520                	.insn	2, 0x7520
   11718:	20726573          	.insn	4, 0x20726573
   1171c:	7270                	.insn	2, 0x7270
   1171e:	6172676f          	jal	a4,38534 <__global_pointer$+0x254d4>
   11722:	2c6d                	.insn	2, 0x2c6d
   11724:	7261                	.insn	2, 0x7261
   11726:	2065                	.insn	2, 0x2065
   11728:	6f79                	.insn	2, 0x6f79
   1172a:	2075                	.insn	2, 0x2075
   1172c:	283f6b6f          	jal	s6,1081ae <__global_pointer$+0xf514e>
   11730:	6579                	.insn	2, 0x6579
   11732:	6f6e2f73          	.insn	4, 0x6f6e2f73
   11736:	0029                	.insn	2, 0x0029
   11738:	7325                	.insn	2, 0x7325
   1173a:	0000                	.insn	2, 0x
   1173c:	6579                	.insn	2, 0x6579
   1173e:	69660073          	.insn	4, 0x69660073
   11742:	656e                	.insn	2, 0x656e
   11744:	000a                	.insn	2, 0x000a
   11746:	0000                	.insn	2, 0x
   11748:	6f6e                	.insn	2, 0x6f6e
   1174a:	0000                	.insn	2, 0x
   1174c:	2769                	.insn	2, 0x2769
   1174e:	206d                	.insn	2, 0x206d
   11750:	72726f73          	.insn	4, 0x72726f73
   11754:	2079                	.insn	2, 0x2079
   11756:	6f74                	.insn	2, 0x6f74
   11758:	6820                	.insn	2, 0x6820
   1175a:	6165                	.insn	2, 0x6165
   1175c:	2072                	.insn	2, 0x2072
   1175e:	6874                	.insn	2, 0x6874
   11760:	7461                	.insn	2, 0x7461
   11762:	000a                	.insn	2, 0x000a
   11764:	74616877          	.insn	4, 0x74616877
   11768:	7420                	.insn	2, 0x7420
   1176a:	6568                	.insn	2, 0x6568
   1176c:	6620                	.insn	2, 0x6620
   1176e:	6375                	.insn	2, 0x6375
   11770:	000a3f6b          	.insn	4, 0x000a3f6b
   11774:	67762323          	sw	s7,1638(a2)
   11778:	7465                	.insn	2, 0x7465
   1177a:	70206863          	bltu	zero,sp,11e8a <_start+0x7b6>
   1177e:	7361                	.insn	2, 0x7361
   11780:	6e690a73          	.insn	4, 0x6e690a73
   11784:	7570                	.insn	2, 0x7570
   11786:	2074                	.insn	2, 0x2074
   11788:	20277127          	.insn	4, 0x20277127
   1178c:	6f74                	.insn	2, 0x6f74
   1178e:	7420                	.insn	2, 0x7420
   11790:	7365                	.insn	2, 0x7365
   11792:	3a74                	.insn	2, 0x3a74
   11794:	0000                	.insn	2, 0x
   11796:	0000                	.insn	2, 0x
   11798:	626b2323          	sw	t1,1574(s6)
   1179c:	6968                	.insn	2, 0x6968
   1179e:	2074                	.insn	2, 0x2074
   117a0:	6170                	.insn	2, 0x6170
   117a2:	000a7373          	.insn	4, 0x000a7373
   117a6:	0000                	.insn	2, 0x
   117a8:	706d742f          	.insn	4, 0x706d742f
   117ac:	6573752f          	.insn	4, 0x6573752f
   117b0:	5f72                	.insn	2, 0x5f72
   117b2:	7270                	.insn	2, 0x7270
   117b4:	6172676f          	jal	a4,385ca <__global_pointer$+0x2556a>
   117b8:	5f6d                	.insn	2, 0x5f6d
   117ba:	6574                	.insn	2, 0x6574
   117bc:	742e7473          	.insn	4, 0x742e7473
   117c0:	7478                	.insn	2, 0x7478
   117c2:	0000                	.insn	2, 0x
   117c4:	72632323          	sw	t1,1830(t1)
   117c8:	6165                	.insn	2, 0x6165
   117ca:	6574                	.insn	2, 0x6574
   117cc:	7020                	.insn	2, 0x7020
   117ce:	7361                	.insn	2, 0x7361
   117d0:	00000a73          	.insn	4, 0x0a73
   117d4:	706f2323          	sw	t1,1798(t5)
   117d8:	6e65                	.insn	2, 0x6e65
   117da:	7020                	.insn	2, 0x7020
   117dc:	7361                	.insn	2, 0x7361
   117de:	00000a73          	.insn	4, 0x0a73
   117e2:	0000                	.insn	2, 0x
   117e4:	72772323          	sw	t2,1830(a4)
   117e8:	7469                	.insn	2, 0x7469
   117ea:	2065                	.insn	2, 0x2065
   117ec:	6170                	.insn	2, 0x6170
   117ee:	000a7373          	.insn	4, 0x000a7373
   117f2:	0000                	.insn	2, 0x
   117f4:	65722323          	sw	s7,1606(tp) # 646 <exit-0xfa6e>
   117f8:	6461                	.insn	2, 0x6461
   117fa:	7020                	.insn	2, 0x7020
   117fc:	7361                	.insn	2, 0x7361
   117fe:	00000a73          	.insn	4, 0x0a73
   11802:	0000                	.insn	2, 0x
   11804:	706d742f          	.insn	4, 0x706d742f
   11808:	7365742f          	.insn	4, 0x7365742f
   1180c:	2e74                	.insn	2, 0x2e74
   1180e:	6962                	.insn	2, 0x6962
   11810:	006e                	.insn	2, 0x006e
   11812:	0000                	.insn	2, 0x
   11814:	250a                	.insn	2, 0x250a
   11816:	00000a73          	.insn	4, 0x0a73
   1181a:	0000                	.insn	2, 0x
   1181c:	65732323          	sw	s7,1606(t1)
   11820:	646e                	.insn	2, 0x646e
   11822:	6120                	.insn	2, 0x6120
   11824:	646e                	.insn	2, 0x646e
   11826:	7220                	.insn	2, 0x7220
   11828:	6365                	.insn	2, 0x6365
   1182a:	2076                	.insn	2, 0x2076
   1182c:	6170                	.insn	2, 0x6170
   1182e:	000a7373          	.insn	4, 0x000a7373
   11832:	0000                	.insn	2, 0x
   11834:	6c612323          	sw	t1,1734(sp)
   11838:	206c                	.insn	2, 0x206c
   1183a:	6574                	.insn	2, 0x6574
   1183c:	20737473          	.insn	4, 0x20737473
   11840:	6170                	.insn	2, 0x6170
   11842:	0a217373          	.insn	4, 0x0a217373
   11846:	0000                	.insn	2, 0x
   11848:	0000                	.insn	2, 0x
   1184a:	0000                	.insn	2, 0x
   1184c:	3231                	.insn	2, 0x3231
   1184e:	2e302e37          	lui	t3,0x2e302
   11852:	2e30                	.insn	2, 0x2e30
   11854:	0031                	.insn	2, 0x0031
   11856:	0000                	.insn	2, 0x
   11858:	0000                	.insn	2, 0x
   1185a:	0000                	.insn	2, 0x
   1185c:	1f90                	.insn	2, 0x1f90
	...

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347          	.insn	4, 0x3a434347
   4:	2820                	.insn	2, 0x2820
   6:	31202967          	.insn	4, 0x31202967
   a:	2e34                	.insn	2, 0x2e34
   c:	2e32                	.insn	2, 0x2e32
   e:	0030                	.insn	2, 0x0030

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	1b41                	.insn	2, 0x1b41
   2:	0000                	.insn	2, 0x
   4:	7200                	.insn	2, 0x7200
   6:	7369                	.insn	2, 0x7369
   8:	01007663          	bgeu	zero,a6,14 <exit-0x100a0>
   c:	0011                	.insn	2, 0x0011
   e:	0000                	.insn	2, 0x
  10:	1004                	.insn	2, 0x1004
  12:	7205                	.insn	2, 0x7205
  14:	3376                	.insn	2, 0x3376
  16:	6932                	.insn	2, 0x6932
  18:	7032                	.insn	2, 0x7032
  1a:	0031                	.insn	2, 0x0031
