#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "system.h"

int do_UserThreadCreate(int f, int arg){
	Thread t = new Thread("UserThread");

	argThread argt = new argThread;

	argThread->func = f;
	argThread->argv = argt;
	
	t->Fork(StartUserThread,argt);

	//TODO

	return 0;
}

static void StartUserThread(int f){
	argThread argt = (argThread) f;
	machine->WriteRegister (PCReg,f->func);
	machine->WriteRegister (4,f->argv);

	//TODO
}

int do_UserThreadExit(){
	return 0;
}

#endif // CHANGED
