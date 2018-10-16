#ifndef atMTLParser_h__
#define atMTLParser_h__

#include "atMesh.h"
#include "atHashMap.h"

enum atMTLKeyword
{
  atMTLNone,
  atMTLNew,
  atMTLAmbientColour,
  atMTLDiffuseColour,
  atMTLSpecularColour,
  atMTLSpecularPower,
  atMTLAlpha,
  atMTLAlphaInv,
  atMTLAmbientMap,
  atMTLDiffuseMap,
  atMTLSpecularColourMap,
  atMTLSpecularHighlightMap,
  atMTLAlphaMap,
  atMTLBumpMap,
  atMTLDisplacementMap,
  atMTLIllumModel
};

class atMTLReader
{
public:
  atMTLReader() = delete;
  static bool Read(const atFilename &file, atMesh *pMesh, const atHashMap<atString, int64_t> &materials = atHashMap<atString, int64_t>());
};

class atMTLWriter
{
public:
  atMTLWriter() = delete;
  static bool Write(const atFilename &file, const atMesh &mesh);
};

#endif // atMTLParser_h__
