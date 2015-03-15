#ifdef CHANGED

#include "bitmap.h"

class FrameProvider
{
  public:
	FrameProvider(int NumPages);
	~FrameProvider();
	int GetEmptyFrame();
	void ReleaseFrame(int numFrame);
	int NumAvailFrame();
  private:
	BitMap *phyMemBitmap;
	int BitmapSize;
};

#endif //CHANGED
