
cc := riscv32-unknown-elf-gcc
as := riscv32-unknown-elf-as
ld := riscv32-unknown-elf-ld
INCLUDE := ./include
SRC := $(wildcard ./kernel/syscall/*.S)

MAIN := ./init/main.c

all:$(MAIN) $(SRC)
	$(cc) -c $(MAIN) -o ./init/main.o -I$(INCLUDE) -nostdlib
	$(cc) -c $(SRC) -o ./kernel/syscall/syscall_imp.o -I$(INCLUDE) -nostdlib
	$(cc) ./init/main.o ./kernel/syscall/syscall_imp.o -o ./init/run -nostdlib
