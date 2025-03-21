module top(
    input clk,
    input int_port0,
    input int_port1,
    input int_port2,
    input int_port3,
    input int_port4,
    input int_port5,
    input int_port6,
    output int_response0,
    output int_response1,
    output int_response2,
    output int_response3,
    output int_response4,
    output int_response5,
    output int_response6,
    // output interrupt_enable,

    // output[31:0] s0_addr,
    // output[31:0] s0_write_data,
    // input[31:0] s0_read_data,
    // output[2:0] s0_mem_op_type,
    // output s0_we,
    // input s0_read_valid,
    // input s0_write_ready,
    // output s0_req,

    // output[31:0] s1_addr,
    // output[31:0] s1_write_data,
    // input[31:0] s1_read_data,
    // output[2:0] s1_mem_op_type,
    // output s1_we,
    // input s1_read_valid,
    // input s1_write_ready,
    // output s1_req,

    // output[31:0] s2_addr,
    // output[31:0] s2_write_data,
    // input[31:0] s2_read_data,
    // output[2:0] s2_mem_op_type,
    // output s2_we,
    // input s2_read_valid,
    // input s2_write_ready,
    // output s2_req,

    // output[31:0] s3_addr,
    // output[31:0] s3_write_data,
    // input[31:0] s3_read_data,
    // output[2:0] s3_mem_op_type,
    // output s3_we,
    // input s3_read_valid,
    // input s3_write_ready,
    // output s3_req,

    // output[31:0] s4_addr,
    // output[31:0] s4_write_data,
    // input[31:0] s4_read_data,
    // output[2:0] s4_mem_op_type,
    // output s4_we,
    // input s4_read_valid,
    // input s4_write_ready,
    // output s4_req,

    // output[31:0] s5_addr,
    // output[31:0] s5_write_data,
    // input[31:0] s5_read_data,
    // output[2:0] s5_mem_op_type,
    // output s5_we,
    // input s5_read_valid,
    // input s5_write_ready,
    // output s5_req,

    // output[31:0] s6_addr,
    // output[31:0] s6_write_data,
    // input[31:0] s6_read_data,
    // output[2:0] s6_mem_op_type,
    // output s6_we,
    // input s6_read_valid,
    // input s6_write_ready,
    // output s6_req,

    // output[31:0] s7_addr,
    // output[31:0] s7_write_data,
    // input[31:0] s7_read_data,
    // output[2:0] s7_mem_op_type,
    // output s7_we,
    // input s7_read_valid,
    // input s7_write_ready,
    // output s7_req,

    // output[31:0] s8_addr,
    // output[31:0] s8_write_data,
    // input[31:0] s8_read_data,
    // output[2:0] s8_mem_op_type,
    // output s8_we,
    // input s8_read_valid,
    // input s8_write_ready,
    // output s8_req,
    
    output[31:0] inst_type_o,
    
    output[31:0] load_addr_v,
    output[31:0] satp,

    output[31:0] write_data_o(write_data),
    input[31:0] read_data_i(m0_read_data),
    output[31:0] mem_op_type_o(m0_mem_op_type),
    output[31:0] we_o(m0_we)
);
// wire[31:0] load_addr_v,load_addr_t;
wire[31:0] m0_read_data;
wire[31:0] write_data,satp;
wire[2:0] m0_mem_op_type;
wire[3:0] interrupt_port;
wire m0_read_req;
wire m0_we,mie;
wire devices_valid,interrupt_flag,interrupt_response;
// assign interrupt_enable=mie&&!interrupt_flag;
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
    .mie_o(mie),
    .satp_o(satp),
    .inst_type(inst_type_o)
);
// mmu mmu(
//     .addr_i(load_addr_v),
//     .addr_t_o(load_addr_t),
//     .satp_i(satp)
// );
// rib rib(
//     .clk(clk),  
//     .rst(),
//     // .m0_addr_i(load_addr_t),
//     .m0_addr_i(load_addr_v),
//     .m0_write_data_i(write_data),
//     .m0_read_data_o(m0_read_data),
//     .m0_mem_op_type_i(m0_mem_op_type),
//     .m0_req_i(m0_read_req),
//     .m0_we_i(m0_we),

