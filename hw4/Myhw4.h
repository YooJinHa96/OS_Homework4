#ifndef __Myhw4_H__
#define __Myhw4_H__
#include "Thread.h"
#include "MsgQueue.h"
#include "Scheduler.h"
#include <unistd.h>

void InsertMessageQueueToTail(pmqd_t mqdm, Message *msg, int priority);
Message *GetMessageQueueToHead(pmqd_t mqdm);

void InsertWaitingQueueToTail(pmqd_t mqdm, Thread *thread);
void InsertReadyQueueToTail(Thread *thread, int priority);
Thread *GetThreadFromReadyqueueHead(int priority);
BOOL DeleteObject(Thread *pObj);
Thread *GetThreadFromWaitingqueueHead(pmqd_t mqdm);
#endif