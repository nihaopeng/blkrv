module pipeline_ctrl (
    input      clk_i,
    input      rst,
    input[31:0] inst_i,
    output reg hold_flag_o,
    input read_valid_i
);
    reg tmp;
    always @(posedge clk_i) begin
        if((inst_i[6:0]==7'b0000011)||(inst_i[6:0]==7'b0100011)) begin
            if(tmp==1'b0) begin
                hold_flag_o<=1'b1;
                tmp<=1'b1;
            end
        end
        else if(tmp&&read_valid_i) begin
            hold_flag_o<=1'b0;
            tmp<=1'b0;
        end
        else begin
            tmp<=1'b0;
        end
    end
endmodule //pipeline_ctrl
