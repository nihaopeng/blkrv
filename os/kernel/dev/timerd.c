#include "drivers.h"
#include "proc.h"

// timer_interrupt_i 必须和 _regist_syscall 在同一文件!
// PIE + --no-relax 下跨模块取地址走 GOT, GOT 未重定位, 同模块 PC-relative 方可正确解析.
// GOT 重定位作为独立任务后续处理.

__attribute__((visibility("hidden")))
void timer_interrupt_i(void) {
    //printk("T");
    scheduler();
}

_regist_syscall(void, timer_interrupt);
