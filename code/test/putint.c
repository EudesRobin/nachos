#include "syscall.h"

int main(){
	SynchPutInt(42);
	PutChar('\n');
	Halt();
}

