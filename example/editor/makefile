CC := riscv32-unknown-elf-gcc
CFLAGS := -march=rv32i -mabi=ilp32 -O1 -I../lib
LDLIBS := ../lib/crt0.o ../lib/libblkrv.a -lgcc
LDFLAGS := -nostdlib -Wl,--no-relax -Wl,-s

all: editor

editor: src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o editor

clean:
	rm -f editor

.PHONY: all clean
