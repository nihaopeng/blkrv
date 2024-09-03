
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

VSRC := $(wildcard vsrc/core/*.v)
VMAIN := vsrc/top.v
CSRC := $(wildcard csrc/src/*.cpp)
CMAIN := csrc/main.cpp
TOP_MODULE :=--top-module top

default:
	@echo $(VSRC)
	@echo $(CSRC)
	$(VERILATOR) -cc --build --trace --exe -j $(TOP_MODULE) $(VMAIN) $(VSRC) $(CMAIN) $(CSRC) 
	obj_dir/Vtop

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.log *.dmp *.vpd core