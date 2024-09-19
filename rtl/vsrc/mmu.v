module mmu(
    input[31:0] addr_i,
    output[31:0] addr_t_o,
    input[31:0] satp_i
);
    assign addr_t_o=(satp_i[31])?(addr_i+{4'h0,satp_i[27:0]}):addr_i;
endmodule