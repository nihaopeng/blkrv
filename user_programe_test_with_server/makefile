all:
	riscv32-unknown-elf-gcc ../blkrv/os/kernel/std/std.c \
	../blkrv/os/kernel/utils/str.c \
	../blkrv/os/kernel/utils/math.c \
	hello.c \
	_start.S \
	-I../blkrv/os/include -nostdlib -fPIE -o hello