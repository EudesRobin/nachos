#include "syscall.h"

void thread1(int *i){
	int j;
	for(j=0;j<*i;j++){
		PutChar('a');
	}
	PutChar('\n');
	UserThreadExit();
}

void thread2(int *i){
	int j;
	for(j=0;j<*i;j++){
		PutChar('b');
	}
	PutChar('\n');
	UserThreadExit();
}

int main(){
	int param,t;
	param=10;
	UserThreadCreate((void (*)(void *))thread1,(void *)(&param));
	t=UserThreadCreate((void (*)(void *))thread2,(void *)(&param));
	UserThreadJoin(t);
	SynchPutString("Main program terminated\n");
	Halt();
}
