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
    output reg[1:0] pc_change_flag_o,
    input read_valid_i,
    output reg write_ready_o,
    input write_ready_i
);
    always @(posedge clk_i) begin
        if(pc_change_flag_o!=2'b00) begin
            pc_change_flag_o<=pc_change_flag_o-1;
        end
        if(interrupt_flag_i) begin
            pc_val_o<=mtvec_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
        end
        else if(syscall_flag_i) begin
            pc_val_o<=stvec_i;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
            pc_change_flag_o<=2'b10;
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
            pc_change_flag_o<=2'b10;
        end
        else if(hold_flag_i) begin
            pc_val_o<=pc_val_o-4;
            inst_o<=32'd0;
            read_valid_o<=1'b1;
            pc_change_flag_o<=2'b10;
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
