
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif


cc := g++ 

# some variable about verilog
VSRC :=$(wildcard vsrc/core/*.v vsrc/*.v)
VINCLUDE :=./vsrc/core
TOP_MODULE :=--top-module top

# some variable about wrapper file
CSRC := $(wildcard csrc/src/*.cpp csrc/main.cpp)
CINCLUDE :=./csrc/include

# the include and src of verilator
SYSPATH :=/usr/local/share/verilator/include
SYSPATH1:=/usr/local/share/verilator/include/vltstd 
SYSSRC := $(wildcard $(SYSPATH)/*.cpp)

# directory of result of verilator compiling
VERILOG2CPP := ./obj_dir

EXECUTABLE := ./BLKRv

#OBJS is a set of `o` files, they are same name with cpp file in CSRC, we have defined the %.o:%.cpp
OBJS := $(CSRC:.cpp=.o)

all:verilog2cc $(OBJS)
	@echo $(VSRC)
	@echo $(CSRC)
	cc $(wildcard $(VERILOG2CPP)/*.o) $(OBJS) -o $(EXECUTABLE) -lstdc++ -lm -pthread
	./BLKRv

# --cc to generate c++ code, --exe for wrapper file, --build for obj_dir .o	
# below is to make verilog code converted into c++ code
verilog2cc:
	$(VERILATOR) --trace --build --cc -j $(TOP_MODULE) $(VSRC) -Mdir $(VERILOG2CPP)

# all cpp file will exeucte below action
%.o:%.cpp
	$(cc) -c $< -o $@ -I$(CINCLUDE) -I$(SYSPATH) -I$(SYSPATH1) -I$(VERILOG2CPP)

clean:
	rm -rf obj_dir
	rm -f $(OBJS)