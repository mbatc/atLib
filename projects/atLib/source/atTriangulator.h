#ifndef atTriangulator_h__
#define atTriangulator_h__

#include "atMesh.h"

class atTriangulator
{
public:
  atTriangulator() = delete;

  atVector<atMesh::Triangle> TriangleFan(const atVector<atMesh::Face> &faces);
  atVector<int64_t> TriangleFan(const atVector<int64_t> &indices);
};

#endif // atTriangulator_h__
