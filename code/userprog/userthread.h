#ifdef CHANGED

	typedef struct{
		int func;
		int argv;
	}argThread;
	
	extern int do_UserThreadCreate(int f, int arg);
	extern int do_UserThreadExit();

#endif // CHANGED
