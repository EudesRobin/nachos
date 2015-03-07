#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "system.h"

static void StartUserThread(int f){
	argThread *argt = (argThread *) f;

	//Save old registers
	currentThread->space->SaveState();
	//Clean registers
	currentThread->space->InitRegisters();

	machine->WriteRegister (PCReg,argt->func);
	machine->WriteRegister (NextPCReg,(argt->func)+4);
	machine->WriteRegister (4,argt->argv);
	int alloc = currentThread->space->AllocStack();
	machine->WriteRegister (StackReg,currentThread->space->StackValue(alloc));
	currentThread->initStackReg=alloc;

	machine->Run();

	//TODO
}

int do_UserThreadCreate(int f, int arg){
	if(!currentThread->space->CheckFreeStack()){
		printf("Error: Stack already full\n");
		return -1;
	}
	Thread *t = new Thread("UserThread");

	if(t==NULL){
		printf("Error: Thread non created");
		return -1;
	}

	argThread *argt = new argThread;

	argt->func = f;
	argt->argv = arg;
	
	t->Fork(StartUserThread,(int)argt);

	//Permet de démarrer le thread créé
	currentThread->Yield();

	return 0;
}


int do_UserThreadExit(){
	currentThread->space->FreeStack(currentThread->initStackReg);
	currentThread->Finish();
	return 0;
}

//stackreg(bitmap)

#endif // CHANGED
