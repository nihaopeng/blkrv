#ifndef _TIMER_H_
#define _TIMER_H_

#include "bus.h"

// CLINT-style timer device.  Fires an interrupt when mtime >= mtimecmp.
// Registered at 0x10000000 on IRQ 7 (shared by both backends).
class Timer : public Device {
public:
    Timer(uint32_t size, uint32_t interval_ticks);
    int process(Bus* bus, uint32_t tick);

    uint32_t read(uint32_t offset, uint8_t op_type) override;
    void write(uint32_t offset, uint32_t data, uint8_t op_type) override;

private:
    uint64_t mtime;          // current tick count
    uint64_t mtimecmp;       // compare value (0 = disabled)
    uint32_t interval;       // default auto-reload interval
};

#endif // _TIMER_H_
