module top(
    input clk,
    output[31:0] s0_addr,
    output[31:0] s0_write_data,
    input[31:0] s0_read_data,
    output[2:0] s0_mem_op_type,
    output s0_we,
    output s0_read_valid,
    input s0_write_ready
);
wire[31:0] load_addr_v,load_addr_t;
wire[31:0] m0_read_data;
wire[31:0] write_data,write_data_mask;
wire[2:0] m0_mem_op_type;
wire m0_read_req;
wire m0_we;
wire devices_valid;
cpu cpu(
    .clk_i(clk),
    .addr_o(load_addr_v),
    .data_i(m0_read_data),
    .data_o(write_data),
    .mem_op_type_o(m0_mem_op_type),
    .read_req_o(m0_read_req),
    .we_o(m0_we),
    .read_valid_i(devices_valid),
    .write_ready_o()
);
mmu mmu(
    .addr_i(load_addr_v),
    .addr_t_o(load_addr_t)
);
rib rib(
    .clk(clk),  
    .rst(),   
    //master0
    .m0_addr_i(load_addr_t),
    .m0_write_data_i(write_data),
    .m0_read_data_o(m0_read_data),
    .m0_mem_op_type_i(m0_mem_op_type),
    .m0_req_i(m0_read_req),
    .m0_we_i(m0_we),

    //master1
    .m1_addr_i(),
    .m1_write_data_i(),
    .m1_read_data_o(),
    .m1_mem_op_type_i(),
    .m1_req_i(),
    .m1_we_i(),

    //slave0
    .s0_addr_o(s0_addr),
    .s0_write_data_o(s0_write_data),
    .s0_read_data_i(s0_read_data),
    .s0_mem_op_type_o(s0_mem_op_type),
    .s0_read_valid_i(s0_read_valid),
    .s0_we_o(s0_we),

    //slave1
    .s1_addr_o(),
    .s1_write_data_o(),
    .s1_read_data_i(),
    .s1_mem_op_type_o(),
    .s1_read_valid_i(1'b0),
    .s1_we_o(),
    
    .hold_flag_o(),
    .read_valid_o(devices_valid),
    .write_ready_i(),
    .write_ready_o()
);
endmodule
