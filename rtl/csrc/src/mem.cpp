#include "mem.h"

mem::mem(std::string mem_file_path,uint64_t size,uint8_t port){//unit of 'size' is BYTE
    this->port=port;
    this->inf.open(mem_file_path,std::ios::binary|std::ios::in);
    this->ouf.open(mem_file_path,std::ios::binary|std::ios::out);
    if (!this->ouf||!this->inf) {
        std::cerr << "can not open the file" << std::endl;
        throw std::runtime_error("file open error");
    }
    if(size%4!=0){
        std::cout<<"The memory size you specified is not a multiple of 4, please modify it"<<std::endl;
        throw std::runtime_error("mem alloc error");
    }
    uint32_t d=0;
    //from the start to init the mem;
    for(uint64_t i=0;i<size;i+=4){
        this->ouf.seekp(i);
        this->ouf.write(reinterpret_cast<const char*>(&d),sizeof(d));
    }
}

mem::~mem(){
    this->inf.close();
    this->ouf.close();
}

uint32_t mem::get4B(uint64_t pointer){
    inf.seekg(pointer);
    uint32_t data;
    inf.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint16_t mem::get2B(uint64_t pointer){
    inf.seekg(pointer);
    uint16_t data;
    inf.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

uint8_t mem::getB(uint64_t pointer){
    inf.seekg(pointer);
    uint8_t data;
    inf.read(reinterpret_cast<char*>(&data), sizeof(data));
    return data;
}

void mem::put4B(uint64_t pointer,uint32_t data){
    ouf.seekp(pointer);
    ouf.write(reinterpret_cast<const char*>(&data), sizeof(data));
    ouf.flush();
}

void mem::put2B(uint64_t pointer,uint16_t data){
    ouf.seekp(pointer);
    ouf.write(reinterpret_cast<const char*>(&data), sizeof(data));
    ouf.flush();
}

void mem::putB(uint64_t pointer,uint8_t data){
    ouf.seekp(pointer);
    ouf.write(reinterpret_cast<const char*>(&data), sizeof(data));
    ouf.flush();
}

void mem::put4B_from_file(uint64_t start_pointer,std::string file_path){
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        throw std::runtime_error("fail to open file");
    }
    std::string line;
    uint32_t value;
    uint64_t pointer=start_pointer;
    while (std::getline(file, line)) {
        // 将十六进制字符串转换为无符号整数
        std::stringstream ss(line);
        ss >> std::hex >> value;
        // 打印或使用value
        this->put4B(pointer,value);
        std::cout << "The value is: " << std::hex << value << std::endl;
        pointer+=4;
    }
    file.close();
}

void mem::ram_interface(Vtop* top){
    switch (this->port)
    {
        case /* constant-expression */0:
        if(top->s0_req){
            if(top->s0_we){
                if(top->s0_mem_op_type==0){
                    this->putB(top->s0_addr,top->s0_write_data);
                    }else if(top->s0_mem_op_type==1){
                        this->put2B(top->s0_addr,top->s0_write_data);
                    }else if(top->s0_mem_op_type==2){
                        this->put4B(top->s0_addr,top->s0_write_data);
                    }
            }else{
                if(top->s0_mem_op_type==0){
                    top->s0_read_data=uint32_t(this->getB(top->s0_addr));
                }else if(top->s0_mem_op_type==1){
                    top->s0_read_data=uint32_t(this->get2B(top->s0_addr));
                }else if(top->s0_mem_op_type==2){
                    top->s0_read_data=uint32_t(this->get4B(top->s0_addr));
                }
            }
        }
        /* code */
        break;
    case 1:
    if(top->s1_req){
        if(top->s1_we){
            if(top->s1_mem_op_type==0){
                this->putB(top->s1_addr,top->s1_write_data);
            }else if(top->s1_mem_op_type==1){
                this->put2B(top->s1_addr,top->s1_write_data);
            }else if(top->s1_mem_op_type==2){
                this->put4B(top->s1_addr,top->s1_write_data);
            }
        }else{
            if(top->s1_mem_op_type==0){
                top->s1_read_data=uint32_t(this->getB(top->s1_addr));
            }else if(top->s1_mem_op_type==1){
                top->s1_read_data=uint32_t(this->get2B(top->s1_addr));
            }else if(top->s1_mem_op_type==2){
                top->s1_read_data=uint32_t(this->get4B(top->s1_addr));
            }
        }
    }
    break;
    case 2:
    if(top->s2_req){
        if(top->s2_we){
            if(top->s2_mem_op_type==0){
                this->putB(top->s2_addr,top->s2_write_data);
            }else if(top->s2_mem_op_type==1){
                this->put2B(top->s2_addr,top->s2_write_data);
            }else if(top->s2_mem_op_type==2){
                this->put4B(top->s2_addr,top->s2_write_data);
            }
        }else{
            if(top->s2_mem_op_type==0){
                top->s2_read_data=uint32_t(this->getB(top->s2_addr));
            }else if(top->s2_mem_op_type==1){
                top->s2_read_data=uint32_t(this->get2B(top->s2_addr));
            }else if(top->s2_mem_op_type==2){
                top->s2_read_data=uint32_t(this->get4B(top->s2_addr));
            }
        }
    }
    break;
    default:
        break;
    }
    
}