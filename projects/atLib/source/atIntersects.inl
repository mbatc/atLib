#include "atIntersects.h"

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

template<typename T, typename T2> bool atIntersects(const atRay<T> &a, const atRay<T2> &b, T *pTime)
{
  atVector3<T> point;
  if (!b.GetClosestPoint(a, &point, pTime))
    return false;
  return a.IsOnRay(point);
}

template<typename T> bool atIntersects(const atAABB<T> &boxA, const atAABB<T> &boxB)
{
  if (boxA.m_max.x < boxB.m_min.x) return false;
  if (boxA.m_max.y < boxB.m_min.y) return false;
  if (boxA.m_max.z < boxB.m_min.z) return false;
  if (boxA.m_min.x > boxB.m_max.x) return false;
  if (boxA.m_min.y > boxB.m_max.y) return false;
  if (boxA.m_min.z > boxB.m_max.z) return false;
  return true;
}

template <typename T> bool atIntersects(const atRay<T> &ray, const atAABB<T> &box, T* pTime)
{
  atVector3<T> invDir = 1.0f / ray.m_dir;
  Vec3 tMin3 = (box.m_min - ray.m_pos.x) * invDir;
  Vec3 tMax3 = (box.m_max - ray.m_pos.x) * invDir;
  float tmin = atMax(atMax(atMin(tMin3.x, tMax3.x), atMin(tMin3.y, tMax3.y)), atMin(tMin3.z, tMax3.z));
  float tmax = atMin(atMin(atMax(tMin3.x, tMax3.x), atMax(tMin3.y, tMax3.y)), atMax(tMin3.z, tMax3.z));
  bool intersects = !(tmax < 0 || tmin > tmax);
  if (pTime) 
    *pTime = intersects ? tmin : tmax;
  return intersects;
}

template <typename T> bool atIntersects(const atRay<T> &ray, const atTriangle<T> &tri, T *pTime)
{
  T time = 0;
  if (!atIntersects(ray, atPlane<T>(tri.a, tri.m_b, tri.c), &time))
    return false;
  atVector3<T> point = ray.At(time);
  T area = tri.Area();
  T testArea = atTriangle<T>(point, tri.m_a, tri.m_b).Area() + atTriangle<T>(point, tri.m_b, tri.m_c).Area() + atTriangle<T>(point, tri.m_a, tri.m_c).Area();
  return testArea <= area;
}

template <typename T> bool atIntersects(const atRay<T> &ray, const atPlane<T> &plane, T *pTime) 
{
  const atVector3<T> &rayDir = ray.m_dir;
  const atVector4<T> &plane = plane.m_coeffs;
  const atVector3<T> &rayPos = ray.m_pos();

  if (plane.m_coeffs.xyz().Dot(rayDir) == 0) return false;
  if (pTime) *pTime = (plane.w - rayPos.x * plane.x - rayPos.y * plane.y - rayPos.z * plane.z) / (plane.x * rayDir.x + plane.y * rayDir.y + plane.z * rayDir.z);
  return true;
}

template <typename T> bool atIntersects(const atTriangle<T> &tri, const atTriangle<T> &tri2, atVector3<T> *pPoint)
{ 
  T time = 0;
  bool result = false;
  if (atIntersects(atRay<T>(tri.m_a, tri.m_b - tri.m_a), tri2, &time))
  {
    if (pPoint) 
      *pPoint = atRay<T>(tri.m_a, tri.m_b - tri.m_a).At(time);
  }
  else if (atIntersects(atRay<T>(tri.m_a, tri.m_c - tri.m_a), tri2, &time))
  {
    if (pPoint)
      *pPoint = atRay<T>(tri.m_a, tri.m_c - tri.m_a).At(time);
  }
  else if (atIntersects(atRay<T>(tri.m_b, tri.m_c - tri.m_b), tri2, &time))
  {
    if (pPoint)
      *pPoint = atRay<T>(tri.m_b, tri.m_c - tri.m_b).At(time);
  }
  else
    return false;
  return true;
}

template <typename T> bool atIntersects(const atRay<T> &a, const atRay<T> &b, T *pTime) { return atIntersects<T, T>(a, b, pTime); }
