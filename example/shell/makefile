all:
	riscv32-unknown-elf-gcc ./src/_start.S ./src/main.c \
	-I../include -nostdlib -fPIE -lgcc -Wl,--no-relax -o shell

clean:
	rm -f shell
