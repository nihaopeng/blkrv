CC := riscv32-unknown-elf-gcc
CFLAGS := -march=rv32i -mabi=ilp32 -O1 -I../lib
LDLIBS := ../lib/crt0.o ../lib/libblkrv.a -lgcc
LDFLAGS := -nostdlib -Wl,--no-relax -Wl,-s

all: cat/cat clear/clear echo/echo ls/ls mkdir/mkdir rm/rm touch/touch

cat/cat: cat/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@
clear/clear: clear/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@
echo/echo: echo/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@
ls/ls: ls/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@
mkdir/mkdir: mkdir/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@
rm/rm: rm/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@
touch/touch: touch/src/main.c
	$(CC) $(CFLAGS) $^ $(LDLIBS) $(LDFLAGS) -o $@

clean:
	rm -f cat/cat clear/clear echo/echo ls/ls mkdir/mkdir rm/rm touch/touch

.PHONY: all clean
