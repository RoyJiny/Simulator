#include "executioner.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

extern int registers[16];
extern int io_registers[22];
extern int memory[MEMORY_SIZE];

int execute_cmd(OPcode opcode, int rd, int rs , int rt, int imm, int PC, char *should_exit)
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
        printf("storing %d in %d\n", registers[rd],(registers[rt] + registers[rs]) % MEMORY_SIZE);
            memory[(registers[rt] + registers[rs]) % MEMORY_SIZE] = registers[rd];
            break;
        case RETI:
            PC = io_registers[IRQRETURN];
            break;
        case IN:
            /*TODO: maybe check the index*/
            registers[rd] = io_registers[registers[rs] + registers[rt]];
            break;
        case OUT:
            io_registers[registers[rs] + registers[rt]] = registers[rd];
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

int run_cmd(char* cmd, char* immidiate, int PC, char *should_exit)
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
        should_exit
    );
}
