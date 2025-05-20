all:
	riscv32-unknown-elf-gcc _start.S ./std.c ./str.c ./math.c main.c \
	-I../os/include -nostdlib -fPIE -lgcc -Wl,--no-relax -o snake