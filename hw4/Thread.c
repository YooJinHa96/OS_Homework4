#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"

int thread_create(thread_t *thread, thread_attr_t *attr, int priority,
                  void *(*start_routine)(void *), void *arg) {
    pid_t pid;
    int flags = SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM;
    void *pStack;
    int pr = priority;
    pStack = malloc(STACK_SIZE);

    pid = clone((void *)start_routine, (void *)pStack + STACK_SIZE, flags, arg);
    kill(pid, SIGSTOP);

    for (int i = 0; i < MAX_THREAD_NUM; i++) {
        if (!pThreadTblEnt[i].bUsed) {
            *thread = i;
            pThreadTblEnt[i].bUsed = 1;
            pThreadTblEnt[i].pThread = malloc(sizeof(Thread));
            pThreadTblEnt[i].pThread->pid = pid;
            pThreadTblEnt[i].pThread->priority = priority;
            break;
        }
    }

    if (pCurrentThread == NULL) { // Testcase Thread create
        InsertReadyQueueToTail(pThreadTblEnt[*thread].pThread, priority);
        pThreadTblEnt[*thread].pThread->status = THREAD_STATUS_READY;
    } else {
        if (pCurrentThread->priority <=
            pThreadTblEnt[*thread].pThread->priority) {
            InsertReadyQueueToTail(pThreadTblEnt[*thread].pThread, priority);
            pThreadTblEnt[*thread].pThread->status = THREAD_STATUS_READY;
        } else {
            InsertReadyQueueToTail(pCurrentThread, pCurrentThread->priority);
            pCurrentThread->status = THREAD_STATUS_READY;
            pThreadTblEnt[*thread].pThread->status = THREAD_STATUS_RUN;
            __ContextSwitch(pCurrentThread->pid,
                            pThreadTblEnt[*thread].pThread->pid);
            pCurrentThread = pThreadTblEnt[*thread].pThread;
        }
    }
    return *thread;
}

int thread_suspend(thread_t tid) {}

int thread_cancel(thread_t tid) {}

int thread_resume(thread_t tid) {}

thread_t thread_self() {}

int thread_join(thread_t tid, void **retval) {}

int thread_exit(void *retval) {}
