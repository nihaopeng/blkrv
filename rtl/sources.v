module top(
    input clk,
    output[31:0] s0_addr,
    output[31:0] s0_write_data,
    input[31:0] s0_read_data,
    input s0_read_enable,
    output s0_we,
);
wire[31:0] addr;
wire[31:0] read_data;
wire[31:0] write_data;
wire m0_read_req;
wire m0_we;
wire re;
cpu cpu(
    .clk_i(clk),
    .addr_o(addr),
    .data_i(read_data),
    .data_o(write_data),
    .read_req_o(m0_read_req),
    .we_o(m0_we),
    .read_enable_i(re)
);
rib rib(
    .clk(clk_i),  
    .rst(1'b0),   
    //master0
    .m0_addr_i(addr),
    .m0_write_data_i(write_data),
    .m0_read_data_o(read_data),
    .m0_req_i(m0_read_req),
    .m0_we_i(m0_we),

    //master1
    // input[31:0] m1_addr_i,
    // input[31:0] m1_write_data_i,
    // output reg[31:0] m1_read_data_o,
    // input m1_req_i,
    // input m1_we_i,

    //slave0
    .s0_addr_o(s0_addr),
    .s0_write_data_o(s0_write_data),
    .s0_read_data_i(s0_read_data),
    .s0_read_enable_i(s0_read_enable),
    .s0_we_o(s0_we),

    //slave1
    // output reg[31:0] s1_addr_o,
    // output reg[31:0] s1_write_data_o,
    // input[31:0] s1_read_data_i,
    // input s1_read_enable_i,
    // output reg s1_we_o,
    
    .hold_flag_o(),
    .read_enable_o(re)
);
endmodule


module cpu(
    input clk_i,
    output reg[31:0] addr_o,
    input[31:0] data_i,
    output reg[31:0] data_o,

    output read_req_o,
    output we_o,
    input read_enable_i
);
wire[31:0] pc_val;
wire[31:0] cur_inst;
assign read_req_o=1'b1;
assign we_o=1'b0;
ifu ifu(
    .clk(clk_i),
    .inst_i(data_i),
    .pc_val_o(pc_val),
    .instruction_o(cur_inst)
);
mmu mmu(
    .addr_i(pc_val),
    .addr_t_o(addr_o)
);
endmodule

`define regWidth 31:0

`define MemAddrBus 31:0
`define MemBus 31:0

module ifu(
    input clk,
    input[31:0] inst_i,
    output[31:0] pc_val_o,
    output[31:0] inst_o
);
    reg[31:0] inst_o;
    pc pc(
        .clk(clk),
        .pc_val(pc_val_o)
    );
    always @(posedge clk) begin
        inst_o<=inst_i;
    end
endmodule


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
        endcase
    end
end

endmodule







module mmu(
    input[31:0] addr_i,
    output[31:0] addr_t_o
);
    assign addr_t_o=addr_i;
endmodule

module pc(
    input clk,
    output reg[31:0] pc_val
);
    always @(posedge clk) begin
        pc_val<=pc_val+1;
    end
endmodule

