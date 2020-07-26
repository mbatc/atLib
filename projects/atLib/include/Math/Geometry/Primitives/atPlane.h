
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

#ifndef atPlane_h__
#define atPlane_h__

#include "atVector4.h"
#include "atRay.h"

template<typename T> class atPlane
{
public:
  atPlane(const atVector4<T> &coeff);
  atPlane(const atVector3<T> &normal, const atVector3<T> &point);
  atPlane(const atVector3<T> &a, const atVector3<T> &b, const atVector3<T> &c);

  atVector3<T> Project(const atVector3<T> &point);

  atVector4<T> m_coeffs;
  atVector4<T> m_point;
};

typedef atPlane<int32_t> atPlaneI;
typedef atPlane<int64_t> atPlaneI64;
typedef atPlane<float> atPlaneF;
typedef atPlane<double> atPlaneD;

#include "atPlane.inl"
#endif // atPlane_h__
