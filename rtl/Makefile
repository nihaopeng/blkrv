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
EXECUTABLE := $(EXECUTABLE_PATH)/BLKRv

all:$(BUILD_ROOT) $(VERILOG_PATH) $(OBJ_PATH) $(EXECUTABLE_PATH) verilog2cc $(OBJS)
	@echo $(VSRC)
	@echo $(CSRC)
	$(cc) $(wildcard $(VERILOG_PATH)/*.o) $(OBJS) -o $(EXECUTABLE) -lstdc++ -lm -pthread
	$(EXECUTABLE)

# create necessary directory
$(BUILD_ROOT):
	@mkdir -p $@

$(VERILOG_PATH):
	@mkdir -p $@

$(OBJ_PATH):
	@mkdir -p $@

$(EXECUTABLE_PATH):
	@mkdir -p $@

# --cc to generate c++ code, --exe for wrapper file, --build for obj_dir .o	
# below is to make verilog code converted into c++ code
verilog2cc:
	$(VERILATOR) --trace --build --cc -j $(TOP_MODULE) $(VSRC) -Mdir $(VERILOG_PATH)

# all cpp file will exeucte below action
$(OBJ_PATH)/%.o: $(CSRC_PATH)/%.cpp
	$(cc) -c $< -o $@ -I$(CINCLUDE) -I$(SYSPATH) -I$(SYSPATH1) -I$(SYSPATH2) -I$(VERILOG_PATH)

clean:
	@rm -rf $(BUILD_ROOT)