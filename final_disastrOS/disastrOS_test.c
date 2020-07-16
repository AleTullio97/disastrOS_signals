#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"
#include "disastrOS_globals.h"
#include "disastrOS_signals.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void function_zero(void* args){
	
  printf("Hello, I'm function zero ( PID : %d ) and I sleep 40 before terminating\n", disastrOS_getpid());
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  disastrOS_sleep(40);
  printf("PID: %d, terminating\n", disastrOS_getpid());
  
  disastrOS_exit(disastrOS_getpid()+1);
}

void function_one(void*args){
	
  printf("Hello, I'm function one ( PID : %d ) and I pause until any signal is received\n", disastrOS_getpid());
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  disastrOS_pause();
  printf("PID: %d, PAUSE FINISHED ! Terminating\n", disastrOS_getpid());
  disastrOS_exit(disastrOS_getpid()+1);
}

void childFunction1(void* args){
  printf("Hello, I am the child function %d\n", disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iteration: %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((1+disastrOS_getpid())*2);
    if(i%2 == 0) disastrOS_raise(DSOS_SIGCHLD);
    disastrOS_sleep(2);
	}
  disastrOS_kill(init_pcb->pid	, DSOS_SIGHUP);  
  printf("PID: %d, terminating\n", disastrOS_getpid());
  disastrOS_exit(disastrOS_getpid()+1);
}


void childFunction2(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating.\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);

  for (int i=0; i<(disastrOS_getpid()+1); ++i){
	if(i == 2){
		int type=0;
		int mode=DSOS_CREATE;
		printf("PID: %d is generating a new thread. mode: %d\n",disastrOS_getpid(), mode);
		printf("opening resource (and creating if necessary)\n");
		int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
		printf("fd=%d\n", fd);
		disastrOS_spawn(function_zero, 0);
		int new_pid = running->syscall_retvalue;
		printf("PID: %d is wainting on his child ( PID : %d ) to terminate.\n",disastrOS_getpid(), new_pid);
		int retval;
		disastrOS_wait(new_pid, &retval);
		if(retval > 0) printf("PID: %d , feel happy my child worked fine!\n",disastrOS_getpid());
		else printf("PID: %d , WORRIED, my child did not terminated as expected\n",disastrOS_getpid());
	}
    printf("PID: %d, iteration: %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((1+disastrOS_getpid())*2);
    if(i % 3 == 0) disastrOS_raise(DSOS_SIGCHLD);
    disastrOS_sleep(2);
    }
    printf("PID: %d, terminating\n", disastrOS_getpid());
	disastrOS_exit(disastrOS_getpid()+1);
}


void childFunction3(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int new_pid=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  
  mode=DSOS_CREATE;
  printf("PID: %d is generating a new thread. mode: %d\n",disastrOS_getpid(), mode);
  printf("opening resource (and creating if necessary)\n");
  fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  disastrOS_spawn(function_one, 0);
  new_pid = running->syscall_retvalue;

  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iteration: %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((1+disastrOS_getpid())*2);
  }
  printf("PID: %d is killing PID %d\n", disastrOS_getpid(), new_pid);
  disastrOS_kill(new_pid, DSOS_SIGHUP);
  disastrOS_sleep(20);
  printf("PID: %d, terminating\n", disastrOS_getpid());
  disastrOS_exit(disastrOS_getpid()+1);
}

void signal_sender(void* args){
	printf("Hello, I am a signals sender and I am used to send random signal to every processes.\n");
	disastrOS_sleep(40);
	for(int pid = disastrOS_getpid(); pid>=0; pid--){
		if( pid%2 == 0){
			 disastrOS_kill(pid, DSOS_SIGHUP);
			 disastrOS_kill(pid, DSOS_SIGHUP);
		}
		else{
			 disastrOS_kill(pid, DSOS_SIGCHLD);
			 disastrOS_kill(pid, DSOS_SIGCHLD);
		}
	}
	disastrOS_exit(disastrOS_getpid()+1);
}

void initFunction(void* args) {
  disastrOS_printStatus();
  printf("Hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 11 nice threads\n");
  int alive_children=0;
  for (int i=0; i<11; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    if(i == 1) disastrOS_spawn(childFunction2, 0);
    else if(i == 2) disastrOS_spawn(childFunction3, 0);
    else if(i == 10) disastrOS_spawn(signal_sender, 0);
    else disastrOS_spawn(childFunction1, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>=0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("\nSHUTDOWN!\n");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("The function pointers are:\n -childFunction1: %p\n -childFunction2: %p\n -childFunction3: %p\n", childFunction1, childFunction2, childFunction3);
  // spawn an init process
  printf("START\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
