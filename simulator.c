#include "defines.h"
#include "executioner.h"
#include "simulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "irq_helper.h"
#include "disk_helper.h"
#include "tracer.h"

#define IRQ_ON ((io_registers[IRQ0ENABLE] & io_registers[IRQ0STATUS]) |\
               (io_registers[IRQ1ENABLE] & io_registers[IRQ1STATUS]) |\
               (io_registers[IRQ2ENABLE] & io_registers[IRQ2STATUS]))

/*declare global variables*/
int registers[16]       = {0};
int io_registers[22]    = {0};
int memory[MEMORY_SIZE] = {0};
char code[CODE_SIZE][CODE_LINE_SIZE+1] = {0};
char handling_irq_flag = 0;

int last_code_line;
int disk_cycles = 0;
int cycles = 0;
int cmd_counter = 0;

void load_initial_memory()
{
    FILE* memory_file = fopen(DMEMIN, "r");
    char line[MEMORY_LINE_SIZE+2];
    int *runner = memory;
    int i = 0;
    while (fgets(line, MEMORY_LINE_SIZE+2, memory_file) != NULL)
    {
        line[MEMORY_LINE_SIZE] = 0;
        *runner = (int)strtol(line, NULL, 16);
        runner++;
        if (++i > MEMORY_SIZE) break;
    }
    fclose(memory_file);
    printf("memory: %d, %d, %d\n", memory[1023], memory[1024], memory[1025]);
}

void extend_imm(char *imm, char *res)
{
    char extention = imm[0] >= '8' ? 'F' :'0';
    for (int i=0; i<8; i++) {
        res[i] = i <= 2 ? extention : *(imm+i-3);
    }
}

void store_memory()
{
    FILE* memory_file = fopen(DMEMOUT, "w");
    char line[MEMORY_LINE_SIZE+1];
    int *runner = memory;
    for(; runner < memory + MEMORY_SIZE; runner++){
        sprintf(line, "%08X", *runner);
        fputs(line, memory_file);
        fputs("\n", memory_file);
    }
    fclose(memory_file);
}

void load_code()
{
    FILE *code_file = fopen(IMEMIN, "r");
    char line[CODE_LINE_SIZE+2];
    last_code_line = 0;
    while(fgets(line, CODE_LINE_SIZE+2, code_file) != NULL) {
        memcpy(code[last_code_line], line, CODE_LINE_SIZE);
        code[last_code_line][CODE_LINE_SIZE] = 0;
        last_code_line++;
    }
    fclose(code_file);
}

void run()
{
    int PC = 0;
    char should_exit = 0;
    int next_cycle_to_trigger_irq2 = get_next_irq2_cycle();
    init_trace();
    // char extended_imm[9];
    // TODO: check when to stop running, writing to disk on last line won't happen
    while (PC < last_code_line && !should_exit) {
        if(io_registers[DISKSTATUS] == 1 && disk_cycles >= DISK_HANDLING_TIME - 1) {
            io_registers[DISKSTATUS] = 0;
            io_registers[DISKCMD] = 0;
            io_registers[IRQ1STATUS] = 1;
        }
        /*disk handling*/

        if(io_registers[DISKCMD] > 0){
            if(io_registers[DISKSTATUS] == 0){
                if(io_registers[DISKCMD] == 1) read_from_disk();
                else write_to_disk();
                disk_cycles = 0;
                io_registers[DISKSTATUS] = 1;
            }
        }

        if(next_cycle_to_trigger_irq2 > 0 && io_registers[TIMERCURRENT] > next_cycle_to_trigger_irq2){
            io_registers[IRQ2STATUS] = 1; /*trigger irq2*/
            next_cycle_to_trigger_irq2 = get_next_irq2_cycle();
        }
        /*irq handling*/
        if(!handling_irq_flag && IRQ_ON){
            handling_irq_flag = 1;
            io_registers[IRQRETURN] = PC;
            PC = io_registers[IRQHANDLER];
        }

        if (code[PC][2] == '1' || code[PC][3] == '1' || code[PC][4] == '1') { // uses immidiate
            //extend_imm(code[PC+1], extended_imm);
            write_imm_to_reg(code[PC+1]);
            write_trace(PC, code[PC]);
            increase_timer(); /*cmd with const takes extra cycle*/
            disk_cycles++;
            cycles++;
            cmd_counter++;
            PC = run_cmd(code[PC], code[PC+1], PC, cycles ,&should_exit);
        } else {
            write_trace(PC, code[PC]);
            cmd_counter++;
            PC = run_cmd(code[PC], NULL, PC, cycles,&should_exit);
        }

        increase_timer();
        cycles++;
        disk_cycles++;

    }
    write_regout();
    write_cycles(cycles, cmd_counter);
    clean_trace();
}