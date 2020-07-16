#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_globals.h"
#include "disastrOS_timer.h"

// at send SIGNAL to the target process
	
void internal_kill() {
  // at sanity check
  assert(running);  
  // at similar to standard POSIX defined kill : int kill(pid_t pid, int sig);
  int pid = running->syscall_args[0];
  int sig = running->syscall_args[1];
    
  // at undefined SIGNAL has been sent
  // at NOTE: can't send multiple signals at the same time
  if( sig!=DSOS_SIGCHLD && sig!=DSOS_SIGHUP ){
	  running->syscall_retvalue=DSOS_EINVAL;
	  // DELETE printf("ERRORE1\n");
	  return;
  }
  
  // at internal_raise()
  if(running->pid == pid){
	  running->syscall_retvalue=0;
	  running->signals|=(sig & running->signals_mask);
	  // DELETE printf("ECCOME2\n");
	  return;    
	  }
  
  PCB* targetPCB=0;
  
  // at SIGCHLD can be sent to the parent only 
  if(sig == DSOS_SIGCHLD){
	  printf("Something is going wrong");
	  if((!running->parent) || (running->parent->pid != pid)){
		  running->syscall_retvalue=DSOS_ESRCH;
		  return;
	  }else {
		  running->parent->signals|=(sig & running->parent->signals_mask);
		  running->syscall_retvalue=0;
		  targetPCB=running->parent;
	  }
	  // DELETE printf("ECCOME3\n");
  }
  
  // at finding the target PCB
  // at if not in the children list return error
  if(sig == DSOS_SIGHUP){
	  printf("PID: %d -> DSOS_SIGHUP is goingo to be sento to pid : %d!\n", disastrOS_getpid(), pid);
      if (!running->children.first){
		  running->syscall_retvalue = DSOS_ESRCH;
		  return;
	  }
	  // at finding the target children PCB
	  PCB* target_pcb=0;
	  PCBPtr* target_pcb_ptr=0;
	  ListItem* aux=running->children.first;
	  while(aux){
		  target_pcb_ptr=(PCBPtr*) aux;
		  target_pcb=target_pcb_ptr->pcb;
		  if (pid==target_pcb->pid) {
			targetPCB = target_pcb;
			break;
			}
		  aux=aux->next;
	  }

	  if(targetPCB){
		  targetPCB->signals |= (sig & targetPCB->signals_mask);
		  running->syscall_retvalue=0;
		  // DELETE printf("TargetPCB trovato : %d\n", targetPCB->pid);
	  }else {
		  printf("Error: PID %d not found in children list\n", pid);
		  running->syscall_retvalue=DSOS_ESRCH;
		  return;
	  }
	  // DELETE printf("ECCOME4\n");
  }
  // at signal has been sent correctly
  if(targetPCB && targetPCB->status==Waiting){
		  if(targetPCB->timer){
			  // DELETE printf("Removing timer \n");
			  TimerList_removeTimer(&timer_list , (TimerItem*) targetPCB->timer);
		  }
		  // DELETE else printf("No timer installed \n");
		  List_detach(&waiting_list, (ListItem*) targetPCB);
		  targetPCB->status=Ready;
		  List_insert(&ready_list, ready_list.last, (ListItem*) targetPCB);
		  targetPCB->timer=0;
		  // DELETE printf("ECCOME SPECIALE");
  }
  // DELETE printf("ECCOME SPECIALE2\n");
}
