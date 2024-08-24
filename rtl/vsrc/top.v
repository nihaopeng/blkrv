module top(
    input clk,
    output[31:0] pc_val
);
IFU ifu(
    .clk(clk),
    .pc(pc_val)
);
endmodule

module IFU(
    input clk,
    output reg[31:0] pc
);
    always @(posedge clk) begin
        pc<=pc+1;
    end
endmodule