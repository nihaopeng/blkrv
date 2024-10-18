#ifndef _NET_CARD_H_
#define _NET_CARD_H_

#include "vmem.h"

class net_card:public vmem
{
private:
    /* data */
public:
    net_card(uint32_t size);
    ~net_card();
    void process(Vtop* top);
};

#endif // !_NET_CARD_H_
