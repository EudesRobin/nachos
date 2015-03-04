// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#ifdef CHANGED
#include "userthread.h"
#endif //CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

#ifdef CHANGED
//----------------------------------------------------------------------
// copyStringFromMachine : Used for SynchPutString
//----------------------------------------------------------------------
void copyStringFromMachine( int from, char *to, unsigned size){
	unsigned int i;
	int tmp;
	for(i=0;i<size;i++){
		if(machine->ReadMem(from+i,1,&tmp))
		to[i]=tmp;
	}
	if(tmp!='\0'){
		to[size-1]='\0';
	}
}


//----------------------------------------------------------------------
// copyStringToMachine : Used for SynchGetString
//----------------------------------------------------------------------
void copyStringToMachine( char *from, int to, unsigned size){
	unsigned int i;
	int tmp;
	for(i=0;i<size-1;i++){
		tmp=from[i];
		machine->WriteMem(to+i,1,tmp);
	}
	tmp='\0';
	machine->WriteMem(to+i,1,tmp);
}
#endif

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
	int type = machine->ReadRegister (2);
#ifndef CHANGED
    if ((which == SyscallException) && (type == SC_Halt))
      {
	  DEBUG ('a', "Shutdown, initiated by user program.\n");
	  interrupt->Halt ();
      }
    else
      {
	  printf ("Unexpected user mode exception %d %d\n", which, type);
	  ASSERT (FALSE);
      }
#else
	if (which == SyscallException){
		switch(type){
			case SC_Halt:{
				DEBUG ('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt ();
				break;
			}
			case SC_PutChar:{
				int c = machine->ReadRegister (4);	//Le caractère à insérer est dans le registre 4
				synchconsole->SynchPutChar((char)c);	//On fait appel à PutChar de synchconsole
				break;
			}
			case SC_SynchPutString:{
				char *buffer=new char[MAX_STRING_SIZE];
				int s = machine->ReadRegister (4);	//L'adresse du string à insérer est dans le registre 4
				copyStringFromMachine(s, buffer, MAX_STRING_SIZE);
				synchconsole->SynchPutString(buffer);	//On fait appel à SynchPutString de synchconsole
				delete buffer;
				break;
			}
			case SC_SynchGetChar:{
				char c = synchconsole->SynchGetChar();
				machine->WriteRegister (2,(int)c);	//On insère dans le registre 2 le caractère lu sur le terminal
				break;
			}
			case SC_SynchGetString:{
				char *buffer=new char[MAX_STRING_SIZE];
				int s = machine->ReadRegister (4);
				int size = machine->ReadRegister (5);
				synchconsole->SynchGetString(buffer,size);
				copyStringToMachine(buffer, s, size);
				delete buffer;
				break;
			}
			case SC_SynchPutInt:{
				int n = machine->ReadRegister (4);	//L'entier à afficher est dans le registre 4
				synchconsole->SynchPutInt(n);	//On fait appel à SynchPutInt de synchconsole
				break;
			}
			case SC_SynchGetInt:{
				int n = machine->ReadRegister (4);	//L'entier à afficher est dans le registre 4
				synchconsole->SynchGetInt((int *)n);	//On fait appel à SynchPutInt de synchconsole
				break;
			}
			case SC_UserThreadCreate:{
				do_UserThreadCreate(machine->ReadRegister (4), machine->ReadRegister (5));	//On créé le thread
				break;
			}
			case SC_UserThreadExit:{
				do_UserThreadExit();	//On quitte le thread
				break;
			}
			default:{
				printf ("Unexpected user mode exception %d %d\n", which, type);
				ASSERT (FALSE);
			}
		}
	}
	else{
		printf ("Unexpected user mode exception %d %d\n", which, type);
		ASSERT (FALSE);
	}
#endif //CHANGED

    // LB: Do not forget to increment the pc before returning!
    UpdatePC ();
    // End of addition
}
