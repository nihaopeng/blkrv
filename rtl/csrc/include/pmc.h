#ifndef _PMC_H_
#define _PMC_H_

#include "vmem.h"

class pmc:public vmem
{
public:
    int should_shutdown;   // 电源管理: 一旦被访问即请求关机
    pmc(uint32_t size);
    ~pmc();
    uint32_t read(uint32_t offset, uint8_t op_type) override;
    void write(uint32_t offset, uint32_t data, uint8_t op_type) override;
};

#endif // !_PMC_H_
