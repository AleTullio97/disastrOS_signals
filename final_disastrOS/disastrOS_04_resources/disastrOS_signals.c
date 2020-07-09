#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#include "disastrOS_signals.h"
#include "disastrOS_constants.h"
#include "disastrOS.h"
#include "disastrOS_globals.h"
#include "disastrOS_pcb.h"

typedef void (*disastrOS_sighandler_t)(void);


// at change running->signals_hanlder[signum] to handler
// at return 0 on success
// at return -1 on failure
int disastrOS_signal(int signum, disastrOS_sighandler_t handler){
	// at undefined SIGNAL has been sent
	if( signum!=DSOS_SIGCHLD && signum!=DSOS_SIGHUP){
		return -1;
	}
	else if(handler==DSOS_SIG_IGN){
		running->signals_mask &= ~signum; // at avoid to receive signum signal
		running->signals &= ~signum;
		return 0;	  
	}
	else if(handler==DSOS_SIG_DFL){
		if(signum==DSOS_SIGCHLD){
			running->signals_handler[DSOS_SIGCHLD] = disastrOS_SIGCHLD_handler;
			return 0;
		}
		else if(signum==DSOS_SIGHUP){
			running->signals_handler[DSOS_SIGHUP] = disastrOS_SIGHUP_handler;
			return 0;
		}
	}// at here we assume that handler is a user define function handler
	else{
		running->signals_handler[signum] = handler;
		return 0;		
	}
	return 0;
}
	
void cpy_signals_handler(PCB* child, PCB* parent){
	int strct_size = sizeof(child->signals_handler);
	char* buf_in = (char*) parent->signals_handler;
	char* buf_out = (char*) child->signals_handler;
	for(int i=0; i<strct_size; i++){
		*buf_out = *buf_in;
		buf_out++; buf_in++;
	}
}  

void signals_handle(){
	int signals = running->signals;
	printf("\nECCOME signals = %x\n",signals);
	if(signals & DSOS_SIGCHLD){
		(*running->signals_handler[DSOS_SIGCHLD-1])();
		running->signals &=~DSOS_SIGCHLD; 
	}
	if(signals & DSOS_SIGHUP){
		(*running->signals_handler[DSOS_SIGHUP-1])();
		running->signals &=~DSOS_SIGHUP;
	}
	if (running){
		disastrOS_debug(" %d\n", running->pid);
		setcontext(&running->cpu_state);
	}else {
		printf("no active processes\n");
		disastrOS_printStatus();
	}
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGCHLD_handler(){
	printf("HEHEHEHE! DSOS_SIGCHLD ignored by DEFAULT!\n");
	// at directly return to the running_pcb context
	setcontext(&running->cpu_state);
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGHUP_handler(){
	printf("HAHAHAHA! DSOS_SIGHUP ignored by DEFAULT!\n");
	// at directly return to the running_pcb context
	setcontext(&running->cpu_state);
}

// at NEW disastrOS syscall wrapper defined HERE
// at send signal sig to pid
int disastrOS_kill(int pid, int sig){
	return disastrOS_syscall(DSOS_CALL_KILL, pid, sig);
}

// at raise is equivalent to the kill syscall, but pid = running->pid
int disastrOS_raise(int sig){
	return disastrOS_syscall(DSOS_CALL_KILL, running->pid, sig);
}
// at wait until a signal is received
int disastrOS_pause(void){
	return disastrOS_syscall(DSOS_CALL_PAUSE);
}


