
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

#ifndef atTriangle_h__
#define atTriangle_h__

#include "atPlane.h"
#include "atAABB.h"

template<typename T> class atTriangle
{
public:
  atTriangle();
  atTriangle(const atVector3<T> &a, const atVector3<T> &b, const atVector3<T> &c);

  // Returns the area of the triangle
  T Area() const;

  // Returns the point at the center of the triangle
  atVector3<T> Center() const;

  // Returns the normalized cross product of the edges ab and ac
  atVector3<T> Normal() const;

  // Returns the cross product of the edges ab and ac
  atVector3<T> Normal2() const;
  
  atVector3<T> BarycentricCoords(const atVector3<T> &point) const;

  atVector3<T> m_a;
  atVector3<T> m_b;
  atVector3<T> m_c;
};

template<typename T> atAABB<T> atBounds(const atTriangle<T> &tri);

typedef atTriangle<int32_t> atTriangleI;
typedef atTriangle<int64_t> atTriangleI64;
typedef atTriangle<float> atTriangleF;
typedef atTriangle<double> atTriangleD;

#include "atTriangle.inl"
#endif // atTriangle_h__
