/*
* File:                 mish.c
* Authur:               Elias Niko
* Email:                c18eno@cs.umu.se
* Current version:      1.0
* Date:                 2019-10-11
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>

#include "execute.h"
#include "parser.h"
#include "mish.h"
#include "sighant.h"


#define STRCMP(a, R, b) strcmp(a, b) R 0


// global int processes
int *process;


void print_prompt(void){
      if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO)) {
            fprintf(stdout, "mish%% ");
            fflush(stdout);
      }
}



void read_command_line(int argc, const char **argv){
      (void)argv;
      command cmds[MAXCOMMANDS + 1];
      char *line = malloc(sizeof(char)*MAXLINELEN);
       if (line == NULL) {
             perror("INPUT BUFFER");
             exit(EXIT_FAILURE);
       }
       while(fgets(line, MAXLINELEN, stdin) != NULL){
             if ((argc = parse(line, cmds)) > 0){
                   run_command(cmds, argc);
                   print_prompt();
             }
             else {
                   print_prompt();
             }
      }
      free(line);
}


void run_command(command cmds[], int argc){
      for (int i = 0; i < argc; i++) {
            cmds[i].internal = 0;
            if(STRCMP(cmds[i].argv[0], == , "cd")){
                  cmds[i].internal = 1;

                  if (run_cd(cmds[i].argc,cmds[i].argv) < 0) {
                        perror("cd");
                  }
            }
            if(STRCMP(cmds[i].argv[0], == , "echo")){
                  cmds[i].internal = 1;
                  run_echo(cmds[i]. argc, cmds[i].argv);
            }
      }
      if (cmds[0].internal == 0) {
            extern_command(cmds, argc);
      }
}

void run_echo(int argc, char *argv[]){
      int counter = 1;
      while(counter < argc){
            printf("%s", argv[counter]);
            if (argv[counter + 1] != NULL) {
                  printf(" ");
            }
            counter++;
      }
      printf("\n");
}


int run_cd(int argc, char *argv[]){
      if (argc == 1) {
            if (chdir(getenv("HOME")) == -1) {
                  perror(argv[1]);
                  return -1;
            }
      }
      if (argc == 2) {
            if (chdir(argv[1]) == -1) {
                  perror(argv[1]);
                  return -1;
            }
      }
      if (argc > 2) {
            fprintf(stderr, "%s: Too many arguments.", argv[0]);
            return -1;
      }
      char currnet_work_dir[PATH_MAX];
      if (getcwd(currnet_work_dir, sizeof(currnet_work_dir)) == NULL){
            perror("Error: getcwd()");
      }
      return 0;
}


void extern_command(command cmds[], int argc) {

      int old_pipe[2];
      int new_pipe[2];
      int pid;

      for (int i = 0; i < argc; i++){
            if (cmds[i+1].argv != NULL){
                  int do_pipe = pipe(new_pipe);
                  if(do_pipe < 0){
                        printf("Error pipe()");
                        exit(1);
                  }
            }
            pid = fork();
            if (pid < 0){
                  perror("Error: Fork()");
                  exit(1);
            }
            else if(pid == 0){
                  if (cmds[i].infile != NULL) {
                        close(STDIN_FILENO);
                        redirect(cmds[i].infile, O_RDONLY, STDIN_FILENO);
                  }
                  if (cmds[i].outfile !=NULL) {
                         close(STDOUT_FILENO);
                         int flg = O_WRONLY | O_CREAT | S_IRWXU | O_EXCL;
                         int red = redirect(cmds[i].outfile, flg, STDOUT_FILENO);
                         if(red < 0)
                              perror("Error: could not redirect stdout to outfile!");
                        }
                  if(i > 0){
                        int dup_old = dupPipe(old_pipe, READ_END, STDIN_FILENO);
                        if(dup_old < 0){
                              perror("Error: Not able to dup!");
                              exit(1);
                        }
                        close(old_pipe[READ_END]);
                        close(old_pipe[WRITE_END]);
                  }
                  if (cmds[i+1].argv != NULL) {
                        int dup_new = dupPipe(new_pipe, WRITE_END, STDOUT_FILENO);
                        if(dup_new < 0){
                              perror("Error: could not dupplicate new_pipe");
                              exit(1);
                        }
                        close(new_pipe[READ_END]);
                        close(new_pipe[WRITE_END]);
                  }
                  int exe = execvp(*cmds[i].argv, cmds[i].argv);
                  if (exe < 0) {
                        exit(1);
                  }
                  exit(1);
            }
            else{
                  process[i] = pid;
                  if(i > 0){
                        close(old_pipe[READ_END]);
                        close(old_pipe[WRITE_END]);
                  }
                  if (cmds[i+1].argv != NULL) {
                        old_pipe[READ_END] = new_pipe[READ_END];
                        old_pipe[WRITE_END] = new_pipe[WRITE_END];
                  }
            }
      }
      if (argc >= 1) {
            close(old_pipe[READ_END]);
            close(old_pipe[WRITE_END]);
            for (int i = 0; i < argc; i++) {
                  // wait for child processes
                  wait(0);

            }
      }
}
/*
* The main function for mish which allocates momory for global variable process.
* calls mysignal to handle SIGINT. and deallocates the memory of global variable process.
*/

int main(int argc, const char **argv){
      mysignal(SIGINT, sighandle);
      process = malloc(sizeof(int)*MAXCOMMANDS);
      print_prompt();
      read_command_line(argc, argv);
      free(process);
      return 0;
}
