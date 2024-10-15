#ifndef _MEM_H_
#define _MEM_H_

#include<fstream>
#include<string>
#include<iostream>
#include<cstdlib>
#include<sstream>
#include "Vtop.h"

class mem{
    public:
        uint8_t port;
        std::string mem_file_path;
        std::fstream fp;
        mem(std::string mem_file_path,uint32_t size);
        ~mem();
        virtual void process(Vtop* top);
        void put4B_from_file(uint32_t start_pointer,std::string file_path);
        void sync();
};
#endif // !_MEM_H_

