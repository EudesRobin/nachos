#ifdef CHANGED

#include "frameprovider.h"
#include "machine.h"


FrameProvider::FrameProvider()
{
	phyMemBitmap = new BitMap(MemorySize);
}

FrameProvider::~FrameProvider(){
	delete phyMemBitmap;
}

int
FrameProvider::GetEmptyFrame()
{
	if((phyMemBitmap->NumClear())<=0){
		printf("Stack overflow\n");
		return -1;
	}
	int tmp = phyMemBitmap->Find();
	phyMemBitmap->Mark(tmp);
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
