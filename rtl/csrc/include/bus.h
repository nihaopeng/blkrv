#ifndef _BUS_H_
#define _BUS_H_

#include <cstdint>
#include <cstddef>
#include <vector>

extern uint64_t main_time;  // 全局仿真时间

class Bus;

// 所有挂载在总线上的设备抽象基类
class Device {
public:
    Bus* bus = nullptr;   // 指向所属总线, 用于设备发起 DMA 请求
    virtual ~Device() = default;
    // 被总线调用的读写接口. offset 为设备内偏移, op_type: 0=byte 1=half 2=word
    virtual uint32_t read(uint32_t offset, uint8_t op_type) {
        (void)offset; (void)op_type; return 0;
    }
    virtual void write(uint32_t offset, uint32_t data, uint8_t op_type) {
        (void)offset; (void)data; (void)op_type;
    }
};

// 总线: 统一地址解码, CPU 与任意设备(DMA)共用 read/write 发起访问
class Bus {
public:
    struct DevEntry {
        Device* dev;
        uint32_t base, size;
        uint8_t irq;      // 中断端口, 无中断用 0xFF
    };
    std::vector<DevEntry> devs;
    uint8_t irq[9] = {};           // 9 根中断线
    uint64_t req_count[9] = {};    // 每设备访问计数 (monitor 统计用)
    uint32_t last_addr = 0;        // 最近一次访问地址 (monitor 对齐统计用)

    void register_dev(Device* dev, uint32_t base, uint32_t size, uint8_t irq);

    // 任意 master (CPU 或 DMA 设备) 统一通过 read/write 访问总线
    uint32_t read(uint32_t addr, uint8_t op_type);
    void write(uint32_t addr, uint32_t data, uint8_t op_type);

    void set_irq(uint8_t n, bool v);
    bool get_irq(uint8_t n) const;

    // 地址解码: 返回命中的设备与设备内偏移, 无命中返回 nullptr
    Device* resolve(uint32_t addr, uint32_t* offset = nullptr);

private:
    // 按 base 升序排列的 devs 下标 (稳定排序, 同 base 保持注册顺序), 供 O(log n) 区间解码
    std::vector<uint32_t> decode_idx;
    Device* resolve(uint32_t addr, uint32_t* offset, size_t* out_idx);
};

#endif // !_BUS_H_
