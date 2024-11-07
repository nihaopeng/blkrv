#ifndef _GPU_H_
#define _GPU_H_


class gpu:public vmem
{
private:
    int if_start_up;
public:
    
    gpu(uint32_t size/* args */);
    ~gpu();
    void process(Vtop* top);
    static void* gpu::thread_function(void* arg)
};

#endif // !_GPU_H_