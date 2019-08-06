
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

#ifndef atAABB_h__
#define atAABB_h__

#include "atVector.h"
#include "atVector3.h"
#include "atIntersects.h"

template<typename T> class atAABB
{
public:
  using Vec3 = atVector3<T>;

  atAABB();
  atAABB(const Vec3 &min, const Vec3 &max);
  atAABB(atVector<Vec3> &points);
  atAABB(const atAABB &copy);
  atAABB(atAABB &&o);

  atAABB<T> OverlappingBox(const atAABB<T> &box) const;

  void GrowToContain(const atVector<Vec3> &points);
  void GrowToContain(const Vec3 &point);
  void GrowToContain(const atAABB<T> &box);
  void ShrinkToFit(const atAABB<T> &box);

  bool Contains(const Vec3 &point) const;

  // Returns the point within the AABB3 that is closest to 'point'
  Vec3 ClosestPoint(const Vec3 &point) const;

  // Returns the closest point that lies on the bounds of the AABB
  Vec3 ClosestPointBounds(const Vec3 &point) const;

  Vec3 Center() const;
  Vec3 Dimensions() const;

  T LongestEdgeLength() const;

  template<typename Type> void GrowToContain(const Type &type);

  Vec3 m_min;
  Vec3 m_max;
  
  const atAABB<T> &operator=(atAABB<T> &&rhs);
  const atAABB<T> &operator=(const atAABB<T> &rhs);
  bool operator==(const atAABB<T> &rhs) const;
  bool operator!=(const atAABB<T> &rhs) const;
};

template<typename T, typename Type> atAABB<T> atBounds(const Type &type) { atAssert(false, "Bounds are not defined for this type"); return atAABB<T>(); }

typedef atAABB<int32_t> atAABBI;
typedef atAABB<int64_t> atAABBI64;
typedef atAABB<float> atAABBF;
typedef atAABB<double> atAABBD;

#include "atAABB.inl"
#endif // atAABB_h__
