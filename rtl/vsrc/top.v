module top(
    input clk,
    output[31:0] s0_addr,
    output[31:0] s0_write_data,
    input[31:0] s0_read_data,
    input s0_read_enable,
    output s0_we,
);
wire[31:0] addr;
wire[31:0] read_data;
wire[31:0] write_data;
wire m0_read_req;
wire m0_we;
wire re;
cpu cpu(
    .clk_i(clk),
    .addr_o(addr),
    .data_i(read_data),
    .data_o(write_data),
    .read_req_o(m0_read_req),
    .we_o(m0_we),
    .read_enable_i(re)
);
rib rib(
    .clk(clk_i),  
    .rst(1'b0),   
    //master0
    .m0_addr_i(addr),
    .m0_write_data_i(write_data),
    .m0_read_data_o(read_data),
    .m0_req_i(m0_read_req),
    .m0_we_i(m0_we),

    //master1
    // input[31:0] m1_addr_i,
    // input[31:0] m1_write_data_i,
    // output reg[31:0] m1_read_data_o,
    // input m1_req_i,
    // input m1_we_i,

    //slave0
    .s0_addr_o(s0_addr),
    .s0_write_data_o(s0_write_data),
    .s0_read_data_i(s0_read_data),
    .s0_read_enable_i(s0_read_enable),
    .s0_we_o(s0_we),

    //slave1
    // output reg[31:0] s1_addr_o,
    // output reg[31:0] s1_write_data_o,
    // input[31:0] s1_read_data_i,
    // input s1_read_enable_i,
    // output reg s1_we_o,
    
    .hold_flag_o(),
    .read_enable_o(re)
);
endmodule