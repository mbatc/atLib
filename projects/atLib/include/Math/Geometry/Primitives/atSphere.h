
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

#ifndef atSphere_h__
#define atSphere_h__

#include "atMath.h"

template <typename T> class atSphere
{
public:
  atSphere(const T &_radius, const atVector3<T> &_position = { 0 });

  T Volume() const;
  T Diameter() const;
  T SurfaceArea() const;

  void GrowToContain(const atVector3<T> &point);
  bool Contains(const atVector3<T> &point) const;
  bool Distance(const atVector3<T> &point) const;
  bool Distance2(const atVector3<T> &point) const;

  atVector3<T> ClosestPoint(const atVector3<T> &point) const;
  atVector3<T> ClosestPointBounds(const atVector3<T> &point) const;

  double m_radius;
  atVector3<T> m_position;
};

#include "atSphere.inl"
#endif // atSphere_h__
