#include "flash.h"

flash::flash(std::string mem_file_path,uint32_t size):nvmem(mem_file_path,size){
}

flash::~flash(){

}
