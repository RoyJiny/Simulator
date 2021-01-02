#include "disk_helper.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int disk_data[NOF_SECTORS][SECTOR_SIZE/sizeof(int)] = {0};
extern int io_registers[22];
extern int memory[MEMORY_SIZE];

void load_initial_disk_memory()
{
    FILE* disk_file = fopen(DISKIN, "r");
    char curr_line[9];
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
    // avoid accessing illegal index
    int sector_index = io_registers[DISKSECTOR] % NOF_SECTORS;
    int buffer_index = io_registers[DISKBUFFER] % MEMORY_SIZE;
    
    int idx = 0;
    while (idx < SECTOR_SIZE/sizeof(int)) {
        if (idx + buffer_index < MEMORY_SIZE) {
            disk_data[sector_index][idx] = memory[idx + buffer_index];
        } else {
            disk_data[sector_index][idx] = 0;
        }
        idx ++;
    }
}

void read_from_disk(){
    // avoid accessing illegal index
    int sector_index = io_registers[DISKSECTOR] % NOF_SECTORS;
    int buffer_index = io_registers[DISKBUFFER] % MEMORY_SIZE;
    int idx = 0;
    while (idx < SECTOR_SIZE/sizeof(int)) {
        if (idx + buffer_index < MEMORY_SIZE) {
            memory[idx + buffer_index] = disk_data[sector_index][idx];
        } else {
            printf("No more space to write in memory\n");
            return;
        }
        idx ++;
    }
}