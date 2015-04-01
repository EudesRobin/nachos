#include "syscall.h"

int main(){
	char s[150];
	SynchGetString(s,150);
	SynchPutString(s);
	PutChar('\n');
	Exit(0);
}

