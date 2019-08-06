#include "atTriangle.h"

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

template<typename T> T atTriangle<T>::Area() const
{
  const atVector3<T> a = m_a - m_b;
  const atVector3<T> b = m_a - m_c;
  return 0.5 * a.Cross(b).Mag();
}

template<typename T> atTriangle<T>::atTriangle() : atTriangle(atVector3<T>::zero(), atVector3<T>::zero(), atVector3<T>::zero()) {}
template<typename T> atTriangle<T>::atTriangle(const atVector3<T>& a, const atVector3<T>& b, const atVector3<T>& c) : m_a(a), m_b(b), m_c(c) {}
template<typename T> atVector3<T> atTriangle<T>::Center() { return (m_a + m_b + m_c) / (T)3; }

template<typename T> atAABB<T> atBounds(const atTriangle<T> &tri)
{
  atAABB<T> ret;
  ret.GrowToContain(tri.m_a);
  ret.GrowToContain(tri.m_b);
  ret.GrowToContain(tri.m_c);
  return ret;
}
