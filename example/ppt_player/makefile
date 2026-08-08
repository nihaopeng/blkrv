CC := riscv32-unknown-elf-gcc
CFLAGS := -march=rv32i -mabi=ilp32 -O1 -I../lib
LDLIBS := ../lib/crt0.o ../lib/libblkrv.a -lgcc
LDFLAGS := -nostdlib -Wl,--no-relax -Wl,-s

all: ppt_player

ppt_player: src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o ppt_player

clean:
	rm -f ppt_player

.PHONY: all clean
