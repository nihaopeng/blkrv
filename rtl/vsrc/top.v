module top(
    input clk,
    output[31:0] pc_val,
    output[31:0] data
);
cpu cpu(
    .clk(clk),
    .pc_val(pc_val),
    .data(data)
);
endmodule