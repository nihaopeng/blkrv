#include "nvmem.h"

nvmem::nvmem(std::string mem_file_path,uint32_t size){
    this->size=size;
    this->mem_file_path=mem_file_path;
    if(access((mem_file_path).c_str(),F_OK)!=0){//file not exist
        this->fp.open(mem_file_path,std::ios::binary|std::ios::out);
        // uint32_t d=0;
        // for(uint64_t i=0;i<size;i+=4){
        //     printf("\r(size:%dByte)process:%f/100",size,(float(i+4)/size)*100);
        //     this->fp.seekp(i);
        //     this->fp.write(reinterpret_cast<const char*>(&d),sizeof(d));
        // }
        char* write_data=(char*)malloc(size);
        if (write_data != NULL) {
            memset(write_data, 0, size); // 初始化为0
        }
        this->fp.write(write_data,size);
        this->fp.close();
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

int nvmem::process(rib* rib,uint32_t tick){
    return 0;
}