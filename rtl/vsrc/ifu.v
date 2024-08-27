module ifu(
    input clk,
    output[31:0] pc_val
);
    pc pc(
        .clk(clk),
        .pc_val(pc_val)
    );
    
endmodule