#ifndef __Myhw4_H__
#define __Myhw4_H__
#include "Thread.h"
#include "MsgQueue.h"
#include "Scheduler.h"

void InsertMessageQueueToTail(pmqd_t mqdm, Message *msg, int priority);
#endif