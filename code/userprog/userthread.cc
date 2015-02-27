#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "system.h"

int do_UserThreadCreate(int f, int arg){
	Thread t = new Thread("UserThread");

	if(t==NULL){
		return -1;
	}

	t->Fork((VoidFunctionPtr)f,arg);

	//TODO

	return 0;
}

#endif // CHANGED
