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
	/*
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
	*/
	return 0;
}


// at here i'm in the interrupt_context
void signals_handler(){
	printf("PID: %d\n<- signals_handler() ->\n",running->pid);
	int signals = running->signals;
	printf("\nSignals received = %x\n",signals);
	if(signals & DSOS_SIGCHLD){
		printf("DSOS_SIGCHLD RECEIVECD!\n");
		running->signals &=~DSOS_SIGCHLD; 
		// at verify the handler is available
		if(running->signal_context[DSOS_SIGCHLD-1]) setcontext(&running->signal_context[DSOS_SIGCHLD-1]);
	}
	if(signals & DSOS_SIGHUP){
		printf("DSOS_SIGHUP RECEIVED!\n");
		running->signals &=~DSOS_SIGHUP;
		// at verify the handler is available
		if(running->signal_context[DSOS_SIGHUP-1]) setcontext(&running->signal_context[DSOS_SIGHUP-1]);
	}
	if (running){
		disastrOS_debug(" %d\n", running->pid);
		setcontext(&running->cpu_state);
	}else {
		printf("WARNING : no active processes\n");
		disastrOS_printStatus();
	}
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGCHLD_handler(){
	printf("PID: %d\nHEHEHEHE! DSOS_SIGCHLD ignored by DEFAULT!\n",running->pid);
	// at directly return to the running_pcb context
	//setcontext(&running->cpu_state);
	signals_handler(); // at continue handling other signals
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGHUP_handler(){
	printf("PID: %d\nHAHAHAHA! DSOS_SIGHUP ignored by DEFAULT!\n",running->pid);
	// at directly return to the running_pcb context
	//setcontext(&running->cpu_state);
	signals_handler(); // at continue handling other signals
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


