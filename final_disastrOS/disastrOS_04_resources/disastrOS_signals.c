#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "disastrOS_signals.h"
#include "disastrOS_constants.h"
#include "disastrOS.h"
#include "disastrOS_globals.h"

// #include "pool_allocator.h"
// #include "disastrOS_pcb.h"
// #include <ucontext.h>

void signals_check(){
  // at swap to signal context 
  printf("\nNothing to be done for now.\n");
  fflush(stdout);

  /*
  if (signal(SIGINT, sigHandler) == SIG_ERR) {
    printf("p1|error can't catch SIGINT\n");
  }
  */
  
  // at swap to process context
}

void  disastrOS_SIGCHLD_handler(int sig_number_){
	if(sig_number_ != DSOS_SIGCHLD){
		printf("!!! WRONG SIGNAL !!!\n");
		exit(EXIT_FAILURE);
	}
	printf("Received DSOS_SIGCHLD");
}

void  disastrOS_SIGHUP_handler(int sig_number_){
	if(sig_number_ != DSOS_SIGCHLD){
		printf("!!! WRONG SIGNAL !!!\n");
		exit(EXIT_FAILURE);
	}
	printf("Received DSOS_SIGCHLD");
}


void sigHandler(int sig_number_) {
  if (sig_number_ != DSOS_SIGHUP){
    printf("p2|error, wrong signal\n");
    exit(EXIT_FAILURE);
  }
  printf("p2|received SIGHUP, stopping consumption\n");
  running = 0;
}
