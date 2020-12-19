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
int run_cmd(char* cmd, char* immidiate, int PC);

#endif