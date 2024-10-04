#ifndef _NET_CARD_H_
#define _NET_CARD_H_

#include "ram.h"

class net_card:public ram
{
private:
    /* data */
public:
    net_card(std::string mem_file_path,uint32_t size);
    ~net_card();
    void process(Vtop* top);
};

#endif // !_NET_CARD_H_
