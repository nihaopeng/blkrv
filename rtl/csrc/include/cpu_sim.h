#ifndef _CPU_SIM_H_
#define _CPU_SIM_H_

#include <cstdint>

// 纯 C++ 功能级 RISC-V (RV32I+Zicsr) 模拟器
// 端口成员与 Verilator 生成的 Vtop 保持一致, 由 main.cpp 统一驱动
class CpuSim {
public:
    // 端口 (与 Vtop 同名同型)
    uint8_t  clk;
    uint32_t load_addr_v;   // 内存访问地址 (取指或 load/store)
    uint32_t write_data;    // 写内存数据
    uint32_t satp;          // satp CSR 输出
    uint8_t  mem_op_type;   // 0=byte 1=half 2=word
    uint8_t  we;            // 写使能
    uint32_t read_data;     // 外部返回的读数据
    uint8_t  read_valid;
    uint32_t inst_type_o;   // 指令类型编码 {lui,auipc,jal,jalr,bj,load,store,calc,calci,sys}
    uint8_t  int_port0, int_port1, int_port2, int_port3, int_port4,
             int_port5, int_port6, int_port7, int_port8;

    CpuSim();
    void eval();   // 主循环每拍调用: 消费上一请求的响应, 执行一条指令, 发出新请求

private:
    enum Phase { PH_INIT, PH_FETCH, PH_DATA };
    Phase phase;
    uint32_t regs[32];
    uint32_t csr[4096];
    uint32_t pc;
    uint32_t ld_rd;       // 挂起 load 的目标寄存器
    uint8_t  ld_funct3;   // 挂起 load 的 funct3 (符号扩展用)
    bool     ld_pending;
    uint32_t regs_cp_m[32];  // 中断(machine)上下文快照
    uint32_t regs_cp_s[32];  // 系统调用(supervisor)上下文快照

    void issue_fetch();
    void execute(uint32_t inst);
    void wb(uint32_t rd, uint32_t val);
    bool interrupt_pending();
    void take_interrupt();
    uint32_t sign_ext(uint32_t val, int bits);
    uint32_t alu_calc(uint32_t a, uint32_t b, uint8_t funct3, uint8_t funct7);
    uint32_t alui_calc(uint32_t a, uint32_t imm, uint8_t funct3);
    bool branch_cond(uint32_t a, uint32_t b, uint8_t funct3);
    void csr_op(uint32_t inst);
};

#endif // !_CPU_SIM_H_
