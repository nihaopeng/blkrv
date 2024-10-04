
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
    output[31:0] satp_o
);
wire[31:0] pc_ifu,pc_id,pc_exu;
wire[31:0] cur_inst;
assign read_req_o=1'b1;
wire lui,auipc,jal,jalr,bj,load,store,calc,calci,sys;
wire[4:0] r1_id,r2_id,rd_id;
wire[2:0] op_type;
wire[6:0] op_type2;
wire[31:0] imm,new_pc,data2regs,r1,r2,exu_addr_v,writeback_data;
wire jump_flag,regs_we,regs_read_valid,ifu_read_valid;
wire if2id_read_valid,id2if_write_ready,id2exu_read_valid,id2exu_write_ready;
wire hold_flag,jump_inst_flag,mret_flag,sret_flag,interrupt_response,syscall_flag;

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
    .we_i(sys),
    .r1_i(r1),
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



module rib(
    input wire clk,  
    input wire rst,   
    //master0
    input[31:0] m0_addr_i,
    input[31:0] m0_write_data_i,
    output reg[31:0] m0_read_data_o,
    input[2:0] m0_mem_op_type_i,
    input m0_req_i,
    input m0_we_i,

    //master1
    input[31:0] m1_addr_i,
    input[31:0] m1_write_data_i,
    output reg[31:0] m1_read_data_o,
    input[2:0] m1_mem_op_type_i,
    input m1_req_i,
    input m1_we_i,

    //slave0
    output reg[31:0] s0_addr_o,
    output reg[31:0] s0_write_data_o,
    input[31:0] s0_read_data_i,
    output[2:0] s0_mem_op_type_o,
    input s0_read_valid_i,
    output reg s0_we_o,
    output s0_req_o,

    //slave1
    output reg[31:0] s1_addr_o,
    output reg[31:0] s1_write_data_o,
    input[31:0] s1_read_data_i,
    output[2:0] s1_mem_op_type_o,
    input s1_read_valid_i,
    output reg s1_we_o,
    output s1_req_o,

    //slave2
    output reg[31:0] s2_addr_o,
    output reg[31:0] s2_write_data_o,
    input[31:0] s2_read_data_i,
    output[2:0] s2_mem_op_type_o,
    input s2_read_valid_i,
    output reg s2_we_o,
    output s2_req_o,

    //slave3
    output reg[31:0] s3_addr_o,
    output reg[31:0] s3_write_data_o,
    input[31:0] s3_read_data_i,
    output[2:0] s3_mem_op_type_o,
    input s3_read_valid_i,
    output reg s3_we_o,
    output s3_req_o,

    //slave4
    output reg[31:0] s4_addr_o,
    output reg[31:0] s4_write_data_o,
    input[31:0] s4_read_data_i,
    output[2:0] s4_mem_op_type_o,
    input s4_read_valid_i,
    output reg s4_we_o,
    output s4_req_o,

    //slave5
    output reg[31:0] s5_addr_o,
    output reg[31:0] s5_write_data_o,
    input[31:0] s5_read_data_i,
    output[2:0] s5_mem_op_type_o,
    input s5_read_valid_i,
    output reg s5_we_o,
    output s5_req_o,

    // //slave6
    // output reg[31:0] s6_addr_o,
    // output reg[31:0] s6_write_data_o,
    // input[31:0] s6_read_data_i,
    // output[2:0] s6_mem_op_type_o,
    // input s6_read_valid_i,
    // output reg s6_we_o,
    // output s6_req_o,

    // //slave7
    // output reg[31:0] s2_addr_o,
    // output reg[31:0] s2_write_data_o,
    // input[31:0] s2_read_data_i,
    // output[2:0] s2_mem_op_type_o,
    // input s2_read_valid_i,
    // output reg s2_we_o,
    // output s2_req_o,

    // //slave8
    // output reg[31:0] s2_addr_o,
    // output reg[31:0] s2_write_data_o,
    // input[31:0] s2_read_data_i,
    // output[2:0] s2_mem_op_type_o,
    // input s2_read_valid_i,
    // output reg s2_we_o,
    // output s2_req_o,

    // //slave9
    // output reg[31:0] s2_addr_o,
    // output reg[31:0] s2_write_data_o,
    // input[31:0] s2_read_data_i,
    // output[2:0] s2_mem_op_type_o,
    // input s2_read_valid_i,
    // output reg s2_we_o,
    // output s2_req_o,


    
    output reg hold_flag_o,
    output reg read_valid_o,
    input write_ready_i,
    output reg write_ready_o
);

