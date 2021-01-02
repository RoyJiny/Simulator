#include "simulator.h"
#include "disk_helper.h"
#include "monitor_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
	printf("loading initial memory\n");
	printf("------------------------\n");
	load_initial_memory();
	printf("loading initial disk memory\n");
	printf("------------------------\n");
	load_initial_disk_memory();
	printf("\nloading code\n");
	printf("------------------------\n");
	load_code();
	printf("\nrunning code\n");
	printf("------------------------\n");
	run();
	printf("\nstoring memory\n");
	printf("------------------------\n");
	store_memory();
	printf("\nstoring disk memory\n");
	printf("------------------------\n");
	store_disk_memory();
	printf("\ncreating monitor files\n");
	printf("------------------------\n");
	write_monitor_data();
	return 0;
}