//     //master1
//     .m1_addr_i(),
//     .m1_write_data_i(),
//     .m1_read_data_o(),
//     .m1_mem_op_type_i(),
//     .m1_req_i(),
//     .m1_we_i(),

//     //slave0
//     .s0_addr_o(s0_addr),
//     .s0_write_data_o(s0_write_data),
//     .s0_read_data_i(s0_read_data),
//     .s0_mem_op_type_o(s0_mem_op_type),
//     .s0_read_valid_i(s0_read_valid),
//     .s0_we_o(s0_we),
//     .s0_req_o(s0_req),

//     //slave1
//     .s1_addr_o(s1_addr),
//     .s1_write_data_o(s1_write_data),
//     .s1_read_data_i(s1_read_data),
//     .s1_mem_op_type_o(s1_mem_op_type),
//     .s1_read_valid_i(s1_read_valid),
//     .s1_we_o(s1_we),
//     .s1_req_o(s1_req),

//     //slave2
//     .s2_addr_o(s2_addr),
//     .s2_write_data_o(s2_write_data),
//     .s2_read_data_i(s2_read_data),
//     .s2_mem_op_type_o(s2_mem_op_type),
//     .s2_read_valid_i(s2_read_valid),
//     .s2_we_o(s2_we),
//     .s2_req_o(s2_req),

//     //slave3
//     .s3_addr_o(s3_addr),
//     .s3_write_data_o(s3_write_data),
//     .s3_read_data_i(s3_read_data),
//     .s3_mem_op_type_o(s3_mem_op_type),
//     .s3_read_valid_i(s3_read_valid),
//     .s3_we_o(s3_we),
//     .s3_req_o(s3_req),

//     //slave4
//     .s4_addr_o(s4_addr),
//     .s4_write_data_o(s4_write_data),
//     .s4_read_data_i(s4_read_data),
//     .s4_mem_op_type_o(s4_mem_op_type),
//     .s4_read_valid_i(s4_read_valid),
//     .s4_we_o(s4_we),
//     .s4_req_o(s4_req),

//     //slave5
//     .s5_addr_o(s5_addr),
//     .s5_write_data_o(s5_write_data),
//     .s5_read_data_i(s5_read_data),
//     .s5_mem_op_type_o(s5_mem_op_type),
//     .s5_read_valid_i(s5_read_valid),
//     .s5_we_o(s5_we),
//     .s5_req_o(s5_req),

//     .s6_addr_o(s6_addr),
//     .s6_write_data_o(s6_write_data),
//     .s6_read_data_i(s6_read_data),
//     .s6_mem_op_type_o(s6_mem_op_type),
//     .s6_read_valid_i(s6_read_valid),
//     .s6_we_o(s6_we),
//     .s6_req_o(s6_req),

//     .s7_addr_o(s7_addr),
//     .s7_write_data_o(s7_write_data),
//     .s7_read_data_i(s7_read_data),
//     .s7_mem_op_type_o(s7_mem_op_type),
//     .s7_read_valid_i(s7_read_valid),
//     .s7_we_o(s7_we),
//     .s7_req_o(s7_req),

//     .s8_addr_o(s8_addr),
//     .s8_write_data_o(s8_write_data),
//     .s8_read_data_i(s8_read_data),
//     .s8_mem_op_type_o(s8_mem_op_type),
//     .s8_read_valid_i(s8_read_valid),
//     .s8_we_o(s8_we),
//     .s8_req_o(s8_req),
    
//     .hold_flag_o(),
//     .read_valid_o(devices_valid),
//     .write_ready_i(),
//     .write_ready_o()
// );

interrupt_ctrl interrupt_ctrl(
    .clk_i(clk),

    .port0_i(int_port0),
    .port1_i(int_port1),
    .port2_i(int_port2),
    .port3_i(int_port3),
    .port4_i(int_port4),
    .port5_i(int_port5),
    .port6_i(int_port6),

    .response0_o(int_response0),
    .response1_o(int_response1),
    .response2_o(int_response2),
    .response3_o(int_response3),
    .response4_o(int_response4),
    .response5_o(int_response5),
    .response6_o(int_response6),

    .interrupt_response_i(interrupt_response),
    .interrupt_port_o(interrupt_port),
    .interrupt_flag_o(interrupt_flag),

    .mie_i(mie)
);
endmodule
