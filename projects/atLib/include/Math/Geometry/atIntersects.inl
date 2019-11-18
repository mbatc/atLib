
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

template<typename T, typename T2> bool atIntersects(const atRay<T2> &ray, const atRay<T> &ray2, T2 *pTime)
{
  atVector3<T> point;
  if (!ray2.ClosestPoint(ray, &point, pTime))
    return false;
  return ray.IsOnRay(point);
}

template<typename T, typename T2> bool atIntersects(const atRay<T2> &ray, const atBVHNode<T> &bvhNode, T2 *pTime)
{
  bool res = false;
  T2 minDist = 0xFFFFFFFF;
  
  if (!atIntersects(ray, bvhNode.bounds))
    return false;

  for (const atBVHNode<T> &node : bvhNode.children)
  {
    T2 dist = 0xFFFFFFFF;
    bool hit = node.isLeaf ? atIntersects(ray, node.primitive, &dist) : atIntersects(ray, node, &dist);

    if (hit && dist < 0)
      printf("%f", minDist);

    if (hit && dist < minDist && dist >= 0)
    {
      minDist = dist;
      res = true;
    }
  }
  if (pTime) 
    *pTime = minDist;
  return res;
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

template<typename T> inline bool atIntersects(const atSphere<T> &sphere, const atSphere<T> &sphere2, atVector3<T> *pPoint)
{
  atVector3<T> aToB = sphere2.m_position - sphere.m_position;
  T distToB = aToB.Length();
  T maxDist = atSquare(sphere.m_radius + sphere2.m_radius);
  bool hit = distToB < maxDist;
  if (hit && pPoint)
  {
    distToB = atSqrt(distToB);
    maxDist = sphere.m_radius + sphere2.m_radius;
    T overlapDist = (maxDist - distToB) / 2;
    *pPoint = atLerp(sphere.m_position, sphere2.m_position, (sphere.m_radius + overlapDist) / maxDist);
  }

  return hit;
}

template<typename T> bool atIntersects(const atRect<T> &rect, const atRect<T> &rect2)
{
  if (rect.m_max.x < rect2.m_min.x) return false;
  if (rect.m_max.y < rect2.m_min.y) return false;
  if (rect.m_min.x > rect2.m_max.x) return false;
  if (rect.m_min.y > rect2.m_max.y) return false;
  return true;
}

template<typename T> inline bool atIntersects(const atAABB<T> &box, const atSphere<T> &sphere)
{
  return sphere.Contains(box.ClosestPoint(sphere.m_position));
}

template<typename T> inline bool atIntersects(const atOBB<T> &obb, const atSphere<T> &sphere)
{
  return atIntersects(obb.m_aabb, sphere);
}

template<typename T> inline bool atIntersects(const atAABB<T> &aabb, const atOBB<T> &obb)
{
  return atIntersects(atOBB<T>(aabb.m_min, aabb.m_max), obb);
}

template<typename T> inline bool atIntersects(const atOBB<T> &obb, const atOBB<T> &obb2)
{
  atVector3<T> obbCenter = obb.Center();
  atVector3<T> obb2Center = obb2.Center();

  { // Uses separating axis theorem to test the boxes are intersecting
    atOBB<T> obb2To1 = obb2;
    atQuaternion<T> invRot = obb.m_orientation.Inverse();
    atVector3<T> toCenter = obb2Center - obbCenter;
    toCenter = invRot.Rotate(toCenter) - toCenter;
    obb2To1.m_orientation *= invRot;
    obb2To1.m_aabb.m_max += toCenter;
    obb2To1.m_aabb.m_min += toCenter;
    if (!atIntersects(obb.m_aabb, atBounds(obb2To1)))
      return false;
  }

  {
    atOBB<T> obb1To2 = obb;
    atQuaternion<T> invRot = obb2.m_orientation.Inverse();
    atVector3<T> toCenter = obbCenter - obb2Center;
    toCenter = invRot.Rotate(toCenter) - toCenter;
    obb1To2.m_orientation *= invRot;
    obb1To2.m_aabb.m_max += toCenter;
    obb1To2.m_aabb.m_min += toCenter;
    if (!atIntersects(obb2.m_aabb, atBounds(obb1To2)))
      return false;
  }

  return true;
}

template<typename T> bool atIntersects(const atRay<T> &ray, const atAABB<T> &box, T *pTime)
{
  atVector3<T> invDir = (T)1 / ray.m_dir;
  atVector3<T> tMin3 = (box.m_min - ray.m_pos) * invDir;
  atVector3<T> tMax3 = (box.m_max - ray.m_pos) * invDir;
  T tmin = atMax(atMax(atMin(tMin3.x, tMax3.x), atMin(tMin3.y, tMax3.y)), atMin(tMin3.z, tMax3.z));
  T tmax = atMin(atMin(atMax(tMin3.x, tMax3.x), atMax(tMin3.y, tMax3.y)), atMax(tMin3.z, tMax3.z));
  bool intersects = !(tmax < 0 || tmin > tmax);
  if (pTime) 
    *pTime = intersects ? tmin : tmax;
  return intersects;
}

template<typename T> inline bool atIntersects(const atRay<T> &ray, const atOBB<T> &obb, T *pTime)
{
  atVector3<T> rotatedPos = obb.WorldToOBB(ray.m_pos);
  atVector3<T> rotatedDir = obb.m_orientation.Inverse().Rotate(ray.m_dir);
  atRay<T> rotatedRay(rotatedPos, rotatedDir);
  return atIntersects(rotatedRay, obb.m_aabb, pTime);
}

template<typename T> bool atIntersects(const atRay<T> &ray, const atTriangle<T> &tri, T *pTime)
{
  T time = 0;
  
  if (!atIntersects(ray, atPlane<T>(tri.m_a, tri.m_b, tri.m_c), &time))
    return false;

  atVector3<T> point = ray.At(time);
  atVector3<T> weights = tri.BarycentricCoords(point);
  if (pTime)
    *pTime = time;

  return weights.x + weights.y + weight.z <= 1.0 && time >= 0;
}

template<typename T> bool atIntersects(const atRay<T> &ray, const atPlane<T> &plane, T *pTime) 
{
  const atVector3<T> &rayDir = ray.m_dir;
  const atVector4<T> &coeffs = plane.m_coeffs;
  const atVector3<T> &rayPos = ray.m_pos;

  if (coeffs.xyz().Dot(rayDir) == 0) return false;
  if (pTime) *pTime = (coeffs.w - rayPos.x * coeffs.x - rayPos.y * coeffs.y - rayPos.z * coeffs.z) / (coeffs.x * rayDir.x + coeffs.y * rayDir.y + coeffs.z * rayDir.z);
  return !pTime || *pTime >= 0;
}

template<typename T> inline bool atIntersects(const atRay<T> &ray, const atSphere<T> &sphere, T *pTime)
{
  atVector3<T> point;
  T time = 0;
  T dist = 0;
  ray.ClosestPoint(sphere.m_position, &point, &time, &dist);
  if (dist < sphere.m_radius)
  {
    if (pTime)
      *pTime = time;
    return true;
  }

  return false;
}

template<typename T> bool atIntersects(const atTriangle<T> &tri, const atTriangle<T> &tri2, atVector3<T> *pPoint)
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

template<typename T> bool atIntersects(const atRay<T> &ray, const atRay<T> &ray2, T *pTime) { return atIntersects<T, T>(ray, ray2, pTime); }
template<typename T, typename T2> bool atIntersects(const atRay<T2> &ray, const atBVH<T> &bvh, T2 *pTime) { return atIntersects(ray, bvh.m_root, pTime); }
