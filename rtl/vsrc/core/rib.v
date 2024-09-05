
module rib(
    input wire clk,  
    input wire rst,   
    //master0
    input[31:0] m0_addr_i,
    input[31:0] m0_write_data_i,
    output reg[31:0] m0_read_data_o,
    input m0_req_i,
    input m0_we_i,

    //master1
    input[31:0] m1_addr_i,
    input[31:0] m1_write_data_i,
    output reg[31:0] m1_read_data_o,
    input m1_req_i,
    input m1_we_i,

    //slave0
    output reg[31:0] s0_addr_o,
    output reg[31:0] s0_write_data_o,
    input[31:0] s0_read_data_i,
    input s0_read_enable_i,
    output reg s0_we_o,

    //slave1
    output reg[31:0] s1_addr_o,
    output reg[31:0] s1_write_data_o,
    input[31:0] s1_read_data_i,
    input s1_read_enable_i,
    output reg s1_we_o,
    
    output reg hold_flag_o,
    output reg read_enable_o
);

parameter [3:0] slave0=4'h0;
parameter [3:0] slave1=4'h1;
parameter [3:0] slave2=4'h2;
parameter [3:0] slave3=4'h3;

always @(*) begin
    read_enable_o=s1_read_enable_i|s0_read_enable_i;
    if(m1_req_i) begin
        case(m1_addr_i[31:28])
            slave0:begin
                s0_addr_o=m1_addr_i;
                s0_write_data_o=m1_write_data_i;
                s0_we_o=m1_we_i;
                m1_read_data_o=s0_read_data_i;
            end
            slave1:begin
                s1_addr_o=m1_addr_i;
                s1_write_data_o=m1_write_data_i;
                s1_we_o=m1_we_i;
                m1_read_data_o=s1_read_data_i;
            end
			default:begin
			end
        endcase
    end
    //m0
    else if(m0_req_i) begin
        case(m1_addr_i[31:28])
            slave0:begin
                s0_addr_o=m0_addr_i;
                s0_write_data_o=m0_write_data_i;
                s0_we_o=m0_we_i;
                m0_read_data_o=s0_read_data_i;
            end
            slave1:begin
                s1_addr_o=m0_addr_i;
                s1_write_data_o=m0_write_data_i;
                s1_we_o=m0_we_i;
                m0_read_data_o=s1_read_data_i;
            end
			default:begin
			end
        endcase
    end
end

endmodule
