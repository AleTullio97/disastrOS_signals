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

// at here i'm in the interrupt_context
void signals_handler(){
	int signals = running->signals;
	if( signals){
		printf("\nPID: %d\n<- signals_handler() ->\n",running->pid);
		printf("\nSignals received = %x\n",signals);
	}
	if(signals & DSOS_SIGCHLD){
		running->signals &=~DSOS_SIGCHLD; 
		setcontext(&running->signal_context[DSOS_SIGCHLD-1]);
	}
	if(signals & DSOS_SIGHUP){
		running->signals &=~DSOS_SIGHUP;
		setcontext(&running->signal_context[DSOS_SIGHUP-1]);
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
	printf("DSOS_SIGCHLD RECEIVECD!\n");
	int res;
	PCB* child_pcb=0;
	PCB* died_pcb=0;
	PCBPtr* child_pcb_ptr=0;
	PCBPtr* died_pcb_ptr=0;
	ListItem* aux=running->children.first;
	while(aux){
		child_pcb_ptr=(PCBPtr*) aux;
		child_pcb=child_pcb_ptr->pcb;
		if (child_pcb->status==Zombie) {
			died_pcb=child_pcb;
			died_pcb_ptr=child_pcb_ptr;
			break;
		}
		aux=aux->next;
	}
	if(died_pcb){
		res = died_pcb->return_value;
		if(res >= 0){
			printf("PID: %d - My child has completed his work.\n", disastrOS_getpid());
		}
		else printf("PID: %d - My child has not completed his work correctly.\nIgnore the cause.", disastrOS_getpid());
		// remove the children pcb from children list
		List_detach(&running->children, (ListItem*) died_pcb_ptr);
		PCBPtr_free(died_pcb_ptr);

		// remove he pc from zombie pool
		List_detach(&zombie_list, (ListItem*) died_pcb);
		running->syscall_retvalue = died_pcb->pid;
		PCB_free(died_pcb);
	}else {
		printf("PID: %d - Can't find a died child.\n", disastrOS_getpid());
	}
	signals_handler(); // at continue handling other signals
}

// at TO BE IMPLEMENTED SOON...
void  disastrOS_SIGHUP_handler(){
	printf("DSOS_SIGHUP RECEIVED!\n");
		
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


