CC := riscv32-unknown-elf-gcc
CFLAGS := -march=rv32i -mabi=ilp32 -O1 -Ilib
LDLIBS := lib/crt0.o lib/libblkrv.a -lgcc
LDFLAGS := -nostdlib -Wl,--no-relax -Wl,-s

all: hello server

hello: src/hello.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o hello

server: server.cpp
	g++ server.cpp -o server -lpthread

clean:
	rm -f hello server
	riscv32-unknown-elf-objdump -D hello > hello.s

.PHONY: all clean
