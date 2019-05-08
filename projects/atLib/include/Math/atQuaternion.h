
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

#ifndef atQuaternion_h__
#define atQuaternion_h__

#include "atMatrix.h"

template<typename T> class atQuaternion
{
public:
  atQuaternion();
  atQuaternion(const T &x, const T &y, const T &z, const T &w);
  atQuaternion(const atVector3<T> &axis, const T &angle);
  atQuaternion(const T &eulerX, const T &eulerY, const T &eulerZ);
  explicit atQuaternion(const atMatrix4x4<T> &rotation);

  void Set(const T &x, const T &y, const T &z, const T &w);
  void Set(const atVector3<T> &axis, const T &angle);
  void Set(const T &eulerX, const T &eulerY, const T &eulerZ);
  void Set(const atMatrix4x4<T> &rotation);

  static atQuaternion<T> zero();
  static atQuaternion<T> Identity();

  static atQuaternion<T> Normalize(const atQuaternion<T> &quat);
  static atQuaternion<T> Inverse(const atQuaternion<T> &quat);
  static atQuaternion<T> Mul(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);
  static atQuaternion<T> Mul(const atQuaternion<T> &lhs, const T &rhs);
  static atQuaternion<T> Add(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);
  static atQuaternion<T> Sub(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);
  static atQuaternion<T> Div(const atQuaternion<T> &lhs, const T &rhs);

  static T AngleTo(const atQuaternion<T> &from, const atQuaternion<T> &to);
  static T Angle(const atQuaternion<T> &quat);
  static T Length(const atQuaternion<T> &quat);
  static T Mag(const atQuaternion<T> &quat);
  static T Dot(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);

  static atVector3<T> Rotate(const atQuaternion<T> &quat, const atVector3<T> &vec);
  static atVector3<T> Axis(const atQuaternion<T> &quat);
  static atVector3<T> EulerAngles(const atQuaternion<T> &quat);

  atQuaternion<T> Slerp(const atQuaternion &to, const T &factor) const;
  atQuaternion<T> Normalize() const;
  atQuaternion<T> Inverse() const;
  atQuaternion<T> Mul(const atQuaternion<T> &rhs) const;
  atQuaternion<T> Mul(const T &rhs) const;
  atQuaternion<T> Add(const atQuaternion<T> &rhs) const;
  atQuaternion<T> Sub(const atQuaternion<T> &rhs) const;
  atQuaternion<T> Div(const T &rhs) const;

  T AngleTo(const atQuaternion<T> &to) const;
  T Angle() const;
  T Length() const;
  T Mag() const;
  T Dot(const atQuaternion<T> &rhs) const;

  atVector3<T> EulerAngles() const;
  atVector3<T> Axis() const;
  atVector3<T> Rotate(const atVector3<T> &vec) const;

  bool operator==(const atQuaternion<T> &rhs) const;
  bool operator!=(const atQuaternion<T> &rhs) const;

  atQuaternion<T> operator*(const atQuaternion<T> &rhs) const;
  atQuaternion<T> operator-(const atQuaternion<T> &rhs) const;
  atQuaternion<T> operator+(const atQuaternion<T> &rhs) const;
  atQuaternion<T> operator*(const T &rhs) const;
  atQuaternion<T> operator/(const T &rhs) const;
  atQuaternion<T> operator-() const;

  const atQuaternion<T>& operator=(const atQuaternion<T> &rhs);
  const atQuaternion<T>& operator*=(const atQuaternion<T> &rhs);
  const atQuaternion<T>& operator*=(const T &rhs);
  
  template<typename T2> const atQuaternion<T>& operator=(const atQuaternion<T2> &rhs);
  template<typename T2> const atQuaternion<T>& operator*=(const atQuaternion<T2> &rhs);
  template<typename T2> const atQuaternion<T>& operator*=(const T2 &rhs);

  template<typename T2> atQuaternion<T> operator*(const atQuaternion<T2> &rhs) const;
  template<typename T2> atQuaternion<T> operator-(const atQuaternion<T2> &rhs) const;
  template<typename T2> atQuaternion<T> operator+(const atQuaternion<T2> &rhs) const;
  template<typename T2> atQuaternion<T> operator*(const T2 &rhs) const;
  template<typename T2> atQuaternion<T> operator/(const T2 &rhs) const;

  const T& at(int64_t index) const;
  const T& operator[](int64_t index) const;
  T& at(int64_t index);
  T& operator[](int64_t index);

  const T* begin() const;
  const T* end() const;
  T* begin();
  T* end();

  T x;
  T y;
  T z;
  T w;
};

template<typename T> atQuaternion<T> operator*(const atVector3<T> &lhs, const atQuaternion<T> &rhs);
template<typename T> atQuaternion<T> operator*(const atQuaternion<T> &lhs, const atVector3<T> &rhs);

#include "atQuaternion.inl"
#endif // atQuaternion_h__
