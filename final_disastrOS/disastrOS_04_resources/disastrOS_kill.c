#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"

// non ho impostato un proesso in waiting list quando riceve un segnale. va bene?

void internal_kill() {
  // at sanity check
  assert(running);  
  
  // at similar to standard POSIX defined kill : int kill(pid_t pid, int sig);
  int pid = running->syscall_args[0];
  int sig = running->syscall_args[1];
  // PCBPtr* targetPCB = PCBPtr_byPID(ListHead* head, pid)
  
  // at undefined SIGNAL has been sent
  if( sig!=DSOS_SIGCHLD || sig!=DSOS_SIGHUP ){
	  running->syscall_retvalue=DSOS_EINVAL;
	  return;
  }
  
  // at case: a raise function has been executed -> don't do nothing
  // at explaination: Since I'm implementing just the SIGHUP and SIGCHLD signals,
  // at the raise fuction does not have so much sense.
  if(running->pid == pid){
	  running->syscall_retvalue=0;
	  return;
  }
  
  /*
  // at case: the process masks the raised signal.
  if((sig & running->signals_mask)!=0){
	  
	  return;
  }
  */
  
  // at here we look in the child list
  if(sig == DSOS_SIGCHLD){
	  // at the SIGCHLD can be sent only to the parent process
	  if((!running->parent) || (running->parent->pid != pid)){
		  running->syscall_retvalue=DSOS_ESRCH;
		  return;
	  }else {
		  running->parent->signals|=sig;
		  running->syscall_retvalue=0;
		  return;
	  }
  }
  
  // at here we look at te parent (if it is alive)
  if( sig == DSOS_SIGHUP){
	  // don't know at the moment how to implement.  
  } 
}
