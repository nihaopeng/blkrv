

module top(
    input clk,
    output[31:0] pc_val
);
ifu ifu(
    .clk(clk),
    .pc_val(pc_val)
);
rib rib(
    .clk(clk),  
    .rst(),
    
    .m0_addr_i(),     // 主设备0读、写地址  
    .m0_data_i(),         // 主设备0写数据  
    .m0_data_o(),         // 主设备0读取到的数据  
    .m0_req_i(),                   // 主设备0访问请求标志  
    .m0_we_i(), 

    .s0_addr_o(),     // 从设备0读、写地址  
    .s0_data_o(),         // 从设备0写数据  
    .s0_data_i(),         // 从设备0读取到的数据  
    .s0_we_o(),                    // 从设备0写标志  

    .s1_addr_o(),     // 从设备1读、写地址  
    .s1_data_o(),         // 从设备1写数据  
    .s1_data_i(),         // 从设备1读取到的数据  
    .s1_we_o(),                    // 从设备1写标志  
    .hold_flag_o()
);
endmodule