# === BLKRv example 公共编译规则层 (分层构建的共享层) ===
#
# 层级结构:
#   example/makefile  顶层编排: lib -> 各用户程序子目录 -> hello/server
#   example/rules.mk  公共编译规则: 本文件 (所有子目录 makefile 共享)
#   <app>/makefile    应用层: 只描述自己的目标, 头部 include ../rules.mk
#
# 顶层 makefile 使用时需先设置 LIBPATH := lib 再 include rules.mk;
# 子目录 makefile 直接 include ../rules.mk (LIBPATH 默认 ../lib)。

CC      := riscv32-unknown-elf-gcc
AR      := riscv32-unknown-elf-ar
LIBPATH ?= ../lib
CFLAGS  := -march=rv32i -mabi=ilp32 -O1 -I$(LIBPATH)
LDLIBS  := $(LIBPATH)/crt0.o $(LIBPATH)/libblkrv.a -lgcc
LDFLAGS := -nostdlib -Wl,--no-relax -Wl,-s
