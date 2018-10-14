#ifndef atMTLParser_h__
#define atMTLParser_h__

#include "atMesh.h"

class atMTLReader
{
public:
  atMTLReader() = delete;
  bool Read(const atFilename &file, atMesh *pMesh);
};

class atMTLWriter
{
public:
  atMTLWriter() = delete;
  bool Write(const atFilename &file, const atMesh &mesh);
};

#endif // atMTLParser_h__
