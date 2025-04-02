all:
	riscv32-unknown-elf-gcc ./std.c ./str.c ./math.c hello.c \
	_start.S \
	-I../os/include -nostdlib -fPIE -o hello

assem:
	riscv32-unknown-elf-gcc ./std.c ./str.c ./math.c hello.c \
	_start.S \
	-I../os/include -nostdlib -fPIE -o hello
	
	riscv32-unknown-elf-objdump -D hello > hello.s