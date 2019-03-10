
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

#include "atRay.h"

template<typename T> bool atRay<T>::IsOnRay(const Vec3 &point, T * pTime) const
{
  T comp = 0;
  // Check which directions have non-zero values
  bool hasX = m_dir.x != 0;
  bool hasY = m_dir.y != 0;
  bool hasZ = m_dir.z != 0;

  // If the ray has 0 direction then it is invalid
  if (!hasX && !hasY && !hasZ) return false;

  // Get time for x, y and z
  T tX = hasX ? TimeX(point.x) : 0;
  T tY = hasY ? TimeY(point.y) : 0;
  T tZ = hasZ ? TimeZ(point.z) : 0;
  if (hasX) comp = tX;
  else if (hasY) comp = tY;
  else if (hasZ) comp = tZ;
  bool res = true;
  // Check if all time values are equal if m_dir is not zero
  // if m_dir is 0 check if the point is the same as m_pos
  res &= hasX ? tX == comp : point.x == m_pos.x;
  res &= hasY ? tY == comp : point.y == m_pos.y;
  res &= hasZ ? tZ == comp : point.z == m_pos.z;

  // Return the time value
  if (pTime && res) *pTime = comp;
  return res;
}

template<typename T> bool atRay<T>::GetClosestPoint(const atRay<T> &ray, Vec3 *pPoint, T *pTime) const
{
  atAssert(pPoint != nullptr, "pPoint must not be nullptr");
  T time = { 0 };
  // Calculate required Dot Products
  T dA_dB = m_dir.Dot(ray.m_dir);
  const T dA_dA = m_dir.Dot(m_dir);
  const T den = dA_dB + dA_dA;
  if (den == 0)
    return false;
  const T dB_B = ray.m_dir.Dot(ray.m_pos);
  const T dB_A = ray.m_dir.Dot(m_pos);
  const T dA_A = m_dir.Dot(m_pos);
  const T dA_B = m_dir.Dot(ray.m_pos);
  const T num = dB_B * dA_dB - dB_A * dA_dB - dA_B - dA_A;

  // Calculate Time
  time = num / den;
  if (pTime)
    *pTime = time;
  *pPoint = At(time);
  return true;
}

template<typename T> bool atRay<T>::GetClosestPoint(const atRay<T>& ray, Vec3 *pPoint, T *pTime, T *pDist) const
{
  if (!GetClosestPoint(ray, pPoint, pTime))
    return false;
  Vec3 p2; 
  ray.GetClosestPoint(*this, &p2);
  *pDist = (p2 - *pPoint).Mag();
  return true;
}

template<typename T> bool atRay<T>::GetClosestPoint(const Vec3 &point, Vec3 *pPoint, T *pTime, T* pDist) const
{
  atAssert(pPoint != nullptr, "pPoint must not be nullptr");
  T time = 0;
  bool res = true;
  if (m_dir != atVector3<T>::zero())
    res = point == m_pos;
  else
    time = m_dir.Dot(point - m_pos) / m_dir.Dot(m_dir);
  if (*pTime)
    *pTime = time;
  *pPoint = At(time);

  if (*pDist)
    *pDist = (*pPoint - point).Mag();
	return res;
}

template<typename T> T atRay<T>::TimeX(const T val) const { return (val - m_pos.x) / m_dir.x; }
template<typename T> T atRay<T>::TimeY(const T val) const { return (val - m_pos.y) / m_dir.y; }
template<typename T> T atRay<T>::TimeZ(const T val) const { return (val - m_pos.z) / m_dir.z; }
template<typename T> atRay<T>::atRay(const Vec3 &pos, const Vec3 &dir) : m_pos(pos), m_dir(dir) {}
template<typename T> typename atRay<T>::Vec3 atRay<T>::At(const T time) const { return m_pos + m_dir * time; }
template<typename T> typename atRay<T>::Vec3 atRay<T>::GetTime(const Vec3 &point) const { return Vec3(m_dir.x != 0 ? TimeX(point.x) : 0, m_dir.y != 0 ? TimeY(point.y) : 0, m_dir.z != 0 ? TimeZ(point.z) : 0); }
