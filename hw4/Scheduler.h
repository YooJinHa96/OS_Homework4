#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Thread.h"
#include <signal.h>
#include <unistd.h>
int RunScheduler(void);
void __ContextSwitch(int curpid, int newpid);
//void sigint_handler(int sign);
Thread *pCurrentThread;

#endif
