#include "pmc.h"

pmc::pmc(uint32_t size):vmem(size){

}

pmc::~pmc(){
    
}

int pmc::process(rib* rib,uint32_t tick){
    if(rib->s7_req){
        return -1;
    }
    return 0;
}