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
static Semaphore *SemPutString;
static Semaphore *SemGetString;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);

	SemPutChar = new Semaphore("PutChar", 1);
	SemGetChar = new Semaphore("GetChar", 1);
	SemPutString = new Semaphore("PutString", 1);
	SemGetString = new Semaphore("GetString", 1);
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

/*
int SynchConsole::SynchGetChar()
{
	SemGetChar->P();
	int ch;
	readAvail->P ();	// wait for character to arrive
	ch = (int)console->GetChar ();
	SemGetChar->V();
	return ch;
}*/

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
	SemPutString->P();
	int i;
	for (i=0;i<MAX_STRING_SIZE && s[i]!='\0';i++){
		if (s[i]=='\0')
			return;		// if end of string, quit
		synchconsole->SynchPutChar ((char)s[i]);
	}
	SemPutString->V();
}

void SynchConsole::SynchGetString(char *s, int n)
{
	SemGetString->P();
	char c;
	int i;
	for (i=0;i<n;i++){
		c = synchconsole->SynchGetChar ();
		if(c==EOF || c=='\n')
			break;
		else
			s[i] = c;
	}
	s[i]='\0';
	SemGetString->V();
}
/*
void SynchConsole::SynchGetString(char *s, int n)
{
	SemGetString->P();
	char c;
	int i;

	c = synchconsole->SynchGetChar ();
	if(c==EOF || c=='\n'){
		s[0]='\0';
		SemGetString->V();
		return;
	}
	else
		s[0] = c;
	for (i=1;i<n;i++){
		c = synchconsole->SynchGetChar ();
		if(c=='\n')
			break;
		else{
			if(c==EOF)
				i--;
			else
				s[i] = c;
		}
	}
	s[i]='\0';
	SemGetString->V();
}
*/

void SynchConsole::SynchPutInt(int n)
{
	char *s = new char(MAX_STRING_SIZE);
	snprintf(s,MAX_STRING_SIZE,"%d",n);
	synchconsole->SynchPutString(s);
}

void SynchConsole::SynchGetInt(int *n)
{
	int res;
	//Les entiers ne peuvent pas avoir plus de 10 caractères, on songe également au \0 de fin
	char *s = new char(12);
	SynchGetString(s,12);
	sscanf(s,"%d",&res);
	//L'écriture se fait sur 4 octets
	machine->WriteMem((int)n,4,res);
	delete s;
}

#endif // CHANGED

