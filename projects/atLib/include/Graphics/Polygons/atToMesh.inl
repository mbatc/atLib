
template<typename T>
inline atMesh atToMesh(const atTriangle<T> &tri)
{
  atMesh mesh;
  mesh.m_positions.reserve(3);
  atMesh::Triangle tri;
  for (int64_t i = 0; i < 3; ++i)
  {
    mesh.m_positions.push_back(tri.points[i]);
    tri.verts[i].position = i;
  }
  mesh.m_triangles.push_back(tri);
  return mesh;
}

template<typename T>
inline atMesh atToMesh(const atRect<T> &aabb)
{
  atMesh mesh;
  for (int64_t y = 0; y < 2; ++y)
    for (int64_t x = 0; x < 2; ++x)
      mesh.m_positions.push_back({ aabb.m_extents[x].x, aabb.m_extents[y].y, 0 });
  atMesh::Triangle tri;
  tri.verts[0].position = 0;
  tri.verts[1].position = 1;
  tri.verts[2].position = 2;
  mesh.m_triangles.push_back(tri);
  tri.verts[2].position = 3;
  mesh.m_triangles.push_back(tri);
  return mesh;
}

template<typename T>
inline atMesh atToMesh(const atPlane<T> &plane)
{
  return atToMesh(atRect<T>({ -0.5, -0.5 }, { 0.5, 0.5 })).SpatialTransform(atMatrixRotFromDir(plane.Normal()));
}

template<typename T>
inline atMesh atToMesh(const atSphere<T> &aabb)
{
  atMesh mesh;
  atRelAssert(false, "atToMesh no implemented for atSphere");
  return mesh;
}

template<typename T>
inline atMesh atToMesh(const atAABB<T> &aabb)
{
  atMesh mesh;
  /*
  for (int64_t z = 0; z < 2; ++z)
    for (int64_t y = 0; y < 2; ++y)
      for (int64_t x = 0; x < 2; ++x)*/
  atRelAssert(false, "atToMesh no implemented for atAABB");
  return mesh;
}

template<typename T>
inline atMesh atToMesh(const atOBB<T> &aabb)
{
  atMesh mesh;
  aabb.
  atRelAssert(false, "atToMesh no implemented for atOBB");
  return mesh;
}
