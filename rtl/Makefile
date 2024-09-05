
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

# VMAIN :=
cc := g++ 

VSRC :=$(wildcard vsrc/core/*.v vsrc/top.v)
VINCLUDE :=./vsrc/core
TOP_MODULE :=--top-module top

CSRC := $(wildcard csrc/src/*.cpp csrc/main.cpp)
CINCLUDE :=./csrc/include

SYSPATH :=/usr/local/share/verilator/include
SYSPATH1:=/usr/local/share/verilator/include/vltstd 
SYSSRC := $(wildcard $(SYSPATH)/*.cpp)

VERILOG2CPP := ./obj_dir

EXECUTABLE := ../BLKRv

#OBJS is a set of `o` files, they are same name with cpp file in CSRC, we have defined the %.o:%.cpp
OBJS := $(CSRC:.cpp=.o)

all:cleanfirst verilog $(OBJS)
	@echo $(VSRC)
	@echo $(CSRC)
	cc  $(wildcard ./obj_dir/*.o) $(OBJS) -o ./BLKRv -lstdc++ -lm -pthread
	-rm $(OBJS)
	./BLKRv

cleanfirst:
	make clean

# --cc to generate c++ code, --exe for wrapper file, --build for obj_dir .o	
# below is to make verilog code converted into c++ code
verilog:
	$(VERILATOR) --trace --cc --build -j $(TOP_MODULE) $(VSRC) -Mdir $(VERILOG2CPP)

# all cpp file will exeucte below action
%.o:%.cpp
	$(cc) -c $< -o $@ -I$(CINCLUDE) -I$(SYSPATH) -I$(SYSPATH1) -I$(VERILOG2CPP)

clean:
	-rm -rf obj_dir
	-rm $(OBJS)