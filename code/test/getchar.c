#include "syscall.h"

int main(){
	int c = SynchGetChar();
	PutChar((char)c);
	PutChar('\n');
	Halt();
}

