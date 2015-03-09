#include "syscall.h"

void thread(int *i){
	if(*i!=-1){
		UserThreadJoin(*i);
		SynchPutString("Thread ");
		SynchPutInt(*i);
		PutChar('\n');
	}
	else{
		SynchPutString("Thread initial\n");
		while(1){
		}
	}
	UserThreadExit();
}

int main(){
	int param=-1;
	int t1 = UserThreadCreate((void (*)(void *))thread,(void *)(&param));
	SynchPutString("Id de thread calculé ");
	SynchPutInt(t1);
	SynchPutString("\n");
	int t2 = UserThreadCreate((void (*)(void *))thread,(void *)(&t1));
	UserThreadJoin(t2);
	SynchPutString("Main program terminated\n");
	Halt();
}
