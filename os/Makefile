cc := riscv32-unknown-elf-gcc
as := riscv32-unknown-elf-as
ld := riscv32-unknown-elf-ld
objdump := riscv32-unknown-elf-objdump

ifeq ($(ENABLE_GPU), 1)
    ccflag += -DENABLE_GPU
endif

BUILD_DIR := ./build/

INCLUDE := ./include

MAIN := ./init/main.c

_START := ./init/_start.S

_PULL_PROG := ./kernel/proc/_pull_prog.S

BOOTSECT := ./boot/bootsect.S

KERNEL_SRC := ./kernel

SRC_FILES = $(shell find ./kernel -name *.c)
OBJ_FILES = $(SRC_FILES:%.c=%.o)

all:build_dir bootsect _start sys $(OBJ_FILES)
	$(cc) -o ./build/init/sysimg ./build/init/_start.o \
	$(OBJ_FILES) \
	./build/init/main.o  -I$(INCLUDE) -nostdlib -lgcc -fPIE -Wl,--no-relax $(ccflag)

	$(objdump) -d ./build/init/sysimg > ./build/init/sysimg.s
#-Wl,--no-relax避免使用gp指针压缩指令,因为我们没有c库,没法初始化该指针

_start:$(_START)
	$(cc) -c $(_START) -o ./build/init/_start.o -march=rv32i_zicsr $(ccflag)

bootsect:$(BOOTSECT)
	$(cc) -c ./boot/bootsect.S -o ./build/boot/bootsect.o $(ccflag)

sys:$(MAIN)
	$(cc) -c ./init/main.c -o ./build/init/main.o -I$(INCLUDE) -fPIE $(ccflag)

%.o:%.c
	$(cc) -c $< -o $@ -I$(INCLUDE) -fPIE -march=rv32i_zicsr $(ccflag)

build_dir:
	@mkdir -p ./build/init
	@mkdir -p ./build/boot

clean:
	@rm -rf ./build
	@rm -rf $(OBJ_FILES)

.PHONY:run clean