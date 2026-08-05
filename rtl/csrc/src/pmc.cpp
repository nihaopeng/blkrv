#include "pmc.h"

pmc::pmc(uint32_t size):vmem(size){
    this->should_shutdown=0;
}

pmc::~pmc(){

}

uint32_t pmc::read(uint32_t offset, uint8_t op_type){
    this->should_shutdown=1;
    return 0;
}

void pmc::write(uint32_t offset, uint32_t data, uint8_t op_type){
    this->should_shutdown=1;
}
