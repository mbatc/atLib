template <typename T> atBVH<T>::atBVH(const atVector<T> &primitives) { Construct(std::move(primitives)); }

template <typename T> void atBVH<T>::Construct(const atVector<T> &primitives)
{
  atVector<Node> leaves(primitives.size());
  for (T &prim : primitives)
  {
    Node leaf;
    leaf.bounds.GrowToContain(prim);
    leaf.primitive = prim;
    leaves.push_back(leaf);
    m_root.bounds.GrowToContain(leaf.bounds);
  }
  ConstructRecursive(m_root, &leaves);
}

template<typename T> void atBVH<T>::ConstructRecursive(Node *pRoot, atVector<Node> *pLeaves)
{
  pRoot->children.resize(8);
  atVec3F64 halfSize = pRoot->bounds.Dimensions() / 2;
  for (int64_t z = 0; z < 2; ++z)
    for (int64_t y = 0; y < 2; ++y)
      for (int64_t x = 0; x < 2; ++x)
      {
        pRoot->children[x + y * 2 + z * 4].bounds.m_min = pRoot->bounds.m_min + halfSize * atVec3F64{ x, y, z };
        pRoot->children[x + y * 2 + z * 4].bounds.m_max = pRoot->bounds.m_min + halfSize * atVec3F64{ x + 1, y + 1, z + 1 };
      }

  atVector<atVector<int64_t>> indices;

  for (int64_t c = 0; c < pRoot->children.size(); ++c)
  {
    int64_t nPrims = 0;
    int64_t lastPrim = 0;
    atAABB<double> actualBounds;
    Node &child = pRoot->children[c];
    for (T &prim : *pLeaves)
    {
      atVector3<double> center = prim.bounds.Center();
      if (!child.bounds.Contains(center))
        continue;
      actualBounds.GrowToContain(prim.bounds);
      lastPrim = &prim - pLeaves->begin();
      ++nPrims;
    }

    if (nPrims == 0)
    {
      pRoot->children.erase(c);
      --c;
      continue;
    }
    else if (nPrims == 1)
    {
      child.isLeaf = true;
      child.primitive = pLeaves[lastPrim];
      pLeaves->erase(lastPrim);
    }
    child.bounds = actualBounds;
  }

  for (Node &child : pRoot->children)
    if(!child.isLeaf)
      ConstructRecursive(&child, pLeaves);
}

template <typename T> template <typename T2> bool atBVH<T>::RayTrace(const atRay<T2> &ray, atMatrix<T2, 4, 4> &modelMat, const T2 *pTime) 
{ 
  atMat4D invModel = modelMat.Inverse();
  atVec3F64 startPos = ray.m_pos * invModel;
  atVec3F64 endPos = startPos + ray.m_dir * invModel;
  atIntersects(atRay(startPos, endPos), 
  for()
  return false; 
}