module top(
    input clk,
    input int_port0,
    input int_port1,
    output int_response0,
    output int_response1,
    output[31:0] s0_addr,
    output[31:0] s0_write_data,
    input[31:0] s0_read_data,
    output[2:0] s0_mem_op_type,
    output s0_we,
    input s0_read_valid,
    input s0_write_ready,
    output s0_req,

    output[31:0] s1_addr,
    output[31:0] s1_write_data,
    input[31:0] s1_read_data,
    output[2:0] s1_mem_op_type,
    output s1_we,
    input s1_read_valid,
    input s1_write_ready,
    output s1_req,

    output[31:0] s2_addr,
    output[31:0] s2_write_data,
    input[31:0] s2_read_data,
    output[2:0] s2_mem_op_type,
    output s2_we,
    input s2_read_valid,
    input s2_write_ready,
    output s2_req
);
wire[31:0] load_addr_v,load_addr_t;
wire[31:0] m0_read_data;
wire[31:0] write_data,write_data_mask;
wire[2:0] m0_mem_op_type;
wire[3:0] interrupt_port;
wire m0_read_req;
wire m0_we,mie;
wire devices_valid,interrupt_flag,interrupt_response;
cpu cpu(
    .clk_i(clk),
    .addr_o(load_addr_v),
    .data_i(m0_read_data),
    .data_o(write_data),
    .mem_op_type_o(m0_mem_op_type),
    .read_req_o(m0_read_req),
    .we_o(m0_we),
    .read_valid_i(devices_valid),
    .write_ready_o(),
    .interrupt_flag_i(interrupt_flag),
    .interrupt_port_i(interrupt_port),
    .interrupt_response_o(interrupt_response),
    .mie_o(mie)
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
    .s0_req_o(s0_req),

    //slave1
    .s1_addr_o(s1_addr),
    .s1_write_data_o(s1_write_data),
    .s1_read_data_i(s1_read_data),
    .s1_mem_op_type_o(s1_mem_op_type),
    .s1_read_valid_i(s1_read_valid),
    .s1_we_o(s1_we),
    .s1_req_o(s1_req),

    //slave2
    .s2_addr_o(s2_addr),
    .s2_write_data_o(s2_write_data),
    .s2_read_data_i(s2_read_data),
    .s2_mem_op_type_o(s2_mem_op_type),
    .s2_read_valid_i(s2_read_valid),
    .s2_we_o(s2_we),
    .s2_req_o(s2_req),
    
    .hold_flag_o(),
    .read_valid_o(devices_valid),
    .write_ready_i(),
    .write_ready_o()
);

interrupt_ctrl interrupt_ctrl(
    .clk_i(clk),

    .port0_i(int_port0),
    .port1_i(int_port1),

    .response0_o(int_response0),
    .response1_o(int_response1),

    .interrupt_response_i(interrupt_response),
    .interrupt_port_o(interrupt_port),
    .interrupt_flag_o(interrupt_flag),

    .mie_i(mie)
);
endmodule
