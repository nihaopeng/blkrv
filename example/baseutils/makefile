CC := riscv32-unknown-elf-gcc
FLAGS := -I../../include -nostdlib -fPIE -lgcc -Wl,--no-relax

all: cat/cat clear/clear echo/echo ls/ls mkdir/mkdir rm/rm touch/touch

cat/cat: cat/src/_start.S cat/src/main.c
	$(CC) $^ $(FLAGS) -o $@
clear/clear: clear/src/_start.S clear/src/main.c
	$(CC) $^ $(FLAGS) -o $@
echo/echo: echo/src/_start.S echo/src/main.c
	$(CC) $^ $(FLAGS) -o $@
ls/ls: ls/src/_start.S ls/src/main.c
	$(CC) $^ $(FLAGS) -o $@
mkdir/mkdir: mkdir/src/_start.S mkdir/src/main.c
	$(CC) $^ $(FLAGS) -o $@
rm/rm: rm/src/_start.S rm/src/main.c
	$(CC) $^ $(FLAGS) -o $@
touch/touch: touch/src/_start.S touch/src/main.c
	$(CC) $^ $(FLAGS) -o $@

clean:
	rm -f cat/cat clear/clear echo/echo ls/ls mkdir/mkdir rm/rm touch/touch

.PHONY: all clean
