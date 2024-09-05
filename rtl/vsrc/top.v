module top(
    input clk,
    output[31:0] s0_addr,
    output[31:0] s0_write_data,
    input[31:0] s0_read_data,
    input s0_read_enable,
    output s0_we
);
wire[31:0] load_addr;
wire[31:0] read_data;
wire[31:0] write_data;
wire m0_read_req;
wire m0_we;
wire re;
cpu cpu(
    .clk_i(clk),
    .addr_o(load_addr),
    .data_i(read_data),
    .data_o(write_data),
    .read_req_o(m0_read_req),
    .we_o(m0_we),
    .read_enable_i(re)
);
rib rib(
    .clk(clk),  
    .rst(1'b0),   
    //master0
    .m0_addr_i(load_addr),
    .m0_write_data_i(write_data),
    .m0_read_data_o(read_data),
    .m0_req_i(m0_read_req),
    .m0_we_i(m0_we),

    //master1
    .m1_addr_i(),
    .m1_write_data_i(),
    .m1_read_data_o(),
    .m1_req_i(),
    .m1_we_i(),

    //slave0
    .s0_addr_o(s0_addr),
    .s0_write_data_o(s0_write_data),
    .s0_read_data_i(s0_read_data),
    .s0_read_enable_i(s0_read_enable),
    .s0_we_o(s0_we),

    //slave1
    .s1_addr_o(),
    .s1_write_data_o(),
    .s1_read_data_i(),
    .s1_read_enable_i(),
    .s1_we_o(),
    
    .hold_flag_o(),
    .read_enable_o(re)
);
endmodule
