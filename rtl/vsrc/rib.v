`define MemAddrBus 31:0
`define MemBus 31:0

module rib(
    input wire clk,  
    input wire rst,  
    // master 0 interface  
    input wire[`MemAddrBus] m0_addr_i,     // 主设备0读、写地址  
    input wire[`MemBus] m0_data_i,         // 主设备0写数据  
    output reg[`MemBus] m0_data_o,         // 主设备0读取到的数据  
    input wire m0_req_i,                   // 主设备0访问请求标志  
    input wire m0_we_i, 
    // slave 0 interface  
    output reg[`MemAddrBus] s0_addr_o,     // 从设备0读、写地址  
    output reg[`MemBus] s0_data_o,         // 从设备0写数据  
    input wire[`MemBus] s0_data_i,         // 从设备0读取到的数据  
    output reg s0_we_o,                    // 从设备0写标志  
    // slave 1 interface  
    output reg[`MemAddrBus] s1_addr_o,     // 从设备1读、写地址  
    output reg[`MemBus] s1_data_o,         // 从设备1写数据  
    input wire[`MemBus] s1_data_i,         // 从设备1读取到的数据  
    output reg s1_we_o,                    // 从设备1写标志  
    
    output reg hold_flag_o
);

endmodule