
module cpu(
    input clk_i,
    output[31:0] addr_o,
    input[31:0] data_i,
    output [31:0] data_o,
    output[2:0] mem_op_type_o,
    output read_req_o,
    output we_o,
    input read_valid_i,
    output write_ready_o,
    input[3:0] interrupt_port_i,//中断端口
    input interrupt_flag_i,
    output interrupt_response_o,
    output mie_o,
    output[31:0] satp_o,
    output[31:0] inst_type
    //assign inst_type={22'd0,lui,auipc,jal,jalr,bj,load,store,calc,calci,sys};
);
wire[31:0] pc_ifu,pc_id,pc_exu;
wire[31:0] cur_inst;
assign read_req_o=1'b1;
wire lui,auipc,jal,jalr,bj,load,store,calc,calci,sys;
assign inst_type={22'd0,lui,auipc,jal,jalr,bj,load,store,calc,calci,sys};
wire[4:0] r1_id,r2_id,rd_id;
wire[2:0] op_type;
wire[6:0] op_type2;
wire[31:0] imm,new_pc,data2regs,r1,r2,exu_addr_v,writeback_data;
wire jump_flag,regs_we,regs_read_valid,ifu_read_valid;
wire if2id_read_valid,id2if_write_ready,id2exu_read_valid,id2exu_write_ready;
wire hold_flag,jump_inst_flag,mret_flag,sret_flag,interrupt_response,syscall_flag;
wire[1:0] pc_change_flag;

ifu ifu(
    .clk_i(clk_i),
    .new_pc_i(new_pc),
    .inst_i(data_i),
    .hold_flag_i(hold_flag),
    .interrupt_flag_i(interrupt_flag_i),
    .syscall_flag_i(syscall_flag),
    .mret_flag_i(mret_flag),
    .sret_flag_i(sret_flag),
    .mtvec_i(mtvec),
    .stvec_i(stvec),
    .mepc_i(mepc),
    .sepc_i(sepc),
    .jump_flag_i(jump_flag),
    .pc_val_o(pc_ifu),
    .inst_o(cur_inst),
    .read_valid_o(if2id_read_valid),
    .read_valid_i(ifu_read_valid),
    .pc_change_flag_o(pc_change_flag),
    .write_ready_o(),
    .write_ready_i()
);

id id(
    .clk(clk_i),
    .pc_i(pc_ifu),
    .inst_i(cur_inst),
    .hold_flag_i(hold_flag),
    .jump_flag_i(jump_flag),
    .interrupt_flag_i(interrupt_flag_i),
    .syscall_flag_i(syscall_flag),
    .mret_flag_i(mret_flag),
    .sret_flag_i(sret_flag),
    .lui_o(lui),
    .auipc_o(auipc),
    .jal_o(jal),
    .jalr_o(jalr),
    .bj_o(bj),
    .load_o(load),
    .store_o(store),
    .calci_o(calci),
    .calc_o(calc),
    .sys_o(sys),
    .imm_o(imm),
    .rd_id_o(rd_id),
    .r1_id_o(r1_id),
    .r2_id_o(r2_id),
    .op_type_o(op_type),
    .op_type2_o(op_type2),
    .pc_o(pc_id),
    .read_valid_o(id2exu_read_valid),
    .read_valid_i(if2id_read_valid),
    .write_ready_o(),
    .write_ready_i()
);
assign addr_o=(load|store)?exu_addr_v:pc_ifu;
assign we_o=(store)?1'b1:1'b0;
assign regs_read_valid=(load)?read_valid_i:id2exu_read_valid;
assign ifu_read_valid=(load)?1'b0:read_valid_i;

pipeline_ctrl ctrl(
    .clk_i(clk_i),
    .rst(),
    .inst_i(cur_inst),
    .jump_flag_i(jump_flag),
    .hold_flag_o(hold_flag),
    .mret_flag_o(mret_flag),
    .sret_flag_o(sret_flag),
    .read_valid_i(read_valid_i),
    .interrupt_flag_i(interrupt_flag_i),
    .interrupt_response_o(interrupt_response_o),
    .syscall_flag_o(syscall_flag)
);
exu exu(
    .clk_i(clk_i),
    .rst(),
    .pc_i(pc_id),
    .lui_i(lui),
    .auipc_i(auipc),
    .jal_i(jal),
    .jalr_i(jalr),
    .bj_i(bj),
    .load_i(load),
    .store_i(store),
    .calci_i(calci),
    .calc_i(calc),
    .sys_i(sys),
    .imm_i(imm),
    .r1_i(r1),
    .r2_i(r2),
    .op_type_i(op_type),
    .op_type2_i(op_type2),
    .pc_o(pc_exu),
    .new_pc_o(new_pc),
    .addr_v_o(exu_addr_v),
    .data2regs_o(data2regs),
    .data2mem_o(data_o),
    // .mask2mem_o(mask_o),
    .mem_op_type_o(mem_op_type_o),
    .dataFRmem_i(data_i),
    .csr_i(csr),
    .jump_flag_o(jump_flag),
    .hold_flag_o(),
    .regs_we_o(regs_we),
    .read_valid_o(),
    .read_valid_i(id2exu_read_valid),
    .write_ready_o(),
    .write_ready_i()
);
regs regs(
	.clk_i(clk_i),
	.rst(),
	.r1_id_i(r1_id),
	.r2_id_i(r2_id),
	.rd_id_i(rd_id),
	.we_i(regs_we),
    .interrupt_flag_i(interrupt_flag_i),
    .syscall_flag_i(syscall_flag),
    .mret_flag_i(mret_flag),
    .sret_flag_i(sret_flag),
	.write_data_i(data2regs),
	.r1_o(r1),
	.r2_o(r2),
    .hold_flag_i(hold_flag),
    .read_valid_o(),
    .read_valid_i(regs_read_valid),
    .write_ready_o(),
    .write_ready_i()
);
wire[31:0] csr,mtvec,mepc,stvec,sepc;

csrs csrs(
    .clk_i(clk_i),
    .rst(),
    .interrupt_flag_i(interrupt_flag_i),
    .syscall_flag_i(syscall_flag),
    .jump_flag_i(jump_flag),
    .pc_change_flag_i(pc_change_flag),
    .we_i(sys),
    .r1_i(r1),
    .r1_id(r1_id),
    .imm_i(imm),
    .mret_flag_i(mret_flag),
    .sret_flag_i(sret_flag),
    .mie_o(mie_o),
    .mcause_i({28'd0,interrupt_port_i}),
    .csr_o(csr),
    .cur_pc_i(pc_ifu),
    .mtvec_o(mtvec),
    .mepc_o(mepc),
    .stvec_o(stvec),
    .sepc_o(sepc),
    .satp_o(satp_o)
);
endmodule
