cc := riscv32-unknown-elf-gcc
as := riscv32-unknown-elf-as
ld := riscv32-unknown-elf-ld

INCLUDE := ./include

MAIN := ./init/main.c

_START := ./init/_start.S

all:build sys _start
	$(cc) -o ./build/sysimg ./build/_start.o ./build/main.o -I$(INCLUDE) -nostdlib -fPIE

_start:$(_START)
	$(cc) -c ./init/_start.S -o ./build/_start.o -march=rv32i_zicsr

sys:$(MAIN)
	$(cc) -c -o ./build/main.o $(MAIN) -I$(INCLUDE) -fPIE

build:
	@mkdir -p build

clean:
	@rm -rf build