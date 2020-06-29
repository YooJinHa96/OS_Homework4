#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"
#include "MsgQueue.h"

int main(int argc, char* argv[]){
	
	int num;
	char msg[100];
	Init();
	num=pmq_open("mq1",1,1,NULL);
	for(int i=0;i<5;i++){
		memset(msg,0,100);
		sprintf(msg,"%dth message",i);
		pmq_send(num,msg,100,i);
	}
	printf("message queue name  : %s\n",qcbTblEntry[num].name);
	Message *p=qcbTblEntry[num].pQcb->pMsgHead;
	while(p!=NULL){

		printf("%s\n",p->data);
		p=p->pNext;
	}
	return 0;
}
