
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

#ifndef atVector3_h__
#define atVector3_h__

#include "atVector2.h"

template <typename T> class atVector3
{
public:
  atVector3();
  atVector3(T val);
  atVector3(T _x, T _y, T _z);
  atVector3(T _x, const atVector2<T> &yz);
  atVector3(const atVector2<T> &xy, T _z);
  atVector3(const atVector3<T> &copy);
  atVector3(atVector3<T> &&move);
                                    
  template <typename T2> atVector3(T2 val);
  template <typename T2> atVector3(T2 _x, T2 _y, T2 _z);
  template <typename T2> atVector3(T2 _x, const atVector2<T2> &yz);
  template <typename T2> atVector3(const atVector2<T2> &xy, T2 _z);
  template <typename T2> atVector3(atVector3<T2> copy);

  static atVector3<T> zero();
  static atVector3<T> one();

  template <typename T2> const atVector3<T>& assign(T2 _x, T2 _y, T2 _z);
  template <typename T2> const atVector3<T>& assign(const atVector3<T2> &rhs);
  const atVector3<T>& assign(T _x, T _y, T _z);
  const atVector3<T>& assign(const atVector3<T> &rhs);

  T& operator[](int64_t index);
  const T& operator[](int64_t index) const;

  atVector3<T> operator-() const;
  atVector3<T> operator+(const atVector3<T> &rhs) const;
  atVector3<T> operator-(const atVector3<T> &rhs) const;
  atVector3<T> operator*(const atVector3<T> &rhs) const;
  atVector3<T> operator/(const atVector3<T> &rhs) const;
  atVector3<T> operator%(const atVector3<T> &rhs) const;
  const atVector3<T>& operator+=(const atVector3<T> &rhs);
  const atVector3<T>& operator-=(const atVector3<T> &rhs);
  const atVector3<T>& operator*=(const atVector3<T> &rhs);
  const atVector3<T>& operator/=(const atVector3<T> &rhs);
  const atVector3<T>& operator%=(const atVector3<T> &rhs);
  const atVector3<T>& operator=(const atVector3<T> &rhs);
  bool operator==(const atVector3<T> &rhs) const;
  bool operator!=(const atVector3<T> &rhs) const;

  template <typename T2> atVector3<T> operator+(const atVector3<T2> &rhs) const;
  template <typename T2> atVector3<T> operator-(const atVector3<T2> &rhs) const;
  template <typename T2> atVector3<T> operator*(const atVector3<T2> &rhs) const;
  template <typename T2> atVector3<T> operator/(const atVector3<T2> &rhs) const;
  template <typename T2> atVector3<T> operator%(const atVector3<T2> &rhs) const;
  template <typename T2> const atVector3<T>& operator+=(const atVector3<T2> &rhs);
  template <typename T2> const atVector3<T>& operator-=(const atVector3<T2> &rhs);
  template <typename T2> const atVector3<T>& operator*=(const atVector3<T2> &rhs);
  template <typename T2> const atVector3<T>& operator/=(const atVector3<T2> &rhs);
  template <typename T2> const atVector3<T>& operator%=(const atVector3<T2> &rhs);
  template <typename T2> const atVector3<T>& operator=(const atVector3<T2> &rhs);
  template <typename T2> bool operator==(const atVector3<T2> &rhs) const;
  template <typename T2> bool operator!=(const atVector3<T2> &rhs) const;

  template <typename T2> atVector3<T> operator+(const T2 &rhs) const;
  template <typename T2> atVector3<T> operator-(const T2 &rhs) const;
  template <typename T2> atVector3<T> operator*(const T2 &rhs) const;
  template <typename T2> atVector3<T> operator/(const T2 &rhs) const;
  template <typename T2> atVector3<T> operator%(const T2 &rhs) const;
  template <typename T2> const atVector3<T>& operator+=(const T2 &rhs);
  template <typename T2> const atVector3<T>& operator-=(const T2 &rhs);
  template <typename T2> const atVector3<T>& operator*=(const T2 &rhs);
  template <typename T2> const atVector3<T>& operator/=(const T2 &rhs);
  template <typename T2> const atVector3<T>& operator%=(const T2 &rhs);
  template <typename T2> const atVector3<T>& operator=(const T2 &rhs);
  template <typename T2> bool operator==(const T2 &rhs) const;
  template <typename T2> bool operator!=(const T2 &rhs) const;
  
  atVector2<T> xy() const;
  atVector2<T> xz() const;
  atVector2<T> yz() const;

  T Mag() const;
  T Length() const;
  T Dot(const atVector3<T> &rhs) const;
  T Angle(const atVector3<T> &rhs) const;

  atVector3<T> Add(const atVector3<T> &rhs);
  atVector3<T> Sub(const atVector3<T> &rhs);
  atVector3<T> Mul(const atVector3<T> &rhs);
  atVector3<T> Div(const atVector3<T> &rhs);

  atVector3<T> Normalize() const;
  atVector3<T> Reflect(const atVector3<T> &norm) const;
  atVector3<T> Cross(const atVector3<T> &rhs) const;

  static T Mag(const atVector3<T> &rhs);
  static T Length(const atVector3<T> &rhs);
  static T Dot(const atVector3<T> &lhs, const atVector3<T> &rhs);
  static T Angle(const atVector3<T> &lhs, const atVector3<T> &rhs);

  static atVector3<T> Normalize(const atVector3<T> &rhs);
  static atVector3<T> Cross(const atVector3<T> &lhs, const atVector3<T> &rhs);
  static atVector3<T> Reflect(const atVector3<T> &dir, const atVector3<T> &norm);

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  T* data();
  const T* data() const;

  T x;
  T y;
  T z;
};

#include "atVector3.inl"
#endif // atVector3_h__
