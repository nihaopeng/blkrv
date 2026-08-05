#include "bus.h"

uint64_t main_time = 0;

void Bus::register_dev(Device* dev, uint32_t base, uint32_t size, uint8_t irq) {
    dev->bus = this;
    DevEntry entry = {dev, base, size, irq};
    devs.push_back(entry);
}

Device* Bus::resolve(uint32_t addr, uint32_t* offset) {
    for (size_t n = 0; n < devs.size(); n++) {
        DevEntry& e = devs[n];
        if (addr >= e.base && addr < e.base + e.size) {
            if (offset) *offset = addr - e.base;
            return e.dev;
        }
    }
    return nullptr;
}

uint32_t Bus::read(uint32_t addr, uint8_t op_type) {
    uint32_t offset;
    Device* dev = resolve(addr, &offset);
    if (!dev) return 0;
    for (size_t n = 0; n < devs.size() && n < 9; n++) {
        if (devs[n].dev == dev) req_count[n]++;
    }
    last_addr = addr;
    return dev->read(offset, op_type);
}

void Bus::write(uint32_t addr, uint32_t data, uint8_t op_type) {
    uint32_t offset;
    Device* dev = resolve(addr, &offset);
    if (!dev) return;
    for (size_t n = 0; n < devs.size() && n < 9; n++) {
        if (devs[n].dev == dev) req_count[n]++;
    }
    last_addr = addr;
    dev->write(offset, data, op_type);
}

void Bus::set_irq(uint8_t n, bool v) {
    if (n < 9) irq[n] = v;
}

bool Bus::get_irq(uint8_t n) const {
    return n < 9 ? irq[n] : false;
}
