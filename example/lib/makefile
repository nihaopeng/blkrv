CC := riscv32-unknown-elf-gcc
AR := riscv32-unknown-elf-ar
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
