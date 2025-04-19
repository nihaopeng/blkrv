all:
	riscv32-unknown-elf-gcc _start.S ./std.c ./str.c ./math.c hello.c \
	-I../os/include -nostdlib -fPIE -lgcc -o hello

	riscv32-unknown-elf-objdump -D hello > hello.s