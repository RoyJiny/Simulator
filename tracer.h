#ifndef TRACER_H
#define TRACER_H


/*
* before starting to log information , user need to call this function
* open neccassery files. 
*/
void init_trace();

/*
* write registers data in regout.txt file.
*/
void write_regout();

/*
* write trace of commands in trace.txt file
*/
void write_trace(int PC, char *inst, char *imm);

/*
* write total command count and total execution time (in cycles).
*/
void write_cycles(int cycles, int nof_cmd);

/*
* write trace everytime a led changed its status
*/
void write_leds(int cycle);

/*
* write trace of hw registers read or write in hwregtrace.txt file
*/
void write_hwreg(int cycle, int reg, char read);

/*
* cleaninig up at the end.
*/
void clean_trace();

void
compareFiles(char *our_file, char *example_file);

#endif