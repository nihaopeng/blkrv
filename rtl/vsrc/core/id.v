module id (
    input      clk,
    input[31:0] pc_i,
    input[31:0] inst_i,
    input hold_flag_i,
    input jump_flag_i,
    input interrupt_flag_i,
    input mret_flag_i,
    output reg lui_o,
    output reg auipc_o,
    output reg jal_o,
    output reg jalr_o,
    output reg bj_o,
    output reg load_o,
    output reg store_o,
    output reg calci_o,
    output reg calc_o,
    output reg sys_o,
    output reg[31:0] imm_o,
    output reg[4:0] rd_id_o,
    output reg[4:0] r1_id_o,
    output reg[4:0] r2_id_o,
    output reg[2:0] op_type_o,
    output reg[6:0] op_type2_o,
    output reg[31:0] pc_o,
    output reg read_valid_o,
    input read_valid_i,
    output write_ready_o,
    input write_ready_i 
);
parameter[6:0] LUI  =7'b0110111;
parameter[6:0] AUIPC=7'b0010111;
parameter[6:0] JAL  =7'b1101111;
parameter[6:0] JALR =7'b1100111;
parameter[6:0] BJ   =7'b1100011;
parameter[6:0] LOAD =7'b0000011;
parameter[6:0] STORE=7'b0100011;
parameter[6:0] CALCI=7'b0010011;
parameter[6:0] CALC =7'b0110011;
parameter[6:0] SYS  =7'b1110011;

always @(posedge clk) begin
    if(interrupt_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(mret_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(hold_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(jump_flag_i) begin
        lui_o<=1'b0;
        auipc_o<=1'b0;
        jal_o<=1'b0;
        jalr_o<=1'b0;
        bj_o<=1'b0;
        load_o<=1'b0;
        store_o<=1'b0;
        calci_o<=1'b0;
        calc_o<=1'b0;
        sys_o<=1'b0;
    end
    else if(read_valid_i) begin
        pc_o<=pc_i;
        lui_o<=(inst_i[6:0]==LUI)?1'b1:1'b0;
        auipc_o<=(inst_i[6:0]==AUIPC)?1'b1:1'b0;
        jal_o<=(inst_i[6:0]==JAL)?1'b1:1'b0;
        jalr_o<=(inst_i[6:0]==JALR)?1'b1:1'b0;
        bj_o<=(inst_i[6:0]==BJ)?1'b1:1'b0;
        load_o<=(inst_i[6:0]==LOAD)?1'b1:1'b0;
        store_o<=(inst_i[6:0]==STORE)?1'b1:1'b0;
        calci_o<=(inst_i[6:0]==CALCI)?1'b1:1'b0;
        calc_o<=(inst_i[6:0]==CALC)?1'b1:1'b0;
        sys_o<=(inst_i[6:0]==SYS)?1'b1:1'b0;
        op_type_o<=inst_i[14:12];
        rd_id_o<=inst_i[11:7];
        r1_id_o<=inst_i[19:15];
        r2_id_o<=inst_i[24:20];
        case(inst_i[6:0])
            LUI:begin
                imm_o[31:12]<=inst_i[31:12];
                imm_o[11:0]<=12'h000;
            end
            AUIPC:begin
                imm_o[31:12]<=inst_i[31:12];
                imm_o[11:0]<=12'h000;
            end
            JAL:begin
                imm_o[20]<=inst_i[31];
                imm_o[10:1]<=inst_i[30:21];
                imm_o[11]<=inst_i[20];
                imm_o[19:12]<=inst_i[19:12];
                imm_o[31:21]<={11{inst_i[31]}};
                imm_o[0]<=1'b0;
            end
            JALR:begin
                imm_o[11:0]<=inst_i[31:20];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            BJ:begin
                imm_o[12]<=inst_i[31];
                imm_o[10:5]<=inst_i[30:25];
                imm_o[4:1]<=inst_i[11:8];
                imm_o[11]<=inst_i[7];
                imm_o[31:13]<={19{inst_i[31]}};
                imm_o[0]<=1'b0;
            end
            LOAD:begin
                imm_o[11:0]<=inst_i[31:20];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            STORE:begin
                imm_o[11:5]<=inst_i[31:25];
                imm_o[4:0]<=inst_i[11:7];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            CALCI:begin
                imm_o[11:0]<=inst_i[31:20];
                imm_o[31:12]<={20{inst_i[31]}};
            end
            CALC:begin
                op_type2_o<=inst_i[31:25];            
            end
            SYS:begin
                imm_o<=inst_i;
            end
            default:begin
            end
        endcase
        read_valid_o<=1'b1;
    end
    else if(read_valid_i==1'b0) begin
        read_valid_o<=1'b0;
    end
    else begin
        //use latch to maintain status;
    end
end

endmodule //id
