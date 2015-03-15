#ifdef CHANGED

#include "bitmap.h"

class FrameProvider
{
  public:
	FrameProvider(int MemorySize);
	~FrameProvider();
	int GetEmptyFrame();
	void ReleaseFrame(int numFrame);
	int NumAvailFrame();
  private:
	BitMap *phyMemBitmap;
	int BitmapSize;
};

#endif //CHANGED
