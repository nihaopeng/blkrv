module regs(
	input clk_i,
	input rst,
	input[4:0] r1_id_i,
	input[4:0] r2_id_i,
	input[4:0] rd_id_i,
	input we_i,
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
assign	r1_o=REGS[r1_id_i];
assign	r2_o=REGS[r2_id_i];

always @(posedge clk_i) begin
    if(read_valid_i) begin
        if(we_i) begin
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
