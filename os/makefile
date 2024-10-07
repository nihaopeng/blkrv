
cc := riscv32-unknown-elf-gcc
as := riscv32-unknown-elf-as
ld := riscv32-unknown-elf-ld

INCLUDE := ./include

MAIN := ./init/main.c

_START := ./init/_start.S

all:sys _start
	$(cc) -o ./init/sysimg ./init/_start.o ./init/main.o -I$(INCLUDE) -nostdlib -fPIE

_start:$(_START)
	$(cc) -c ./init/_start.S -o ./init/_start.o -march=rv32i_zicsr

sys:$(MAIN)
	$(cc) -c -o ./init/main.o $(MAIN) -I$(INCLUDE) -fPIE
