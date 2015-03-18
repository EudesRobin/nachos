#ifdef CHANGED

#include "system.h"
#include "addrspace.h"

int ForkExec(char *s){
	OpenFile *executable = fileSystem->Open (s);
	AddrSpace *space;

	if (executable == NULL)
	{
		printf ("Unable to open file %s\n", s);
		return -1;
	}
	space = new AddrSpace (executable);
	currentThread->space = space;

	delete executable;		// close file

	space->InitRegisters ();	// set the initial register values
	space->RestoreState ();	// load page table register

	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		// machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"
	return 0;
}

#endif // CHANGED
