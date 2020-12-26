#ifndef DISK_HELPER_H
#define DISK_HELPER_H

#define NOF_SECTORS 128
#define SECTOR_SIZE 512
#define DISK_HANDLING_TIME 1024

/*
* load diskin into a buffer.
*/
void load_initial_disk_memory();

/*
* store buffer in file diskout.
*/
void store_disk_memory();

void write_to_disk();

void read_from_disk();

#endif
