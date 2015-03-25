#include "syscall.h"

#define NbProcess 12

int main()
{
	int i;
	for(i=0;i<NbProcess;i++){
		ForkExec("massthread");
	}
	Exit(0);
}
