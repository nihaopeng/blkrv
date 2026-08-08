CC := riscv32-unknown-elf-gcc
FLAGS := -I../../include -nostdlib -fPIE -lgcc -Wl,--no-relax

all: editor

editor: src/_start.S src/main.c
	$(CC) $^ $(FLAGS) -o $@

clean:
	rm -f editor

.PHONY: all clean
