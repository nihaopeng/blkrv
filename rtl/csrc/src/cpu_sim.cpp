#include "cpu_sim.h"
#include <cstdio>

CpuSim::CpuSim(){
    clk=0;
    load_addr_v=0; write_data=0; satp=0; read_data=0; read_valid=0; inst_type_o=0;
    mem_op_type=2; we=0;
    int_port0=int_port1=int_port2=int_port3=int_port4=int_port5=int_port6=int_port7=int_port8=0;
    for(int i=0;i<32;i++) regs[i]=0;
    for(int i=0;i<4096;i++) csr[i]=0;
    pc=0;
    ld_rd=0; ld_funct3=0; ld_pending=false;
    phase=PH_INIT;
}

// 每拍调用一次. 状态机: 发取指请求 -> 等待响应 -> 执行(或发 load/store 请求) -> 等待响应 -> 再取指
void CpuSim::eval(){
    this->satp = csr[0x180];//延迟一拍生效: 上条指令修改 satp 不影响本条指令的取指
    if(phase==PH_INIT){//首次调用, 发出第一个取指请求
        issue_fetch();
        return;
    }
    // PH_DATA: 先消费 load/store 的响应数据
    if(phase==PH_DATA && ld_pending){
        uint32_t data=read_data;
        switch(ld_funct3){//按 exu.v 的符号扩展语义
            case 0: data=(uint32_t)(int32_t)(int8_t)(data); break; // LB
            case 1: data=(uint32_t)(int32_t)(int16_t)(data); break;// LH
            case 2: break;                                          // LW
            case 4: data=data&0xff; break;                          // LBU
            case 5: data=data&0xffff; break;                        // LHU
            default: break;
        }
        regs[ld_rd]=data;
        ld_pending=false;
    }
    // 无论哪个阶段, 每次 eval 都检查中断 (原仅 PH_FETCH 检查, DATA 阶段可能丢中断)
    if(interrupt_pending()){
        take_interrupt();
        issue_fetch();
        return;
    }
    if(phase==PH_FETCH){//read_data 携带取到的指令
        execute(read_data);
    }else{//PH_DATA: 取下一指令
        issue_fetch();
    }
}

void CpuSim::issue_fetch(){
    load_addr_v=pc;
    we=0;
    write_data=0;
    mem_op_type=2;
    phase=PH_FETCH;
}

void CpuSim::wb(uint32_t rd, uint32_t val){
    if(rd!=0) regs[rd]=val;
}

uint32_t CpuSim::sign_ext(uint32_t val, int bits){
    uint32_t mask=(1u<<bits)-1;
    return (val&mask) | ((val&(1u<<(bits-1))) ? ~mask : 0);
}

bool CpuSim::interrupt_pending(){
    
    if((csr[0x300]&(1<<3))==0) return false;//mstatus.MIE=0
    return int_port0||int_port1||int_port2||int_port3||int_port4
        ||int_port5||int_port6||int_port7||int_port8;
}

void CpuSim::take_interrupt(){
    uint32_t port=0;
    if(int_port0) port=0;
    else if(int_port1) port=1;
    else if(int_port2) port=2;
    else if(int_port3) port=3;
    else if(int_port4) port=4;
    else if(int_port5) port=5;
    else if(int_port6) port=6;
    else if(int_port7) port=7;
    else if(int_port8) port=8;
    else return;
        for(int i=0;i<32;i++) regs_cp_m[i]=regs[i];//保存全部寄存器 (regs.v: REGS_CP_M<=REGS)
    csr[0x341]=pc;                             // mepc
    csr[0x342]=port;                           // mcause
    csr[0x300]=(csr[0x300]&~(1<<3))|(((csr[0x300]>>3)&1)<<7);// MPIE=MIE, MIE=0
    csr[0x182]=csr[0x180];                     // satp_i_cp=satp
    csr[0x180]=0;                              // 关 MMU
    this->satp = 0;                             //立即同步: 中断 handler 用 identity 映射
    pc=csr[0x305];                             // mtvec
    inst_type_o=0;
}

