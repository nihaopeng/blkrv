
module cpu(
    input clk_i,
    output reg[31:0] addr_o,
    input[31:0] data_i,
    output reg[31:0] data_o,

    output read_req_o,
    output we_o,
    input read_enable_i
);
wire[31:0] pc_val;
wire[31:0] cur_inst;
assign read_req_o=1'b1;
assign we_o=1'b0;
ifu ifu(
    .clk(clk_i),
    .inst_i(data_i),
    .pc_val_o(pc_val),
    .instruction_o(cur_inst)
);
mmu mmu(
    .addr_i(pc_val),
    .addr_t_o(addr_o)
);
endmodule