#ifndef atMeshParser_h__
#define atMeshParser_h__

#include "atMesh.h"

class atMeshReader
{
public:
  atMeshReader() = delete;
  static bool Read(const atFilename& file, atMesh *pMesh);
};

class atMeshWriter
{
public:
  atMeshWriter() = delete;
  static bool Write(const atFilename &file, const atMesh &mesh);
};

#endif // atMeshParser_h__