void CpuSim::execute(uint32_t inst){
    inst_type_o=0;
    uint32_t opcode=inst&0x7f;
    uint32_t rd=(inst>>7)&0x1f;
    uint32_t rs1=(inst>>15)&0x1f;
    uint32_t rs2=(inst>>20)&0x1f;
    uint32_t funct3=(inst>>12)&0x7;
    uint32_t funct7=inst>>25;
    uint32_t npc=pc+4;
    uint32_t imm;

    switch(opcode){
        case 0x37:{//LUI
            wb(rd, inst&0xfffff000);
            inst_type_o=1<<9;
            break;
        }
        case 0x17:{//AUIPC
            wb(rd, pc+(inst&0xfffff000));
            inst_type_o=1<<8;
            break;
        }
        case 0x6f:{//JAL
            imm=((inst>>31)&1)<<20 | ((inst>>21)&0x3ff)<<1 | ((inst>>20)&1)<<11 | ((inst>>12)&0xff)<<12;
            imm=sign_ext(imm,21);
            wb(rd, pc+4);
            npc=pc+imm;
            inst_type_o=1<<7;
            break;
        }
        case 0x67:{//JALR
            imm=sign_ext(inst>>20,12);
            npc=(regs[rs1]+imm)&~1u;//先算目标再写回, rd==rs1 时结果才正确
            wb(rd, pc+4);
            inst_type_o=1<<6;
            break;
        }
        case 0x63:{//分支
            imm=((inst>>31)&1)<<12 | ((inst>>7)&1)<<11 | ((inst>>25)&0x3f)<<5 | ((inst>>8)&0xf)<<1;
            imm=sign_ext(imm,13);
            inst_type_o=1<<5;
            if(branch_cond(regs[rs1], regs[rs2], funct3)) npc=pc+imm;
            break;
        }
        case 0x03:{//LOAD
            imm=sign_ext(inst>>20,12);
            load_addr_v=regs[rs1]+imm;
            we=0;
            // 与 exu.v 的 mem_op_type 映射一致
            mem_op_type=(funct3==4)?1:(funct3==5)?2:funct3;
            write_data=0;
            ld_rd=rd;
            ld_funct3=funct3;
            ld_pending=true;
            pc=npc;
            phase=PH_DATA;
            inst_type_o=1<<4;
            return;//发出数据请求, 等待响应
        }
        case 0x23:{//STORE
            imm=((inst>>31)&1)<<11 | ((inst>>25)&0x3f)<<5 | ((inst>>8)&0xf)<<1 | ((inst>>7)&1);
            imm=sign_ext(imm,12);
            load_addr_v=regs[rs1]+imm;
            we=1;
            mem_op_type=funct3;
            // 与 exu.v 的 data2mem 掩码一致
            write_data=(funct3==0)?(regs[rs2]&0xff):(funct3==1)?(regs[rs2]&0xffff):regs[rs2];
            ld_pending=false;
            pc=npc;
            phase=PH_DATA;
            inst_type_o=1<<3;
            return;
        }
        case 0x13:{//CALCI
            imm=sign_ext(inst>>20,12);
            wb(rd, alui_calc(regs[rs1], imm, funct3));
            inst_type_o=1<<1;
            break;
        }
        case 0x33:{//CALC
            wb(rd, alu_calc(regs[rs1], regs[rs2], funct3, funct7));
            inst_type_o=1<<2;
            break;
        }
        case 0x73:{//SYSTEM: ecall/mret/sret/csr
            inst_type_o=1<<0;
            if(funct3==0){
                if(inst==0x00000073){//ECALL
                    for(int i=0;i<32;i++) regs_cp_s[i]=regs[i];//保存全部寄存器 (regs.v: REGS_CP_S<=REGS)
                    csr[0x141]=pc+4;       // sepc = ecall+4
                    csr[0x181]=csr[0x180]; // satp_s_cp=satp
                    csr[0x180]=0;
                    this->satp = 0;         //立即同步: handler 用 identity 映射取指
                    npc=csr[0x105];        // stvec
                }else if(inst==0x30200073){//MRET
                    for(int i=0;i<32;i++) regs[i]=regs_cp_m[i];//恢复寄存器 (regs.v: REGS<=REGS_CP_M)
                    csr[0x300]=(csr[0x300]&~(1<<3))|(((csr[0x300]>>7)&1)<<3);// MIE=MPIE
                    csr[0x180]=csr[0x182]; // satp=satp_i_cp
                    csr[0x182]=0;
                    this->satp = csr[0x180];//立即同步: 恢复用户态 satp
                    npc=csr[0x341];        // mepc
                }else if(inst==0x10200073){//SRET
                    for(int i=0;i<32;i++){ if(i!=10) regs[i]=regs_cp_s[i]; }//恢复寄存器, x10(a0) 保留返回值 (regs.v 同)
                    csr[0x180]=csr[0x181]; // satp=satp_s_cp
                    csr[0x181]=0;
                    this->satp = csr[0x180];//立即同步: 恢复用户态 satp
                    npc=csr[0x141];        // sepc
                }else{
                    // EBREAK 等暂不处理
                }
            }else{
                csr_op(inst);
            }
            break;
        }
        default:
            break;
    }
    pc=npc;
    issue_fetch();
}

