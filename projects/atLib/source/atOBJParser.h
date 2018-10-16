#ifndef atOBJParser_h__
#define atOBJParser_h__

#include "atMesh.h"

enum atOBJKeyword
{
  atOBJObject, // unsupported
  atOBJGroup, // unsupported
  atOBJSmoothShading, // unsupported
  atOBJVertex,
  atOBJNormal,
  atOBJTexCoord,
  atOBJLine,
  atOBJFace,
  atOBJMatLib,
  atOBJMatRef,
  atOBJComment,
  atOBJNone
};

#define atOBJInvalidIndex INT64_MAX

class atOBJReader
{
public:
  atOBJReader() = delete;
  static bool Read(const atFilename &file, atMesh *pMesh);
  
  // This probably belongs in atScan as it is a very generic string parsing function
  template <typename T> static T ParseVector(char **ppSrc, const int64_t srcLen, int64_t *pLen = nullptr);
};

class atOBJWriter
{
public:
  atOBJWriter() = delete;
  static bool Write(const atFilename &file, const atMesh &mesh);
};

#include "atOBJReader.inl"
#endif // atOBJParser_h__
