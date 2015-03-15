#ifdef CHANGED

#include "frameprovider.h"
#include "synch.h"
#include "sysdep.h"

static void shuffle (int *first, int size)
{
	int i,rand,swap;
	for(i=size-1;i>0;i--){
		rand=Random()%i;
		swap=first[i];
		first[i]=first[rand];
		first[rand]=swap;
	}
}

static Semaphore *semFrame = new Semaphore("semFrame",1);


FrameProvider::FrameProvider(int MemorySize)
{
	phyMemBitmap = new BitMap(MemorySize);
	permutations = new int[MemorySize];
	retour_permutations = new int[MemorySize];
	int i;
	for(i=0;i<MemorySize;i++)
		permutations[i]=i;
	//On a défini Time() dans sysdep.h afin d'accéder au temps sans conflit d'inclusions
	RandomInit((unsigned)Time());
	shuffle(permutations, MemorySize);
	for(i=0;i<MemorySize;i++)
		retour_permutations[permutations[i]]=i;
}

FrameProvider::~FrameProvider(){
	delete phyMemBitmap;
	delete permutations;
	delete retour_permutations;
}

int
FrameProvider::GetEmptyFrame()
{
	if((phyMemBitmap->NumClear())<=0){
		printf("Stack overflow\n");
		return -1;
	}
	semFrame->P();
	int tmp = phyMemBitmap->Find();
	semFrame->V();
	phyMemBitmap->Mark(tmp);
	return permutations[tmp];
}

void
FrameProvider::ReleaseFrame(int numFrame)
{
	if(!phyMemBitmap->Test(retour_permutations[numFrame])){
	//if(!phyMemBitmap->Test(numFrame)){
		printf("Error numFrame %d\n",numFrame);
		return;
	}
	phyMemBitmap->Clear(retour_permutations[numFrame]);
	//phyMemBitmap->Clear(numFrame);
}

int
FrameProvider::NumAvailFrame()
{
	return phyMemBitmap->NumClear();
}

#endif //CHANGED
