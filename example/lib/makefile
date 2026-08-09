# === BLKRv 运行时库层: crt0 启动代码 + libblkrv 系统调用库 ===
include ../rules.mk

# lib 位于 example/lib, 头文件就在本目录, 覆盖 CFLAGS 的 -I 路径
CFLAGS := -march=rv32i -mabi=ilp32 -O1 -I.

all: crt0.o libblkrv.a

crt0.o: crt0.S
	$(CC) $(CFLAGS) -c $< -o $@

libblkrv.o: libblkrv.c blkrv.h
	$(CC) $(CFLAGS) -c $< -o $@

libblkrv.a: libblkrv.o
	$(AR) rcs $@ $^

clean:
	rm -f crt0.o libblkrv.o libblkrv.a

.PHONY: all clean
