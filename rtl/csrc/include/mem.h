#include<fstream>
#include<string>
#include<iostream>
#include<cstdlib>

class mem{
    public:
        std::string mem_file_path;
        std::ifstream inf;
        std::ofstream ouf;
        mem(std::string mem_file_path,uint64_t size);
        ~mem();
        uint32_t get4B(uint64_t pointer);
        uint16_t get2B(uint64_t pointer);
        uint8_t getB(uint64_t pointer);
        void put4B(uint64_t pointer,uint32_t data);
        void put2B(uint64_t pointer,uint16_t data);
        void putB(uint64_t pointer,uint8_t data);
};