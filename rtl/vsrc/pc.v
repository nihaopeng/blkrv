module pc(
    input clk,
    output reg[31:0] pc_val
);
    always @(posedge clk) begin
        pc_val<=pc_val+1;
    end
endmodule