#ifndef _SCREEN_H_
#define _SCREEN_H_

#include"ram.h"

class screen:public ram
{
public:
    screen(std::string mem_file_path,uint32_t size);
    ~screen();
    void process(Vtop* top) override;
};

#endif // !_SCREEN_H_
