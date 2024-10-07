module exu (
    input      clk_i,
    input      rst,
    input[31:0] pc_i,
    input lui_i,
    input auipc_i,
    input jal_i,
    input jalr_i,
    input bj_i,
    input load_i,
    input store_i,
    input calci_i,
    input calc_i,
    input sys_i,
    input[31:0] imm_i,
    input[31:0] r1_i,
    input[31:0] r2_i,
    input[2:0] op_type_i,
    input[6:0] op_type2_i,
    output reg[31:0] pc_o,
    output reg[31:0] new_pc_o,
    output reg[31:0] addr_v_o,
    output[31:0] data2regs_o,
    output[31:0] data2mem_o,
    // output[31:0] mask2mem_o,
    input[31:0] dataFRmem_i,
    input[31:0] csr_i,
    output[2:0] mem_op_type_o,
    output jump_flag_o,
    output hold_flag_o,
    output regs_we_o,

    output read_valid_o,
    input read_valid_i,
    output write_ready_o,
    input write_ready_i
);
wire regs_we,bj_jump_flag,jump_flag;
wire[31:0] r1,r2,alu_outdata,alui_outdata,data2regs,bj_new_pc,data2mem;
wire[2:0] mem_op_type;

always @(posedge clk_i) begin
    if(read_valid_i) begin
        pc_o<=pc_i;//we use pc_o in jump action actually
    end
    else begin
        //use latch to maintain the status;
    end
end

alu alu(
	.clk(clk_i),
    .rst(),
	.r1_i(r1_i),
	.r2_i(r2_i),
	.outdata_o(alu_outdata),
	.op_type_i(op_type_i),
	.op_type2_i(op_type2_i)
);
alui alui(
	.clk(clk_i),
    .rst(),
	.r1_i(r1_i),
    .imm_i(imm_i),
	.outdata_o(alui_outdata),
	.op_type_i(op_type_i)
);
trans trans(
    .clk(clk_i),
    .rst(),
    .imm_i(imm_i),
    .r1_i(r1_i),
    .r2_i(r2_i),
    .pc_i(pc_o),
    .op_type_i(op_type_i),
    .new_pc_o(bj_new_pc),
    .jump_flag_o(bj_jump_flag)
);
assign data2regs_o=(calc_i)?alu_outdata:
                 (calci_i)?alui_outdata:
                 (lui_i)?imm_i:
                 (auipc_i)?(pc_o+imm_i):
                 (jal_i)?(pc_o+4):
                 (jalr_i)?(pc_o+4):
                 (load_i)?((op_type_i==3'b000)?{24'd0,dataFRmem_i[7:0]}:
                           (op_type_i==3'b001)?{16'd0,dataFRmem_i[15:0]}:
                           (op_type_i==3'b010)?dataFRmem_i:
                           (op_type_i==3'b100)?{{24{dataFRmem_i[7]}},dataFRmem_i[7:0]}:
                           (op_type_i==3'b101)?{{16{dataFRmem_i[15]}},dataFRmem_i[15:0]}:32'd0):
                 (sys_i)?((op_type_i==3'b001||op_type_i==3'b010||op_type_i==3'b011||op_type_i==3'b101||op_type_i==3'b110||op_type_i==3'b111)?csr_i:32'd0):(32'd0);

assign addr_v_o=(load_i|store_i)?(r1_i+imm_i):32'd0;

assign data2mem_o=(op_type_i==3'b000)?({24'd0,r2_i[7:0]}):
                  (op_type_i==3'b001)?({16'd0,r2_i[15:0]}):
                  (op_type_i==3'b010)?({r2_i}):32'd0;

assign mem_op_type_o=(load_i)?((op_type_i==3'b100)?3'b001:((op_type_i==3'b101)?3'b010:op_type_i)):
                     (store_i)?(op_type_i):3'b010;
                     
wire csr_we;
assign csr_we=(sys_i&&(op_type_i==3'b001||op_type_i==3'b010||op_type_i==3'b011||op_type_i==3'b101||op_type_i==3'b110||op_type_i==3'b111))?1'b1:1'b0;
// assign mask2mem_o=(op_type_i==3'b000)?({24'd0,8'hff}):
//                   (op_type_i==3'b001)?({16'd0,16'hffff}):
//                   (op_type_i==3'b010)?({32'hffffffff}):32'd0;

always @(*) begin
    if(jal_i) begin
        new_pc_o=pc_o+imm_i;
    end
    else if(jalr_i) begin
        new_pc_o=r1_i+imm_i;
    end
    else if(bj_i) begin
        new_pc_o=bj_new_pc;
    end
    else begin
        new_pc_o=32'd0;
    end
end

assign jump_flag=(bj_jump_flag&&bj_i)?1'b1:1'b0;
assign jump_flag_o=(jal_i|jalr_i|jump_flag)?1'b1:1'b0;
assign regs_we_o=calc_i|calci_i|lui_i|auipc_i|jal_i|jalr_i|load_i|csr_we;
endmodule //exu
