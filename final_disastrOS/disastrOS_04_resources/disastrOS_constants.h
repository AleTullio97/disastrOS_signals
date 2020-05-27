#pragma once

#define MAX_NUM_PROCESSES 1024
#define MAX_NUM_RESOURCES 1024
#define MAX_NUM_RESOURCES_PER_PROCESS 32
#define MAX_NUM_DESCRIPTORS_PER_PROCESS 32

#define STACK_SIZE        16384

// at signals are defined here
#define MAX_SIGNALS 32
#define DEFINED_SIG 2
#define DSOS_SIGCHLD 0x1
#define DSOS_SIGHUP  0x2

// at macros for signal handler
#define DSOS_SIG_DFL 0	// at set default behavior 
#define DSOS_SIG_IGN 1 	// at explicitly ignore the signal


// errors
#define DSOS_ESYSCALL_ARGUMENT_OUT_OF_BOUNDS -1
#define DSOS_ESYSCALL_NOT_IMPLEMENTED -2
#define DSOS_ESYSCALL_OUT_OF_RANGE -3
#define DSOS_EFORK  -4
#define DSOS_EWAIT  -5
#define DSOS_ESPAWN  -6
#define DSOS_ESLEEP  -7
#define DSOS_ERESOURCECREATE -8
#define DSOS_ERESOURCEOPEN -9
#define DSOS_ERESOURCENOEXCL -10
#define DSOS_ERESOURCENOFD -11
#define DSOS_ERESOURCECLOSE -12
#define DSOS_ERESOURCEINUSE -13
#define DSOS_EUNDEFSH -14		// at Undefined Signal Handler

// at the following errors concerns signals 
#define DSOS_EINVAL -14 // at thrown when an invalid signal is sent.
#define DSOS_ESRCH -15 // at the target process or process group does not exist.

// syscall numbers
#define DSOS_MAX_SYSCALLS 32
#define DSOS_MAX_SYSCALLS_ARGS 8
#define DSOS_CALL_PREEMPT   1
#define DSOS_CALL_FORK      2
#define DSOS_CALL_WAIT      3
#define DSOS_CALL_EXIT      4
#define DSOS_CALL_SPAWN     5
#define DSOS_CALL_SLEEP     6
#define DSOS_CALL_OPEN_RESOURCE 7
#define DSOS_CALL_CLOSE_RESOURCE 8
#define DSOS_CALL_DESTROY_RESOURCE 9
#define DSOS_CALL_SHUTDOWN  10

// at defining syscall numbers
#define DSOS_CALL_KILL	 11
#define DSOS_CALL_PAUSE  12

//resources
#define DSOS_CREATE 0x1
#define DSOS_READ 0x2
#define DSOS_WRITE 0x3
#define DSOS_EXCL 0x4

// scheduling
#define ALPHA 0.5f
#define INTERVAL 100 // milliseconds for timer tick


























