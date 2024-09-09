module alui (
    input      clk,
    input      rst,
    input[31:0] r1_i,
    input[31:0] imm_i,
    input[2:0] op_type_i,
    output reg[31:0] outdata_o
);
    parameter[2:0] ADDI =3'b000;
    parameter[2:0] SLTI =3'b010;
    parameter[2:0] SLTIU=3'b011;
    parameter[2:0] XORI =3'b100;
    parameter[2:0] ORI  =3'b110;
    parameter[2:0] ANDI =3'b111;
    parameter[2:0] SLLI =3'b001;
    parameter[2:0] SRI  =3'b101;
    always @(*) begin
        case(op_type_i)
            ADDI:begin
                outdata_o=r1_i+imm_i;
            end
            SLTI:begin
                outdata_o=($signed(r1_i)<$signed(imm_i))?32'd1:32'd0;
            end
            SLTIU:begin
                outdata_o=($unsigned(r1_i)<$unsigned(imm_i))?32'd1:32'd0;
            end
            XORI:begin
                outdata_o=r1_i^imm_i;
            end
            ORI:begin
                outdata_o=r1_i|imm_i;
            end
            ANDI:begin
                outdata_o=r1_i&imm_i;
            end
            SLLI:begin
                outdata_o=r1_i<<imm_i[4:0];
            end
            SRI:begin
                case(imm_i[11:5])
                    7'b0000000:begin
                        outdata_o=r1_i>>imm_i[4:0];
                    end
                    7'b0100000:begin
                        outdata_o=r1_i>>>imm_i[4:0];
                    end
                    default:begin
                        outdata_o=32'd0;
                    end
                endcase
            end
            default:begin
                outdata_o=32'd0;
            end
        endcase
    end
endmodule //alui
