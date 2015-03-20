#include "syscall.h"

int main()
{
	ForkExec("userpages0");
	PutChar('a');
	Exit(0);
}
