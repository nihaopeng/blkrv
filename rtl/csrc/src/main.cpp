#include<verilated.h>
#include<verilated_vcd_c.h> //可选，如果要导出vcd则需要加上
#include "Vtop.h"  // create `top.v`,so use `Vtop.h`
#include "devices.h"

int main(int argc, char** argv, char** env) {
    std::cout<<"\033[3;1;31mstarting sim...\033[0m"<<std::endl;
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};

    VerilatedVcdC* tfp = new VerilatedVcdC; //初始化VCD对象指针
    contextp->traceEverOn(true); //打开追踪功能
    top->trace(tfp, 0); //
    tfp->open("wave.vcd"); //设置输出的文件wave.vcd
    
    std::cout<<"start initializing devices..."<<std::endl;
    devices mydevices;
    std::cout<<mydevices.my_bios->get4B(0)<<std::endl;
    clock_t start,end;
    start=clock();
    top->clk=0;
    for(int i=0;;i++){

        top->clk=0;
        top->eval();
        // tfp->dump(contextp->time()); //dump wave
        // contextp->timeInc(1);

        mydevices.process(top);

        top->clk=1;
        top->eval();
        // tfp->dump(contextp->time()); //dump wave
        // contextp->timeInc(1);
    }
    end=clock();
    printf("timecost:%f s\n",((double)(end-start))/CLOCKS_PER_SEC);
    delete top;
    tfp->close();
    delete contextp;
    return 0;
}

// int main(int argc, char** argv, char** env) {
//     std::cout<<"\033[3;1;31mstarting sim...\033[0m"<<std::endl;
//     Vtop* top = new Vtop;
    
//     std::cout<<"\033[3;1;31mstart init the mem...\033[0m"<<std::endl;
//     mem ram("./devices/ram",1024*50,0);
//     mem keyboard("./devices/keyboard",4,1);
//     mem screen("./devices/screen",4,2);
//     screen.put4B(0,0);
//     ram.put4B_from_file(0,"./test/interrupt_test.bin");
//     ram.put4B(104,52);
    
//     top->clk=0;
//     for(int i=0;;i++){
//         top->s0_read_valid=0;
//         top->clk=0;
//         top->eval();

//         ram.ram_interface(top);
//         keyboard.ram_interface(top);
//         screen.ram_interface(top);
//         if(top->int_response1==1){
//             top->int_port1=0;
//         }
//         if(kbhit()){
//             top->int_port1=1;
//             char ch=getchar();
// 			std::cout<<ch<<std::endl;
//             keyboard.putB(0,ch);
//             top->s1_read_valid=1;
//         }
//         top->s0_read_valid=1;
//         if(screen.get4B(0)){
//             printf("%c",char(screen.get4B(0)));
//             screen.put4B(0,0);
//         }
//         top->clk=1;
//         top->eval();
//     }
//     delete top;
//     return 0;
// }
