#include "executioner.h"
#include "monitor_helper.h"
#include "tracer.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

extern int registers[16];
extern int io_registers[22];
extern int memory[MEMORY_SIZE];
extern char handling_irq_flag;

int execute_cmd(OPcode opcode, int rd, int rs , int rt, int PC, int cycle, char *should_exit)
{
    int mask;
    PC++;
    if (rd >= REG_COUNT || rt >= REG_COUNT || rs >= REG_COUNT) {
        printf("Illegal register value\n");
        return PC & PC_MASK;
    }
    if (rd < 0 || rt < 0 || rs < 0) {
        printf("Illegal register value\n");
        return PC & PC_MASK;
    }
    
    switch(opcode){
        case ADD:
            if (rd > 0) registers[rd] = registers[rs] + registers[rt];
            break;
	    case SUB:
            if (rd > 0) registers[rd] = registers[rs] - registers[rt];
            break;
        case AND:
            if (rd > 0) registers[rd] = registers[rs] & registers[rt];
            break;
        case OR:
            if (rd > 0) registers[rd] = registers[rs] | registers[rt];
            break;
        case XOR:
            if (rd > 0) registers[rd] = registers[rs] ^ registers[rt];
            break;
        case MUL:
            if (rd > 0) registers[rd] = registers[rs] * registers[rt];
            break;
        case SLL:
            if (rd > 0) registers[rd] = registers[rs] << registers[rt];
            break;
        case SRA:
            if (rd > 0) registers[rd] = registers[rs] >> registers[rt];
            break;
        case SRL:
            mask = 0xFFFFFFFF << (32-registers[rt]);
            mask = ~mask;
            if (rd > 0) registers[rd] = (registers[rs] >> registers[rt]) & mask;
            break;
        case BEQ:
            if (registers[rs] == registers[rt]) PC = registers[rd];
            break;
        case BNE:
            if (registers[rs] != registers[rt]) PC = registers[rd];
            break;
        case BLT:
            if (registers[rs] < registers[rt]) PC = registers[rd];
            break;
        case BGT:
            if (registers[rs] > registers[rt]) PC = registers[rd];
            break;
        case BLE:
            if (registers[rs] <= registers[rt]) PC = registers[rd];
            break;
        case BGE:
            if (registers[rs] >= registers[rt]) PC = registers[rd];
            break;
        case JAL:
            registers[RA] = PC;
            PC = registers[rd];
            break;
        case LW:
            if (rd > 0) registers[rd] = memory[(registers[rt] + registers[rs]) % MEMORY_SIZE];
            break;
        case SW:
            memory[(registers[rt] + registers[rs]) % MEMORY_SIZE] = registers[rd];
            break;
        case RETI:
            PC = io_registers[IRQRETURN];
            handling_irq_flag = 0;
            break;
        case IN:
            /*TODO: maybe check the index*/
            if (rd > 0) {
                registers[rd] = registers[rs] + registers[rt] != MONITORCMD
                    ? io_registers[registers[rs] + registers[rt]]
                    : 0;
                write_hwreg(cycle, registers[rs] + registers[rt], 1);
            }
            break;
        case OUT:
            if (registers[rs] + registers[rt] == MONITORCMD && registers[rd] == 1) {
                write_pixel(io_registers[MONITORX], io_registers[MONITORY], (unsigned char)io_registers[MONITORDATA]);
            } else {
                /*some led have changed*/
                if(registers[rs] + registers[rt] == LEDS && io_registers[LEDS] != registers[rd]){
                    write_leds(cycle);
                }
                io_registers[registers[rs] + registers[rt]] = registers[rd];
            }
            write_hwreg(cycle, registers[rs] + registers[rt], 0);
            break;
        case HALT:
            printf("halt simulator, goodbye\n");
            *should_exit = 1;
            break;
        default:
            printf("Illegal opcode\n");
            break;
    }
    return PC & PC_MASK;
}

void write_imm_to_reg(char *immidiate)
{
    static int sign_mask = 0x80000; // check sign bit
    static int negative_mask = 0xFFF00000;
    static int positive_mask = 0x00000000;
    int mask;
    int imm;
    if (immidiate != NULL) {
        imm = (int)strtol(immidiate, NULL, 16);
        mask = (imm & sign_mask) > 0 ? negative_mask : positive_mask;
        imm = imm | mask;
        registers[IMM] = imm;
    }
}

int run_cmd(char* cmd, char* immidiate, int PC, int cycle ,char *should_exit)
{
    int command = (int)strtol(cmd, NULL, 16);
    
    // printf("cmd: %s\nimm: %s\nPC: %d\n\n", cmd, immidiate, PC);
    if (immidiate != NULL) {
        PC ++;
    }
    return execute_cmd(
        (command & OPCODE_MASK) >> 12,
        (command & RD_MASK) >> 8,
        (command & RS_MASK) >> 4,
        command & RT_MASK,
        PC,
        cycle,
        should_exit
    );
}
