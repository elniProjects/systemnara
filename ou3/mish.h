/*
* File:                 mish.h
* Authur:               Elias Niko
* Email:                c18eno@cs.umu.se
* Current version:      1.0
* Date:                 2019-10-11
*/


#ifndef MISH_H
#define MISH_H



/*
* prints mish%% to terminal

* argumnt: void

* returns: -
*/

void print_prompt(void);

/*
* Reads the command from the user line by line and uses parse() to pipe.

* argumnt: argc, constant char pointer to argv

* returns: -
*/
void read_command_line(int argc, const char **argv);

/*
* Runs the internal commands which is separated by parse() with |.
* if the commands are internal runs those commands in the main process,
* otherwise send them to extern_command().

* argumnt: cmds as commands into array
            argc as number of commands

* returns: -
*/
void run_command(command cmds[], int argc);

/*
* This function runs the cd (change directory).

* argumnt: argc as number of commands
            argc as char pointer

* returns: -
*/
int run_cd(int argc, char *argv[]);

/*
* This function runs the external commands wich is not implemented in the mish.
* this function forks child processes and gives the commands to childs.

* argumnt: cmds as sctruct command array.
            argc as number of commands.

* returns: -
*/
void extern_command(command cmds[], int argc);

/*
* This function is implemented as a simple non advanced echo which prints
* the arguments to stdout.

* argumnt: argc as number of commands
            argv as a char pointer array as the command

* returns: -
*/

void run_echo(int argc, char *argv[]);
#endif
