#include "atTriangulator.h"

atVector<atMesh::Triangle> atTriangulator::TriangleFan(const atVector<atMesh::Face>& faces)
{
  int64_t nFaces = 0;
  for (const atMesh::Face &f : faces)
    nFaces += atMax(0, f.verts.size() - 2);
  atVector<atMesh::Triangle> tris(nFaces);
  for (const atMesh::Face &f : faces)
    for (int64_t t = 0; t < f.verts.size() - 2; ++t)
    {
      tris.push_back(atMesh::Triangle());
      atMesh::Triangle &tri = *(tris.end() - 1);
      tri.mat = f.mat;
      tri.verts[0] = f.verts[0];
      tri.verts[1] = f.verts[t + 1];
      tri.verts[2] = f.verts[t + 2];
    }
  return tris;
}

atVector<int64_t> atTriangulator::TriangleFan(const atVector<int64_t> &indices)
{
  const int64_t nFaces = indices.size() - 2;
  atVector<int64_t> ret(nFaces * 3);
  for (int64_t f = 0; f < nFaces; ++f)
    ret.push_back({indices[0], indices[f + 1], indices[f + 2] });
  return ret;
}

