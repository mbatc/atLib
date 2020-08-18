#ifndef atFBXParser_h__
#define atFBXParser_h__

#include "atMesh.h"

class atFBXReader
{
public:
  atFBXReader() = delete;

  static bool Read(const atFilename &file, atMesh *pMesh);
};

class atFBXWriter
{
public:
  atFBXWriter() = delete;

  static bool Write(const atFilename &file, const atMesh &mesh);
};

#endif // atFBXParser_h__
