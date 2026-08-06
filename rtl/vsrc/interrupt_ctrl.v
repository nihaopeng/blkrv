module interrupt_ctrl (
    input clk_i,

    input port0_i,
    input port1_i,
    input port2_i,
    input port3_i,
    input port4_i,
    input port5_i,
    input port6_i,
    input port7_i,
    input port8_i,

    output reg[3:0] interrupt_port_o,
    output reg interrupt_flag_o,
    input mie_i
);
    always @(posedge clk_i) begin
        if(mie_i&&!interrupt_flag_o) begin
            if(port0_i) begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b1;
            end
            else if(port1_i) begin
                interrupt_port_o<=4'h1;
                interrupt_flag_o<=1'b1;
            end
            else if(port2_i) begin
                interrupt_port_o<=4'h2;
                interrupt_flag_o<=1'b1;
            end
            else if(port3_i) begin
                interrupt_port_o<=4'h3;
                interrupt_flag_o<=1'b1;
            end
            else if(port4_i) begin
                interrupt_port_o<=4'h4;
                interrupt_flag_o<=1'b1;
            end
            else if(port5_i) begin
                interrupt_port_o<=4'h5;
                interrupt_flag_o<=1'b1;
            end
            else if(port6_i) begin
                interrupt_port_o<=4'h6;
                interrupt_flag_o<=1'b1;
            end
            else if(port7_i) begin
                interrupt_port_o<=4'h7;
                interrupt_flag_o<=1'b1;
            end
            else if(port8_i) begin
                interrupt_port_o<=4'h8;
                interrupt_flag_o<=1'b1;
            end
            else begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b0;
            end
        end
        else begin
            interrupt_port_o<=4'h0;
            interrupt_flag_o<=1'b0;
        end
    end
endmodule //interrupt_ctrl
