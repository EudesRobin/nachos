// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"

#ifdef CHANGED
#include "bitmap.h"
#include "synch.h"
#endif //CHANGED

#ifndef CHANGED
#define UserStackSize		1024	// increase this as necessary!
#else
#define UserStackSize		2048
#define NbPagesThread		4
#endif //CHANGED
class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

	#ifdef CHANGED
	//L'utilisateur utilise au plus divRoundUp(UserStackSize,PageSize) threads concurrents 
	Semaphore *TabSemJoin[divRoundUp(UserStackSize,PageSize)];
	bool CheckFreeStack ();
	int AllocStack ();
	void FreeStack (int numStack);
	int StackValue(int BitmapValue);
	void CheckLastThread();
	bool Test(int BitmapValue);
	#endif //CHANGED

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
	#ifdef CHANGED
	BitMap *stack;//Variables globales
	bool askEnd;
	Semaphore *BlockMultiThread;
	Semaphore *SemThread;
	int nbThreads;
	#endif //CHANGED

};

#endif // ADDRSPACE_H
