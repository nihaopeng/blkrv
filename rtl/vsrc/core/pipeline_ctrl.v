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
    output reg syscall_flag_o
);
    reg hold_flag,jump_inst_flag,mret_flag,sret_flag;
    
    assign hold_flag_o=hold_flag;
    assign mret_flag_o=mret_flag;
    assign sret_flag_o=sret_flag;

    always @(posedge clk_i) begin
        if(jump_flag_i==1'b0) begin//由于此模块与译码模块处于同一流水段，因此需要避免产生jump信号时又产生hold信号导致流水线混乱
            if((inst_i[6:0]==7'b0000011||inst_i[6:0]==7'b0100011)&&hold_flag==1'b0&&mret_flag==1'b0&&interrupt_flag_i==1'b0) begin
                hold_flag<=1'b1;
            end
            else if(hold_flag&&read_valid_i) begin
                hold_flag<=1'b0;
            end
            else begin
                hold_flag<=1'b0;
            end

            if((inst_i==32'h30200073)&&mret_flag==1'b0) begin
                mret_flag<=1'b1;
            end
            else if(inst_i==32'h10200073&&interrupt_flag_i==1'b0) begin
                sret_flag<=1'b1;
            end
            else if(mret_flag) begin
                mret_flag<=1'b0;
            end
            else if(sret_flag) begin
                sret_flag<=1'b0;
            end
            else begin
                mret_flag<=1'b0;
                sret_flag<=1'b0;
            end

            if(inst_i==32'h00000073&&interrupt_flag_i==1'b0) begin
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
endmodule //pipeline_ctrl
