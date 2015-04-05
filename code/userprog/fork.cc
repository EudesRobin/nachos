#ifdef CHANGED

#include "system.h"
#include "addrspace.h"

static void StartUserProcess(int f){
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();	// load page table register

	machine->Run ();		// jump to the user progam
}

int ForkExec(char *s){
	OpenFile *executable = fileSystem->Open (s);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", s);
		return -1;
	}

	Thread *t = new Thread("UserProcess");

	if(t==NULL){
		printf("Error: Thread non created\n");
		return -1;
	}

	MajNbProcess(1);
	
	space = new AddrSpace (executable);
	t->space = space;

	delete executable;		// close file

	if(space->StackOverflow()){
		printf("Error: memory full, the process could not be started\n");
		delete space;
		delete t;
		MajNbProcess(-1);
		return -1;
	}

	t->ForkExec(StartUserProcess,0);

	//Permet de démarrer le processus créé
	currentThread->Yield();

	return 0;
}

#endif // CHANGED
