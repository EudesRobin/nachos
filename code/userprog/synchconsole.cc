#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
#include "console.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *SemPutChar;
static Semaphore *SemGetChar;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	SemPutChar = new Semaphore("PutChar", 1);
	SemGetChar = new Semaphore("GetChar", 1);

	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	SemPutChar->P();
	console->PutChar (ch);
	writeDone->P ();	// wait for write to finish
	SemPutChar->V();
}

char SynchConsole::SynchGetChar()
{
	SemGetChar->P();
	char ch;
	readAvail->P ();	// wait for character to arrive
	ch = console->GetChar ();
	SemGetChar->V();
	return ch;
}

void SynchConsole::SynchPutString(const char s[])
{
/*
    for (i=0;;i++)
      {
      if (s[i]=='\0')
	      return;		// if end of string, quit
	  console->PutChar (s[i]);
	  writeDone->P ();
      }
*/
}

void SynchConsole::SynchGetString(char *s, int n)
{
/*
    for (i=0;i<n;i++)
      {
          readAvail->P ();	// wait for character to arrive
	  s[i] = console->Getchar ();
      }
    s[n]='\0';
*/
}

#endif // CHANGED

