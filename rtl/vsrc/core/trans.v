module trans (
    input      clk,
    input      rst,
    input[31:0] imm_i,
    input[31:0] r1_i,
    input[31:0] r2_i,
    input[31:0] pc_i,
    input[2:0] op_type_i,
    output reg[31:0] new_pc_o,
    output reg jump_flag_o
);
    parameter[2:0] BEQ =3'b000;
    parameter[2:0] BNE =3'b001;
    parameter[2:0] BLT =3'b100;
    parameter[2:0] BGE =3'b101;
    parameter[2:0] BLTU=3'b110;
    parameter[2:0] BGEU=3'b111;
    always @(*) begin
        case(op_type_i)
            BEQ:begin
                new_pc_o=(r1_i==r2_i)?(pc_i+imm_i):32'd0;
                jump_flag_o=(r1_i==r2_i)?1'b1:1'b0;
            end
            BNE:begin
                new_pc_o=(r1_i!=r2_i)?(pc_i+imm_i):32'd0;
                jump_flag_o=(r1_i!=r2_i)?1'b1:1'b0;
            end
            BLT:begin
                new_pc_o=($signed(r1_i)<$signed(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($signed(r1_i)<$signed(r2_i))?1'b1:1'b0;
            end
            BGE:begin
                new_pc_o=($signed(r1_i)>=$signed(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($signed(r1_i)>=$signed(r2_i))?1'b1:1'b0;
            end
            BLTU:begin
                new_pc_o=($unsigned(r1_i)<$unsigned(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($unsigned(r1_i)<$unsigned(r2_i))?1'b1:1'b0;
            end
            BGEU:begin
                new_pc_o=($unsigned(r1_i)>=$unsigned(r2_i))?(pc_i+imm_i):32'd0;
                jump_flag_o=($unsigned(r1_i)>=$unsigned(r2_i))?1'b1:1'b0;
            end
            default:begin
                new_pc_o=32'd0;
                jump_flag_o=1'b0;
            end
        endcase
    end
endmodule //trans