parameter [3:0] slave0=4'h0;
parameter [3:0] slave1=4'h1;
parameter [3:0] slave2=4'h2;
parameter [3:0] slave3=4'h3;
parameter [3:0] slave4=4'h4;
parameter [3:0] slave5=4'h5;
parameter [3:0] slave6=4'h6;
parameter [3:0] slave7=4'h7;
parameter [3:0] slave8=4'h8;
parameter [3:0] slave9=4'h9;
// parameter [3:0] slave10=4'h2;
// parameter [3:0] slave3=4'h3;
// parameter [3:0] slave0=4'h0;
// parameter [3:0] slave1=4'h1;
// parameter [3:0] slave2=4'h2;
// parameter [3:0] slave3=4'h3;

always @(*) begin
    read_valid_o=s5_read_valid_i|s4_read_valid_i|s3_read_valid_i|s2_read_valid_i|s1_read_valid_i|s0_read_valid_i;
    m0_read_data_o=32'd0;
    m1_read_data_o=32'd0;
    s0_addr_o=32'd0;
    s0_write_data_o=32'd0;
    s0_mem_op_type_o=3'd0;
    s0_we_o=1'b0;
    s0_req_o=1'b0;
    s1_addr_o=32'd0;
    s1_write_data_o=32'd0;
    s1_mem_op_type_o=3'd0;
    s1_we_o=1'b0;
    s1_req_o=1'b0;
    s2_addr_o=32'd0;
    s2_write_data_o=32'd0;
    s2_mem_op_type_o=3'd0;
    s2_we_o=1'b0;
    s2_req_o=1'b0;
    s3_addr_o=32'd0;
    s3_write_data_o=32'd0;
    s3_mem_op_type_o=3'd0;
    s3_we_o=1'b0;
    s3_req_o=1'b0;
    s4_addr_o=32'd0;
    s4_write_data_o=32'd0;
    s4_mem_op_type_o=3'd0;
    s4_we_o=1'b0;
    s4_req_o=1'b0;
    s5_addr_o=32'd0;
    s5_write_data_o=32'd0;
    s5_mem_op_type_o=3'd0;
    s5_we_o=1'b0;
    s5_req_o=1'b0;
    if(m1_req_i) begin
        case(m1_addr_i[31:28])
            slave0:begin
                s0_addr_o={4'd0,m1_addr_i[27:0]};
                s0_write_data_o=m1_write_data_i;
                s0_we_o=m1_we_i;
                s0_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s0_read_data_i;
                s0_req_o=1'b1;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave1:begin
                s1_addr_o={4'd0,m1_addr_i[27:0]};
                s1_write_data_o=m1_write_data_i;
                s1_we_o=m1_we_i;
                s1_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s1_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b1;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave2:begin
                s2_addr_o={4'd0,m1_addr_i[27:0]};
                s2_write_data_o=m1_write_data_i;
                s2_we_o=m1_we_i;
                s2_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s2_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b1;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave3:begin
                s3_addr_o={4'd0,m1_addr_i[27:0]};
                s3_write_data_o=m1_write_data_i;
                s3_we_o=m1_we_i;
                s3_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s3_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b1;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave4:begin
                s4_addr_o={4'd0,m1_addr_i[27:0]};
                s4_write_data_o=m1_write_data_i;
                s4_we_o=m1_we_i;
                s4_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s4_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b1;
                s5_req_o=1'b0;
            end
            slave5:begin
                s5_addr_o={4'd0,m1_addr_i[27:0]};
                s5_write_data_o=m1_write_data_i;
                s5_we_o=m1_we_i;
                s5_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s5_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b1;
            end
			default:begin
			end
        endcase
    end
    //m0
    else if(m0_req_i) begin
        case(m0_addr_i[31:28])
            slave0:begin
                s0_addr_o={4'd0,m0_addr_i[27:0]};
                s0_write_data_o=m0_write_data_i;
                s0_we_o=m0_we_i;
                s0_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s0_read_data_i;
                s0_req_o=1'b1;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave1:begin
                s1_addr_o={4'd0,m0_addr_i[27:0]};
                s1_write_data_o=m0_write_data_i;
                s1_we_o=m0_we_i;
                s1_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s1_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b1;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave2:begin
                s2_addr_o={4'd0,m0_addr_i[27:0]};
                s2_write_data_o=m0_write_data_i;
                s2_we_o=m0_we_i;
                s2_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s2_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b1;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave3:begin
                s3_addr_o={4'd0,m0_addr_i[27:0]};
                s3_write_data_o=m0_write_data_i;
                s3_we_o=m0_we_i;
                s3_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s3_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b1;
                s4_req_o=1'b0;
                s5_req_o=1'b0;
            end
            slave4:begin
                s4_addr_o={4'd0,m0_addr_i[27:0]};
                s4_write_data_o=m0_write_data_i;
                s4_we_o=m0_we_i;
                s4_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s4_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b1;
                s5_req_o=1'b0;
            end
            slave5:begin
                s5_addr_o={4'd0,m0_addr_i[27:0]};
                s5_write_data_o=m0_write_data_i;
                s5_we_o=m0_we_i;
                s5_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s5_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
                s3_req_o=1'b0;
                s4_req_o=1'b0;
                s5_req_o=1'b1;
            end
			default:begin
			end
        endcase
    end
end

endmodule


module top(
    input clk,
    input int_port0,
    input int_port1,
    input int_port2,
    input int_port3,
    input int_port4,
    input int_port5,
    output int_response0,
    output int_response1,
    output int_response2,
    output int_response3,
    output int_response4,
    output int_response5,

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
    output s2_req,

    output[31:0] s3_addr,
    output[31:0] s3_write_data,
    input[31:0] s3_read_data,
    output[2:0] s3_mem_op_type,
    output s3_we,
    input s3_read_valid,
    input s3_write_ready,
    output s3_req,

    output[31:0] s4_addr,
    output[31:0] s4_write_data,
    input[31:0] s4_read_data,
    output[2:0] s4_mem_op_type,
    output s4_we,
    input s4_read_valid,
    input s4_write_ready,
    output s4_req,

    output[31:0] s5_addr,
    output[31:0] s5_write_data,
    input[31:0] s5_read_data,
    output[2:0] s5_mem_op_type,
    output s5_we,
    input s5_read_valid,
    input s5_write_ready,
    output s5_req
);
wire[31:0] load_addr_v,load_addr_t;
wire[31:0] m0_read_data;
wire[31:0] write_data,write_data_mask,satp;
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
    .mie_o(mie),
    .satp_o(satp)
);
mmu mmu(
    .addr_i(load_addr_v),
    .addr_t_o(load_addr_t),
    .satp_i(satp)
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

    //slave3
    .s3_addr_o(s3_addr),
    .s3_write_data_o(s3_write_data),
    .s3_read_data_i(s3_read_data),
    .s3_mem_op_type_o(s3_mem_op_type),
    .s3_read_valid_i(s3_read_valid),
    .s3_we_o(s3_we),
    .s3_req_o(s3_req),

    //slave4
    .s4_addr_o(s4_addr),
    .s4_write_data_o(s4_write_data),
    .s4_read_data_i(s4_read_data),
    .s4_mem_op_type_o(s4_mem_op_type),
    .s4_read_valid_i(s4_read_valid),
    .s4_we_o(s4_we),
    .s4_req_o(s4_req),

    //slave5
    .s5_addr_o(s5_addr),
    .s5_write_data_o(s5_write_data),
    .s5_read_data_i(s5_read_data),
    .s5_mem_op_type_o(s5_mem_op_type),
    .s5_read_valid_i(s5_read_valid),
    .s5_we_o(s5_we),
    .s5_req_o(s5_req),
    
    .hold_flag_o(),
    .read_valid_o(devices_valid),
    .write_ready_i(),
    .write_ready_o()
);

interrupt_ctrl interrupt_ctrl(
    .clk_i(clk),

    .port0_i(int_port0),
    .port1_i(int_port1),
    .port2_i(int_port2),
    .port3_i(int_port3),
    .port4_i(int_port4),
    .port5_i(int_port5),

    .response0_o(int_response0),
    .response1_o(int_response1),
    .response2_o(int_response2),
    .response3_o(int_response3),
    .response4_o(int_response4),
    .response5_o(int_response5),

    .interrupt_response_i(interrupt_response),
    .interrupt_port_o(interrupt_port),
    .interrupt_flag_o(interrupt_flag),

    .mie_i(mie)
);
endmodule


module mmu(
    input[31:0] addr_i,
    output[31:0] addr_t_o,
    input[31:0] satp_i
);
    assign addr_t_o=(satp_i[31])?(addr_i+{4'h0,satp_i[27:0]}):addr_i;
endmodule

module interrupt_ctrl (
    input clk_i,

    input port0_i,
    input port1_i,
    input port2_i,
    input port3_i,
    input port4_i,
    input port5_i,

    output reg response0_o,
    output reg response1_o,
    output reg response2_o,
    output reg response3_o,
    output reg response4_o,
    output reg response5_o,

    input interrupt_response_i,
    output reg[3:0] interrupt_port_o,
    output reg interrupt_flag_o,
    input mie_i
);
    always @(posedge clk_i) begin
        response0_o<=1'b0;
        response1_o<=1'b0;
        response2_o<=1'b0;
        response3_o<=1'b0;
        response4_o<=1'b0;
        response5_o<=1'b0;
        if(mie_i) begin
            if(port0_i) begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b1;
                response0_o<=interrupt_response_i;
            end
            else if(port1_i) begin
                interrupt_port_o<=4'h1;
                interrupt_flag_o<=1'b1;
                response1_o<=interrupt_response_i;
            end
            else if(port2_i) begin
                interrupt_port_o<=4'h2;
                interrupt_flag_o<=1'b1;
                response2_o<=interrupt_response_i;
            end
            else if(port3_i) begin
                interrupt_port_o<=4'h3;
                interrupt_flag_o<=1'b1;
                response3_o<=interrupt_response_i;
            end
            else if(port4_i) begin
                interrupt_port_o<=4'h4;
                interrupt_flag_o<=1'b1;
                response4_o<=interrupt_response_i;
            end
            else if(port5_i) begin
                interrupt_port_o<=4'h5;
                interrupt_flag_o<=1'b1;
                response5_o<=interrupt_response_i;
            end
            else begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b0;
            end
        end
        else begin
            interrupt_port_o<=4'h0;
            interrupt_flag_o<=1'b0;
        end
    end
endmodule //interrupt_ctrl


module trans (
    input      clk,
    input      rst,
    input[31:0] imm_i,
    input[31:0] r1_i,
    input[31:0] r2_i,
    input[31:0] pc_i,
    input[2:0] op_type_i,
    output reg[31:0] new_pc_o,
    output reg jump_flag_o
);
    parameter[2:0] BEQ =3'b000;
    parameter[2:0] BNE =3'b001;
    parameter[2:0] BLT =3'b100;
    parameter[2:0] BGE =3'b101;
    parameter[2:0] BLTU=3'b110;
    parameter[2:0] BGEU=3'b111;
    always @(*) begin
        case(op_type_i)
            BEQ:begin
                new_pc_o=(r1_i==r2_i)?(pc_i+imm_i):32'd0;
                jump_flag_o=(r1_i==r2_i)?1'b1:1'b0;
            end
            BNE:begin
                new_pc_o=(r1_i!=r2_i)?(pc_i+imm_i):32'd0;
                jump_flag_o=(r1_i!=r2_i)?1'b1:1'b0;
            end
            BLT:begin
                new_pc_o=($signed(r1_i)<$signed(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($signed(r1_i)<$signed(r2_i))?1'b1:1'b0;
            end
            BGE:begin
                new_pc_o=($signed(r1_i)>=$signed(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($signed(r1_i)>=$signed(r2_i))?1'b1:1'b0;
            end
            BLTU:begin
                new_pc_o=($unsigned(r1_i)<$unsigned(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($unsigned(r1_i)<$unsigned(r2_i))?1'b1:1'b0;
            end
            BGEU:begin
                new_pc_o=($unsigned(r1_i)>=$unsigned(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($unsigned(r1_i)>=$unsigned(r2_i))?1'b1:1'b0;
            end
            default:begin
                new_pc_o=32'd0;
                jump_flag_o=1'b0;
            end
        endcase
    end
endmodule //trans


`define regWidth 31:0

`define MemAddrBus 31:0
`define MemBus 31:0

module ifu(
    input clk_i,
    input jump_flag_i,//00 to step, 01 for new pc, 10 to stop step
    input[31:0] new_pc_i,
    input[31:0] inst_i,
    input hold_flag_i,
    input interrupt_flag_i,
    input syscall_flag_i,
    input mret_flag_i,
    input sret_flag_i,
    input[31:0] mtvec_i,
    input[31:0] mepc_i,
    input[31:0] stvec_i,
    input[31:0] sepc_i,
    output reg[31:0] pc_val_o,
    output reg[31:0] inst_o,
    output reg read_valid_o,
    input read_valid_i,
    output reg write_ready_o,
    input write_ready_i
);
    always @(posedge clk_i) begin
        if(interrupt_flag_i) begin
            pc_val_o<=mtvec_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else if(syscall_flag_i) begin
            pc_val_o<=stvec_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else if(mret_flag_i) begin
            pc_val_o<=mepc_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else if(sret_flag_i) begin
            pc_val_o<=sepc_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else if(jump_flag_i) begin
            pc_val_o<=new_pc_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else if(hold_flag_i) begin
            pc_val_o<=pc_val_o-4;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else begin
            if(read_valid_i) begin
                pc_val_o<=pc_val_o+4;
                inst_o<=inst_i;
                read_valid_o<=1'b1;
            end
            else begin
                pc_val_o<=pc_val_o;
                inst_o<=inst_o;
                read_valid_o<=1'b0;
            end
        end
    end
endmodule


module pipeline_ctrl (
    input      clk_i,
    input      rst,
    input[31:0] inst_i,
    input jump_flag_i,
    output hold_flag_o,
    output mret_flag_o,
    output sret_flag_o,
    input read_valid_i,
    input interrupt_flag_i,
    output reg interrupt_response_o,
    output reg syscall_flag_o
);
    reg hold_flag,jump_inst_flag,mret_flag,sret_flag;
    
    assign hold_flag_o=hold_flag;
    assign mret_flag_o=mret_flag;
    assign sret_flag_o=sret_flag;
    // assign jump_inst_flag_o=jump_inst_flag;
    // reg tmp;
    // reg tmp1;
    // reg[1:0] tmp1;
    always @(posedge clk_i) begin
        if(jump_flag_i==1'b0) begin//由于此模块与译码模块处于同一流水段，因此需要避免产生jump信号时又产生hold信号导致流水线混乱
            if((inst_i[6:0]==7'b0000011||inst_i[6:0]==7'b0100011)&&hold_flag==1'b0&&mret_flag==1'b0) begin
                hold_flag<=1'b1;
                // tmp<=1'b1;
            end
            else if(hold_flag&&read_valid_i) begin
                hold_flag<=1'b0;
                // tmp<=1'b0;
            end
            else begin
                hold_flag<=1'b0;
            end

            if((inst_i==32'h30200073)&&mret_flag==1'b0) begin
                mret_flag<=1'b1;
            end
            else if(inst_i==32'h10200073) begin
                sret_flag<=1'b1;
            end
            else if(mret_flag) begin
                mret_flag<=1'b0;
                // tmp<=1'b0;
            end
            else if(sret_flag) begin
                sret_flag<=1'b0;
            end
            else begin
                mret_flag<=1'b0;
                sret_flag<=1'b0;
            end

            if(inst_i==32'h00000073) begin
                syscall_flag_o<=1'b1;
            end
            else if(syscall_flag_o) begin
                syscall_flag_o<=1'b0;
            end
            else begin
                syscall_flag_o<=1'b0;
            end
        end 
        
    end

    always @(*) begin
        if(interrupt_flag_i) begin
            interrupt_response_o=1'b1;
        end
        else begin
            interrupt_response_o=1'b0;
        end
    end
    // parameter[6:0] JAL  =7'b1101111;
    // parameter[6:0] JALR =7'b1100111;
    // parameter[6:0] BJ   =7'b1100011;
    // always @(posedge clk_i) begin
    //     if(hold_flag==1'b0) begin
    //         // if((inst_i[6:0]==JAL||inst_i[6:0]==JALR||inst_i[6:0]==BJ)&&tmp1==2'b00)begin
    //         //     jump_inst_flag<=1'b1;
    //         //     tmp1<=2'd2;
    //         // end
    //         // else if(tmp1==2'b01) begin//stop two hop
    //         //     jump_inst_flag<=1'b0;
    //         //     tmp1<=2'b00;
    //         // end
    //         // else if(tmp1!=2'b00) begin
    //         //     jump_inst_flag<=1'b1;
    //         //     tmp1<=tmp1-1;
    //         // end
    //         if((inst_i[6:0]==JAL||inst_i[6:0]==JALR||inst_i[6:0]==BJ)&&tmp1==1'b0)begin
    //             jump_inst_flag<=1'b1;
    //             tmp1<=1'd1;
    //         end
    //         else if(tmp1==1'b1) begin//stop two hop
    //             jump_inst_flag<=1'b0;
    //             tmp1<=1'b0;
    //         end
    //     end
    //     // else begin
    //     //     jump_inst_flag<=1'b0;
    //     //     tmp1<=2'b00;
    //     // end
        
    // end
endmodule //pipeline_ctrl


module csrs (
    input      clk_i,
    input      rst,
    input interrupt_flag_i,
    input syscall_flag_i,
    input we_i,
    input[31:0] r1_i,
    input[31:0] imm_i,
    input mret_flag_i,
    input sret_flag_i,
    output mie_o,
    output[31:0] csr_o,
    input[31:0] cur_pc_i,
    input[31:0] mcause_i,
    output[31:0] mtvec_o,
    output[31:0] mepc_o,
    output[31:0] satp_o,
    output[31:0] stvec_o,
    output[31:0] sepc_o
);
parameter[11:0] mtvec_a  =12'h305;
parameter[11:0] mepc_a   =12'h341;
parameter[11:0] mcause_a =12'h342;
parameter[11:0] mstatus_a=12'h300;
parameter[11:0] satp_a   =12'h180;
parameter[11:0] stvec_a  =12'h105;
parameter[11:0] sepc_a   =12'h141;
parameter[2:0]  CSRRW    =3'b001;
parameter[2:0]  CSRRS    =3'b010;
parameter[2:0]  CSRRC    =3'b011;
parameter[2:0]  CSRRWI   =3'b101;
parameter[2:0]  CSRRSI   =3'b110;
parameter[2:0]  CSRRCI   =3'b111;
parameter[4:0]  mie      =5'd3;
parameter[4:0]  mpie     =5'd7;
reg[31:0] REGS[4095:0];
wire[31:0] mstatus_d;
assign mstatus_d=REGS[mstatus_a];//tmp
assign mie_o=REGS[mstatus_a][mie];
assign csr_o=REGS[imm_i[31:20]];
assign mtvec_o=REGS[mtvec_a];
assign mepc_o=REGS[mepc_a];
assign satp_o=REGS[satp_a];
assign stvec_o=REGS[stvec_a];
assign sepc_o=REGS[sepc_a];
always @(posedge clk_i) begin
    if(interrupt_flag_i) begin
        REGS[mepc_a]<=cur_pc_i-8;
        REGS[mcause_a]<=mcause_i<<2;
        REGS[mstatus_a][mie]<=1'b0;
        REGS[mstatus_a][mpie]<=1'b1;
    end
    else if(syscall_flag_i) begin
        REGS[sepc_a]<=cur_pc_i-4;
    end
    else if(mret_flag_i) begin
        REGS[mstatus_a][mie]<=REGS[mstatus_a][mpie];
    end
    //sret 不做其他操作
    else if(we_i) begin
        case(imm_i[14:12])
            CSRRW:begin
                REGS[imm_i[31:20]]<=r1_i;
            end
            CSRRS:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]|r1_i;
            end
            CSRRC:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]&(~r1_i);
            end
            CSRRWI:begin
                REGS[imm_i[31:20]]<={27'd0,imm_i[19:15]};
            end
            CSRRSI:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]|{27'd0,imm_i[19:15]};
            end
            CSRRCI:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]|(~{27'd0,imm_i[19:15]});
            end
            default:begin
            end
        endcase
    end
end
endmodule //csrs


module alu (
    input      clk,
    input      rst,
	input[31:0] r1_i,
	input[31:0] r2_i,
	output reg[31:0] outdata_o,
	input[2:0] op_type_i,
	input[6:0] op_type2_i
);
parameter[2:0] SUBORADD=3'b000;
parameter[2:0] SLL=3'b001;
parameter[2:0] SLT=3'b010;
parameter[2:0] SLTU=3'b011;
parameter[2:0] XOR=3'b100;
parameter[2:0] SR=3'b101;
parameter[2:0] OR=3'b110;
parameter[2:0] AND=3'b111;
always @(*) begin
    case(op_type_i)
        SUBORADD:begin
			case(op_type2_i)
				7'b0100000:begin
					outdata_o=r1_i-r2_i;
				end
				7'b0000000:begin
					outdata_o=r1_i+r2_i;
				end
				default:begin
					outdata_o=32'd0;
				end
			endcase
        end
		SLL:begin
			outdata_o=r1_i<<r2_i[4:0];
		end
		SLT:begin
			outdata_o=($signed(r1_i)<$signed(r2_i))?32'd1:32'd0;
		end
		SLTU:begin
			outdata_o=($unsigned(r1_i)<$unsigned(r2_i))?32'd1:32'd0;
		end
		XOR:begin
			outdata_o=r1_i^r2_i;
		end
		SR:begin
			case(op_type2_i)
				7'b0100000:begin
					outdata_o=r1_i>>>r2_i[4:0];
				end
				7'b0000000:begin
					outdata_o=r1_i>>r2_i[4:0];
				end
				default:begin
					outdata_o=32'd0;
				end
			endcase
		end
		OR:begin
			outdata_o=r1_i|r2_i;
		end
		AND:begin
			outdata_o=r1_i&r2_i;
		end
        default:begin
            outdata_o=32'd0;
        end
    endcase
end
    

endmodule //alu


module id (
    input      clk,
    input[31:0] pc_i,
    input[31:0] inst_i,
    input hold_flag_i,
    input jump_flag_i,
    input interrupt_flag_i,
    input syscall_flag_i,
    input mret_flag_i,
    input sret_flag_i,
    output reg lui_o,
    output reg auipc_o,
    output reg jal_o,
    output reg jalr_o,
    output reg bj_o,
    output reg load_o,
    output reg store_o,
    output reg calci_o,
    output reg calc_o,
    output reg sys_o,
    output reg[31:0] imm_o,
    output reg[4:0] rd_id_o,
    output reg[4:0] r1_id_o,
    output reg[4:0] r2_id_o,
    output reg[2:0] op_type_o,
    output reg[6:0] op_type2_o,
    output reg[31:0] pc_o,
    output reg read_valid_o,
    input read_valid_i,
    output write_ready_o,
    input write_ready_i 
);
parameter[6:0] LUI  =7'b0110111;
parameter[6:0] AUIPC=7'b0010111;
parameter[6:0] JAL  =7'b1101111;
parameter[6:0] JALR =7'b1100111;
parameter[6:0] BJ   =7'b1100011;
parameter[6:0] LOAD =7'b0000011;
parameter[6:0] STORE=7'b0100011;
parameter[6:0] CALCI=7'b0010011;
parameter[6:0] CALC =7'b0110011;
parameter[6:0] SYS  =7'b1110011;

always @(posedge clk) begin
    if(interrupt_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(syscall_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(mret_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(sret_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(hold_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(jump_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(read_valid_i) begin
        pc_o<=pc_i;
        lui_o<=(inst_i[6:0]==LUI)?1'b1:1'b0;
        auipc_o<=(inst_i[6:0]==AUIPC)?1'b1:1'b0;
        jal_o<=(inst_i[6:0]==JAL)?1'b1:1'b0;
        jalr_o<=(inst_i[6:0]==JALR)?1'b1:1'b0;
        bj_o<=(inst_i[6:0]==BJ)?1'b1:1'b0;
        load_o<=(inst_i[6:0]==LOAD)?1'b1:1'b0;
        store_o<=(inst_i[6:0]==STORE)?1'b1:1'b0;
        calci_o<=(inst_i[6:0]==CALCI)?1'b1:1'b0;
        calc_o<=(inst_i[6:0]==CALC)?1'b1:1'b0;
        sys_o<=(inst_i[6:0]==SYS)?1'b1:1'b0;
        op_type_o<=inst_i[14:12];
        rd_id_o<=inst_i[11:7];
        r1_id_o<=inst_i[19:15];
        r2_id_o<=inst_i[24:20];
        case(inst_i[6:0])
            LUI:begin
                imm_o[31:12]<=inst_i[31:12];
                imm_o[11:0]<=12'h000;
            end
            AUIPC:begin
                imm_o[31:12]<=inst_i[31:12];
                imm_o[11:0]<=12'h000;
            end
            JAL:begin
                imm_o[20]<=inst_i[31];
                imm_o[10:1]<=inst_i[30:21];
                imm_o[11]<=inst_i[20];
                imm_o[19:12]<=inst_i[19:12];
                imm_o[31:21]<={11{inst_i[31]}};
                imm_o[0]<=1'b0;
            end
            JALR:begin
                imm_o[11:0]<=inst_i[31:20];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            BJ:begin
                imm_o[12]<=inst_i[31];
                imm_o[10:5]<=inst_i[30:25];
                imm_o[4:1]<=inst_i[11:8];
                imm_o[11]<=inst_i[7];
                imm_o[31:13]<={19{inst_i[31]}};
                imm_o[0]<=1'b0;
            end
            LOAD:begin
                imm_o[11:0]<=inst_i[31:20];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            STORE:begin
                imm_o[11:5]<=inst_i[31:25];
                imm_o[4:0]<=inst_i[11:7];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            CALCI:begin
                imm_o[11:0]<=inst_i[31:20];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            CALC:begin
                op_type2_o<=inst_i[31:25];            
            end
            SYS:begin
                imm_o<=inst_i;
            end
            default:begin
            end
        endcase
        read_valid_o<=1'b1;
    end
    else if(read_valid_i==1'b0) begin
        read_valid_o<=1'b0;
    end
    else begin
        //use latch to maintain status;
    end
end

endmodule //id


module regs(
	input clk_i,
	input rst,
	input[4:0] r1_id_i,
	input[4:0] r2_id_i,
	input[4:0] rd_id_i,
	input we_i,
    input interrupt_flag_i,
    input syscall_flag_i,
    input mret_flag_i,
    input sret_flag_i,
	input[31:0] write_data_i,
	output[31:0] r1_o,
	output[31:0] r2_o,

    input hold_flag_i,
    output reg read_valid_o,
    input read_valid_i,
    output write_ready_o,
    input write_ready_i 
);
reg[31:0] REGS[31:0];
reg[31:0] REGS_CP[31:0];
assign	r1_o=REGS[r1_id_i];
assign	r2_o=REGS[r2_id_i];
integer i;
always @(posedge clk_i) begin
    if(interrupt_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS_CP[i]<=REGS[i];
        end
        read_valid_o<=1'b1;
    end
    else if(syscall_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS_CP[i]<=REGS[i];
        end
        read_valid_o<=1'b1;
    end
    else if(mret_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS[i]<=REGS_CP[i];
        end
    end
    else if(sret_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS[i]<=REGS_CP[i];
        end
    end
    else if(read_valid_i) begin
        if(we_i&&rd_id_i!=5'd0) begin
            REGS[rd_id_i]<=write_data_i;		
        end
        read_valid_o<=1'b1;
    end
    else begin
        //use latch to maintain the status;
        read_valid_o<=1'b0;
    end
end
endmodule




module load (
    input      clk,
    input      rst,
    input[31:0] r1_i,
    input[2:0] op_type_i,
    input[31:0] imm_i,
    output reg[31:0] addr_v_o
);
    
endmodule //load




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

assign mem_op_type_o=(load_i)?(op_type_i):
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


module alui (
    input      clk,
    input      rst,
    input[31:0] r1_i,
    input[31:0] imm_i,
    input[2:0] op_type_i,
    output reg[31:0] outdata_o
);
    parameter[2:0] ADDI =3'b000;
    parameter[2:0] SLTI =3'b010;
    parameter[2:0] SLTIU=3'b011;
    parameter[2:0] XORI =3'b100;
    parameter[2:0] ORI  =3'b110;
    parameter[2:0] ANDI =3'b111;
    parameter[2:0] SLLI =3'b001;
    parameter[2:0] SRI  =3'b101;
    always @(*) begin
        case(op_type_i)
            ADDI:begin
                outdata_o=r1_i+imm_i;
            end
            SLTI:begin
                outdata_o=($signed(r1_i)<$signed(imm_i))?32'd1:32'd0;
            end
            SLTIU:begin
                outdata_o=($unsigned(r1_i)<$unsigned(imm_i))?32'd1:32'd0;
            end
            XORI:begin
                outdata_o=r1_i^imm_i;
            end
            ORI:begin
                outdata_o=r1_i|imm_i;
            end
            ANDI:begin
                outdata_o=r1_i&imm_i;
            end
            SLLI:begin
                outdata_o=r1_i<<imm_i[4:0];
            end
            SRI:begin
                case(imm_i[11:5])
                    7'b0000000:begin
                        outdata_o=r1_i>>imm_i[4:0];
                    end
                    7'b0100000:begin
                        outdata_o=r1_i>>>imm_i[4:0];
                    end
                    default:begin
                        outdata_o=32'd0;
                    end
                endcase
            end
            default:begin
                outdata_o=32'd0;
            end
        endcase
    end
endmodule //alui


