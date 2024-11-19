cc := riscv32-unknown-elf-gcc
as := riscv32-unknown-elf-as
ld := riscv32-unknown-elf-ld

BUILD_DIR := ./build/

INCLUDE := ./include

MAIN := ./init/main.c

_START := ./init/_start.S

_PULL_PROG := ./kernel/proc/_pull_prog.S

BOOTSECT := ./boot/bootsect.S

CONSOLE := ./console/console.c

KERNEL_SRC := ./kernel

SRC_FILES = $(shell find ./kernel -name *.c)
OBJ_FILES = $(SRC_FILES:%.c=%.o)

all:build_dir bootsect _start u_console sys $(OBJ_FILES) _pull_prog
	$(cc) -o ./build/init/sysimg  ./build/init/_pull_prog.o \
	$(OBJ_FILES) ./build/init/_start.o \
	./build/init/console.o \
	./build/init/main.o  -I$(INCLUDE) -nostdlib -fPIE -Wl,--no-relax
#-Wl,--no-relax避免使用gp指针压缩指令,因为我们没有c库,没法初始化该指针

_start:$(_START)
	$(cc) -c $(_START) -o ./build/init/_start.o -march=rv32i_zicsr

_pull_prog:$(_PULL_PROG)
	$(cc) -c $(_PULL_PROG) -o ./build/init/_pull_prog.o -march=rv32i_zicsr

u_console:$(CONSOLE)
	$(cc) -c $(CONSOLE) -o ./build/init/console.o -I$(INCLUDE) -fPIE

bootsect:$(BOOTSECT)
	$(cc) -c ./boot/bootsect.S -o ./build/boot/bootsect.o

sys:$(MAIN)
	$(cc) -c ./init/main.c -o ./build/init/main.o -I$(INCLUDE) -fPIE

%.o:%.c
	$(cc) -c $< -o $@ -I$(INCLUDE) -fPIE -march=rv32i_zicsr

build_dir:
	@mkdir -p ./build/init
	@mkdir -p ./build/boot

clean:
	@rm -rf ./build
	@rm -rf $(OBJ_FILES)
