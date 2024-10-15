#include "mem.h"

mem::mem(std::string mem_file_path,uint32_t size){//unit of 'size' is BYTE
    this->mem_file_path=mem_file_path;
    if(access((mem_file_path).c_str(),F_OK)!=0){//file not exist
        this->fp.open(mem_file_path,std::ios::binary|std::ios::out);
        uint32_t d=0;
        for(uint64_t i=0;i<size;i+=4){
            printf("\r(size:%dByte)process:%f/100",size,(float(i+4)/size)*100);
            this->fp.seekp(i);
            this->fp.write(reinterpret_cast<const char*>(&d),sizeof(d));
        }
        std::cout<<std::endl;
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

mem::~mem(){
    this->fp.close();
}

void mem::process(Vtop* top){}

void mem::put4B_from_file(uint32_t start_pointer,std::string file_path){
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
        fp.seekp(pointer);
        fp.write(reinterpret_cast<const char*>(&value), sizeof(value));
        fp.flush();
        std::cout << "The value is: " << std::hex << value << std::endl;
        // fp.seekg(pointer);
        // uint32_t data;
        // fp.read(reinterpret_cast<char*>(&data), sizeof(data));
        // std::cout<<data<<std::endl;
        pointer+=4;
    }
    
    file.close();
}

void mem::sync(){
    this->fp.close();
    this->fp.open(this->mem_file_path,std::ios::binary|std::ios::out|std::ios::in);
}