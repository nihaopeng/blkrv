#include "bus.h"
#include <algorithm>

uint64_t main_time = 0;

void Bus::register_dev(Device* dev, uint32_t base, uint32_t size, uint8_t irq) {
    dev->bus = this;
    DevEntry entry = {dev, base, size, irq};
    devs.push_back(entry);
    // 重建按 base 升序的解码索引 (设备数量很少, 每次注册重建开销可忽略;
    // 稳定排序保证同 base 时先注册者优先, 与旧线性扫描语义一致)
    decode_idx.resize(devs.size());
    for(size_t n = 0; n < devs.size(); n++) decode_idx[n] = (uint32_t)n;
    std::stable_sort(decode_idx.begin(), decode_idx.end(),
                     [&](uint32_t a, uint32_t b){ return devs[a].base < devs[b].base; });
}

Device* Bus::resolve(uint32_t addr, uint32_t* offset, size_t* out_idx) {
    // 二分查找最后一个 base <= addr 的设备
    size_t lo = 0, hi = decode_idx.size();
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (devs[decode_idx[mid]].base <= addr) lo = mid + 1;
        else hi = mid;
    }
    // decode_idx[0..lo) 的 base 均 <= addr, 按 base 降序检查区间包含
    // (本项目设备区间互不重叠, 通常第一次比较即命中)
    while (lo > 0) {
        size_t n = decode_idx[lo - 1];
        const DevEntry& e = devs[n];
        if (addr < e.base + e.size) {
            if (offset) *offset = addr - e.base;
            if (out_idx) *out_idx = n;
            return e.dev;
        }
        lo--;
    }
    return nullptr;
}

Device* Bus::resolve(uint32_t addr, uint32_t* offset) {
    return resolve(addr, offset, nullptr);
}

uint32_t Bus::read(uint32_t addr, uint8_t op_type) {
    uint32_t offset;
    size_t idx;
    Device* dev = resolve(addr, &offset, &idx);
    if (!dev) return 0;
    if (idx < 9) req_count[idx]++;
    last_addr = addr;
    return dev->read(offset, op_type);
}

void Bus::write(uint32_t addr, uint32_t data, uint8_t op_type) {
    uint32_t offset;
    size_t idx;
    Device* dev = resolve(addr, &offset, &idx);
    if (!dev) return;
    if (idx < 9) req_count[idx]++;
    last_addr = addr;
    dev->write(offset, data, op_type);
}

void Bus::set_irq(uint8_t n, bool v) {
    if (n < 9) irq[n] = v;
}

bool Bus::get_irq(uint8_t n) const {
    return n < 9 ? irq[n] : false;
}
