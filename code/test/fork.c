#include "syscall.h"

int main()
{
	ForkExec("userpages0");
	ForkExec("userpages1");
	Exit(0);
}
