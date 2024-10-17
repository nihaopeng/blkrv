cc := riscv32-unknown-elf-gcc
as := riscv32-unknown-elf-as
ld := riscv32-unknown-elf-ld

INCLUDE := ./include

MAIN := ./init/main.c

_START := ./init/_start.S

BOOTSECT := ./boot/bootsect.S

all:build_dir sys u_console bootsect _start 
	$(cc) -o ./build/init/sysimg ./build/init/_start.o ./build/init/main.o ./build/init/console.o -I$(INCLUDE) -nostdlib -fPIE

_start:$(_START)
	$(cc) -c ./init/_start.S -o ./build/init/_start.o -march=rv32i_zicsr

u_console:./console/console.c
	$(cc) -c ./console/console.c -o ./build/init/console.o -I$(INCLUDE) -fPIE

bootsect:$(BOOTSECT)
	$(cc) -c ./boot/bootsect.S -o ./build/boot/bootsect.o -march=rv32i_zicsr

sys:$(MAIN)
	$(cc) -c ./init/main.c -o ./build/init/main.o -I$(INCLUDE) -fPIE

build_dir:
	@mkdir -p ./build/init
	@mkdir -p ./build/boot

clean:
	@rm -rf ./build