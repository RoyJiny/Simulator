#include "tracer.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

extern int registers[16];
extern int io_registers[22];

FILE *trace;

void init_trace()
{
    trace = fopen(TRACE, "w");
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

void write_trace(int PC, char *inst, char *imm)
{
    char line[3 + 5 + 16*8 + 17 + 1];
    sprintf(line, "%03X %s ", PC, inst);
    int *runner = registers;
    int i = 0;
    for(; runner < registers + 16; runner++){
        if (i == 1 && imm != NULL) {
            sprintf(&line[10 + i*9], "000%s ", imm);
        } else {
            sprintf(&line[10 + i*9], "%08X ", *runner);
        }
        i++;
    }
    fputs(line, trace);
    fputs("\n", trace);
}

void clean_trace()
{
    fclose(trace);
}