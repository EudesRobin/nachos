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
		int a=1001;
		int j;	
		//Calcul sale pour "ralentir" le premier thread
		for(j=0;j<1000;j++){
			if(a%2){
				a=a*2;
			}
			else{
				a=a/2;
			}
		}
	}
	UserThreadExit();
}

int main(){
	int param=-1;
	int t1 = UserThreadCreate((void (*)(void *))thread,(void *)(&param));
	int t2 = UserThreadCreate((void (*)(void *))thread,(void *)(&t1));
	UserThreadJoin(t2);
	SynchPutString("Main program terminated\n");
	Halt();
}
