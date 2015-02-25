#include "syscall.h"

int main(){
	SynchPutString("bon\127our\n");
	Halt();
}

