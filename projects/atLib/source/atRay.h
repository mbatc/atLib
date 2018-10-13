
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

#ifndef atRay_h__
#define atRay_h__

#include "atVector3.h"

template <typename T> class atRay
{
public:
  using Vec3 = atVector3<T>;
  
  atRay(const Vec3 &pos, const Vec3 &dir);

  Vec3 At(const T time) const;
  Vec3 GetTime(const Vec3 &point) const;
  bool IsOnRay(const Vec3 &point, T *pTime) const;
  bool GetClosestPoint(const atRay<T> &ray, Vec3 *pPoint, T *pTime = nullptr) const;
  bool GetClosestPoint(const atRay<T> &ray, Vec3 *pPoint, T* pTime, T* pDist) const;
  bool GetClosestPoint(const Vec3 &point, Vec3 *pPoint, T *pTime = nullptr, T* pDist = nullptr) const;

  T TimeX(const T val) const;
  T TimeY(const T val) const;
  T TimeZ(const T val) const;

  Vec3 m_pos;
  Vec3 m_dir;
};

#include "atRay.inl"
#endif // atRay_h__
