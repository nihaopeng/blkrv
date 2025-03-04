module aluf(
    input      clk,
    input      rst,
	input[31:0] r1_i,
	input[31:0] r2_i,
	output reg[31:0] outdata_o,
	input[2:0] op_type_i,
	input[6:0] op_type2_i
);
parameter[2:0] SUBORADD=3'b000;
parameter[2:0] SLL=3'b001;
parameter[2:0] SLT=3'b010;
parameter[2:0] SLTU=3'b011;
parameter[2:0] XOR=3'b100;
parameter[2:0] SR=3'b101;
parameter[2:0] OR=3'b110;
parameter[2:0] AND=3'b111;
always @(*) begin
    case(op_type_i)
        SUBORADD:begin
			case(op_type2_i)
				7'b0100000:begin
					outdata_o=r1_i-r2_i;
				end
				7'b0000000:begin
					outdata_o=r1_i+r2_i;
				end
				default:begin
					outdata_o=32'd0;
				end
			endcase
        end
		SLL:begin
			outdata_o=r1_i<<r2_i[4:0];
		end
		SLT:begin
			outdata_o=($signed(r1_i)<$signed(r2_i))?32'd1:32'd0;
		end
		SLTU:begin
			outdata_o=($unsigned(r1_i)<$unsigned(r2_i))?32'd1:32'd0;
		end
		XOR:begin
			outdata_o=r1_i^r2_i;
		end
		SR:begin
			case(op_type2_i)
				7'b0100000:begin
					outdata_o=r1_i>>>r2_i[4:0];
				end
				7'b0000000:begin
					outdata_o=r1_i>>r2_i[4:0];
				end
				default:begin
					outdata_o=32'd0;
				end
			endcase
		end
		OR:begin
			outdata_o=r1_i|r2_i;
		end
		AND:begin
			outdata_o=r1_i&r2_i;
		end
        default:begin
            outdata_o=32'd0;
        end
    endcase
end
    

endmodule //alu
