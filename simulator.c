#include "defines.h"
#include "executioner.h"
#include "simulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*declare global variables*/
int registers[16]       = {0};
int io_registers[22]    = {0};
int memory[MEMORY_SIZE] = {0};
char code[CODE_SIZE][CODE_LINE_SIZE+1] = {0};
int last_code_line;

void load_initial_memory()
{
    FILE* memory_file = fopen(DMEMIN, "r");
    char line[MEMORY_LINE_SIZE+1];
    int *runner = memory;
    while (fgets(line, MEMORY_LINE_SIZE+1, memory_file) != NULL)
    {
        line[MEMORY_LINE_SIZE] = 0;
        *runner = (int)strtol(line, NULL, 16);
        runner++;
    }
    fclose(memory_file);
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
    while (PC < last_code_line) {
        // printf("line is '%s'\n", code[PC]);
        if (code[PC][2] == '1' || code[PC][3] == '1' || code[PC][4] == '1') { // uses immidiate
            PC = run_cmd(code[PC], code[PC+1], PC, &should_exit);
        } else {
            PC = run_cmd(code[PC], NULL, PC, &should_exit);
        }
        if (should_exit == 1) {
            return;
        }
    }
}

