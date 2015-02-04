#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, readAvail, writeDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar (ch);
	writeDone->P ();	// wait for write to finish
}

char SynchConsole::SynchGetChar()
{
	readAvail->P ();	// wait for character to arrive
	char ch = console->GetChar ();
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

