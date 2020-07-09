#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"


void internal_pause(){
  assert(running);
  
  if (ready_list.first){
	 
    PCB* next_process=(PCB*) List_detach(&ready_list, ready_list.first);
    // at wait until a signal is received
    running->status=Waiting;
    List_insert(&waiting_list, waiting_list.last, (ListItem*) running);
    next_process->status=Running;
    running=next_process;
    printf("\nEND OF PAUSE!!!\n");
    // at here I am in the trap context.
	// at lets check signals at this point
	setcontext(&signal_context);
  }
}
