#ifndef atBVH_h__
#define atBVH_h__

#include "atRay.h"

template <typename T> class atBVH
{
public:
  struct Node
  {
    bool isLeaf = false;
    T primitive;
    atAABB<double> bounds;
    atVector<Node> children;
  };

  atBVH(const atVector<T> &primitives);

  void Construct(const atVector<T> &primitives);

  // Recursively construct the tree - removes items from pPrimitives as they are assigned to tree node
  void ConstructRecursive(Node *pRoot, atVector<Node> *pLeaves);

  template <typename T2> bool RayTrace(const atRay<T2> &ray, atMatrix<T2, 4, 4> &modelMat, const T2 *pTime);

protected:
  Node m_root;
};

template <typename T, typename T2> bool atIntersects(const atRay<T> &a, const atBVH<T2> &b);

#endif // atBVH_h__
