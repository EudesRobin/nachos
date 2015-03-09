#ifdef CHANGED
#include "thread.h"

	typedef struct{
		int func;
		int argv;
	}argThread;
	
	extern int do_UserThreadCreate(int f, int arg);
	extern int do_UserThreadExit();
	extern int UserThreadJoin(Thread t);

#endif // CHANGED
