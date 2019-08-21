#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commando.h"

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering
    char line[MAX_LENGTH];
    int ntokens = 0;
    char *tokens[NAME_MAX];
    cmdctl_t command;
    command.size = 0;
    cmd_t *c;

    while (1) {
      printf("@> ");
      char *ret = fgets(line,MAX_LINE,stdin);
      if (!ret) break;



      // Parse and execute command
      parse_into_tokens(ret,tokens,&ntokens);
      if (ntokens == 0) {
      }

      else{
        if (strcmp(tokens[0], "echo") == 0) {
          int i=0;
          while(tokens[i] != NULL){
          printf("%s ", tokens[i]);    // EXTRA SPACE AFTER LAST TOKEN
          i++;
          }
        printf("\n");
        } else if (strcmp(tokens[0], "help") == 0) {
          printf("COMMANDO COMMANDS\n");
          printf("help               : show this message\n");
          printf("exit               : exit the program\n");
          printf("list               : list all jobs that have been started giving information on each\n");
          printf("pause nanos secs   : pause for the given number of nanseconds and seconds\n");
          printf("output-for int     : print the output for given job number\n");
          printf("output-all         : print output for all jobs\n");
          printf("wait-for int       : wait until the given job number finishes\n");
          printf("wait-all           : wait for all jobs to finish\n");
          printf("command arg1 ...   : non-built-in is run as a job\n");

        } else if(strcmp(tokens[0], "list") ==0){
          printf("%-5s%-10s%-7s%-9s%-5s%s\n","JOB", "#PID", "STAT", "STR_STAT", "OUTB", "COMMAND");
          cmdctl_print(&command);

        }else if (strcmp(tokens[0], "pause") == 0) {
          pause_for(atol(tokens[1]),atoi(tokens[2]));

        }else if (strcmp(tokens[0], "output-for") == 0) {
          cmd_print_output(c );

        }else if (strcmp(tokens[0], "output-all") == 0) {
            cmd_print_output(c);

        }else if (strcmp(tokens[0], "wait-for") == 0) {
          cmd_update_state(c,NOBLOCK);

        }else if (strcmp(tokens[0], "wait-all") == 0) {
          while(command.size != 0){
            cmdctl_update_state(&command,NOBLOCK);
          }

        }else if (strcmp(tokens[0], "exit") == 0) {
          cmdctl_freeall(&command);
          break;

        }else {
            c = cmd_new(tokens);
            cmdctl_add(&command,c);
            cmd_start(c);

        }

      }
      cmdctl_update_state(&command,NOBLOCK);
    }

    return 0;
  }
