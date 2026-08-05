all:
	riscv32-unknown-elf-gcc ./src/_start.S ./src/main.c ../src/std.c ../src/str.c ../src/math.c \
	-I../include -nostdlib -fPIE -lgcc -Wl,--no-relax -o test_cli

clean:
	rm -f test_cli
