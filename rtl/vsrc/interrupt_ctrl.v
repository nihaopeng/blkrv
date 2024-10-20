module interrupt_ctrl (
    input clk_i,

    input port0_i,
    input port1_i,
    input port2_i,
    input port3_i,
    input port4_i,
    input port5_i,
    input port6_i,

    output reg response0_o,
    output reg response1_o,
    output reg response2_o,
    output reg response3_o,
    output reg response4_o,
    output reg response5_o,
    output reg response6_o,

    input interrupt_response_i,
    output reg[3:0] interrupt_port_o,
    output reg interrupt_flag_o,
    input mie_i
);
    always @(posedge clk_i) begin
        response0_o<=1'b0;
        response1_o<=1'b0;
        response2_o<=1'b0;
        response3_o<=1'b0;
        response4_o<=1'b0;
        response5_o<=1'b0;
        response6_o<=1'b0;
        if(mie_i&&!interrupt_flag_o) begin
            if(port0_i) begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b1;
                response0_o<=interrupt_response_i;
            end
            else if(port1_i) begin
                interrupt_port_o<=4'h1;
                interrupt_flag_o<=1'b1;
                response1_o<=interrupt_response_i;
            end
            else if(port2_i) begin
                interrupt_port_o<=4'h2;
                interrupt_flag_o<=1'b1;
                response2_o<=interrupt_response_i;
            end
            else if(port3_i) begin
                interrupt_port_o<=4'h3;
                interrupt_flag_o<=1'b1;
                response3_o<=interrupt_response_i;
            end
            else if(port4_i) begin
                interrupt_port_o<=4'h4;
                interrupt_flag_o<=1'b1;
                response4_o<=interrupt_response_i;
            end
            else if(port5_i) begin
                interrupt_port_o<=4'h5;
                interrupt_flag_o<=1'b1;
                response5_o<=interrupt_response_i;
            end
            else if(port5_i) begin
                interrupt_port_o<=4'h6;
                interrupt_flag_o<=1'b1;
                response6_o<=interrupt_response_i;
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
