#ifndef _MONITOR_H_
#define _MONITOR_H_

#include<string>
#include<fstream>
#include "vmem.h"

class monitor:public vmem
{
public:
    std::string data_path;
    std::fstream fp;
    monitor(uint32_t size,std::string data_path);
    ~monitor();
    void process(Vtop* top,uint32_t tick=0) override;
};

#endif // !_MONITOR_H_
