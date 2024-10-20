#include "pmc.h"

pmc::pmc(uint32_t size):vmem(size){

}

pmc::~pmc(){
    
}

int pmc::powm(Vtop* top){
    if(top->s7_req){
        return -1;
    }
    return 0;
}