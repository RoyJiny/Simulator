#include "disk_helper.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int disk_data[NOF_SECTORS][SECTOR_SIZE/sizeof(int)] = {0};
extern int io_registers[22];

void load_initial_disk_memory()
{
    FILE* disk_file = fopen(DISKIN, "r");
    char* curr_line[9];
    int *sector_runner = *disk_data;
    while(fgets(curr_line, 9, disk_file) != NULL){
        curr_line[8] = 0;
        *sector_runner = (int)strtol(curr_line, NULL, 16);
        sector_runner++;
    }
    fclose(disk_file);
}

void store_disk_memory()
{
    FILE* disk_file = fopen(DISKOUT, "w");
    char line[9];
    int *sector_runner = *disk_data;
    for(; sector_runner < *disk_data + NOF_SECTORS*(SECTOR_SIZE/sizeof(int)); sector_runner++){
        sprintf(line, "%08X", *sector_runner);
        fputs(line, disk_file);
        fputs("\n", disk_file);
    }
    fclose(disk_file);
}

void write_to_disk(){
    
}

void read_from_disk(){
    
}