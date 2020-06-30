#include "Scheduler.h"
#include "Init.h"
#include "Myhw4.h"
#include "Thread.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
int counter = 0;
void sigint_handler(int signo) { alarm(2); }
void sig_handler(int sign) {

    if (sign == SIGALRM) {
        printf("alarm arrive !!\n");
        int count = 0;
        int t_priority;
        Thread *newThread;
        Thread *oldThread;
        // Readyqueue empty check
        for (int i = 0; i < MAX_READYQUEUE_NUM; i++) {
            if (pReadyQueueEnt[i].queueCount == 0) {
                count++;
            }
        }
        if (count == MAX_READYQUEUE_NUM) {
        } else if (count < MAX_READYQUEUE_NUM) {

            for (int i = 0; i < MAX_READYQUEUE_NUM; i++) {
                if (pReadyQueueEnt[i].queueCount != 0) {
                    t_priority = i;
                    newThread = GetThreadFromReadyqueueHead(t_priority);
                    newThread->status = THREAD_STATUS_RUN;
                    InsertReadyQueueToTail(pCurrentThread,
                                           pCurrentThread->priority);
                    pCurrentThread->status = THREAD_STATUS_READY;
                    oldThread = pCurrentThread;
                    pCurrentThread = newThread;
                    printf("alarm sds !!\n");
                    __ContextSwitch(oldThread->pid, newThread->pid);

                    break;
                }
            }
        }
    }
    alarm(2);
}
int RunScheduler(void) {

    int count = 0;
    int t_priority;
    Thread *newThread;
    Thread *oldThread;
    signal(SIGALRM, sig_handler);
    //  signal( SIGALRM, sigint_handler);
    // Readyqueue empty check
    for (int i = 0; i < MAX_READYQUEUE_NUM; i++) {
        if (pReadyQueueEnt[i].queueCount == 0) {
            count++;
        }
    }
    if (count == MAX_READYQUEUE_NUM) {
        // alarm(TIMESLICE);
    } else if (pCurrentThread == NULL) { // thread test case start
        pCurrentThread =
            GetThreadFromReadyqueueHead(0); // Assume Testcase prirority = 0
        kill(pCurrentThread->pid, SIGCONT);
        // alarm(TIMESLICE);

    } else if (count < MAX_READYQUEUE_NUM) {

        for (int i = 0; i < MAX_READYQUEUE_NUM; i++) {
            if (pReadyQueueEnt[i].queueCount != 0) {
                t_priority = i;
                newThread = GetThreadFromReadyqueueHead(t_priority);
                newThread->status = THREAD_STATUS_RUN;
                InsertReadyQueueToTail(pCurrentThread,
                                       pCurrentThread->priority);
                pCurrentThread->status = THREAD_STATUS_READY;
                oldThread = pCurrentThread;
                pCurrentThread = newThread;
                __ContextSwitch(oldThread->pid, newThread->pid);
                break;
            }
        }
    }

    alarm(2);
}

void __ContextSwitch(int curpid, int newpid) {
    kill(curpid, SIGSTOP);
    kill(newpid, SIGCONT);

    // pCurrentThead = pThreadTbEnt[newpid].pThread;
}
