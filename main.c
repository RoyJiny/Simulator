#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {
	printf("loading initial memory\n");
	printf("------------------------\n");
	load_initial_memory();
	printf("\nloading code\n");
	printf("------------------------\n");
	load_code();
	printf("\nrunning code\n");
	printf("------------------------\n");
	run();
	printf("\nstoring memory\n");
	printf("------------------------\n");
	store_memory();
	return 0;
}