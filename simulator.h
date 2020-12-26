#ifndef SIMULATOR_H
#define SIMULATOR_H

/*
* load dmemin into a buffer.
*/
void load_initial_memory();

/*
* load assemly code into a buffer.
*/
void load_code();

/*
* store buffer in file dmemout.
*/
void store_memory();

/*
* run simulator
*/
void run();

#endif