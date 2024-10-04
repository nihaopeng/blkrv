#ifndef _RAM_H_
#define _RAM_H_

#include "mem_rw.h"

class ram:public mem_rw
{
public:
    ram(std::string mem_file_path,uint32_t size);
    ~ram();
    void process(Vtop* top) override;
};

#endif // !_RAM_H_

