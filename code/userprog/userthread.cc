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
	
	t->Fork(StartUserThread,(VoidFunctionPtr)f,argt);

	//TODO

	return 0;
}

static void StartUserThread(int f){
	
}

int do_UserThreadExit(){
	return 0;
}

#endif // CHANGED
