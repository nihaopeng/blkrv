module top(
    input clk,
    input int_port0,
    input int_port1,
    input int_port2,
    input int_port3,
    input int_port4,
    input int_port5,
    input int_port6,
    input int_port7,
    input int_port8,
    
    output[31:0] inst_type_o,
    
    output[31:0] load_addr_v,
    output[31:0] satp,
    // output[31:0] asid_csr,

    output[31:0] write_data,
    input[31:0] read_data,
    output[2:0] mem_op_type,
    input read_valid,
    output we,

    // sfence.vma 执行指示 (组合脉冲), 供 C++ 主循环刷新 TLB
    output sfence_flag_o,
    output[4:0] sfence_asid_o
);
wire[3:0] interrupt_port;
wire m0_read_req;
wire mie;
wire interrupt_flag;
cpu cpu(
    .clk_i(clk),
    .addr_o(load_addr_v),
    .data_i(read_data),
    .data_o(write_data),
    .mem_op_type_o(mem_op_type),
    .read_req_o(m0_read_req),
    .we_o(we),
    .read_valid_i(read_valid),
    .write_ready_o(),
    .interrupt_flag_i(interrupt_flag),
    .interrupt_port_i(interrupt_port),
    .mie_o(mie),
    .satp_o(satp),
    // .asid_csr_o(asid_csr),
    .inst_type(inst_type_o),
    .sfence_flag_o(sfence_flag_o),
    .sfence_asid_o(sfence_asid_o)
);

interrupt_ctrl interrupt_ctrl(
    .clk_i(clk),

    .port0_i(int_port0),
    .port1_i(int_port1),
    .port2_i(int_port2),
    .port3_i(int_port3),
    .port4_i(int_port4),
    .port5_i(int_port5),
    .port6_i(int_port6),
    .port7_i(int_port7),
    .port8_i(int_port8),

    .interrupt_port_o(interrupt_port),
    .interrupt_flag_o(interrupt_flag),

    .mie_i(mie)
);
endmodule
