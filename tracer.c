#include "tracer.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int registers[16];
extern int io_registers[22];

FILE *trace;
FILE *hwreg_trace;
FILE *leds_trace;

char io_regs_names[22][13]; /*12 is the max string lenght of io register*/

void init_trace()
{
    trace = fopen(TRACE, "w");
    hwreg_trace = fopen(HWREGTRACE, "w");
    leds_trace = fopen(LEDS_FILE, "w");
    /*for hwreg trace log*/
    strcpy(io_regs_names[0], "irq0enable");
    strcpy(io_regs_names[1], "irq1enable");
    strcpy(io_regs_names[2], "irq2enable");
    strcpy(io_regs_names[3], "irq0status");
    strcpy(io_regs_names[4], "irq1status");
    strcpy(io_regs_names[5], "irq2status");
    strcpy(io_regs_names[6], "irqhandler");
    strcpy(io_regs_names[7], "irqreturn");
    strcpy(io_regs_names[8], "clks");
    strcpy(io_regs_names[9], "leds");
    strcpy(io_regs_names[10], "reserved");
    strcpy(io_regs_names[11], "timerenable");
    strcpy(io_regs_names[12], "timercurrent");
    strcpy(io_regs_names[13], "timermax");
    strcpy(io_regs_names[14], "diskcmd");
    strcpy(io_regs_names[15], "disksector");
    strcpy(io_regs_names[16], "diskbuffer");
    strcpy(io_regs_names[17], "diskstatus");
    strcpy(io_regs_names[18], "monitorcmd");
    strcpy(io_regs_names[19], "monitorx");
    strcpy(io_regs_names[20], "monitory");
    strcpy(io_regs_names[21], "monitordata");
}

void write_regout()
{
    FILE *regout = fopen(REGOUT, "w");
    char line[9];
    int *runner = &registers[2];
    for(; runner < registers + 16; runner++){
        sprintf(line, "%08X", *runner);
        fputs(line, regout);
        fputs("\n", regout);
    }
    fclose(regout);
}

void write_cycles(int cycles, int nof_cmd)
{
    FILE *cycles_file = fopen(CYCLES, "w");
    char line[11 + 11 + 1];//int takes at most 10 decimal digits
    sprintf(line, "%d\n%d", cycles, nof_cmd);
    fputs(line, cycles_file);
    fclose(cycles_file);
}

void write_trace(int PC, char *inst, char *imm)
{
   // char line[3 + 5 + 16*8 + 17 + 1];
    //sprintf(line, "%03X %s ", PC, inst);
    fprintf(trace, "%03X %s ", PC, inst);
    int *runner = registers;
    int i = 0;
    for(; runner < registers + 16; runner++){
        if (i == 1 && imm != NULL) {
            //sprintf(&line[10 + i*9], "000%s ", imm);
            fprintf(trace, "000%s ", imm);
        } else {
            //sprintf(&line[10 + i*9], "%08X ", *runner);
            fprintf(trace, "%08X ", *runner);
        }
        i++;
    }
    //fputs(line, trace);
    //fputs("\n", trace);
    fprintf(trace, "\n");
}

void write_hwreg(int cycle, int reg, char read)
{
    IO_Register io_reg = (IO_Register) reg;
    char line[11 + 1 + 13 + 1 + 6 + 1 + 9]; //int takes at most 10 decimal digits
    char rw[6];
    if(read == 1) strcpy(rw, "READ");
    else strcpy(rw, "WRITE");
    sprintf(line, "%d %s %s %08X\n", cycle, rw ,io_regs_names[io_reg], io_registers[io_reg]);
    fputs(line, hwreg_trace);
}

void write_leds(int cycle)
{
    char line[11 + 1 + 8 + 1]; //int takes at most 10 decimal digits
    sprintf(line, "%d %08X\n", cycle, io_registers[LEDS]);
    fputs(line, leds_trace);
}

void clean_trace()
{
    fclose(trace);
    fclose(hwreg_trace);
    fclose(leds_trace);
}