#include "syscall.h"

#define NbThread 12

//Variable inutile à supprimer plus tard
static int numProcess=0;

void thread(int *i){
	if(*i!=-1){
		UserThreadJoin(i[0]);
		//SynchPutString("Thread ");
		//SynchPutInt(i[0]);
		//PutChar('\n');
	}
	else{
		//SynchPutString("Thread initial\n");
	}	
	//Calcul sale pour "ralentir" les threads
	int a=1001;
	int j;
	for(j=0;j<1500;j++){
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
	int i,tmp,param[2*(NbThread+1)];
	param[0]=-1;
	param[1]=current;
	tmp=UserThreadCreate((void (*)(void *))thread,(void *)(param));
	param[2]=tmp;
	param[3]=current;
	for(i=1;i<NbThread;i++){
		tmp=UserThreadCreate((void (*)(void *))thread,(void *)(param+2*i));
		param[2*(i+1)]=tmp;
		param[2*(i+1)+1]=current;
	}
	Exit(0);
}
