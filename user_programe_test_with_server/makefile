all:
	riscv32-unknown-elf-gcc ./std.c ./str.c ./math.c hello.c \
	_start.S \
	-I../os/include -nostdlib -fPIE -o hello

test:
	/home/riscv_unknown_gcc/riscvf_build/bin/riscv32-unknown-elf-gcc \
	./std.c ./str.c ./math.c tmpTest.c \
	_start.S \
	-I../os/include -nostdlib -fPIE -o tmpTest
	
	./tmpTest