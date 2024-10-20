
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

    //slave3
    output reg[31:0] s3_addr_o,
    output reg[31:0] s3_write_data_o,
    input[31:0] s3_read_data_i,
    output[2:0] s3_mem_op_type_o,
    input s3_read_valid_i,
    output reg s3_we_o,
    output s3_req_o,

    //slave4
    output reg[31:0] s4_addr_o,
    output reg[31:0] s4_write_data_o,
    input[31:0] s4_read_data_i,
    output[2:0] s4_mem_op_type_o,
    input s4_read_valid_i,
    output reg s4_we_o,
    output s4_req_o,

    //slave5
    output reg[31:0] s5_addr_o,
    output reg[31:0] s5_write_data_o,
    input[31:0] s5_read_data_i,
    output[2:0] s5_mem_op_type_o,
    input s5_read_valid_i,
    output reg s5_we_o,
    output s5_req_o,

    //slave6
    output reg[31:0] s6_addr_o,
    output reg[31:0] s6_write_data_o,
    input[31:0] s6_read_data_i,
    output[2:0] s6_mem_op_type_o,
    input s6_read_valid_i,
    output reg s6_we_o,
    output s6_req_o,

    //slave7
    output reg[31:0] s7_addr_o,
    output reg[31:0] s7_write_data_o,
    input[31:0] s7_read_data_i,
    output[2:0] s7_mem_op_type_o,
    input s7_read_valid_i,
    output reg s7_we_o,
    output s7_req_o,

    // //slave8
    // output reg[31:0] s2_addr_o,
    // output reg[31:0] s2_write_data_o,
    // input[31:0] s2_read_data_i,
    // output[2:0] s2_mem_op_type_o,
    // input s2_read_valid_i,
    // output reg s2_we_o,
    // output s2_req_o,

    // //slave9
    // output reg[31:0] s2_addr_o,
    // output reg[31:0] s2_write_data_o,
    // input[31:0] s2_read_data_i,
    // output[2:0] s2_mem_op_type_o,
    // input s2_read_valid_i,
    // output reg s2_we_o,
    // output s2_req_o,

    output reg hold_flag_o,
    output reg read_valid_o,
    input write_ready_i,
    output reg write_ready_o
);

parameter [3:0] slave0=4'h0;
parameter [3:0] slave1=4'h1;
parameter [3:0] slave2=4'h2;
parameter [3:0] slave3=4'h3;
parameter [3:0] slave4=4'h4;
parameter [3:0] slave5=4'h5;
parameter [3:0] slave6=4'h6;
parameter [3:0] slave7=4'h7;
parameter [3:0] slave8=4'h8;
parameter [3:0] slave9=4'h9;
// parameter [3:0] slave10=4'h2;
// parameter [3:0] slave3=4'h3;
// parameter [3:0] slave0=4'h0;
// parameter [3:0] slave1=4'h1;
// parameter [3:0] slave2=4'h2;
// parameter [3:0] slave3=4'h3;

