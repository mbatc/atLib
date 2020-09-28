#ifndef atSVG_h__
#define atSVG_h__

#include "atImage.h"

class atSVG
{
public:
  static atImage Rasterize(const atFilename &path, const atVec2I &res, const int64_t &dpi = 96);
};

#endif // atSVG_h__
