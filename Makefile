# === BLKRv 顶层 Makefile: 编排 + 执行 ===

BACKEND ?= verilog
ENABLE_GPU ?= 0
FILE_PATH ?= ../data.txt
SYSPATH ?= /home/pengyt/softwares/verilator/usable/share/verilator/include

# 传递给子 make 的变量
export BACKEND
export ENABLE_GPU
export SYSPATH

# ---------------- 编译 ----------------

compile: compile-os compile-rtl

compile-os:
	@make -C ./os

compile-rtl:
	@make -C ./rtl

# ---------------- 运行 ----------------

run: compile loadmem
	@cd rtl && ./build/dist/BLKRv $(FILE_PATH)

# ---------------- 设备镜像加载 ----------------

loadmem: compile-rtl-mkutils mkbios
	@./rtl/build/dist/manage_mem -src ./rtl/build/bios/bios.o -tar ./rtl/devices/bios \
		-src_start 52 -src_end 1139 -tar_start 0 -tar_end 1087
	@./rtl/build/dist/manage_mem -src ./os/build/boot/bootsect.o -tar ./rtl/devices/flash \
		-src_start 52 -src_end 563 -tar_start 0 -tar_end 511
	@./rtl/build/dist/manage_mem -src ./os/build/init/sysimg -tar ./rtl/devices/flash \
		-src_start 0 -src_end 1048576 -tar_start 512 -tar_end 1049088

compile-rtl-mkutils:
	@make -C ./rtl mkutils

mkbios:
	@riscv32-unknown-elf-gcc -c ./rtl/asm/bios.s -o ./rtl/build/bios/bios.o -march=rv32i_zicsr

# ---------------- 可视化 ----------------

vis:
	mkdir -p ./blkrvis/data
	cp $(file_path) ./blkrvis/data/data.txt
	python3 -m http.server 8000 -d ./blkrvis

# ---------------- 清理 ----------------

clean:
	@make clean -C ./os
	@make clean -C ./rtl

.PHONY: run clean compile compile-os compile-rtl loadmem mkbios vis compile-rtl-mkutils
