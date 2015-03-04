#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "system.h"

static void StartUserThread(int f){
	argThread *argt = (argThread *) f;
	machine->WriteRegister (PCReg,argt->func);
	machine->WriteRegister (NextPCReg,argt->func+4);
	machine->WriteRegister (4,argt->argv);

	//TODO
}

int do_UserThreadCreate(int f, int arg){
	Thread *t = new Thread("UserThread");

	argThread *argt = new argThread;

	argt->func = f;
	argt->argv = arg;
	
	t->Fork(StartUserThread,(int)argt);

	//TODO

	return 0;
}


int do_UserThreadExit(){
	return 0;
}

#endif // CHANGED
