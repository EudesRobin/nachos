#include "syscall.h"

#define NbProcess 12

int main()
{
	int i;
	for(i=0;i<NbProcess;i++){
		SynchPutInt(i);
		PutChar('\n');
		ForkExec("massthread");
	}
	Exit(0);
}
