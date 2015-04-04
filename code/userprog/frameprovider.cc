#ifdef CHANGED

#include "frameprovider.h"
#include "synch.h"
#include "sysdep.h"

static Semaphore *semFrame = new Semaphore("semFrame",1);


FrameProvider::FrameProvider(int NumPages)
{
	phyMemBitmap = new BitMap(NumPages);
	BitmapSize = NumPages;
}

FrameProvider::~FrameProvider(){
	delete phyMemBitmap;
}

int
FrameProvider::GetEmptyFrame()
{
	semFrame->P();
	if((phyMemBitmap->NumClear())<=0){
		semFrame->V();
		return -1;
	}
	//Vesrion aléatoire
//	RandomInit(Time());
//	int tmp=Random()%(BitmapSize-1);
//	while(phyMemBitmap->Test(tmp)){
//		tmp=Random()%(BitmapSize-1);
//	}
//	phyMemBitmap->Mark(tmp);
	//Version non aléatoire
	int tmp = phyMemBitmap->Find();
	semFrame->V();
	return tmp;
}

void
FrameProvider::ReleaseFrame(int numFrame)
{
	if(!phyMemBitmap->Test(numFrame)){
		printf("Error numFrame %d\n",numFrame);
		return;
	}
	phyMemBitmap->Clear(numFrame);
}

int
FrameProvider::NumAvailFrame()
{
	return phyMemBitmap->NumClear();
}

#endif //CHANGED
