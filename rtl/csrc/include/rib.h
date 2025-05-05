#ifndef _RIB_H_
#define _RIB_H_

#include<fstream>
#include "Vtop.h"
// #include "mmu.h"

#define DISPATCH(n,dev) do { \
    this->s##n##_req = 1; \
    this->s##n##_addr = addr - dev##_start_addr; \
    this->s##n##_write_data = top->write_data; \
    this->s##n##_we = top->we; \
    this->s##n##_mem_op_type = top->mem_op_type; \
} while(0)

#define REQ(n) this->s##n##_req

#define READ_DATA(n) this->s##n##_read_data

#define READ_VALID(n) this->s##n##_read_valid

#define SET_INT(n) top->int_port##n=this->int_port##n

#define SET_READ_DATA(n) if(REQ(n)){top->read_data=READ_DATA(n);}

#define ZERO_REQ(n) REQ(n)=0

class mmu;
class rib{
    public:
        mmu* my_mmu;
        uint32_t int_port0=0;
        uint32_t int_port1=0;
        uint32_t int_port2=0;
        uint32_t int_port3=0;
        uint32_t int_port4=0;
        uint32_t int_port5=0;
        uint32_t int_port6=0;
        uint32_t int_port7=0;
        uint32_t int_port8=0;

        uint32_t inst_type_o=0;

        uint32_t s0_addr=0;
        uint32_t s0_write_data=0;
        uint32_t s0_mem_op_type=0;
        uint32_t s0_we=0;
        uint32_t s0_req=0;
        uint32_t s0_read_data=0;
        uint32_t s0_read_valid=0;
        uint32_t s1_addr=0;
        uint32_t s1_write_data=0;
        uint32_t s1_mem_op_type=0;
        uint32_t s1_we=0;
        uint32_t s1_req=0;
        uint32_t s1_read_data=0;
        uint32_t s2_addr=0;
        uint32_t s2_write_data=0;
        uint32_t s2_mem_op_type=0;
        uint32_t s2_we=0;
        uint32_t s2_req=0;
        uint32_t s2_read_data=0;
        uint32_t s2_read_valid=0;
        uint32_t s3_addr=0;
        uint32_t s3_write_data=0;
        uint32_t s3_mem_op_type=0;
        uint32_t s3_we=0;
        uint32_t s3_req=0;
        uint32_t s3_read_data=0;
        uint32_t s4_addr=0;
        uint32_t s4_write_data=0;
        uint32_t s4_mem_op_type=0;
        uint32_t s4_we=0;
        uint32_t s4_req=0;
        uint32_t s4_read_data=0;
        uint32_t s5_addr=0;
        uint32_t s5_write_data=0;
        uint32_t s5_mem_op_type=0;
        uint32_t s5_we=0;
        uint32_t s5_req=0;
        uint32_t s5_read_data=0;
        uint32_t s6_addr=0;
        uint32_t s6_write_data=0;
        uint32_t s6_mem_op_type=0;
        uint32_t s6_we=0;
        uint32_t s6_req=0;
        uint32_t s6_read_data=0;
        uint32_t s7_addr=0;
        uint32_t s7_write_data=0;
        uint32_t s7_mem_op_type=0;
        uint32_t s7_we=0;
        uint32_t s7_req=0;
        uint32_t s7_read_data=0;
        uint32_t s8_addr=0;
        uint32_t s8_write_data=0;
        uint32_t s8_mem_op_type=0;
        uint32_t s8_we=0;
        uint32_t s8_req=0;
        uint32_t s8_read_data=0;

        uint32_t bios_start_addr     = 0x00000000;
        uint32_t ram_start_addr      = 0x00100000;
        uint32_t keyboard_start_addr = 0x10100000;
        uint32_t screen_start_addr   = 0x10200000;
        uint32_t gpu_start_addr      = 0x20200000;
        uint32_t nic_start_addr      = 0x30200000;
        uint32_t flash_start_addr    = 0x40200000;
        uint32_t pmc_start_addr      = 0x60200000;
        uint32_t monitor_start_addr  = 0x60300000;
        uint32_t other_start_addr    = 0x60400000;
        rib();
        ~rib();
        void fresh_port(Vtop* top);
        void get_flag(Vtop* top);
        uint32_t dispatch(Vtop* top,uint32_t addr);
        void set_flag(Vtop* top);
};

#endif // !_RIB_H_