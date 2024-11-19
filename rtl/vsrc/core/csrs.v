module csrs (
    input      clk_i,
    input      rst,
    input interrupt_flag_i,
    input syscall_flag_i,
    input jump_flag_i,
    input[1:0] pc_change_flag_i,
    input we_i,
    input[31:0] r1_i,
    input[4:0] r1_id,
    input[31:0] imm_i,
    input mret_flag_i,
    input sret_flag_i,
    output mie_o,
    output[31:0] csr_o,
    input[31:0] cur_pc_i,
    input[31:0] mcause_i,
    output[31:0] mtvec_o,
    output[31:0] mepc_o,
    output[31:0] satp_o,
    output[31:0] stvec_o,
    output[31:0] sepc_o
);
parameter[11:0] mtvec_a  =12'h305;
parameter[11:0] mepc_a   =12'h341;
parameter[11:0] mcause_a =12'h342;
parameter[11:0] mstatus_a=12'h300;
parameter[11:0] satp_a   =12'h180;
parameter[11:0] satp_s_cp_a=12'h181;
parameter[11:0] satp_i_cp_a=12'h182;
parameter[11:0] stvec_a  =12'h105;
parameter[11:0] sepc_a   =12'h141;
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
assign satp_o=REGS[satp_a];
assign stvec_o=REGS[stvec_a];
assign sepc_o=REGS[sepc_a];
always @(posedge clk_i) begin
    if(interrupt_flag_i) begin
        if(pc_change_flag_i==2'b10) begin
            REGS[mepc_a]<=cur_pc_i;
        end
        else if(pc_change_flag_i==2'b01) begin
            REGS[mepc_a]<=cur_pc_i-4;
        end
        else begin
            REGS[mepc_a]<=cur_pc_i-8;
        end
        REGS[mcause_a]<=mcause_i;
        REGS[mstatus_a][mie]<=1'b0;
        REGS[mstatus_a][mpie]<=1'b1;
        REGS[satp_i_cp_a]<=REGS[satp_a];
        REGS[satp_a]<=32'h0;
    end
    else if(syscall_flag_i) begin
        REGS[sepc_a]<=cur_pc_i-4;
        REGS[satp_s_cp_a]<=REGS[satp_a];
        REGS[satp_a]<=32'h0;
    end
    else if(mret_flag_i) begin
        REGS[mstatus_a][mie]<=REGS[mstatus_a][mpie];
        REGS[satp_a]<=REGS[satp_i_cp_a];
        REGS[satp_i_cp_a]<=32'd0;
    end
    else if(sret_flag_i) begin
        // REGS[satp_a][31]<=1'b1;
        REGS[satp_a]<=REGS[satp_s_cp_a];
        REGS[satp_s_cp_a]<=32'd0;
    end
    //sret 不做其他操作
    else if(we_i) begin
        case(imm_i[14:12])
            CSRRW:begin
                REGS[imm_i[31:20]]<=(r1_id==5'd0)?REGS[imm_i[31:20]]:r1_i;
            end
            CSRRS:begin
                REGS[imm_i[31:20]]<=(r1_id==5'd0)?REGS[imm_i[31:20]]:REGS[imm_i[31:20]]|r1_i;
            end
            CSRRC:begin
                REGS[imm_i[31:20]]<=(r1_id==5'd0)?REGS[imm_i[31:20]]:REGS[imm_i[31:20]]&(~r1_i);
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
