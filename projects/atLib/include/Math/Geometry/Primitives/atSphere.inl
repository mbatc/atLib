#include "atSphere.h"

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

template<typename T> inline T atSphere<T>::Diameter() const { return m_radius * 2; }

template<typename T> inline atSphere<T>::atSphere(const T &_radius, const atVector3<T> &_position)
  : m_position(_position)
  , m_radius(_radius)
{}

template<typename T> inline T atSphere<T>::Volume() const
{
  return T((4.0 / 3.0) * atPi * m_radius * m_radius * m_radius);
}

template<typename T> inline T atSphere<T>::SurfaceArea() const
{
  return T(4 * atPi * m_radius * m_radius);
}

template<typename T> inline void atSphere<T>::GrowToContain(const atVector3<T> &point)
{
  m_radius = atMax(m_radius, (point - m_pos).Mag());
}

template<typename T> inline bool atSphere<T>::Contains(const atVector3<T> &point) const
{
  return (m_pos - point).Length() < m_radius * m_radius;
}

template<typename T> inline bool atSphere<T>::Distance(const atVector3<T> &point) const
{
  return atSqrt(Distance2(point));
}

template<typename T> inline bool atSphere<T>::Distance2(const atVector3<T> &point) const
{
  return (m_pos - point).Length();
}

template<typename T> inline atVector3<T> atSphere<T>::ClosestPoint(const atVector3<T> &point)
{
  atVector3<T> fromCenter = point - m_pos;
  T dist = fromCenter.Mag();
  return atMin(dist, m_radius) * fromCenter.Normalize();
}

template<typename T> inline atVector3<T> atSphere<T>::ClosestPointBounds(const atVector3<T> &point)
{
  return (point - m_pos).Normalize() * m_radius;
}
