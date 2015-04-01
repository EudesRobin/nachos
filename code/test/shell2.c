#include "syscall.h"

int
main ()
{
	char s[150];
    while (1)
      {
	SynchPutString("$>");
	SynchGetString(s,150);
	ForkExec(s);
	int a=1001;
	int j;
	for(j=0;j<3000;j++){
		if(a%2){
			a=a*2;
		}
		else{
			a=a/2;
		}
	}
      }
}
