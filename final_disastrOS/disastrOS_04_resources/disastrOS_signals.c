#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#include "disastrOS_signals.h"
#include "disastrOS_constants.h"
#include "disastrOS.h"
#include "disastrOS_globals.h"



// #include "pool_allocator.h"
// #include "disastrOS_pcb.h"
// #include <ucontext.h>

/*
typedef void (*disastrOS_sighandler_t)(int);
// at signum is the signal we want to capture
// at handler is the function pointer

disastrOS_sighandler_t signal(int signum, disastrOS_sighandler_t handler){
	//  SIG_DFL: default behavior
	//  SIG_IGN: explicitly ignore the signal
}
*/

void signals_handle(){
	int signals = running->signals;
	if(signals & DSOS_SIGCHLD){
		disastrOS_SIGCHLD_handler();
	}
	if(signals & DSOS_SIGHUP){
		disastrOS_SIGHUP_handler();
	}
	//printf("\nECCOME signals = %x\n",signals);
	setcontext(&TRAMPOLINE);
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGCHLD_handler(){
	printf("DSOS_SIGCHLD ignored!\n");
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGHUP_handler(){
	printf("DSOS_SIGHUO ignored!\n");
}





