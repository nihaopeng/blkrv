#include "nvmem.h"
#include <iostream>
#include <stdexcept>

// 页缓存实现 (纯 C++ STL, 可移植, 不依赖 mmap):
// - 读: 命中缓存=内存速度; 未命中=一次 4KB 文件读
// - 写: 更新缓存并标记脏页, sync()/析构时统一写回
static const uint32_t NVMEM_PAGE = 4096;

nvmem::nvmem(std::string mem_file_path,uint32_t size){
    this->size=size;
    this->mem_file_path=mem_file_path;
    this->fp.open(mem_file_path,std::ios::binary|std::ios::in|std::ios::out);
    if(!this->fp){
        // 文件不存在: 创建并填零
        this->fp.close();
        std::ofstream out(mem_file_path,std::ios::binary);
        std::vector<uint8_t> zero(NVMEM_PAGE,0);
        uint32_t left=size;
        while(left>0){
            uint32_t n=left<NVMEM_PAGE?left:NVMEM_PAGE;
            out.write(reinterpret_cast<const char*>(zero.data()),n);
            left-=n;
        }
        out.close();
        this->fp.open(mem_file_path,std::ios::binary|std::ios::in|std::ios::out);
    }
    if(!this->fp){
        std::cout<<"can not open the file:"<<mem_file_path<<std::endl;
        throw std::runtime_error("file open error");
    }
    if(size%4!=0){
        std::cout<<"The memory size you specified is not a multiple of 4, please modify it"<<std::endl;
        throw std::runtime_error("mem alloc error");
    }
}

nvmem::~nvmem(){
    this->sync();
}

// 取页: 未命中则从文件读入 (尾部不足 4KB 的部分按实际长度读, 其余补零)
std::vector<uint8_t>& nvmem::page(uint32_t idx){
    auto it=this->cache.find(idx);
    if(it==this->cache.end()){
        std::vector<uint8_t> p(NVMEM_PAGE,0);
        this->fp.seekg((std::streamoff)idx*NVMEM_PAGE);
        uint32_t off=idx*NVMEM_PAGE;
        uint32_t left=off<this->size?this->size-off:0;
        uint32_t n=left<NVMEM_PAGE?left:NVMEM_PAGE;
        if(n>0) this->fp.read(reinterpret_cast<char*>(p.data()),n);
        it=this->cache.emplace(idx,std::move(p)).first;
    }
    return it->second;
}

uint8_t nvmem::getB(uint32_t pointer){
    return this->page(pointer/NVMEM_PAGE)[pointer%NVMEM_PAGE];
}

uint16_t nvmem::get2B(uint32_t pointer){
    return (uint16_t)(this->getB(pointer) | (this->getB(pointer+1)<<8));
}

uint32_t nvmem::get4B(uint32_t pointer){
    return (uint32_t)this->getB(pointer)
         | ((uint32_t)this->getB(pointer+1)<<8)
         | ((uint32_t)this->getB(pointer+2)<<16)
         | ((uint32_t)this->getB(pointer+3)<<24);
}

void nvmem::putB(uint32_t pointer,uint8_t data){
    this->page(pointer/NVMEM_PAGE)[pointer%NVMEM_PAGE]=data;
    this->dirty.insert(pointer/NVMEM_PAGE);
}

void nvmem::put2B(uint32_t pointer,uint16_t data){
    this->putB(pointer,(uint8_t)(data&0xff));
    this->putB(pointer+1,(uint8_t)((data>>8)&0xff));
}

void nvmem::put4B(uint32_t pointer,uint32_t data){
    this->putB(pointer,(uint8_t)(data&0xff));
    this->putB(pointer+1,(uint8_t)((data>>8)&0xff));
    this->putB(pointer+2,(uint8_t)((data>>16)&0xff));
    this->putB(pointer+3,(uint8_t)((data>>24)&0xff));
}

void nvmem::sync(){
    for(uint32_t idx:this->dirty){
        uint32_t off=idx*NVMEM_PAGE;
        uint32_t left=off<this->size?this->size-off:0;
        uint32_t n=left<NVMEM_PAGE?left:NVMEM_PAGE;
        this->fp.seekp((std::streamoff)off);
        this->fp.write(reinterpret_cast<const char*>(this->cache[idx].data()),n);
    }
    this->fp.flush();
    this->dirty.clear();
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
