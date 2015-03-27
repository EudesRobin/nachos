#include "syscall.h"

#define NbThread 12

static int numProcess=0;

void thread(int *i){
	if(*i!=-1){
		UserThreadJoin(i[0]);
		SynchPutString("Process ");
		SynchPutInt(i[1]);
		PutChar('\n');
	}
	else{
		SynchPutString("Process ");
		SynchPutInt(i[1]);
		SynchPutString(", Thread initial\n");
	}	
	//Calcul sale pour "ralentir" les threads
	int a=1001;
	int j;
	for(j=0;j<1000;j++){
		if(a%2){
			a=a*2;
		}
		else{
			a=a/2;
		}
	}
	UserThreadExit();
}

int main()
{
	int current=numProcess++;
	int i,tmp,param[2];
	param[0]=-1;
	param[1]=current;
	tmp=UserThreadCreate((void (*)(void *))thread,(void *)(&param));
	param[0]=tmp;
	for(i=1;i<NbThread;i++){
		tmp=UserThreadCreate((void (*)(void *))thread,(void *)(&param));
	}
	Exit(0);
}
