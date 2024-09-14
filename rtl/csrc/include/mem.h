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
        std::ifstream inf;
        std::ofstream ouf;
        mem(std::string mem_file_path,uint64_t size,uint8_t port);
        ~mem();
        uint32_t get4B(uint64_t pointer);
        uint16_t get2B(uint64_t pointer);
        uint8_t getB(uint64_t pointer);
        void put4B(uint64_t pointer,uint32_t data);
        void put2B(uint64_t pointer,uint16_t data);
        void putB(uint64_t pointer,uint8_t data);
        void put4B_from_file(uint64_t start_pointer,std::string file_path);
        void ram_interface(Vtop* top);
};
#endif // !_MEM_H_

