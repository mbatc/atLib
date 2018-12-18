
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
  template <typename T> using VecType = atVector3<T>;
  static const int64_t element_count = 3;

  atVector3() : x(0), y(0), z(0) {}
  atVector3(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) {}
  atVector3(const VecType<T> &copy) : x(copy.x), y(copy.y), z(copy.z) {}

  atVector3(VecType<T> &&move)
  {
    memcpy(data(), move.data(), sizeof(T) * 3);
    memset(move.data(), 0, sizeof(T) * 3);
  }

  // Approaching Template Town

  template <typename T2> atVector3(const std::initializer_list<T2> &list)
  {
    for (int64_t i = 0; i < element_count && i < (int64_t)list.size(); ++i)
      data()[i] = (T)*(list.begin() + i);
  }

  template <typename T2> atVector3(const T2 _x, const T2 _y, const T2 _z) : x((T)_x), y((T)_y), z((T)_z) {}
  template <typename T2> atVector3(const T2 val) : x((T)val), y((T)val), z((T)val) {}
  template <typename T2> atVector3(const VecType<T2> &copy) { assign(copy); }
  template <typename T2> atVector3(const atVector2<T2> &vec2, const T2 _z) { assign(vec2.x, vec2.y, _z); }
  template <typename T2> atVector3(const T2 _x, const atVector2<T2> &vec2) { assign(_x, vec2.x, vec2.y); }

  static VecType<T> zero() { return VecType<T>(0, 0, 0); }
  static VecType<T> one() { return VecType<T>(1, 1, 1); }

  template <typename T2> void assign(const T2 _x, const T2 _y, const T2 _z) { x = (T)_x; y = (T)_y; z = (T)_z; }
  template <typename T2> void assign(const VecType<T2> &rhs) { atVectorMath::Assign(this, rhs); }
  void assign(const T _x, const T _y, const T _z) { assign<T>(_x, _y, _z); }
  void assign(const VecType<T> &rhs) { assign<T>(rhs); }

  T& operator[](const int64_t index) { return data()[index]; }
  const T& operator[](const int64_t index) const { return data()[index]; }

  VecType<T> operator-() const { return atVectorMath::Subtract(VecType<T>::zero(), *this); }
  VecType<T> operator+(const VecType<T> &rhs) const { return atVectorMath::Add(*this, rhs); }
  VecType<T> operator-(const VecType<T> &rhs) const { return atVectorMath::Subtract(*this, rhs); }
  VecType<T> operator*(const VecType<T> &rhs) const { return atVectorMath::Multiply(*this, rhs); }
  VecType<T> operator/(const VecType<T> &rhs) const { return atVectorMath::Divide(*this, rhs); }
  VecType<T> operator+=(const VecType<T> &rhs) { *this = *this + rhs; return *this; }
  VecType<T> operator-=(const VecType<T> &rhs) { *this = *this - rhs; return *this; }
  VecType<T> operator*=(const VecType<T> &rhs) { *this = *this * rhs; return *this; }
  VecType<T> operator/=(const VecType<T> &rhs) { *this = *this / rhs; return *this; }
  VecType<T> operator=(const VecType<T> &rhs) { return atVectorMath::Assign(this, rhs); }
  bool operator==(const VecType<T> &rhs) const { return atVectorMath::Equals(*this, rhs); }
  bool operator!=(const VecType<T> &rhs) const { return atVectorMath::NotEqual(*this, rhs); }

  template <typename T2> VecType<T> operator+(const VecType<T2> &rhs) const { return atVectorMath::Add(*this, rhs); }
  template <typename T2> VecType<T> operator-(const VecType<T2> &rhs) const { return atVectorMath::Subtract(*this, rhs); }
  template <typename T2> VecType<T> operator*(const VecType<T2> &rhs) const { return atVectorMath::Multiply(*this, rhs); }
  template <typename T2> VecType<T> operator/(const VecType<T2> &rhs) const { return atVectorMath::Divide(*this, rhs); }
  template <typename T2> VecType<T> operator+=(const VecType<T2> &rhs) { *this = *this + rhs; return *this; }
  template <typename T2> VecType<T> operator-=(const VecType<T2> &rhs) { *this = *this - rhs; return *this; }
  template <typename T2> VecType<T> operator*=(const VecType<T2> &rhs) { *this = *this * rhs; return *this; }
  template <typename T2> VecType<T> operator/=(const VecType<T2> &rhs) { *this = *this / rhs; return *this; }
  template <typename T2> VecType<T> operator=(const VecType<T2> &rhs) { return atVectorMath::Assign(this, rhs); }
  template <typename T2> bool operator==(const VecType<T2> &rhs) const { return atVectorMath::Equals(*this, rhs); }
  template <typename T2> bool operator!=(const VecType<T2> &rhs) const { return atVectorMath::NotEqual(*this, rhs); }

  template <typename T2> VecType<T> operator+(const T2 &rhs) const { return atVectorMath::Add(*this, rhs); }
  template <typename T2> VecType<T> operator-(const T2 &rhs) const { return atVectorMath::Subtract(*this, rhs); }
  template <typename T2> VecType<T> operator*(const T2 &rhs) const { return atVectorMath::Multiply(*this, rhs); }
  template <typename T2> VecType<T> operator/(const T2 &rhs) const { return atVectorMath::Divide(*this, rhs); }
  template <typename T2> VecType<T> operator+=(const T2 &rhs) { *this = *this + rhs; return *this; }
  template <typename T2> VecType<T> operator-=(const T2 &rhs) { *this = *this - rhs; return *this; }
  template <typename T2> VecType<T> operator*=(const T2 &rhs) { *this = *this * rhs; return *this; }
  template <typename T2> VecType<T> operator/=(const T2 &rhs) { *this = *this / rhs; return *this; }
  template <typename T2> VecType<T> operator=(const T2 &rhs) { return atVectorMath::Assign(this, rhs); }
  template <typename T2> bool operator==(const T2 &rhs) const { return atVectorMath::Equals(*this, rhs); }
  template <typename T2> bool operator!=(const T2 &rhs) const { return atVectorMath::NotEqual(*this, rhs); }

  operator atMatrix<T, 3, 1>() const { return atMatrix<T, 3, 1>({ x, y, z }); }

  atVector2<T> xy() const { return atVector2<T>(x, y); }
  atVector2<T> xz() const { return atVector2<T>(x, z); }
  atVector2<T> xw() const { return atVector2<T>(x, w); }
  atVector2<T> yz() const { return atVector2<T>(y, z); }
  atVector2<T> yw() const { return atVector2<T>(y, w); }
  atVector2<T> zw() const { return atVector2<T>(z, w); }

  T Mag() const { return Mag(*this); }
  T Length() const { return Length(*this); }
  T Dot(const VecType<T> &rhs) const { return Dot(*this, rhs); }
  T Angle(const VecType<T> &rhs) const { return Angle(*this, rhs); }

  VecType<T> Normalize() const { return Normalize(*this); }
  VecType<T> Reflect(const VecType<T> &norm) { return Reflect(*this, norm); }
  VecType<T> Cross(const VecType<T> &rhs) const { return atVectorMath::Cross(*this, rhs); }

  static T Mag(const VecType<T> &rhs) { return atVectorMath::Mag(rhs); }
  static T Length(const VecType<T> &rhs) { return atVectorMath::Length(rhs); }
  static T Dot(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Dot(lhs, rhs); }
  static T Angle(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Angle(lhs, rhs); }

  static VecType<T> Normalize(const VecType<T> &rhs) { return atVectorMath::Normalize(rhs); }
  static VecType<T> Cross(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Cross(lhs, rhs); }
  static VecType<T> Reflect(const VecType<T> &dir, const VecType<T> &norm) { return atVectorMath::Reflect(dir, norm); }

  T* begin() { return data(); }
  T* end() { return data() + element_count; }
  const T* begin() const { return data(); }
  const T* end() const { return data() + element_count; }

  T* data() { return &x; }
  const T* data() const { return &x; }

  T x;
  T y;
  T z;
};

#endif // atVector3_h__
