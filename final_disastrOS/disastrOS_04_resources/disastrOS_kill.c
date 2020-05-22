#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"

void internal_kill() {
  
  // at: similar to linux defined kill : int kill(pid_t pid, int sig);
  int pid = running->syscall_args[0];
  int sig = running->syscall_args[1];
  
  // at: TO BE DEFINED...
  
  /*
  // if we were looking for a process not in child list
  // we need to return an error
  if(pid_to_wait>0){
    if(! awaited_pcb){
      pid_to_wait = -1;
      running->syscall_retvalue = DSOS_EWAIT;
      return;
    } 
  }
  
  // if the pid is in zombie status, we return the value and free the memory
  if (awaited_pcb && awaited_pcb->status==Zombie) {
    // remove the awaited pcb from children list
    List_detach(&running->children, (ListItem*) awaited_pcb_ptr);
    PCBPtr_free(awaited_pcb_ptr);

    // remove he pc from zombie pool
    List_detach(&zombie_list, (ListItem*) awaited_pcb);
    running->syscall_retvalue = awaited_pcb->pid;
    if (result)
      *result = awaited_pcb->return_value;
    PCB_free(awaited_pcb);
    return;
  }

  // all fine, but the process is not a zombie
  // need to sleep
  running->status=Waiting;
  List_insert(&waiting_list, waiting_list.last, (ListItem*) running);

  // pick the next
  PCB* next_running= (PCB*) List_detach(&ready_list, ready_list.first);
  running=next_running;
	*/
}
