#include "MsgQueue.h"
#include "Myhw4.h"
#include <stdio.h>
#include <string.h>
pmqd_t pmq_open(const char *name, int flags, mode_t perm, pmq_attr *attr) {
    int index = 0;
    for (int i = 0; i < MAX_QCB_NUM; i++) { // Message queue check
        if (strcmp(name, qcbTblEntry[i].name) == 0) {
            qcbTblEntry[i].openCount++;
            return i;
        }
    }
    // message queue creat
    for (int i = 0; i < MAX_QCB_NUM; i++) {
        if (qcbTblEntry[i].bUsed == 0) {
            index = i;
            strcpy(qcbTblEntry[i].name,name);
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

int pmq_close(pmqd_t mqd) {}
int pmq_send(pmqd_t mqd, char *msg_ptr, size_t msg_len, unsigned int msg_prio) {
    Message *msg = malloc(sizeof(Message));
    strcpy(msg->data, msg_ptr);
    msg->priority = msg_prio;
    msg->size = msg_len;
    if(qcbTblEntry[mqd].bUsed==0){
        return -1;
    }
    InsertMessageQueueToTail(mqd,msg,msg_prio);
    return 0;
    
}
ssize_t pmq_receive(pmqd_t mqd, char *msg_ptr, size_t msg_len,
                    unsigned int *msg_prio) {}
