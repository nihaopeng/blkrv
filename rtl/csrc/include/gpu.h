#ifndef _GPU_H_
#define _GPU_H_
#include "vmem.h"
#include "tgaimage.h"
#include "draw2d.h"
#include <atomic>

class gpu:public vmem
{
private:
    int if_start_up;
public:
    gpu(uint32_t size);
    ~gpu();
    void process(Vtop* top);
    static void* gpu::thread_function(void* arg);
    
};

#endif // !_GPU_H_