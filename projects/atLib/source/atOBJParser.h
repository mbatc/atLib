#ifndef atOBJParser_h__
#define atOBJParser_h__

#include "atMesh.h"

enum atOBJKeyword
{
  atOBJVertex,
  atOBJNormal,
  atOBJTexCoord,
  atOBJLine,
  atOBJFace,
  atOBJMatLib,
  atOBJMatRef,
  atOBJNone
};

class atOBJReader
{
public:
  atOBJReader() = delete;
  static bool Read(const atFilename &file, atMesh *pMesh);
};

class atOBJWriter
{
public:
  atOBJWriter() = delete;
  static bool Write(const atFilename &file, const atMesh &mesh);
};

#endif // atOBJParser_h__
