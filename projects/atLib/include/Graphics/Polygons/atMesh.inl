
template<typename T> inline atMesh::atMesh(const T &o) { *this = atToMesh(o); }

template<typename T> atMesh atToMesh(const T &o)
{
  static_assert(false, "atToMesh(const T &o) not defined for type T");
  return atMesh();
};