#include "atSVG.h"

#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION

#pragma warning( disable : 4244 )
#include "nanosvg.h"
#include "nanosvgrast.h"
#pragma warning( default : 4244 )

atImage atSVG::Rasterize(const atFilename &path, const atVec2I &res, const int64_t &dpi)
{
  atImage dstImg(res);
  NSVGrasterizer *pRasterizer = nsvgCreateRasterizer();
  NSVGimage *pSVG = nsvgParseFromFile(path.c_str(), "px", 96);
  nsvgRasterize(pRasterizer, pSVG, 0, 0, 1, (uint8_t*)dstImg.Pixels().data(), res.x, res.y, res.x * sizeof(atCol));
  nsvgDelete(pSVG);
  nsvgDeleteRasterizer(pRasterizer);
  return dstImg;
}
