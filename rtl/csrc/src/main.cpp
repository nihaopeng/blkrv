#include<verilated.h>
#include<verilated_vcd_c.h> //可选，如果要导出vcd则需要加上
#include "Vtop.h"  // create `top.v`,so use `Vtop.h`
#include "devices.h"

extern vluint64_t main_time;

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
    
    
    // mydevices.my_gpu->put4B(0x800001,50);//x0
    // mydevices.my_gpu->put4B(0x800005,50);//y0
    // mydevices.my_gpu->put4B(0x800009,100);//x1
    // mydevices.my_gpu->put4B(0x80000d,100);//y1
    // mydevices.my_gpu->put4B(0x800011,255);//b
    // mydevices.my_gpu->put4B(0x800012,255);//g
    // mydevices.my_gpu->put4B(0x800013,255);//r
    // mydevices.my_gpu->put4B(0x800014,255);//a
    // mydevices.my_gpu->put4B(0,2);

    clock_t start,end;
    start=clock();
    top->clk=0;
    int i=0;
    for(;;i++){

        top->clk=0;
        top->eval();
        // tfp->dump(main_time); //dump wave
        // main_time+=1;


        // if(top->s1_req&&top->s1_addr==0x00014f48){
        //     // system("sync");
        //     printf("w:%x cnt:%d val:%d\n",top->s1_write_data,i,mydevices.my_flash->get4B(top->s1_addr));
        // }
        // if(top->s1_req&&top->s1_addr==0x00015234){
        //     // system("sync");
        //     printf("w:%x cnt:%d val:%d\n",top->s1_write_data,i,mydevices.my_flash->get4B(top->s1_addr));
        // }
        // if(i>=43858737){
        //     printf("inst:%x,%x\n",top->s1_addr,top->s2_addr);
        // }
        mydevices.process(top);
        // if(top->s1_req&&top->s1_addr==0x00000ad8){
        //     printf("test1 val:%d\n",mydevices.my_ram->get4B(top->s1_addr));
        // }
        if(mydevices.my_pmc->powm(top)){
            break;
        }
        // if(top->s1_req&&top->s1_addr==0x00000ad8){
        //     printf("test2 val:%d\n",mydevices.my_ram->get4B(top->s1_addr));
        // }
        top->clk=1;
        top->eval();
        // if(top->s1_req&&top->s1_addr==0x00000ad8){
        //     printf("test3 val:%d\n",mydevices.my_ram->get4B(top->s1_addr));
        // }
        // tfp->dump(main_time); //dump wave
        // main_time+=1;

        // tfp->dump(contextp->time()); //dump wave
        // contextp->timeInc(1);
    }
    end=clock();
    printf("ticktimes:%d,timecost:%f s\ndevices shuting down...\n",i,((double)(end-start))/CLOCKS_PER_SEC);
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
