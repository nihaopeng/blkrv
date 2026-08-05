# === BLKRv RTL 编译: 不执行、不加载镜像、只产出 BLKRv 可执行文件 ===

ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
SYSPATH ?=/usr/local/share/verilator/include
else
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
SYSPATH ?=$(VERILATOR_ROOT)/share/verilator/include
endif

cc = g++
ccflag = -lstdc++ -lm -pthread
ifeq ($(ENABLE_GPU), 1)
    ccflag += -DENABLE_GPU -lfltk -lfltk_images
endif

# CPU backend
BACKEND ?= verilog
ifeq ($(BACKEND), sim)
    ccflag += -DBACKEND_SIM
    CPU_STEP :=
    LINK_VERILOG :=
else
    CPU_STEP := verilog2cc
    LINK_VERILOG = $(wildcard $(VERILOG_PATH)/*.o)
endif

# verilator include
SYSPATH1:=$(SYSPATH)/vltstd
SYSPATH2:=$(SYSPATH)

# verilog 源文件
VSRC :=$(wildcard vsrc/core/*.v vsrc/*.v)
TOP_MODULE :=--top-module top

# 构建输出目录
BUILD_ROOT := ./build
OBJ_PATH := $(BUILD_ROOT)/objs
VERILOG_PATH := $(BUILD_ROOT)/verilog
EXECUTABLE_PATH := $(BUILD_ROOT)/dist
DEVICES_PATH := ./devices

# C++ 包装文件
CSRC_PATH := ./csrc/src
ifeq ($(BACKEND), sim)
CSRC := $(wildcard $(CSRC_PATH)/*.cpp)
else
CSRC := $(filter-out $(CSRC_PATH)/cpu_sim.cpp,$(wildcard $(CSRC_PATH)/*.cpp))
endif
OBJS := $(patsubst $(CSRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(CSRC))
CINCLUDE :=./csrc/include

# 最终可执行文件
EXECUTABLE_NAME := $(EXECUTABLE_PATH)/BLKRv

# ---------------- 编译目标 ----------------

all:info build_dir $(CPU_STEP) $(OBJS) mkutils mkexecutable

build_dir:
	@mkdir -p $(VERILOG_PATH)
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(EXECUTABLE_PATH)
	@mkdir -p $(BUILD_ROOT)/bios
	@mkdir -p $(DEVICES_PATH)

verilog2cc:
	$(VERILATOR) --trace --build --cc $(TOP_MODULE) $(VSRC) -Mdir $(VERILOG_PATH)

$(OBJ_PATH)/%.o: $(CSRC_PATH)/%.cpp
	$(cc) -c $< -o $@ -I$(CINCLUDE) -I$(SYSPATH) -I$(SYSPATH1) -I$(SYSPATH2) -I$(VERILOG_PATH) $(ccflag)

mkexecutable:
	$(cc) $(LINK_VERILOG) $(OBJS) -o $(EXECUTABLE_NAME) $(ccflag)

mkutils:./utils/manage_mem.cpp
	$(cc) ./utils/manage_mem.cpp -o $(EXECUTABLE_PATH)/manage_mem

info:
	@echo $(VSRC)
	@echo $(CSRC)

# ---------------- 清理 ----------------

clean:
	@rm -rf $(BUILD_ROOT)

.PHONY:all build_dir verilog2cc mkexecutable mkutils info clean
