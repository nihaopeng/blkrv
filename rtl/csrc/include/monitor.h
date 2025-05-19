#ifndef _MONITOR_H_
#define _MONITOR_H_

#include<string>
#include<fstream>
#include <chrono>
#include "rib.h"
#include "mmu.h"

class monitor
{
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    uint32_t size;
    uint8_t* mem_space;
    uint32_t is_enable=0;
    uint64_t lui_times=0,auipc_times=0,jal_times=0,jalr_times=0,bj_times=0,load_times=0,
    store_times=0,calc_times=0,calci_times=0,sys_times=0;
    uint64_t s0_req=0,s1_req=0,s2_req=0,s3_req=0,s4_req=0,s5_req=0,s6_req=0;
    uint64_t align_times=0;
    uint64_t none_align_times=0;
    uint64_t hit_times=0;
    uint64_t mmu_enable_times=0;
    uint64_t sum_ticks=0;
    std::string data_path;
    std::fstream fp;
    std::fstream fp_bin;
    monitor(uint32_t size,std::string data_path);
    ~monitor();
    uint8_t getB(uint32_t pointer);
    uint16_t get2B(uint32_t pointer);
    uint32_t get4B(uint32_t pointer);
    void putB(uint32_t pointer,uint8_t data);
    void put2B(uint32_t pointer,uint16_t data);
    void put4B(uint32_t pointer,uint32_t data);
    int process(rib* rib,mmu* my_mmu,uint32_t tick=0);
};

#endif // !_MONITOR_H_
