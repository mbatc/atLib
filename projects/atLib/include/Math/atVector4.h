
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

#ifndef atVector4_h__
#define atVector4_h__

#include "atVector3.h"

template<typename T> class atVector4
{
public:
  atVector4();
  atVector4(T val);
  atVector4(T _x, T _y, T _z, T _w);
  atVector4(const atVector3<T> &xyz, T _w);
  atVector4(T _x, const atVector3<T> &yzw);
  atVector4(T _x, T _y, const atVector2<T> &zw);
  atVector4(T _x, const atVector2<T> &yz, T _w);
  atVector4(const atVector2<T> &xy, T _z, T _w);
  atVector4(const atVector2<T> &xy, const atVector2<T> &yw);

  template<typename T2> atVector4(T2 val);
  template<typename T2> atVector4(T2 _x, T2 _y, T2 _z, T2 _w);
  template<typename T2> atVector4(const atVector3<T2> &xyz, T2 _w);
  template<typename T2> atVector4(T2 _x, const atVector3<T2> &yzw);
  template<typename T2> atVector4(T2 _x, T2 _y, const atVector2<T2> &zw);
  template<typename T2> atVector4(T2 _x, const atVector2<T2> &yz, T2 _w);
  template<typename T2> atVector4(const atVector2<T2> &xy, T2 _z, T2 _w);
  template<typename T2> atVector4(const atVector2<T2> &xy, const atVector2<T2> &yw);
  template<typename T2> atVector4(atVector4<T2> copy);

  atVector4(const atVector4<T> &copy);
  atVector4(atVector4<T> &&move);

  static atVector4<T> zero();
  static atVector4<T> one();

  template<typename T2> const atVector4<T>& assign(T2 _x, T2 _y, T2 _z, T2 _w);
  template<typename T2> const atVector4<T>& assign(const atVector4<T2> &rhs);
  const atVector4<T>& assign(T x, T y, T z, T w);
  const atVector4<T>& assign(const atVector4<T> &rhs);

  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  atVector4<T> operator-() const;
  atVector4<T> operator+(const atVector4<T> &rhs) const;
  atVector4<T> operator-(const atVector4<T> &rhs) const;
  atVector4<T> operator*(const atVector4<T> &rhs) const;
  atVector4<T> operator/(const atVector4<T> &rhs) const;
  atVector4<T> operator%(const atVector4<T> &rhs) const;
  const atVector4<T>& operator+=(const atVector4<T> &rhs);
  const atVector4<T>& operator-=(const atVector4<T> &rhs);
  const atVector4<T>& operator*=(const atVector4<T> &rhs);
  const atVector4<T>& operator/=(const atVector4<T> &rhs);
  const atVector4<T>& operator%=(const atVector4<T> &rhs);
  const atVector4<T>& operator=(const atVector4<T> &rhs);
  bool operator==(const atVector4<T> &rhs) const;
  bool operator!=(const atVector4<T> &rhs) const;

  template<typename T2> atVector4<T> operator+(const atVector4<T2> &rhs) const;
  template<typename T2> atVector4<T> operator-(const atVector4<T2> &rhs) const;
  template<typename T2> atVector4<T> operator*(const atVector4<T2> &rhs) const;
  template<typename T2> atVector4<T> operator/(const atVector4<T2> &rhs) const;
  template<typename T2> atVector4<T> operator%(const atVector4<T2> &rhs) const;
  template<typename T2> const atVector4<T>& operator+=(const atVector4<T2> &rhs);
  template<typename T2> const atVector4<T>& operator-=(const atVector4<T2> &rhs);
  template<typename T2> const atVector4<T>& operator*=(const atVector4<T2> &rhs);
  template<typename T2> const atVector4<T>& operator/=(const atVector4<T2> &rhs);
  template<typename T2> const atVector4<T>& operator%=(const atVector4<T2> &rhs);
  template<typename T2> const atVector4<T>& operator=(const atVector4<T2> &rhs);

  template<typename T2> atVector4<T> operator+(const T2 &rhs) const;
  template<typename T2> atVector4<T> operator-(const T2 &rhs) const;
  template<typename T2> atVector4<T> operator*(const T2 &rhs) const;
  template<typename T2> atVector4<T> operator/(const T2 &rhs) const;
  template<typename T2> atVector4<T> operator%(const T2 &rhs) const;
  template<typename T2> const atVector4<T>& operator+=(const T2 &rhs);
  template<typename T2> const atVector4<T>& operator-=(const T2 &rhs);
  template<typename T2> const atVector4<T>& operator*=(const T2 &rhs);
  template<typename T2> const atVector4<T>& operator/=(const T2 &rhs);
  template<typename T2> const atVector4<T>& operator%=(const T2 &rhs);
  template<typename T2> const atVector4<T>& operator=(const T2 &rhs);

  atVector2<T> xy() const;
  atVector2<T> xz() const;
  atVector2<T> xw() const;
  atVector2<T> yz() const;
  atVector2<T> yw() const;
  atVector2<T> zw() const;
  atVector3<T> xyz() const;
  atVector3<T> xzw() const;
  atVector3<T> xyw() const;
  atVector3<T> yzw() const;

  atVector4<T> Add(const atVector4<T> &rhs) const;
  atVector4<T> Sub(const atVector4<T> &rhs) const;
  atVector4<T> Mul(const atVector4<T> &rhs) const;
  atVector4<T> Div(const atVector4<T> &rhs) const;

  T Mag() const;
  T Length() const;
  T Dot(const atVector4<T> &rhs) const;
  T Angle(const atVector4<T> &rhs) const;

  atVector4<T> Normalize() const;
  atVector4<T> Reflect(const atVector4<T> &norm) const;
  atVector4<T> Project(const atVector4<T> &to) const;

  static T Mag(const atVector4<T> &rhs);
  static T Length(const atVector4<T> &rhs);
  static T Dot(const atVector4<T> &lhs, const atVector4<T> &rhs);
  static T Angle(const atVector4<T> &lhs, const atVector4<T> &rhs);

  static atVector4<T> Reflect(const atVector4<T> &dir, const atVector4<T> &norm);
  static atVector4<T> Normalize(const atVector4<T> &rhs);
  static atVector4<T> Project(const atVector4<T> &vec, const atVector4<T> &to);

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  T* data();
  const T* data() const;

  T x;
  T y;
  T z;
  T w;
};

#include "atVector4.inl"
#endif // atVector4_h__
