#ifndef atDisplay_h__
#define atDisplay_h__

#include "atMath.h"

class atDisplay
{
public:
  atDisplay() = delete;

  // Get the number of displays
  static int64_t Count();
  
  static int64_t MaximizedWidth();
  static int64_t MaximizedHeight();
  static atVec2I MaximizedResolution();

  static int64_t ScreenWidth();
  static int64_t ScreenHeight();
  static atVec2I ScreenResolution();
};

#endif // atDisplay_h__
