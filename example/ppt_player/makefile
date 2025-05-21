all:
	riscv32-unknown-elf-gcc ./src/_start.S ./src/std.c ./src/str.c ./src/math.c ./src/main.c \
	-I../include -nostdlib -fPIE -lgcc -Wl,--no-relax -o ppt_player