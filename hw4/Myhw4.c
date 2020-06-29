
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