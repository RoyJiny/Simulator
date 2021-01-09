#ifndef EXECUTIONER_H
#define EXECUTIONER_H

/*
* execute cmd.
*
* @param cmd: command to be executed.
* @param immidiate: if command doesn't use immidiate then immediate = NULL,
* else, immidiate value.
* @param PC: command PC.
* @ret return updated PC.
*/
int run_cmd(char* cmd, char* immidiate, int PC, int cycle,char *should_exit);

/*
 write the value of the immidiate to the register
 this is a seperate function so we could have the right actions order when writing the trace
*/
void write_imm_to_reg(char *immidiate);

#endif