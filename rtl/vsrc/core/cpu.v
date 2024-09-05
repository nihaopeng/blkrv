
module cpu(
    input clk_i,
    output reg[31:0] addr_o,
    input[31:0] data_i,
    output reg[31:0] data_o,

    output read_req_o,
    output we_o,
    input read_enable_i
);
wire[31:0] pc_val;
wire[31:0] cur_inst;
assign read_req_o=1'b1;
assign we_o=1'b0;
wire calc;
wire[4:0] r1_id,r2_id,rd_id;
wire[2:0] op_type;
wire[6:0] op_type2;
wire[31:0] r1,r2,alu_outdata;
ifu ifu(
    .clk_i(clk_i),
    .inst_i(data_i),
    .pc_val_o(pc_val),
    .inst_o(cur_inst)
);
id id(
    .clk(clk_i),
    .inst_i(cur_inst),
    .lui_o(),
    .auipc_o(),
    .jal_o(),
    .jalr_o(),
    .bj_o(),
    .load_o(),
    .store_o(),
    .calci_o(),
    .calc_o(calc),
    .sys_o(),
    .imm_o(),
    .rd_id_o(rd_id),
    .r1_id_o(r1_id),
    .r2_id_o(r2_id),
    .op_type_o(op_type),
    .op_type2_o(op_type2)
);
regs regs(
	.clk_i(clk_i),
	.rst(),
	.r1_id_i(r1_id),
	.r2_id_i(r2_id),
	.rd_id_i(rd_id),
	.we_i(),
	.write_data_i(alu_outdata),
	.r1_o(r1),
	.r2_o(r2)
);
alu alu(
	.clk(clk_i),
    .rst(),
    .calc_i(calc),
	.r1_i(r1),
	.r2_i(r2),
	.outdata_o(alu_outdata),
	.op_type_i(op_type),
	.op_type2_i(op_type2)
);
mmu mmu(
    .addr_i(pc_val),
    .addr_t_o(addr_o)
);
endmodule
