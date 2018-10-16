#include "atMeshParser.h"
#include "atOBJParser.h"

bool atMeshReader::Read(const atFilename& file, atMesh *pMesh)
{
  atString ext = file.Extension().to_lower();
  if (ext == "obj") return atOBJReader::Read(file, pMesh);
  return false;
}