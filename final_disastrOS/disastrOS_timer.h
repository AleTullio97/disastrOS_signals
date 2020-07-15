#pragma once
#include "disastrOS_pcb.h"

struct PCB;

typedef struct  {
  ListItem list;
  int awakening_time;
  struct PCB* pcb;
} TimerItem;

typedef ListHead TimerHandler;

void Timer_init();

void TimerList_init(ListHead* timers);

TimerItem* TimerList_add(ListHead* timers, int awakening_time, struct PCB* pcb);

TimerItem* TimerList_current(ListHead* timers, int current_time);

int TimerList_removeCurrent(ListHead* timers);

// at remove a timer element
int TimerList_removeTimer(ListHead* timers, TimerItem* item);

void TimerList_print(ListHead* timers);

int TimerItem_free(TimerItem* item);
