#include "MsgQueue.h"
#include "Myhw4.h"
#include <stdio.h>
#include <string.h>
pmqd_t pmq_open(const char *name, int flags, mode_t perm, pmq_attr *attr)
{
    int index = 0;
    for (int i = 0; i < MAX_QCB_NUM; i++)
    { // Message queue check
        if (strcmp(name, qcbTblEntry[i].name) == 0)
        {
            qcbTblEntry[i].openCount++;
            return i;
        }
    }
    // message queue creat
    for (int i = 0; i < MAX_QCB_NUM; i++)
    {
        if (qcbTblEntry[i].bUsed == 0)
        {
            index = i;
            strcpy(qcbTblEntry[i].name, name);
            qcbTblEntry[i].bUsed = 1;
            qcbTblEntry[i].mode = perm;
            qcbTblEntry[i].openCount++;
            Qcb *qcb = malloc(sizeof(Qcb));
            qcb->pMsgHead = NULL;
            qcb->pMsgTail = NULL;
            qcb->pWaitQHead = NULL;
            qcb->pWaitQTail = NULL;
            qcbTblEntry[i].pQcb = qcb;
            return index;
        }
    }
}

int pmq_close(pmqd_t mqd) {

    if(qcbTblEntry[mqd].bUsed==0){
        return -1;
    }
    else{
        qcbTblEntry[mqd].openCount--;
        return 0;
    }
}
int pmq_send(pmqd_t mqd, char *msg_ptr, size_t msg_len, unsigned int msg_prio)
{
    Message *msg = malloc(sizeof(Message));
    strcpy(msg->data, msg_ptr);
    msg->priority = msg_prio;
    msg->size = msg_len;
    if (qcbTblEntry[mqd].bUsed == 0)
    {
        return -1;
    }
    InsertMessageQueueToTail(mqd, msg, msg_prio);

    if(qcbTblEntry[mqd].pQcb->pWaitQHead==NULL){
    return 0;
    }
    else{
        Thread *p=GetThreadFromWaitingqueueHead(mqd);
        p->status=THREAD_STATUS_READY;
        InsertReadyQueueToTail(p,p->priority);
    }
}
ssize_t pmq_receive(pmqd_t mqd, char *msg_ptr, size_t msg_len, unsigned int *msg_prio)
{

    int len = -1;
    if (qcbTblEntry[mqd].bUsed == 0)
    {
        return len;
    }
    if (qcbTblEntry[mqd].pQcb->pMsgHead != NULL)
    {
        Message *p = GetMessageQueueToHead(mqd);
        strcpy(msg_ptr, p->data);
        *msg_prio = p->priority;
        len = strlen(p->data);
        free(p);
        return len;
    }
    else
    { //message x block status
        pCurrentThread->status = THREAD_STATUS_WAIT;

        int count = 0;
        int t_priority;
        Thread *newThread;
        Thread *oldThread;
        // Readyqueue empty check
        for (int i = 0; i < MAX_READYQUEUE_NUM; i++)
        {
            if (pReadyQueueEnt[i].queueCount == 0)
            {
                count++;
            }
        }
        if (count == MAX_READYQUEUE_NUM)
        {
        }
        else if (count < MAX_READYQUEUE_NUM)
        {

            for (int i = 0; i < MAX_READYQUEUE_NUM; i++)
            {
                if (pReadyQueueEnt[i].queueCount != 0)
                {
                back:
                    t_priority = i;
                    newThread = GetThreadFromReadyqueueHead(t_priority);
                    newThread->status = THREAD_STATUS_RUN;
                    InsertWaitingQueueToTail(mqd, pCurrentThread);
                    oldThread = pCurrentThread;
                    pCurrentThread = newThread;
                    __ContextSwitch(oldThread->pid, newThread->pid);

                    //stop -> start receive -> send -> receive
                    if (qcbTblEntry[mqd].pQcb->pMsgHead != NULL)
                    {
                        Message *p = GetMessageQueueToHead(mqd);
                        strcpy(msg_ptr, p->data);
                        *msg_prio = p->priority;
                        len = strlen(p->data);
                        free(p);
                        return len;
                    }
                    else{
                        goto back;
                    }
                }
            }
        }
    }
}
