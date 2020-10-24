
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

#ifndef _atVector2_h__
#define _atVector2_h__


template<typename T> class atVector2
{
public:
  enum { ElementCount = 2 };

  atVector2();
  atVector2(T val);
  atVector2(T _x, T _y);
  atVector2(const atVector2<T> &copy);
  atVector2(atVector2<T> &&move);

  template<typename T2> explicit atVector2(T2 val);
  template<typename T2> atVector2(T2 _x, T2 _y);
  template<typename T2> atVector2(const atVector2<T2> &copy);

  static atVector2<T> zero();
  static atVector2<T> one();
  const atVector2<T>& assign(T x, T y);
  const atVector2<T>& assign(const atVector2<T> &rhs);
  template<typename T2> const atVector2<T>& assign(T2 x, T2 y);
  template<typename T2> const atVector2<T>& assign(const atVector2<T2> &rhs);

  T* data();
  const T* data() const;
  T& operator[](int64_t index);
  const T& operator[](int64_t index) const;

  atVector2<T> operator-() const;
  atVector2<T> operator+(const atVector2<T> &rhs) const;
  atVector2<T> operator-(const atVector2<T> &rhs) const;
  atVector2<T> operator*(const atVector2<T> &rhs) const;
  atVector2<T> operator/(const atVector2<T> &rhs) const;
  atVector2<T> operator%(const atVector2<T> &rhs) const;
  const atVector2<T>& operator+=(const atVector2<T> &rhs);
  const atVector2<T>& operator-=(const atVector2<T> &rhs);
  const atVector2<T>& operator*=(const atVector2<T> &rhs);
  const atVector2<T>& operator/=(const atVector2<T> &rhs);
  const atVector2<T>& operator%=(const atVector2<T> &rhs);
  const atVector2<T>& operator=(const atVector2<T> &rhs);
  bool operator==(const atVector2<T> &rhs) const;
  bool operator!=(const atVector2<T> &rhs) const;

  template<typename T2> atVector2<T> operator+(const atVector2<T2> &rhs) const;
  template<typename T2> atVector2<T> operator-(const atVector2<T2> &rhs) const;
  template<typename T2> atVector2<T> operator*(const atVector2<T2> &rhs) const;
  template<typename T2> atVector2<T> operator/(const atVector2<T2> &rhs) const;
  template<typename T2> atVector2<T> operator%(const atVector2<T2> &rhs) const;
  template<typename T2> const atVector2<T>& operator+=(const atVector2<T2> &rhs);
  template<typename T2> const atVector2<T>& operator-=(const atVector2<T2> &rhs);
  template<typename T2> const atVector2<T>& operator*=(const atVector2<T2> &rhs);
  template<typename T2> const atVector2<T>& operator/=(const atVector2<T2> &rhs);
  template<typename T2> const atVector2<T>& operator%=(const atVector2<T2> &rhs);
  template<typename T2> const atVector2<T>& operator=(const atVector2<T2> &rhs);

  template<typename T2> atVector2<T> operator+(const T2 &rhs) const;
  template<typename T2> atVector2<T> operator-(const T2 &rhs) const;
  template<typename T2> atVector2<T> operator*(const T2 &rhs) const;
  template<typename T2> atVector2<T> operator/(const T2 &rhs) const;
  template<typename T2> atVector2<T> operator%(const T2 &rhs) const;
  template<typename T2> const atVector2<T>& operator+=(const T2 &rhs);
  template<typename T2> const atVector2<T>& operator-=(const T2 &rhs);
  template<typename T2> const atVector2<T>& operator*=(const T2 &rhs);
  template<typename T2> const atVector2<T>& operator/=(const T2 &rhs);
  template<typename T2> const atVector2<T>& operator%=(const T2 &rhs);
  template<typename T2> const atVector2<T>& operator=(const T2 &rhs);

  atVector2<T> Add(const atVector2<T> &rhs);
  atVector2<T> Sub(const atVector2<T> &rhs);
  atVector2<T> Mul(const atVector2<T> &rhs);
  atVector2<T> Div(const atVector2<T> &rhs);

  T Mag() const;
  T Length() const;
  T Dot(const atVector2<T> &rhs) const;
  T Angle(const atVector2<T> &rhs) const;

  atVector2<T> Normalize() const;
  atVector2<T> Reflect(const atVector2<T> &norm) const;
  atVector2<T> Project(const atVector2<T> &to) const;

  // Returns a vector with where each component is the max of this and the input vectors components.
  atVector2<T> Max(const atVector2<T> &b) const;

  // Returns a vector with where each component is the min of this and the input vectors components.
  atVector2<T> Min(const atVector2<T> &b) const;

  // Returns a vector with where each component is clamped to the min/max of this and the input vectors components
  atVector2<T> Clamp(const atVector2<T> &min, const atVector2<T> &max) const;

  static T Mag(const atVector2<T> &rhs);
  static T Length(const atVector2<T> &rhs);
  static T Dot(const atVector2<T> &lhs, const atVector2<T> &rhs);
  static T Angle(const atVector2<T> &lhs, const atVector2<T> &rhs);

  static atVector2<T> Project(const atVector2<T> &vec, const atVector2<T> &to);
  static atVector2<T> Reflect(const atVector2<T> &dir, const atVector2<T> &norm);
  static atVector2<T> Normalize(const atVector2<T> &rhs);

  // Returns a vector with where each component is the max of the input vectors components.
  static atVector2<T> Max(const atVector2<T> &a, const atVector2<T> &b);

  // Returns a vector with where each component is the min of the input vectors components.
  static atVector2<T> Min(const atVector2<T> &a, const atVector2<T> &b);

  // Returns a vector with where each component is clamped to the min/max of the input vectors components
  static atVector2<T> Clamp(const atVector2<T> &vec, const atVector2<T> &min, const atVector2<T> &max);

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  T x;
  T y;
};

#include "atVector2.inl"
#endif
