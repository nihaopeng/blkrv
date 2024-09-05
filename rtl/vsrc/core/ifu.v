module ifu(
    input clk_i,
    input[31:0] inst_i,
    output[31:0] pc_val_o,
    output reg[31:0] inst_o
);
    pc pc(
        .clk_i(clk_i),
        .pc_val_o(pc_val_o)
    );
    always @(posedge clk_i) begin
        inst_o<=inst_i;
    end
endmodule
