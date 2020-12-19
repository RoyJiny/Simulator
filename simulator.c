#include "defines.h"
#include "executioner.h"
#include "simulator.h"
#include <stdlib.h>
#include <stdio.h>

/*declare global variables*/
int registers[16]       = {0};
int io_registers[22]    = {0};
int memory[MEMORY_SIZE] = {0};


void load_initial_memory()
{
    FILE* memory_file = fopen(DMEMIN, 'r');
    char line[MEMORY_LINE_SIZE+1];
    int *runner = memory;
    while (fgets(line, MEMORY_LINE_SIZE, memory_file) != NULL)
    {
        line[MEMORY_LINE_SIZE] = 0;
        *runner = (int)strtol(line, NULL, 16);
        runner++;
    }
    fclose(memory_file);
}

void store_memory()
{
    FILE* memory_file = fopen(DMEMOUT, 'w');
    char line[MEMORY_LINE_SIZE+1];
    int *runner = memory;
    for(; runner < memory + MEMORY_SIZE; runner++){
        sprintf(line, "%08X", *runner);
        fputs(line, memory_file);
        fputs("\n", memory_file);
    }
    fclose(memory_file);
}

