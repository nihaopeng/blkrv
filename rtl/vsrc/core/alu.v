module alu (
    input      clk,
    input      rst,
    input calc_i,
	input[31:0] r1_i,
	input[31:0] r2_i,
	output[31:0] outdata_o,
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

        end
        default:begin
            
        end
    endcase
end
    

endmodule //alu
