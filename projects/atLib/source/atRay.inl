
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

template <typename T> bool atRay<T>::IsOnRay(const Vec3 &point, T * pTime)
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

template <typename T> bool atRay<T>::GetClosestPoint(const atRay<T> &ray, Vec3 *pPoint, T *pTime)
{
  atAssert(pPoint != nullptr, "pPoint must not be nullptr");
  T time = { 0 };
  // Calculate required Dot Products
  T dA_dB_2 = m_dir.Dot(ray.m_dir);
  if (dA_dB_2 == 0)
    return false;

  dA_dB_2 *= dA_dB_2;

  const T dA_dA = m_dir.Dot(m_dir);
  const T dB_dB = ray.m_dir.Dot(ray.m_dir);
  const T dB_B = ray.m_dir.Dot(ray.m_pos);
  const T dA_A = m_dir.Dot(m_pos);
  const T dA_B = m_dir.Dot(ray.m_pos);

  // Calculate major parts of equation
  const T coeff = (1 + (dA_dA * dB_dB) / dA_dB_2);
  const T partA = (1 - dB_B);
  const T partB = (dB_dB * (dA_A - dA_B) / dA_dB_2);

  // Calculate Time
  time = coeff * (partA - partB);
  if (pTime)
    *pTime = time;
  *pPoint = At(time);
  return true;
}

template <typename T> T atRay<T>::TimeX(const T val) { return (val - m_pos.x) / m_dir.x; }
template <typename T> T atRay<T>::TimeY(const T val) { return (val - m_pos.y) / m_dir.y; }
template <typename T> T atRay<T>::TimeZ(const T val) { return (val - m_pos.z) / m_dir.z; }
template <typename T> atRay<T>::atRay(const Vec3 &pos, const Vec3 &dir) : m_pos(pos), m_dir(dir) {}
template <typename T> typename atRay<T>::Vec3 atRay<T>::At(const T time) { return m_pos + m_dir * time; }
template <typename T> typename atRay<T>::Vec3 atRay<T>::GetTime(const Vec3 &point) { return GetTime(m_dir.x != 0 ? TimeX(point.x) : 0, m_dir.y != 0 ? TimeY(point.y) : 0, m_dir.z != 0 ? TimeZ(point.z) : 0); }
