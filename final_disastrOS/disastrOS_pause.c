#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"


void internal_pause(){
  assert(running);
  // at signal already received
  if(running->signals) running->syscall_retvalue=DSOS_EINTR;
  // at else sleep until a signal is received
  else
  {
	if (running->timer) {
		printf("process has already a timer!!!\n");
		running->syscall_retvalue=DSOS_ESLEEP;
		return;
	}
	int cycles_to_sleep=MAX_TIME_TO_PAUSE;
	int wake_time=disastrOS_time+cycles_to_sleep;
  
	TimerItem* new_timer=TimerList_add(&timer_list, wake_time, running);
	if (! new_timer) {
		printf("no new timer!!!\n");
		running->syscall_retvalue=DSOS_ESLEEP;
		return;
	} 
	running->status=Waiting;
	running->timer=(struct TimerItem*)new_timer;
	List_insert(&waiting_list, waiting_list.last, (ListItem*) running);
	if (ready_list.first)
		running=(PCB*) List_detach(&ready_list, ready_list.first);
	else {
		running=0;
		printf ("they are all sleeping\n");
		disastrOS_printStatus();
	}
  }
}
