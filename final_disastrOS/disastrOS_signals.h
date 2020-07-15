#include "disastrOS_pcb.h"


// at defining signal handler in disastrOS
typedef void (*disastrOS_sighandler_t)(void);

int disastrOS_signal(int signum, disastrOS_sighandler_t handler);

int disastrOS_pause(void);

void cpy_signals_handler(PCB* child, PCB* parent);

void signals_handler();

void disastrOS_SIGCHLD_handler();

void disastrOS_SIGHUP_handler();




