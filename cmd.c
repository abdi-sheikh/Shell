#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "commando.h"

cmd_t *cmd_new(char *argv[]){
  int i;
  cmd_t *cmd = malloc(sizeof(cmd_t));
  for(i =0; i < ARG_MAX; i++){
      if (!argv[i])
        break;
      cmd -> argv[i] = strdup(argv[i]);
  }
    cmd -> argv[i] = NULL;
    cmd -> finished = 0;
    strcpy(cmd -> name, argv[0]);
    snprintf(cmd -> str_status,BUFSIZE, "INIT");
    cmd -> pid = -1;
    cmd -> status = -1;
    cmd -> output = NULL;
    cmd -> output_size = -1;

    return cmd;
}

void cmd_free(cmd_t *cmd){
  for(int i =0; ; i++){
    if(cmd->argv[i] == NULL)
      break;
    free(cmd -> argv[i]);
  }
  if(cmd -> output != NULL){
    free(cmd -> output);
  }

  free(cmd);

}

void cmd_start(cmd_t *cmd){
  pipe(cmd -> out_pipe);
  snprintf(cmd -> str_status, BUFSIZE ,"RUN");
  cmd -> pid = fork();
  if(cmd -> pid > 0){
    close(cmd -> out_pipe[PWRITE]);

  } else{
    dup2(cmd -> out_pipe[PWRITE],STDOUT_FILENO);
    close(cmd -> out_pipe[PREAD]);
    execvp(cmd -> name, cmd -> argv);
  }
}

void cmd_update_state(cmd_t *cmd, int block){
  if (cmd -> finished != 1){
    int status;
    pid_t child = waitpid(cmd -> pid, &status, block);
    if(child == cmd -> pid && WIFEXITED(status)){
      cmd -> finished = 1;
      cmd -> status = WEXITSTATUS(status);
      snprintf(cmd -> str_status, BUFSIZE ,"EXIT(%d)", cmd -> status);
      cmd_fetch_output(cmd);
      printf("@!!! %s[#%d]: %s\n",cmd ->name, child, cmd -> str_status);
    }

  }

}

char *read_all(int fd, int *nread){

    char *buffer = malloc(BUFSIZE);
    char *t_buffer = malloc(BUFSIZE);
    char *tmp_buffer = realloc(buffer, BUFSIZE * 2);
    int bytes_read;

    while((bytes_read = read(fd, t_buffer, BUFSIZE)) > 0){
      if(strlen(buffer) > BUFSIZE){
        strcpy(tmp_buffer,buffer);
        strcat(tmp_buffer,buffer);
      }
      tmp_buffer[bytes_read] = '\0';
    //reallocate buffer in the loop

    }
    *nread = bytes_read;



    return tmp_buffer;

}


void cmd_fetch_output(cmd_t *cmd){

  if (cmd -> finished ==0){
    printf("%s[#%d] not finished yet\n", cmd ->name, cmd -> pid);
  }
  else{
      read_all(cmd -> out_pipe[PREAD], &cmd -> output_size);
      close(cmd->out_pipe[PREAD]);
    }
  }

void cmd_print_output(cmd_t *cmd){
  if(cmd -> output != NULL){
    printf("%s[%d] has no output yet\n",cmd ->name, cmd -> pid );
  }
  else{
    printf("%s[%d]\n",cmd ->name, cmd -> pid );
  }

}
