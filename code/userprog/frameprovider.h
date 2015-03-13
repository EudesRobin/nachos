#ifdef CHANGED

#include "bitmap.h"

class FrameProvider
{
  public:
	FrameProvider();
	~FrameProvider();
	int GetEmptyFrame();
	void ReleaseFrame(int numFrame);
	int NumAvailFrame();
  private:
	BitMap *phyMemBitmap;

};

#endif //CHANGED
