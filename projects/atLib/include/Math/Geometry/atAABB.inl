
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#include "atAABB.h"
#include "atLimits.h"

template<typename T> atAABB<T>::atAABB(const atAABB &copy)
  : m_min(copy.m_min)
  , m_max(copy.m_max)
{}

template<typename T> atAABB<T>::atAABB(atAABB &&o)
  : m_min(std::move(o.m_min))
  , m_max(std::move(o.m_max))
{}

template<typename T> void atAABB<T>::GrowToContain(const Vec3 &point)
{
  for (int64_t dim = 0; dim < 3; ++dim)
  {
    m_min[dim] = atMin(point[dim], m_min[dim]);
    m_max[dim] = atMax(point[dim], m_max[dim]);
  }
}

template<typename T> void atAABB<T>::GrowToContain(const atAABB<T> &box)
{
  for (int64_t dim = 0; dim < 3; ++dim)
  {
    m_min[dim] = atMin(box.m_min[dim], m_min[dim]);
    m_max[dim] = atMax(box.m_max[dim], m_max[dim]);
  }
}

template<typename T> void atAABB<T>::ShrinkToFit(const atAABB<T> &box)
{
  for (int64_t dim = 0; dim < 3; ++dim)
  {
    m_min[dim] = atMax(box.m_min[dim], m_min[dim]);
    m_max[dim] = atMin(box.m_max[dim], m_max[dim]);
  }
}

template<typename T> bool atAABB<T>::Contains(const Vec3 &point) const
{
  if (point.x < m_min.x) return false;
  if (point.y < m_min.y) return false;
  if (point.z < m_min.z) return false;
  if (point.x > m_max.x) return false;
  if (point.y > m_max.y) return false;
  if (point.z > m_max.z) return false;
  return true;
}

template<typename T> const atAABB<T>& atAABB<T>::operator=(atAABB<T> &&rhs)
{
  m_min = rhs.m_min;
  m_max = rhs.m_max;
  rhs.m_min = atVector3<T>::zero();
  rhs.m_max = atVector3<T>::zero();
  return *this;
}

template<typename T> const atAABB<T>& atAABB<T>::operator=(const atAABB<T> &rhs)
{
  m_min = rhs.m_min;
  m_max = rhs.m_max;
  return *this;
}

template<typename T> inline T atAABB<T>::LongestEdgeLength() const
{
  Vec3 dims = Dimensions();
  return atMax(dims.x, dims.y, dims.z);
}

template<typename T> inline typename atAABB<T>::Vec3 atAABB<T>::ClosestPointBounds(const Vec3 &point) const
{
  Vec3 closestPoint = ClosestPoint(point);
  Vec3 distToMin[2] = { closestPoint - m_min, closestPoint - m_max };
  int64_t smallestIndex = 0;
  T smallest = atLimitsMax<T>();
  for (int64_t i = 0; i < 6; ++i)
  {
    const T &val = distToMin[i / 3][i % 3];
    const T absVal = abs(val);
    if (absVal < smallest)
    {
      smallestIndex = i;
      smallest = absVal;
    }
  }

  closestPoint[smallestIndex % 3] -= distToMin[smallestIndex / 3][smallestIndex % 3];

  return closestPoint;
}

template<typename T> inline typename atAABB<T>::Vec3 atAABB<T>::GetClosestFaceNormal(const Vec3 &point) const
{
  atVector3<T> testVec = (point - Center()) / Dimensions();
  if (abs(testVec.x) < abs(testVec.y) || abs(testVec.x) < abs(testVec.z)) testVec.x = 0;
  if (abs(testVec.y) < abs(testVec.x) || abs(testVec.y) < abs(testVec.z)) testVec.y = 0;
  if (abs(testVec.z) < abs(testVec.x) || abs(testVec.z) < abs(testVec.y)) testVec.z = 0;
  return testVec.Normalize();
}

template<typename T> inline typename atAABB<T>::Vec3 atAABB<T>::ClosestPoint(const Vec3 &point) const { return point.Clamp(m_min, m_max); }
template<typename T> inline atAABB<T> atAABB<T>::OverlappingBox(const atAABB<T> &box) const { return atAABB<T>(m_min.Max(box.m_min), m_max.Min(box.m_max)); }
template<typename T> bool atAABB<T>::operator==(const atAABB<T> &rhs) const { return m_min == rhs.m_min && m_max == rhs.m_max; }
template<typename T> bool atAABB<T>::operator!=(const atAABB<T> &rhs) const { return !(*this == rhs); }
template<typename T> typename atAABB<T>::Vec3 atAABB<T>::Center() const { return (m_min + m_max) / (T)2; }
template<typename T> typename atAABB<T>::Vec3 atAABB<T>::Dimensions() const { return m_max - m_min; }
template<typename T> template<typename Type> void atAABB<T>::GrowToContain(const Type &type) { GrowToContain(atBounds(type)); }
template<typename T> atAABB<T>::atAABB() { m_min = atLimitsMax<T>(); m_max = -m_min; }
template<typename T> atAABB<T>::atAABB(const Vec3 &min, const Vec3 &max) : m_min(min), m_max(max) {}
template<typename T> atAABB<T>::atAABB(atVector<Vec3> &points) { GrowToContain(points); }
template<typename T> void atAABB<T>::GrowToContain(const atVector<Vec3> &points) { for (const Vec3 p : points) GrowToContain(points); }
