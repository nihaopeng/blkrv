CC := riscv32-unknown-elf-gcc
CFLAGS := -march=rv32i -mabi=ilp32 -O1 -I../lib
LDLIBS := ../lib/crt0.o ../lib/libblkrv.a -lgcc
LDFLAGS := -nostdlib -Wl,--no-relax -Wl,-s

all: snake

snake: src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o snake

clean:
	rm -f snake

.PHONY: all clean
