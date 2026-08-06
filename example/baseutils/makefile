CC = riscv32-unknown-elf-gcc
CFLAGS = -I../../include -nostdlib -fPIE -lgcc -Wl,--no-relax

all: ls cat

ls: ls/src/_start.S ls/src/main.c
	$(CC) ./ls/src/_start.S ./ls/src/main.c $(CFLAGS) -o ./ls/ls

cat: cat/src/_start.S cat/src/main.c
	$(CC) ./cat/src/_start.S ./cat/src/main.c $(CFLAGS) -o ./cat/cat

clean:
	rm -f ./ls/ls ./cat/cat
