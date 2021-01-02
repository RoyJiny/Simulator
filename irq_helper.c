#include "defines.h"
#include "irq_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char file_open_flag = 0;
FILE* irq2_file;
extern int io_registers[22];

int get_next_irq2_cycle(){
    if(!file_open_flag){
        irq2_file = fopen(IRQ2IN, "r");
        file_open_flag = 1;
    }
    char next_cylce[12]; /*2^32 has 10 digits*/
    if( fgets(next_cylce, 12, irq2_file) != NULL){
        return atoi(next_cylce);
    }
    else{
        fclose(irq2_file);
        return -1;
    }
}

void increase_timer()
{
    // if(io_registers[TIMERENABLE] == 1) io_registers[TIMERCURRENT]++;
    io_registers[TIMERCURRENT]++;
    if((io_registers[TIMERCURRENT]) == io_registers[TIMERMAX]){
        io_registers[TIMERCURRENT] = 0;
        io_registers[IRQ0STATUS] = 1; /*trigger irq0*/
    }
}