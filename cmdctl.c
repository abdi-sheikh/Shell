#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commando.h"

void cmdctl_add(cmdctl_t *ctl, cmd_t *cmd){
    if(ctl -> size < MAX_CHILD){
      ctl -> cmd[ctl -> size++] = cmd;
    }
    else{
      printf("cmdctl_new error\n" );
    }
}
// Add the given cmd to the ctl structure. update the cmd[] array and
// size field.

void cmdctl_print(cmdctl_t *ctl){
  for(int i = 0; i < ctl -> size; i ++){
    printf("%-5d#%-8d%5d%11s%5d%s\n",i, (*ctl -> cmd) -> pid, (*ctl -> cmd) -> status, (*ctl -> cmd) -> str_status, (*ctl -> cmd) -> output_size, (*ctl -> cmd) -> argv[i]);
  }
}
//
//
void cmdctl_update_state(cmdctl_t *ctl, int block){
  for(int i = 0; i < ctl -> size; i ++){
    cmd_update_state(ctl -> cmd[i], block);
  }

}

void cmdctl_freeall(cmdctl_t *ctl){
    for(int i = 0; i < ctl -> size; i ++){
    cmd_free(ctl -> cmd[i]);
  }
}
// // Call cmd_free() on all of the constituent cmd_t's.
