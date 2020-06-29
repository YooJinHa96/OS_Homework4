
#include "Myhw4.h"
void InsertMessageQueueToTail(pmqd_t mqdm, Message *msg, int priority)
{

    if (qcbTblEntry[mqdm].pQcb->pMsgHead != NULL)
    {
        Message *pm = qcbTblEntry[mqdm].pQcb->pMsgHead;
        while (pm != NULL)
        {
            if (priority > pm->priority)
            {

                if (pm == qcbTblEntry[mqdm].pQcb->pMsgHead)
                {

                    qcbTblEntry[mqdm].pQcb->pMsgHead = msg;
                    msg->pNext = pm;
                    msg->pPrev=NULL;
                    pm->pPrev = msg;
                }
                else if(pm!=qcbTblEntry[mqdm].pQcb->pMsgHead){
                    
                    pm->pPrev->pNext=msg;
                    msg->pPrev=pm->pPrev;
                    msg->pNext=pm;
                    pm->pPrev=msg;
                }
                return ;
            }
            pm=pm->pNext;
        }
        //Priority Smallest
        pm=qcbTblEntry[mqdm].pQcb->pMsgTail;
        pm->pNext=msg;
        msg->pPrev=pm;
        qcbTblEntry[mqdm].pQcb->pMsgTail=msg;
        return ;
    }
    else
    {
        qcbTblEntry[mqdm].pQcb->pMsgHead = msg;
        qcbTblEntry[mqdm].pQcb->pMsgTail = msg;
        msg->pNext = NULL;
        msg->pPrev = NULL;
    }
}

Message *GetMessageQueueToHead(pmqd_t mqdm){
    Message *pm=qcbTblEntry[mqdm].pQcb->pMsgHead;

    if(pm->pNext==NULL){
        qcbTblEntry[mqdm].pQcb->pMsgHead=NULL;
        qcbTblEntry[mqdm].pQcb->pMsgTail=NULL;
        return pm;
    }
    else if(pm->pNext!=NULL){
        qcbTblEntry[mqdm].pQcb->pMsgHead=pm->pNext;
        pm->pNext->pPrev=NULL;
        pm->pNext=NULL;
        return pm;
    }
}

void InsertWaitingQueueToTail(pmqd_t mqdm,Thread *thread){
    Thread *p=qcbTblEntry[mqdm].pQcb->pWaitQTail;
    if(p!=NULL){
        p->phNext=thread;
        thread->phPrev=p;
        thread->phNext=NULL;
        qcbTblEntry[mqdm].pQcb->pWaitQTail=thread;
    }
    else if(p==NULL){
        qcbTblEntry[mqdm].pQcb->pWaitQHead=thread;
        qcbTblEntry[mqdm].pQcb->pWaitQTail=thread;
        thread->phNext=NULL;
        thread->phPrev=NULL;
    }
    return ;
}

void InsertReadyQueueToTail(Thread *thread, int priority)
{
    int hashKey = priority;
    thread->priority = priority;
    if (pReadyQueueEnt[hashKey].pTail != NULL)
    {
        Thread *o = pReadyQueueEnt[hashKey].pTail;
        pReadyQueueEnt[hashKey].pTail = thread;
        thread->phNext = NULL;
        thread->phPrev = o;
        o->phNext = thread;
    }
    else
    {
        pReadyQueueEnt[hashKey].pHead = thread;
        pReadyQueueEnt[hashKey].pTail = thread;
        thread->phNext = NULL;
        thread->phPrev = NULL;
    }
    pReadyQueueEnt[hashKey].queueCount++;
}

Thread *GetThreadFromReadyqueueHead(int priority)
{

    int hashkey = priority;
    Thread *t = pReadyQueueEnt[hashkey].pHead;
    if (DeleteObject(t) == 0)
    {
      
    }

    if (t != NULL)
    {
        return t;
    }
    else
    {
        return NULL;
    }
}

BOOL DeleteObject(Thread *pObj)
{
    int hashkey = pObj->priority;
    Thread *o = pReadyQueueEnt[hashkey].pHead;
    if (pReadyQueueEnt[hashkey].queueCount == 0)
    {
        return 0;
    }
    else if (pReadyQueueEnt[hashkey].queueCount == 1)
    {
        if (o == pObj)
        {
            pReadyQueueEnt[hashkey].pHead = NULL;
            pReadyQueueEnt[hashkey].pTail = NULL;
            pReadyQueueEnt[hashkey].queueCount--;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        for (o; o != NULL; o = o->phNext)
        {
            if (o == pObj)
            {
                if (o->phPrev == NULL)
                {
                    pReadyQueueEnt[hashkey].pHead = o->phNext;
                    o->phNext->phPrev = NULL;
                }
                else if (o->phNext == NULL)
                {
                    pReadyQueueEnt[hashkey].pTail = o->phPrev;
                    o->phPrev->phNext = NULL;
                }
                else
                {
                    o->phPrev->phNext = o->phNext;
                    o->phNext->phPrev = o->phPrev;
                }
                pReadyQueueEnt[hashkey].queueCount--;
                return 1;
            }
        }
    }
    return 0;
}

Thread *GetThreadFromWaitingqueueHead(pmqd_t mqdm){
    Thread *p=qcbTblEntry[mqdm].pQcb->pWaitQHead;
    if(p->phNext==NULL){
        qcbTblEntry[mqdm].pQcb->pWaitQHead=NULL;
        qcbTblEntry[mqdm].pQcb->pWaitQTail=NULL;
        return p;
    }
    else if(p->phNext!=NULL){
        qcbTblEntry[mqdm].pQcb->pWaitQHead=p->phNext;
        p->phNext->phPrev=NULL;
        p->phNext=NULL;
        return p;
    }
}