always @(*) begin
    read_valid_o=s5_read_valid_i|s4_read_valid_i|s3_read_valid_i|s2_read_valid_i|s1_read_valid_i|s0_read_valid_i;
    m0_read_data_o=32'd0;
    m1_read_data_o=32'd0;
    s0_addr_o=32'd0;
    s0_write_data_o=32'd0;
    s0_mem_op_type_o=3'd0;
    s0_we_o=1'b0;
    s0_req_o=1'b0;
    s1_addr_o=32'd0;
    s1_write_data_o=32'd0;
    s1_mem_op_type_o=3'd0;
    s1_we_o=1'b0;
    s1_req_o=1'b0;
    s2_addr_o=32'd0;
    s2_write_data_o=32'd0;
    s2_mem_op_type_o=3'd0;
    s2_we_o=1'b0;
    s2_req_o=1'b0;
    s3_addr_o=32'd0;
    s3_write_data_o=32'd0;
    s3_mem_op_type_o=3'd0;
    s3_we_o=1'b0;
    s3_req_o=1'b0;
    s4_addr_o=32'd0;
    s4_write_data_o=32'd0;
    s4_mem_op_type_o=3'd0;
    s4_we_o=1'b0;
    s4_req_o=1'b0;
    s5_addr_o=32'd0;
    s5_write_data_o=32'd0;
    s5_mem_op_type_o=3'd0;
    s5_we_o=1'b0;
    s5_req_o=1'b0;
    s6_addr_o=32'd0;
    s6_write_data_o=32'd0;
    s6_mem_op_type_o=3'd0;
    s6_we_o=1'b0;
    s6_req_o=1'b0;
    s7_addr_o=32'd0;
    s7_write_data_o=32'd0;
    s7_mem_op_type_o=3'd0;
    s7_we_o=1'b0;
    s7_req_o=1'b0;
    if(m1_req_i) begin
        case(m1_addr_i[31:28])
            slave0:begin
                s0_addr_o={4'd0,m1_addr_i[27:0]};
                s0_write_data_o=m1_write_data_i;
                s0_we_o=m1_we_i;
                s0_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s0_read_data_i;
                s0_req_o=1'b1;
            end
            slave1:begin
                s1_addr_o={4'd0,m1_addr_i[27:0]};
                s1_write_data_o=m1_write_data_i;
                s1_we_o=m1_we_i;
                s1_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s1_read_data_i;
                s1_req_o=1'b1;
            end
            slave2:begin
                s2_addr_o={4'd0,m1_addr_i[27:0]};
                s2_write_data_o=m1_write_data_i;
                s2_we_o=m1_we_i;
                s2_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s2_read_data_i;
                s2_req_o=1'b1;
            end
            slave3:begin
                s3_addr_o={4'd0,m1_addr_i[27:0]};
                s3_write_data_o=m1_write_data_i;
                s3_we_o=m1_we_i;
                s3_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s3_read_data_i;
                s3_req_o=1'b1;
            end
            slave4:begin
                s4_addr_o={4'd0,m1_addr_i[27:0]};
                s4_write_data_o=m1_write_data_i;
                s4_we_o=m1_we_i;
                s4_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s4_read_data_i;
                s4_req_o=1'b1;
            end
            slave5:begin
                s5_addr_o={4'd0,m1_addr_i[27:0]};
                s5_write_data_o=m1_write_data_i;
                s5_we_o=m1_we_i;
                s5_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s5_read_data_i;
                s5_req_o=1'b1;
            end
            slave6:begin
                s6_addr_o={4'd0,m1_addr_i[27:0]};
                s6_write_data_o=m1_write_data_i;
                s6_we_o=m1_we_i;
                s6_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s6_read_data_i;
                s6_req_o=1'b1;
            end
            slave7:begin
                s7_addr_o={4'd0,m1_addr_i[27:0]};
                s7_write_data_o=m1_write_data_i;
                s7_we_o=m1_we_i;
                s7_mem_op_type_o=m1_mem_op_type_i;
                m1_read_data_o=s7_read_data_i;
                s7_req_o=1'b1;
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
            end
            slave1:begin
                s1_addr_o={4'd0,m0_addr_i[27:0]};
                s1_write_data_o=m0_write_data_i;
                s1_we_o=m0_we_i;
                s1_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s1_read_data_i;
                s1_req_o=1'b1;
            end
            slave2:begin
                s2_addr_o={4'd0,m0_addr_i[27:0]};
                s2_write_data_o=m0_write_data_i;
                s2_we_o=m0_we_i;
                s2_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s2_read_data_i;
                s2_req_o=1'b1;
            end
            slave3:begin
                s3_addr_o={4'd0,m0_addr_i[27:0]};
                s3_write_data_o=m0_write_data_i;
                s3_we_o=m0_we_i;
                s3_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s3_read_data_i;
                s3_req_o=1'b1;
            end
            slave4:begin
                s4_addr_o={4'd0,m0_addr_i[27:0]};
                s4_write_data_o=m0_write_data_i;
                s4_we_o=m0_we_i;
                s4_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s4_read_data_i;
                s4_req_o=1'b1;
            end
            slave5:begin
                s5_addr_o={4'd0,m0_addr_i[27:0]};
                s5_write_data_o=m0_write_data_i;
                s5_we_o=m0_we_i;
                s5_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s5_read_data_i;
                s5_req_o=1'b1;
            end
            slave6:begin
                s6_addr_o={4'd0,m0_addr_i[27:0]};
                s6_write_data_o=m0_write_data_i;
                s6_we_o=m0_we_i;
                s6_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s6_read_data_i;
                s6_req_o=1'b1;
            end
            slave7:begin
                s7_addr_o={4'd0,m0_addr_i[27:0]};
                s7_write_data_o=m0_write_data_i;
                s7_we_o=m0_we_i;
                s7_mem_op_type_o=m0_mem_op_type_i;
                m0_read_data_o=s7_read_data_i;
                s7_req_o=1'b1;
            end
			default:begin
			end
        endcase
    end
end

endmodule
