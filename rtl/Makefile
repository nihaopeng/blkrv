ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

cc := g++ 

# the include and src of verilator
SYSPATH :=/usr/local/share/verilator/include
SYSPATH1:=/usr/local/share/verilator/include/vltstd 
SYSPATH2:=/usr/share/verilator/include
# SYSSRC := $(wildcard $(SYSPATH)/*.cpp)

# some variable about verilog
VSRC :=$(wildcard vsrc/core/*.v vsrc/*.v)
VINCLUDE :=./vsrc/core
TOP_MODULE :=--top-module top

# directory of result of compiling
BUILD_ROOT := ./build
OBJ_PATH := $(BUILD_ROOT)/objs
VERILOG_PATH := $(BUILD_ROOT)/verilog
EXECUTABLE_PATH := $(BUILD_ROOT)/dist

# some variable about wrapper file
CSRC_PATH := ./csrc/src
CSRC := $(wildcard $(CSRC_PATH)/*.cpp)
OBJS := $(patsubst $(CSRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(CSRC))
CINCLUDE :=./csrc/include

# final executable file
EXECUTABLE_NAME := $(EXECUTABLE_PATH)/BLKRv

all:info build_dir verilog2cc $(OBJS) mkexecutable

# create necessary directory
build_dir:
	@mkdir -p $(VERILOG_PATH)
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(EXECUTABLE_PATH)
	@mkdir -p $(BUILD_ROOT)/bios

# --cc to generate c++ code, --exe for wrapper file, --build for obj_dir .o	
# below is to make verilog code converted into c++ code
verilog2cc:
	$(VERILATOR) --trace --build --cc -j $(TOP_MODULE) $(VSRC) -Mdir $(VERILOG_PATH)

# all cpp file will exeucte below action
$(OBJ_PATH)/%.o: $(CSRC_PATH)/%.cpp
	$(cc) -c $< -o $@ -I$(CINCLUDE) -I$(SYSPATH) -I$(SYSPATH1) -I$(SYSPATH2) -I$(VERILOG_PATH)

mkexecutable:
	$(cc) $(wildcard $(VERILOG_PATH)/*.o) $(OBJS) -o $(EXECUTABLE_NAME) -lstdc++ -lm -pthread

info:
	@echo $(VSRC)
	@echo $(CSRC)

mkutils:
	$(cc) ./utils/manage_mem.cpp -o $(EXECUTABLE_PATH)/manage_mem

mkbios:
	@riscv32-unknown-elf-gcc -c ./asm/bios.s -o $(BUILD_ROOT)/bios/bios.o -march=rv32i_zicsr

loadmem:mkutils mkbios
#	加载bios中的拉取指令
	@$(EXECUTABLE_PATH)/manage_mem -src $(BUILD_ROOT)/bios/bios.o -tar ./devices/bios -src_start 52 -src_end 1139 -tar_start 0 -tar_end 1087
#	加载bootsect
	@$(EXECUTABLE_PATH)/manage_mem -src ../os/build/boot/bootsect.o -tar ./devices/flash -src_start 52 -src_end 127 -tar_start 0 -tar_end 75
#	加载sys
	@$(EXECUTABLE_PATH)/manage_mem -src ../os/build/init/sysimg -tar ./devices/flash -src_start 0 -src_end 4768 -tar_start 512 -tar_end 5280

run:loadmem
	$(EXECUTABLE_NAME)

clean:
	@rm -rf $(BUILD_ROOT)