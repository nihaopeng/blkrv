module pc(
    input clk_i,
    output reg[31:0] pc_val_o
);
    always @(posedge clk_i) begin
        pc_val_o<=pc_val_o+4;
    end
endmodule