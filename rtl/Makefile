
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

default:
	$(VERILATOR) -cc --build --trace --exe -j --top-module top vsrc/top.v csrc/main.cpp
	obj_dir/Vtop
