#pragma once

/*
typedef void (*disastrOS_sighandler_t)(int);

disastrOS_sighandler_t signal(int signum, disastrOS_sighandler_t handler);
*/

void signals_handle();

void disastrOS_SIGCHLD_handler();

void disastrOS_SIGHUP_handler();

