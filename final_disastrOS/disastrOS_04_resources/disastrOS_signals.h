#include "disastrOS_pcb.h"


// at defining signal handler in disastrOS
typedef void (*disastrOS_sighandler_t)(void);

disastrOS_sighandler_t signal(int signum, disastrOS_sighandler_t handler);

void cpy_signals_handler(PCB* child, PCB* parent);
  
void signals_handle();

void disastrOS_SIGCHLD_handler();

void disastrOS_SIGHUP_handler();

