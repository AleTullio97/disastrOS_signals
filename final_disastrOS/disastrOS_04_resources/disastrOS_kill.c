#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"

// at send SIGNAL to the target process

void internal_kill() {
  // at sanity check
  assert(running);  

  // at similar to standard POSIX defined kill : int kill(pid_t pid, int sig);
  int pid = running->syscall_args[0];
  int sig = running->syscall_args[1];
  // PCBPtr* targetPCB = PCBPtr_byPID(ListHead* head, pid)
  //printf("SENDING SIGNAL: %d TO PID: %d\n",sig,pid);
    
  // at undefined SIGNAL has been sent
  if( sig!=DSOS_SIGCHLD && sig!=DSOS_SIGHUP ){
	  running->syscall_retvalue=DSOS_EINVAL;
	  //printf("ERRORE1\n");
	  return;
  }
  
  // at case: a raise function has been executed -> don't do nothing
  // at explaination: Since I'm implementing just the SIGHUP and SIGCHLD signals,
  // at the raise fuction does not have so much sense.
  if(running->pid == pid){
	  running->syscall_retvalue=0;
	  running->signals|=(sig & running->signals_mask);
	  //printf("ECCOME2\n");
	  return;
  }
  
  PCB* targetPCB=0;
  // at SIGCHLD can be sent to the parent only 
  if(sig == DSOS_SIGCHLD){
	  if((!running->parent) || (running->parent->pid != pid)){
		  running->syscall_retvalue=DSOS_ESRCH;
	  }else {
		  running->parent->signals|=(sig & running->parent->signals_mask);
		  running->syscall_retvalue=0;
		  targetPCB=running->parent;
	  }
	  //printf("ECCOME3\n");
  }
  
  // at finding the target PCB
  // at if not in the children list return error
  if(sig == DSOS_SIGHUP){
	  targetPCB = PCB_byPID(&(running->children), pid);
	  if(targetPCB)
	  {
		  targetPCB->signals |= (sig & targetPCB->signals_mask);
		  running->syscall_retvalue=0;
	  }
	  else {
		  running->syscall_retvalue=DSOS_ESRCH;
	  }
	  //printf("ECCOME4\n");
  }
  // at ie if the process has received any signals
  if(targetPCB){
	  if(targetPCB->status==Waiting){
		  List_detach(&waiting_list, (ListItem*) targetPCB);
		  targetPCB->status=Ready;
		  targetPCB->timer=0;
		  List_insert(&ready_list, (ListItem*) targetPCB, (ListItem*) targetPCB);
	  }
  }
}
