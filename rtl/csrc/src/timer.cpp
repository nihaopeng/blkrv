#include "timer.h"

// Default interval: 100000 ticks (~100ms at typical sim speed)
Timer::Timer(uint32_t size, uint32_t interval_ticks) {
    mtime = 0;
    mtimecmp = 0;
    interval = interval_ticks;
}

// Called every tick from devices::process()
int Timer::process(Bus* bus, uint32_t tick) {
    mtime = tick;
    // Clear IRQ first (level-sensitive)
    bus->set_irq(7, false);
    // Fire if mtimecmp is set and mtime reached it
    if (mtimecmp != 0 && mtime >= mtimecmp) {
        bus->set_irq(7, true);
        // Auto-reload for periodic ticks
        mtimecmp += interval;
    }
    return 0;
}

uint32_t Timer::read(uint32_t offset, uint8_t op_type) {
    (void)op_type;
    switch (offset) {
        case 0:  return (uint32_t)(mtime & 0xffffffff);       // mtime_lo
        case 4:  return (uint32_t)(mtime >> 32);              // mtime_hi
        case 8:  return (uint32_t)(mtimecmp & 0xffffffff);    // mtimecmp_lo
        case 12: return (uint32_t)(mtimecmp >> 32);           // mtimecmp_hi
        default: return 0;
    }
}

void Timer::write(uint32_t offset, uint32_t data, uint8_t op_type) {
    (void)op_type;
    switch (offset) {
        case 0:  mtime = data; break;                         // mtime_lo (updates full mtime? simplified)
        case 4:  mtime = ((uint64_t)data << 32); break;       // mtime_hi
        case 8:  mtimecmp = (mtimecmp & 0xffffffff00000000ULL) | data; break;  // mtimecmp_lo
        case 12: mtimecmp = (mtimecmp & 0xffffffffULL) | ((uint64_t)data << 32); break; // mtimecmp_hi
        default: break;
    }
}
