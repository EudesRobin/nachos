#include "syscall.h"

void putint(){
	SynchPutInt(42);
	PutChar('\n');
 }

int main(){
	putint();
	Halt();
}