// 与 alu.v 一致: funct3 选运算, funct7 区分 ADD/SUB, SRL/SRA
uint32_t CpuSim::alu_calc(uint32_t a, uint32_t b, uint8_t funct3, uint8_t funct7){
    switch(funct3){
        case 0: return (funct7==0x20)?(a-b):(a+b);
        case 1: return a<<(b&0x1f);
        case 2: return ((int32_t)a<(int32_t)b)?1:0;
        case 3: return (a<b)?1:0;
        case 4: return a^b;
        case 5: return (funct7==0x20)?((uint32_t)((int32_t)a>>(b&0x1f))):(a>>(b&0x1f));
        case 6: return a|b;
        case 7: return a&b;
        default: return 0;
    }
}

// 与 alui.v 一致
uint32_t CpuSim::alui_calc(uint32_t a, uint32_t imm, uint8_t funct3){
    switch(funct3){
        case 0: return a+imm;
        case 1: return a<<(imm&0x1f);
        case 2: return ((int32_t)a<(int32_t)imm)?1:0;
        case 3: return (a<imm)?1:0;
        case 4: return a^imm;
        case 5: return ((imm>>5)&0x7f)==0x20?((uint32_t)((int32_t)a>>(imm&0x1f))):(a>>(imm&0x1f));
        case 6: return a|imm;
        case 7: return a&imm;
        default: return 0;
    }
}

// 与 trans.v 一致
bool CpuSim::branch_cond(uint32_t a, uint32_t b, uint8_t funct3){
    switch(funct3){
        case 0: return a==b;                                    // BEQ
        case 1: return a!=b;                                    // BNE
        case 4: return (int32_t)a<(int32_t)b;                   // BLT
        case 5: return (int32_t)a>=(int32_t)b;                  // BGE
        case 6: return a<b;                                     // BLTU
        case 7: return a>=b;                                    // BGEU
        default: return false;
    }
}

// 与 csrs.v 的 CSR 指令语义一致
void CpuSim::csr_op(uint32_t inst){
    uint32_t csr_addr=inst>>20;
    uint32_t rd=(inst>>7)&0x1f;
    uint32_t rs1=(inst>>15)&0x1f;
    uint32_t zimm=rs1;
    uint32_t funct3=(inst>>12)&0x7;

    // REGS_CP_M CSR window (0x3c0 ~ 0x3df), for context switch save/restore
    if(csr_addr >= 0x3c0 && csr_addr <= 0x3df){
        uint32_t ridx = csr_addr - 0x3c0;
        uint32_t old = regs_cp_m[ridx];
        switch(funct3){
            case 1: if(rs1!=0) regs_cp_m[ridx]=regs[rs1]; break;       // CSRRW
            case 2: if(rs1!=0) regs_cp_m[ridx]=old|regs[rs1]; break;   // CSRRS
            case 3: if(rs1!=0) regs_cp_m[ridx]=old&(~regs[rs1]); break;// CSRRC
            case 5: regs_cp_m[ridx]=zimm; break;                       // CSRRWI
            case 6: regs_cp_m[ridx]=old|zimm; break;                   // CSRRSI
            case 7: regs_cp_m[ridx]=old&(~zimm); break;                // CSRRCI
            default: break;
        }
        wb(rd, old);
        return;
    }

    uint32_t old=csr[csr_addr];
    switch(funct3){
        case 1: if(rs1!=0) csr[csr_addr]=regs[rs1]; break;       // CSRRW
        case 2: if(rs1!=0) csr[csr_addr]=old|regs[rs1]; break;   // CSRRS
        case 3: if(rs1!=0) csr[csr_addr]=old&(~regs[rs1]); break;// CSRRC
        case 5: csr[csr_addr]=zimm; break;                       // CSRRWI
        case 6: csr[csr_addr]=old|zimm; break;                   // CSRRSI
        case 7: csr[csr_addr]=old&(~zimm); break;                // CSRRCI
        default: break;
    }
    wb(rd, old);
}
