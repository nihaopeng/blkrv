all:
	riscv32-unknown-elf-gcc ../os/kernel/std/std.c \
	../os/kernel/utils/str.c \
	../os/kernel/utils/math.c \
	hello.c \
	_start.S \
	-I../os/include -nostdlib -fPIE -o hello