all:
	riscv32-unknown-elf-gcc ./src/_start.S ./src/std.c ./src/str.c ./src/math.c ./src/hello.c \
	-I./include -nostdlib -fPIE -lgcc -Wl,--no-relax -o hello

	g++ server.cpp -o server -lpthread

	riscv32-unknown-elf-objdump -D hello > hello.s