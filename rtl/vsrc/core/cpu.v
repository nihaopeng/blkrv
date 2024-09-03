
module cpu(
    input clk_i,
    output[31:0] pc_o,
    output[31:0] data_o
);
wire cur_inst;
wire[31:0] addr_t;
ifu ifu(
    .clk(clk),
    .inst_i(data),
    .pc_val_o(pc_val),
    .instruction_o(cur_inst)
);
mmu mmu(
    .addr_i(pc_val),
    .addr_t_o(addr_t)
);
rib rib(
    
);
endmodule