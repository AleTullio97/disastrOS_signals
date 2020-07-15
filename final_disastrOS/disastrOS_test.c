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
	
  //printf("Hello, I'm function zero ( PID : %d ) and I sleep 40 before terminating\n", disastrOS_getpid());
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  //disastrOS_sleep(40);
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

void childFunction(void* args){
  printf("Hello, I am the child function %d\n", disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iteration: %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((1+disastrOS_getpid())*2);
    //disastrOS_kill(init_pcb->pid	, DSOS_SIGHUP);
    disastrOS_raise(DSOS_SIGCHLD);
    //disastrOS_sleep(2);
	}
    /*
    int res = disastrOS_signal(DSOS_SIGCHLD, DSOS_SIG_IGN);
    if(res<0){
		printf("Cannot change signal handler, Something went 	wrong\n");
	}
    res = disastrOS_signal(DSOS_SIGHUP, DSOS_SIG_IGN);
    if(res<0){
		printf("Cannot change signal handler, Something went wrong\n");
	}

    disastrOS_raise(DSOS_SIGHUP);
  }
	*/
  printf("PID: %d, terminating\n", disastrOS_getpid());
  disastrOS_exit(disastrOS_getpid()+1);
}


void childFunction2(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
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
		printf("PID: %d is wainting on his child ( PID : %d )to terminate.\n",disastrOS_getpid(), new_pid);
		int retval;
		disastrOS_wait(new_pid, &retval);
		if(retval > 0) printf("PID: %d , feel happy my child worked fine!\n",disastrOS_getpid());
		else printf("PID: %d , WORRIED, my child didn't terminated as expected",disastrOS_getpid());
	}
    printf("PID: %d, iteration: %d\n", disastrOS_getpid(), i);
    disastrOS_sleep((1+disastrOS_getpid())*2);
    //disastrOS_kill(init_pcb->pid	, DSOS_SIGHUP);
    disastrOS_raise(DSOS_SIGCHLD);
    //disastrOS_sleep(2);
    /*
    int res = disastrOS_signal(DSOS_SIGCHLD, DSOS_SIG_IGN);
    if(res<0){
		printf("Cannot change signal handler, Something went 	wrong\n");
	}
    res = disastrOS_signal(DSOS_SIGHUP, DSOS_SIG_IGN);
    if(res<0){
		printf("Cannot change signal handler, Something went wrong\n");
	}

    disastrOS_raise(DSOS_SIGHUP);
    * */
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

void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    if(i == 1) disastrOS_spawn(childFunction2, 0);
    else if(i == 2) disastrOS_spawn(childFunction3, 0);
    else disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("shutdown!");
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
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
