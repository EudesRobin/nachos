// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

#include <strings.h>		/* for bzero */

#ifdef CHANGED
#include "synch.h"

static void ReadAtVirtual(OpenFile *executable, int virtualaddr, int numBytes, int position, TranslationEntry *pageTable,unsigned numPages){
	if ((numBytes <= 0) ||  (virtualaddr < 0) || ((unsigned)virtualaddr > numPages*PageSize)){
		printf("Erreur ReadAtVirtual\n");
		return;
	}
	int numPages_old = machine->pageTableSize;
	TranslationEntry *page_old = machine->pageTable;
	char *into = new char[numBytes+(numBytes%4)];
	executable->ReadAt(into,numBytes,position);
	int i;

	machine->pageTable = pageTable ;
	machine->pageTableSize = numPages ;
	
	for(i=0;i<numBytes;i+=4){
		machine->WriteMem((int)(virtualaddr+i), 4, (int)(*(int *)(into+i)));
	}

	machine->pageTable = page_old ;
	machine->pageTableSize = numPages_old ;

	delete [] into;
}
#endif //CHANGED

//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;
	#ifdef CHANGED
    int numPages_old ;
    TranslationEntry *page_old ;

	//ReadAt lit l'entête d'executable (début de lecture: position 0, nombre de bytes lu: sizeof(NoffHeader) )
	//La valeur de l'entête est stockée dans noffH
	#endif //CHANGED
    executable->ReadAt ((char *) &noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    ASSERT (numPages <= NumPhysPages);	// check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory

    DEBUG ('a', "Initializing address space, num pages %d, size %d\n",
	   numPages, size);

// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++)
      {
	  pageTable[i].virtualPage = i;	// for now, virtual page # = phys page #
	#ifndef CHANGED
	  pageTable[i].physicalPage = i;
	#else
	  pageTable[i].physicalPage = frameProvider->GetEmptyFrame();
	#endif //CHANGED
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }


	#ifdef CHANGED
	for(i=0;i<divRoundUp(UserStackSize,PageSize);i++)
		this->TabSemJoin[i] = new Semaphore("SemJoin",1);
	#endif //CHANGED

// zero out the entire address space, to zero the unitialized data segment 
// and the stack segment
	#ifndef CHANGED
    bzero (machine->mainMemory, size);
	#else
	page_old = machine->pageTable ;
	numPages_old = machine->pageTableSize ;

	machine->pageTable = pageTable ;
	machine->pageTableSize = numPages ;

	for (i = 0; i < numPages*PageSize; i+=4)
	{
		machine->WriteMem(i, 4, 0);
	}
	machine->pageTable = page_old ;
	machine->pageTableSize = numPages_old ;

	//Main program's stack marked
	stack = new BitMap(divRoundUp(UserStackSize,PageSize));
	printf("freestack: %d\n",stack->NumClear());
	for(i=0;i<NbPagesThread;i++){
		stack->Mark(i);
	}

	askEnd=false;
	BlockMultiThread = new Semaphore("BlockMultiThread",0);
	SemThread = new Semaphore("SemThread",1);
	nbThreads=0;
	#endif //CHANGED


// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size %d\n",
		 noffH.code.virtualAddr, noffH.code.size);
	#ifndef CHANGED
	  executable->ReadAt (&(machine->mainMemory[noffH.code.virtualAddr]),
			      noffH.code.size, noffH.code.inFileAddr);
	#else
	ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
	#endif //CHANGED
      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size %d\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
	#ifndef CHANGED
	  executable->ReadAt (&
			      (machine->mainMemory
			       [noffH.initData.virtualAddr]),
			      noffH.initData.size, noffH.initData.inFileAddr);
	#else
	ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
	#endif //CHANGED
      }

}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
	#ifdef CHANGED
	unsigned i;
	for(i=0;i<divRoundUp(UserStackSize,PageSize);i++){
		delete this->TabSemJoin[i];
	}
	for(i=0;i<numPages;i++){
		frameProvider->ReleaseFrame(pageTable[i].physicalPage);
	}
	delete stack;
	#endif //CHANGED
  // LB: Missing [] for delete
  // delete pageTable;
  delete [] pageTable;
  // End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to %d\n",
	   numPages * PageSize - 16);
}
#ifdef CHANGED
//----------------------------------------------------------------------
// AddrSpace::CheckFreeStack
//      If there is an available position in the Stack, returns true.
//	Else false.
//----------------------------------------------------------------------

bool
AddrSpace::CheckFreeStack ()
{
	return (stack->NumClear())>0;
}

//----------------------------------------------------------------------
// AddrSpace::AllocStack
//      If there is a place in the stack that is free, returns a free
//      position in the stack. If it does'nt exist, returns -1.
//----------------------------------------------------------------------

int
AddrSpace::AllocStack ()
{
	SemThread->P();
	//On veut allouer 4 pages pour chaque thread
	if((stack->NumClear())<=(NbPagesThread-1)){
		printf("Stack overflow\n");
		return -1;
	}
	int tmp = stack->Find();
	for(int i=1;i<NbPagesThread;i++){
		if(stack->Test(tmp+i)){
			printf("Pages block(%d) is not available\n",NbPagesThread);
			return -1;
		}
		stack->Mark(tmp+i);
	}
	nbThreads++;
	SemThread->V();
	return tmp;
}

//----------------------------------------------------------------------
// AddrSpace::FreeStack
//      If the position numStack is set, clear it in the stack.
//----------------------------------------------------------------------

void
AddrSpace::FreeStack (int numStack)
{
	SemThread->P();
	for(int i=0;i<NbPagesThread;i++){
		if(!stack->Test(numStack+i)){
			printf("Error numStack %d\n",numStack);
			return;
		}
		stack->Clear(numStack+i);
	}
	nbThreads--;
	if(askEnd && nbThreads==0){
		BlockMultiThread->V();
	}
	SemThread->V();
}


//----------------------------------------------------------------------
// AddrSpace::StackValue
//      Returns the stack value associated to the bitmap value.
//----------------------------------------------------------------------

int
AddrSpace::StackValue(int BitmapValue)
{
	return PageSize*numPages - BitmapValue*PageSize;
}

//----------------------------------------------------------------------
// AddrSpace::CheckLastThread
//      Blocks Halt() when there are other alive threads.
//----------------------------------------------------------------------

void
AddrSpace::CheckLastThread ()
{
	if(nbThreads!=0){
		askEnd=true;
		BlockMultiThread->P();
		askEnd=false;
	}
}

bool
AddrSpace::Test(int BitmapValue)
{
	return this->stack->Test(BitmapValue);
}
#endif //CHANGED

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
