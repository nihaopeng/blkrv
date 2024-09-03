module ifu(
    input clk,
    input[31:0] inst_i,
    output[31:0] pc_val_o,
    output[31:0] inst_o
);
    reg[31:0] inst_o;
    pc pc(
        .clk(clk),
        .pc_val(pc_val_o)
    );
    always @(posedge clk) begin
        inst_o<=inst_i;
    end
endmodule