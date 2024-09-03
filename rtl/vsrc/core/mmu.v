module mmu(
    input[31:0] addr_i,
    output[31:0] addr_t_o
);
    assign addr_t_o=addr_i;
endmodule