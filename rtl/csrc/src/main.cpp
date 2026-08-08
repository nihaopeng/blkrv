#include <iostream>
#include <ctime>
#include <csignal>
#include <cstdlib>
#include "bus.h"
#include "devices.h"
#include "mmu.h"
#include "monitor.h"
#include "utils.h"

#ifdef BACKEND_SIM
#include "cpu_sim.h"
#else
#include <verilated.h>
#include <verilated_vcd_c.h> //可选，如果要导出vcd则需要加上
#include "Vtop.h"  // create `top.v`,so use `Vtop.h`
#endif

// Ctrl+C / kill 时先恢复终端, 避免退出后终端停留在 raw 模式无法输入
static void sig_restore_tty(int){
    utils::restore_tty();
    _exit(130);
}

int main(int argc, char** argv, char** env) {
    std::cout<<"\033[3;1;31mstarting sim...\033[0m"<<std::endl;

    signal(SIGINT, sig_restore_tty);
    signal(SIGTERM, sig_restore_tty);

    Bus my_bus;
    devices my_devices(&my_bus);
    mmu my_mmu(&my_bus);

    monitor* my_monitor;
    if(argc>=2)
        my_monitor=new monitor(1<<28,argv[1]);
    else
        my_monitor=new monitor(1<<28,"../data.txt");
    my_bus.register_dev(my_monitor,0x60300000,1<<20,0xFF);

    clock_t start,end;
    start=clock();
    int i=0;

#ifdef BACKEND_SIM
    CpuSim cpu;
    CpuSim& p=cpu;
    uint32_t last_satp=0;
    for(;;i++){
        p.eval();//执行一个周期: 消费上一请求的响应, 发出新请求

        if(p.sfence_pending){// sfence.vma: 按 ASID 主动失效 (与 satp 变化全量刷互为兜底)
            my_mmu.my_tlb.flush_asid(p.sfence_asid);
            p.sfence_pending = false;
        }
        if(p.satp != last_satp){// satp 变化 = 进程切换/进出内核, TLB 必须刷新
            my_mmu.my_tlb.flush();
            last_satp = p.satp;
        }
        uint32_t phys=my_mmu.convert(p.load_addr_v,p.satp);
        if(p.we){
            my_bus.write(phys,p.write_data,p.mem_op_type);
            p.read_data=0;
        }else{
            p.read_data=my_bus.read(phys,p.mem_op_type);
        }
        p.read_valid=1;
        p.int_port0=my_bus.get_irq(0);
        p.int_port1=my_bus.get_irq(1);
        p.int_port2=my_bus.get_irq(2);
        p.int_port3=my_bus.get_irq(3);
        p.int_port4=my_bus.get_irq(4);
        p.int_port5=my_bus.get_irq(5);
        p.int_port6=my_bus.get_irq(6);
        p.int_port7=my_bus.get_irq(7);
        p.int_port8=my_bus.get_irq(8);
        my_monitor->process(&my_bus,&my_mmu,main_time,p.inst_type_o);
        if(my_devices.process(&my_bus,i)){
            break;
        }
        // 周期性同步: 把 nvmem 脏页写回文件, 避免异常终止丢失最近写入
        if((i & 0x3FFFF) == 0){
            my_devices.my_flash->sync();
            my_devices.my_bios->sync();
        }
    }
#else
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop* top = new Vtop{contextp};

    VerilatedVcdC* tfp = new VerilatedVcdC; //初始化VCD对象指针
    contextp->traceEverOn(true); //打开追踪功能
    top->trace(tfp, 0); //
    tfp->open("wave.vcd"); //设置输出的文件wave.vcd

    std::cout<<"start initializing devices..."<<std::endl;
    top->clk=0;
    Vtop& p=*top;
    uint32_t last_satp=0;
    for(;;i++){
        p.clk=0;
        p.eval();
        if(p.sfence_flag_o){// sfence.vma: 按 ASID 主动失效
            my_mmu.my_tlb.flush_asid(p.sfence_asid_o);
        }
        if(p.satp != last_satp){// satp 变化 = 进程切换/进出内核, TLB 必须刷新
            my_mmu.my_tlb.flush();
            last_satp = p.satp;
        }
        //经过mmu转换虚址后，将数据请求发给各设备
        uint32_t phys=my_mmu.convert(p.load_addr_v,p.satp);
        if(p.we){
            my_bus.write(phys,p.write_data,p.mem_op_type);
            p.read_data=0;
        }else{
            p.read_data=my_bus.read(phys,p.mem_op_type);
        }
        p.read_valid=1;
        p.int_port0=my_bus.get_irq(0);
        p.int_port1=my_bus.get_irq(1);
        p.int_port2=my_bus.get_irq(2);
        p.int_port3=my_bus.get_irq(3);
        p.int_port4=my_bus.get_irq(4);
        p.int_port5=my_bus.get_irq(5);
        p.int_port6=my_bus.get_irq(6);
        p.int_port7=my_bus.get_irq(7);
        p.int_port8=my_bus.get_irq(8);
        my_monitor->process(&my_bus,&my_mmu,main_time,p.inst_type_o);
        if(my_devices.process(&my_bus,i)){
            break;
        }
        if((i & 0x3FFFF) == 0){// 周期性同步 nvmem 脏页
            my_devices.my_flash->sync();
            my_devices.my_bios->sync();
        }

        p.clk=1;
        p.eval();
        if(p.sfence_flag_o){
            my_mmu.my_tlb.flush_asid(p.sfence_asid_o);
        }
    }
    delete top;
    tfp->close();
    delete contextp;
#endif

    end=clock();
    utils::restore_tty();   // 正常退出 (shell exit) 前恢复宿主终端状态
    printf("ticktimes:%d,timecost:%f s\ndevices shuting down...\n",i,((double)(end-start))/CLOCKS_PER_SEC);
    return 0;
}
