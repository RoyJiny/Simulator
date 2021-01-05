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

int execute_cmd(OPcode opcode, int rd, int rs , int rt, int imm, int PC, int cycle, char *should_exit)
{
    int mask;
    registers[IMM] = imm;
    PC++;
    switch(opcode){
        case ADD:
            registers[rd] = registers[rs] + registers[rt];
            break;
	    case SUB:
            registers[rd] = registers[rs] - registers[rt];
            break;
        case AND:
            registers[rd] = registers[rs] & registers[rt];
            break;
        case OR:
            registers[rd] = registers[rs] | registers[rt];
            break;
        case XOR:
            registers[rd] = registers[rs] ^ registers[rt];
            break;
        case MUL:
            registers[rd] = registers[rs] * registers[rt];
            break;
        case SLL:
            registers[rd] = registers[rs] << registers[rt];
            break;
        case SRA:
            registers[rd] = registers[rs] >> registers[rt];
            break;
        case SRL:
            mask = 0xFFFFFFFF << (32-registers[rt]);
            mask = ~mask;
            registers[rd] = (registers[rs] >> registers[rt]) & mask;
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
            registers[rd] = memory[(registers[rt] + registers[rs]) % MEMORY_SIZE];
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
            registers[rd] = registers[rs] + registers[rt] != MONITORCMD
                ? io_registers[registers[rs] + registers[rt]]
                : 0;
            write_hwreg(cycle, registers[rs] + registers[rt], 1);
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
            printf("default opcode\n");
            break;
    }
    return PC & PC_MASK;
}

int run_cmd(char* cmd, char* immidiate, int PC, int cycle ,char *should_exit)
{
    int command = (int)strtol(cmd, NULL, 16);
    int imm = 0;
    // printf("cmd: %s\nimm: %s\nPC: %d\n\n", cmd, immidiate, PC);
    if (immidiate != NULL) {
        imm = (int)strtol(immidiate, NULL, 16);
        PC ++;
    }
    return execute_cmd(
        (command & OPCODE_MASK) >> 12,
        (command & RD_MASK) >> 8,
        (command & RS_MASK) >> 4,
        command & RT_MASK,
        imm,
        PC,
        cycle,
        should_exit
    );
}
