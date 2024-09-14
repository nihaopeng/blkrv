module csrs (
    input      clk_i,
    input      rst,
    input interrupt_flag_i,
    input we_i,
    input[31:0] r1_i,
    input[31:0] imm_i,
    input mret_flag_i,
    output mie_o,
    output[31:0] csr_o,
    input[31:0] cur_pc_i,
    input[31:0] mcause_i,
    output[31:0] mtvec_o,
    output[31:0] mepc_o
);
parameter[11:0] mtvec_a  =12'h305;
parameter[11:0] mepc_a   =12'h341;
parameter[11:0] mcause_a =12'h342;
parameter[11:0] mstatus_a=12'h300;
parameter[2:0]  CSRRW    =3'b001;
parameter[2:0]  CSRRS    =3'b010;
parameter[2:0]  CSRRC    =3'b011;
parameter[2:0]  CSRRWI   =3'b101;
parameter[2:0]  CSRRSI   =3'b110;
parameter[2:0]  CSRRCI   =3'b111;
parameter[4:0]  mie      =5'd3;
parameter[4:0]  mpie     =5'd7;
reg[31:0] REGS[4095:0];
wire[31:0] mstatus_d;
assign mstatus_d=REGS[mstatus_a];//tmp
assign mie_o=REGS[mstatus_a][mie];
assign csr_o=REGS[imm_i[31:20]];
assign mtvec_o=REGS[mtvec_a];
assign mepc_o=REGS[mepc_a];
always @(posedge clk_i) begin
    if(interrupt_flag_i) begin
        REGS[12'h341]<=cur_pc_i-8;
        REGS[mcause_a]<=mcause_i<<2;
        REGS[mstatus_a][mie]<=1'b0;
        REGS[mstatus_a][mpie]<=1'b1;
    end
    else if(mret_flag_i) begin
        REGS[mstatus_a][mie]<=REGS[mstatus_a][mpie];
    end
    else if(we_i) begin
        case(imm_i[14:12])
            CSRRW:begin
                REGS[imm_i[31:20]]<=r1_i;
            end
            CSRRS:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]|r1_i;
            end
            CSRRC:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]&(~r1_i);
            end
            CSRRWI:begin
                REGS[imm_i[31:20]]<={27'd0,imm_i[19:15]};
            end
            CSRRSI:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]|{27'd0,imm_i[19:15]};
            end
            CSRRCI:begin
                REGS[imm_i[31:20]]<=REGS[imm_i[31:20]]|(~{27'd0,imm_i[19:15]});
            end
            default:begin
            end
        endcase
    end
end
endmodule //csrs
