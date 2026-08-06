CC = riscv32-unknown-elf-gcc
CFLAGS = -I../../include -nostdlib -fPIE -lgcc -Wl,--no-relax

all: ls cat echo mkdir touch rm clear

ls: ls/src/_start.S ls/src/main.c
	$(CC) ./ls/src/_start.S ./ls/src/main.c $(CFLAGS) -o ./ls/ls

cat: cat/src/_start.S cat/src/main.c
	$(CC) ./cat/src/_start.S ./cat/src/main.c $(CFLAGS) -o ./cat/cat

echo: echo/src/_start.S echo/src/main.c
	$(CC) ./echo/src/_start.S ./echo/src/main.c $(CFLAGS) -o ./echo/echo

mkdir: mkdir/src/_start.S mkdir/src/main.c
	$(CC) ./mkdir/src/_start.S ./mkdir/src/main.c $(CFLAGS) -o ./mkdir/mkdir

touch: touch/src/_start.S touch/src/main.c
	$(CC) ./touch/src/_start.S ./touch/src/main.c $(CFLAGS) -o ./touch/touch

rm: rm/src/_start.S rm/src/main.c
	$(CC) ./rm/src/_start.S ./rm/src/main.c $(CFLAGS) -o ./rm/rm

clear: clear/src/_start.S clear/src/main.c
	$(CC) ./clear/src/_start.S ./clear/src/main.c $(CFLAGS) -o ./clear/clear

clean:
	rm -f ./ls/ls ./cat/cat ./echo/echo ./mkdir/mkdir ./touch/touch ./rm/rm ./clear/clear
