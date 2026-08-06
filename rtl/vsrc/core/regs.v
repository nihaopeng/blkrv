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
	    input write_ready_i,

	    // REGS_CP_M CSR access (for context switch)
	    input  [4:0]  cp_idx_i,
	    output [31:0] cp_data_o,
	    input         cp_we_i,
	    input  [31:0] cp_wdata_i,
	    // 0 = REGS_CP_M (中断上下文), 1 = REGS_CP_S (ecall 上下文)
	    input         cp_sel_i,
	    // 读路径索引 (组合, 跟随当前指令) — 与写路径 cp_idx_i/cp_sel_i 分开
	    input  [4:0]  cp_idx_rd_i,
	    input         cp_sel_rd_i
	);
reg[31:0] REGS[31:0];
reg[31:0] REGS_CP_S[31:0];
reg[31:0] REGS_CP_M[31:0];
assign	r1_o=REGS[r1_id_i];
assign	r2_o=REGS[r2_id_i];

// REGS_CP_M / REGS_CP_S read port (combinational), 由 cp_sel_i 选择
assign	cp_data_o = cp_sel_rd_i ? REGS_CP_S[cp_idx_rd_i] : REGS_CP_M[cp_idx_rd_i];

integer i;
always @(posedge clk_i) begin
    if(interrupt_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS_CP_M[i]<=REGS[i];
        end
        read_valid_o<=1'b1;
    end
    else if(syscall_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS_CP_S[i]<=REGS[i];
        end
        read_valid_o<=1'b1;
    end
    else if(mret_flag_i) begin
        for(i=0;i<32;i++) begin
            REGS[i]<=REGS_CP_M[i];
        end
    end
    else if(sret_flag_i) begin
        for(i=0;i<10;i++) begin
            REGS[i]<=REGS_CP_S[i];
        end
        for(i=11;i<32;i++) begin
            REGS[i]<=REGS_CP_S[i];
        end
    end
    // REGS_CP_M / REGS_CP_S CSR write (for context switch)
    else if(cp_we_i) begin
        if(cp_sel_i) REGS_CP_S[cp_idx_i] <= cp_wdata_i;
        else         REGS_CP_M[cp_idx_i] <= cp_wdata_i;
        // 修复: cp 窗口写不能遮蔽普通寄存器写回, 否则 lw→csrw 依赖链的写回会丢失
        if(read_valid_i && we_i && rd_id_i!=5'd0) begin
            REGS[rd_id_i] <= write_data_i;
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
