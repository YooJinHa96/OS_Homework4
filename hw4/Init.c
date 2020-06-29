#include "Init.h"
#include "MsgQueue.h"
#include "Scheduler.h"
#include "Thread.h"
#include <string.h>
void Init(void) {
    for (int i = 0; i < MAX_QCB_NUM; i++) {
        memset(qcbTblEntry[i].name, 0, PMQ_NANE_LEN_MAX);
        qcbTblEntry[i].mode = 0;
        qcbTblEntry[i].openCount = 0;
        qcbTblEntry[i].bUsed = 0;
        qcbTblEntry[i].pQcb = NULL;
    }
}
