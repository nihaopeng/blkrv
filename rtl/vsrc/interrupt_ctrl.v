module interrupt_ctrl (
    input clk_i,

    input port0_i,
    input port1_i,

    output reg response0_o,
    output reg response1_o,

    input interrupt_response_i,
    output reg[3:0] interrupt_port_o,
    output reg interrupt_flag_o,
    input mie_i
);
    always @(posedge clk_i) begin
        if(mie_i) begin
            if(port0_i) begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b1;
                response0_o<=interrupt_response_i;
                response1_o<=1'b0;
            end
            else if(port1_i) begin
                interrupt_port_o<=4'h1;
                interrupt_flag_o<=1'b1;
                response0_o<=1'b0;
                response1_o<=interrupt_response_i;
            end
            else begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b0;
                response0_o<=1'b0;
                response1_o<=1'b0;
            end
        end
        else begin
            interrupt_port_o<=4'h0;
            interrupt_flag_o<=1'b0;
            if(port0_i) begin
                response0_o<=interrupt_response_i;
                response1_o<=1'b0;
            end
            else if(port1_i) begin
                response0_o<=1'b0;
                response1_o<=interrupt_response_i;
            end
            else begin
                interrupt_port_o<=4'h0;
                interrupt_flag_o<=1'b0;
                response0_o<=1'b0;
                response1_o<=1'b0;
            end
        end
    end
endmodule //interrupt_ctrl
