#ifdef CHANGED

#include "userthread.h"
#include "thread.h"
#include "addrspace.h"
#include "system.h"

static void StartUserThread(int f){
	argThread *argt = (argThread *) f;

	//Save old registers
	space->SaveState();
	//Clean registers
	space->InitRegisters();

	machine->WriteRegister (PCReg,argt->func);
	machine->WriteRegister (NextPCReg,(argt->func)+4);
	machine->WriteRegister (4,argt->argv);
	int alloc = space->AllocStack();
	Machine->WriteRegister (StackReg,alloc);
	//Ajout champ dans thread définissant l'endroit dans la pile

	machine->Run();

	//TODO
}

int do_UserThreadCreate(int f, int arg){
	//Verif pile dispo
	Thread *t = new Thread("UserThread");

	//Verif t!=NULL

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

//stackreg(bitmap)

#endif // CHANGED
