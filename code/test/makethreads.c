#include "syscall.h"

void thread(int *i){
	SynchPutString("Thread ");
	SynchPutInt(*i);
	PutChar('\n');
	UserThreadExit();
}

int main(){
	int param[1];
	param[0]=1;
	SynchPutString("Main program beginning\n");
	UserThreadCreate((void (*)(void *))thread,(void *)(param));
	SynchPutString("Main program terminated\n");
	Halt();
}

