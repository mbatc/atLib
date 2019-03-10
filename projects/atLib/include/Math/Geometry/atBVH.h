

// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#ifndef atBVH_h__
#define atBVH_h__

#include "atRay.h"
#include "atAABB.h"

template<typename T> struct atBVHNode
{
  atBVHNode();
  bool isLeaf = false;
  T primitive;
  atAABB<double> bounds;
  atVector<atBVHNode<T>> children;
};

template<typename T> atBVHNode<T>::atBVHNode() {}

template<typename T> class atBVH
{
public:
  atBVH(const atVector<T> &primitives);

  void Construct(const atVector<T> &primitives);

  // Recursively construct the tree - removes items from pPrimitives as they are assigned to tree node
  void ConstructRecursive(atBVHNode<T> *pRoot, atVector<atBVHNode<T>> *pLeaves);

  template<typename T2> bool RayTrace(const atRay<T2> &ray, atMatrix4x4<T2> &modelMat, T2 *pTime);

  atBVHNode<T> m_root;
};

template<typename T, typename T2> bool atIntersects(const atRay<T2> &a, const atBVH<T> &b, T2 *pTime);

#include "atBVH.inl"
#endif // atBVH_h__

