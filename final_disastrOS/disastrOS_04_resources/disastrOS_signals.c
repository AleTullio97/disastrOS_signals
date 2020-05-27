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

/*
typedef void (*disastrOS_sighandler_t)(int);
// at signum is the signal we want to capture
// at handler is the function pointer

disastrOS_sighandler_t signal(int signum, disastrOS_sighandler_t handler){
	//  SIG_DFL: default behavior
	//  SIG_IGN: explicitly ignore the signal
}
*/

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
	if(signals & DSOS_SIGCHLD){
		(*running->signals_handler[DSOS_SIGCHLD-1])();
	}
	if(signals & DSOS_SIGHUP){
		(*running->signals_handler[DSOS_SIGHUP-1])();
	}
	//printf("\nECCOME signals = %x\n",signals);
	/*
	if (log_file)
		fprintf(log_file, "TIME: %d\tPID: %d\tACTION: %s %d\n",
	    disastrOS_time,
	    running->pid,
	    "SYSCALL_OUT",
	    syscall_num);
	*/
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
	printf("HEHEHEHE! DSOS_SIGCHLD ignored!\n");
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGHUP_handler(){
	printf("HAHAHAHA! DSOS_SIGHUP ignored!\n");
}





