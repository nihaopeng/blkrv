#include "nvmem.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

nvmem::nvmem(std::string mem_file_path,uint32_t size){
    this->size=size;
    this->mem_file_path=mem_file_path;
    std::ifstream probe(mem_file_path);
    if(!probe.good()){//file not exist
        probe.close();
        this->fp.open(mem_file_path,std::ios::binary|std::ios::out);
        char* write_data=(char*)malloc(size);
        if (write_data != NULL) {
            memset(write_data, 0, size); // 初始化为0
        }
        this->fp.write(write_data,size);
        this->fp.close();
        free(write_data);
    }else{
        this->fp.open(mem_file_path,std::ios::binary|std::ios::out|std::ios::in);//同时使用in和out是为了修改某一部分内容而不清空文件
    }
    if (!this->fp) {
        // system("pwd");
        std::cout << "can not open the file:"<<mem_file_path << std::endl;
        throw std::runtime_error("file open error");
    }
    if(size%4!=0){
        std::cout<<"The memory size you specified is not a multiple of 4, please modify it"<<std::endl;
        throw std::runtime_error("mem alloc error");
    }
}

nvmem::~nvmem(){}

void nvmem::put4B(uint32_t pointer,uint32_t data){
    fp.seekp(pointer);
    fp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    fp.flush();
}

void nvmem::put2B(uint32_t pointer,uint16_t data){
    fp.seekp(pointer);
    fp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    fp.flush();
}

void nvmem::putB(uint32_t pointer,uint8_t data){
    fp.seekp(pointer);
    fp.write(reinterpret_cast<const char*>(&data), sizeof(data));
    fp.flush();
}

uint32_t nvmem::get4B(uint32_t pointer){
    fp.seekg(pointer);
    uint32_t data;
    fp.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint16_t nvmem::get2B(uint32_t pointer){
    fp.seekg(pointer);
    uint16_t data;
    fp.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint8_t nvmem::getB(uint32_t pointer){
    fp.seekg(pointer);
    uint8_t data;
    fp.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

void nvmem::sync(){
    this->fp.close();
    this->fp.open(this->mem_file_path,std::ios::binary|std::ios::out|std::ios::in);
}

uint32_t nvmem::read(uint32_t offset, uint8_t op_type){
    switch(op_type){
        case 0:return uint8_t(this->getB(offset));
        case 1:return uint16_t(this->get2B(offset));
        case 2:return uint32_t(this->get4B(offset));
        default:return 0;
    }
}

void nvmem::write(uint32_t offset, uint32_t data, uint8_t op_type){
    switch(op_type){
        case 0:this->putB(offset,uint8_t(data));break;
        case 1:this->put2B(offset,uint16_t(data));break;
        case 2:this->put4B(offset,uint32_t(data));break;
        default:break;
    }
}
