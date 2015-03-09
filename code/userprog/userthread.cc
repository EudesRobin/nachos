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
	printf("valeur désirée %d\n",alloc);
	machine->WriteRegister (StackReg,currentThread->space->StackValue(alloc));
	currentThread->initStackReg=alloc;

	machine->Run();
}

int do_UserThreadCreate(int f, int arg){
	if(!currentThread->space->CheckFreeStack()){
		printf("Error: Stack already full\n");
		return -1;
	}
	Thread *t = new Thread("UserThread");

	if(t==NULL){
		printf("Error: Thread non created\n");
		return -1;
	}

	argThread *argt = new argThread;

	argt->func = f;
	argt->argv = arg;
	
	t->Fork(StartUserThread,(int)argt);

	currentThread->space->TabSemJoin[t->initStackReg]->P();

	//initStackReg peut être utilisé comme identifiant du thread
	machine->WriteRegister(2, t->initStackReg);

	//Permet de démarrer le thread créé
	currentThread->Yield();

	return 0;
}

int UserThreadJoin(int t){
	if(currentThread->dependance!=-1){
		printf("Le thread possède déjà une dépendance\n");
		return -1;
	}
	if(currentThread->initStackReg==t){
		printf("Tentative de dépendance vers soi-même impossible\n");
		return -1;
	}
	currentThread->dependance=t;
	currentThread->space->TabSemJoin[t]->P();
	return 0;
}


int do_UserThreadExit(){
	currentThread->space->FreeStack(currentThread->initStackReg);
	currentThread->space->TabSemJoin[currentThread->initStackReg]->V();
	if(currentThread->dependance!=-1)
		currentThread->space->TabSemJoin[currentThread->dependance]->V();
	currentThread->Finish();
	return 0;
}

//stackreg(bitmap)

#endif // CHANGED
