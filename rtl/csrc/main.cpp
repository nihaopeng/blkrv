#include<iostream>
#include<verilated.h>
#include<verilated_vcd_c.h> //可选，如果要导出vcd则需要加上
#include "Vtop.h"  // create `top.v`,so use `Vtop.h`

int main(int argc, char** argv, char** env) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};

    VerilatedVcdC* tfp = new VerilatedVcdC; //初始化VCD对象指针
    contextp->traceEverOn(true); //打开追踪功能
    top->trace(tfp, 0); //
    tfp->open("wave.vcd"); //设置输出的文件wave.vcd
 
    top->clk=0;
    for(int i=0;i<100;i++){
        top->clk=!top->clk;
        top->eval();
        tfp->dump(contextp->time()); //dump wave
        contextp->timeInc(1);
    }
    delete top;
    tfp->close();
    delete contextp;
    return 0;
}