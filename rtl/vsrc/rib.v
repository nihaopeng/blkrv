
module rib(
    input wire clk,  
    input wire rst,   
    //master0
    input[31:0] m0_addr_i,
    input[31:0] m0_write_data_i,
    output reg[31:0] m0_read_data_o,
    input[2:0] m0_mem_op_type_i,
    input m0_req_i,
    input m0_we_i,

    //master1
    input[31:0] m1_addr_i,
    input[31:0] m1_write_data_i,
    output reg[31:0] m1_read_data_o,
    input[2:0] m1_mem_op_type_i,
    input m1_req_i,
    input m1_we_i,

    //slave0
    output reg[31:0] s0_addr_o,
    output reg[31:0] s0_write_data_o,
    input[31:0] s0_read_data_i,
    output[2:0] s0_mem_op_type_o,
    input s0_read_valid_i,
    output reg s0_we_o,
    output s0_req_o,

    //slave1
    output reg[31:0] s1_addr_o,
    output reg[31:0] s1_write_data_o,
    input[31:0] s1_read_data_i,
    output[2:0] s1_mem_op_type_o,
    input s1_read_valid_i,
    output reg s1_we_o,
    output s1_req_o,

    //slave2
    output reg[31:0] s2_addr_o,
    output reg[31:0] s2_write_data_o,
    input[31:0] s2_read_data_i,
    output[2:0] s2_mem_op_type_o,
    input s2_read_valid_i,
    output reg s2_we_o,
    output s2_req_o,
    
    output reg hold_flag_o,
    output reg read_valid_o,
    input write_ready_i,
    output reg write_ready_o
);

parameter [3:0] slave0=4'h0;
parameter [3:0] slave1=4'h1;
parameter [3:0] slave2=4'h2;
parameter [3:0] slave3=4'h3;

always @(*) begin
    read_valid_o=s2_read_valid_i|s1_read_valid_i|s0_read_valid_i;
    if(m1_req_i) begin
        case(m1_addr_i[31:28])
            slave0:begin
                s0_addr_o={4'd0,m1_addr_i[27:0]};
                s0_write_data_o=m1_write_data_i;
                s0_we_o=m1_we_i;
                s0_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s0_read_data_i;
                s0_req_o=1'b1;
                s1_req_o=1'b0;
            end
            slave1:begin
                s1_addr_o={4'd0,m1_addr_i[27:0]};
                s1_write_data_o=m1_write_data_i;
                s1_we_o=m1_we_i;
                s1_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s1_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b1;
                s2_req_o=1'b0;
            end
            slave2:begin
                s2_addr_o={4'd0,m1_addr_i[27:0]};
                s2_write_data_o=m1_write_data_i;
                s2_we_o=m1_we_i;
                s2_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s2_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b1;
            end
			default:begin
			end
        endcase
    end
    //m0
    else if(m0_req_i) begin
        case(m0_addr_i[31:28])
            slave0:begin
                s0_addr_o={4'd0,m0_addr_i[27:0]};
                s0_write_data_o=m0_write_data_i;
                s0_we_o=m0_we_i;
                s0_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s0_read_data_i;
                s0_req_o=1'b1;
                s1_req_o=1'b0;
                s2_req_o=1'b0;
            end
            slave1:begin
                s1_addr_o={4'd0,m0_addr_i[27:0]};
                s1_write_data_o=m0_write_data_i;
                s1_we_o=m0_we_i;
                s1_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s1_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b1;
                s2_req_o=1'b0;
            end
            slave2:begin
                s2_addr_o={4'd0,m0_addr_i[27:0]};
                s2_write_data_o=m0_write_data_i;
                s2_we_o=m0_we_i;
                s2_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s2_read_data_i;
                s0_req_o=1'b0;
                s1_req_o=1'b0;
                s2_req_o=1'b1;
            end
			default:begin
			end
        endcase
    end
end

endmodule
