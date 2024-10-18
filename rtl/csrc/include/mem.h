#ifndef _MEM_H_
#define _MEM_H_

#include<fstream>
#include<string>
#include<iostream>
#include<cstdlib>
#include<sstream>
#include "Vtop.h"

class mem{
    public:
        uint32_t size;
        mem();
        ~mem();
        virtual void process(Vtop* top);
};
#endif // !_MEM_H_

