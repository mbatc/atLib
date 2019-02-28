
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

#ifndef atMatrix_h__
#define atMatrix_h__

#include <minmax.h>
#include "atAssert.h"
#include "atTypes.h"

#include "atVector4.h"

template <typename T> class atMatrix4x4
{
public:
  atMatrix4x4(T _m[16]);
  atMatrix4x4(
    T _00, T _01, T _02, T _03,
    T _10, T _11, T _12, T _13,
    T _20, T _21, T _22, T _23,
    T _30, T _31, T _32, T _33);

  atMatrix4x4(atMatrix4x4<T> &&move);
  atMatrix4x4(const atMatrix4x4<T> &copy = Identity());
  template <typename T2> atMatrix4x4(atMatrix4x4<T2> copy);

  static atMatrix4x4<T> Identity();
  atMatrix4x4<T> Transpose() const;
  atMatrix4x4<T> Cofactors() const;
  atMatrix4x4<T> Inverse() const;

  T Determinate() const;

  template <typename T2> atMatrix4x4<T> Mul(const T2 &rhs) const;
  template <typename T2> atMatrix4x4<T> Mul(const atMatrix4x4<T2> &rhs) const;
  template <typename T2> atMatrix4x4<T> operator*(const atMatrix4x4<T2> &rhs) const;

  template <typename T2> atMatrix4x4<T> Add(const atMatrix4x4<T2> &rhs) const;
  template <typename T2> atMatrix4x4<T> Sub(const atMatrix4x4<T2> &rhs) const;

  atMatrix4x4<T> Mul(const T &rhs) const;
  atMatrix4x4<T> Sub(const T &rhs) const;
  atMatrix4x4<T> Add(const T &rhs) const;
  atMatrix4x4<T> Mul(const atMatrix4x4<T> &rhs) const;
  atVector4<T> Mul(const atVector4<T> &rhs) const;
  atVector3<T> Mul(const atVector3<T> &rhs) const;

  atVector4<T> operator*(const atVector4<T> &rhs) const;
  atVector3<T> operator*(const atVector3<T> &rhs) const;
  atMatrix4x4<T> operator*(const T &rhs) const;
  atMatrix4x4<T> operator*(const atMatrix4x4<T> &rhs) const;

  bool operator==(const atMatrix4x4<T> &rhs) const;
  bool operator!=(const atMatrix4x4<T> &rhs) const;
  const atMatrix4x4<T> &operator=(const atMatrix4x4<T> &copy);
  template <typename T2> const atMatrix4x4<T>& operator=(const atMatrix4x4<T2> &copy);

  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  union
  {
    T m[16];
    atVector4<T> r[4];
  };
};

#include "atMatrix.inl"
#endif // atMatrix_h__
