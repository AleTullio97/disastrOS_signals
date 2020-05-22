#pragma once
#include "disastrOS_pcb.h"

/*
  Global variables used by disastrOS

*/

extern PCB* init_pcb; // pcb of the init process
extern PCB* running;  // pcb of the currently running process
extern int last_pid;  // last pid to be generates
extern ListHead ready_list; // list of the ready processes
extern ListHead waiting_list; // list of the waiting processes
extern ListHead zombie_list;  // zombies
extern ListHead resources_list; // resources

// at: when a signal arrives, it is attached to a signal_list.
// at: When a process changes is state into running, it
// at: checks and handles all the signals it received (if any).
extern ListHead signal_list;

extern ListHead timer_list;     //timers (concrete)
extern volatile int disastrOS_time; //global time of disastrOS

// a resource can be a device, a file or an ipc thing

typedef void(*SyscallFunctionType)();

// at: the following array is used to obtain the signal mask by signal number.
extern int sig_mask[2];


// these are used for "trapping" in protected mode
extern ucontext_t main_context;

// at: we declare the signal ucontext HERE
// at: all the signals are handled in the signal_context.
extern ucontext_t signal_context;

// this is used for shutting down (set it to 1, and return the control to main)
extern int shutdown_now;