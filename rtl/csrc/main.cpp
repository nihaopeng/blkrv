#include<verilated.h>
#include<verilated_vcd_c.h> //可选，如果要导出vcd则需要加上
#include "Vtop.h"  // create `top.v`,so use `Vtop.h`
#include "mem.h"

int main(int argc, char** argv, char** env) {
    std::cout<<"\033[3;1;31mstarting sim...\033[0m"<<std::endl;
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};

    VerilatedVcdC* tfp = new VerilatedVcdC; //初始化VCD对象指针
    contextp->traceEverOn(true); //打开追踪功能
    top->trace(tfp, 0); //
    tfp->open("wave.vcd"); //设置输出的文件wave.vcd
    
    std::cout<<"\033[3;1;31mstart init the mem...\033[0m"<<std::endl;
    mem ram("./devices/ram",1024*50);
    ram.put4B_from_file(0,"./test/test.asm");
    
    top->clk=0;
    for(int i=0;i<100;i++){
        top->clk=0;
        // if(top->s0_we){
        //     ram.put4B()
        // }
        top->s0_read_valid=0;
        top->eval();
        tfp->dump(contextp->time()); //dump wave
        contextp->timeInc(1);
        if(top->s0_we){
            if(top->s0_mem_op_type==0){
                ram.putB(top->s0_addr,top->s0_write_data);
            }else if(top->s0_mem_op_type==1){
                ram.put2B(top->s0_addr,top->s0_write_data);
            }else if(top->s0_mem_op_type==2){
                ram.put4B(top->s0_addr,top->s0_write_data);
            }
        }else{
            if(top->s0_mem_op_type==0){
                top->s0_read_data=uint32_t(ram.getB(top->s0_addr));
            }else if(top->s0_mem_op_type==1){
                top->s0_read_data=uint32_t(ram.get2B(top->s0_addr));
            }else if(top->s0_mem_op_type==2){
                top->s0_read_data=uint32_t(ram.get4B(top->s0_addr));
            }
        }
        top->s0_read_valid=1;
        top->clk=1;
        top->eval();
        tfp->dump(contextp->time()); //dump wave
        contextp->timeInc(1);
    }
    delete top;
    tfp->close();
    delete contextp;
    return 0;
}
