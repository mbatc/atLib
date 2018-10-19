
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

template <typename T> class atVector4
{
public:
  template <typename T> using VecType = atVector4<T>;

  static const int64_t element_count = 4;

  atVector4(const T _x = { 0 }, const T _y = { 0 }, const T _z = { 0 }, const T _w = { 0 }) : x(_x), y(_y), z(_z), w(_w) {}
  atVector4(const VecType<T> &copy) : x(copy.x), y(copy.y), z(copy.z), w(copy.w) {}

  atVector4(VecType<T> &&move)
  {
    memcpy(data(), move.data(), sizeof(T) * 4);
    memset(move.data(), 0, sizeof(T) * 4);
  }

  // Approaching Template Town

  template <typename T2> atVector4(const std::initializer_list<T2> &list)
  {
    for (int64_t i = 0; i < element_count && i < (int64_t)list.size(); ++i)
      data()[i] = (T)*(list.begin() + i);
  }

  template <typename T2> atVector4(const T2 _x = { 0 }, const T2 _y = { 0 }, const T2 _z = { 0 }, const T2 _w = { 0 }) : x((T)_x), y((T)_y), z((T)_z), w((T)_w) {}
  template <typename T2> atVector4(const VecType<T2> &copy) { assign(copy); } 
  template <typename T2> atVector4(const atVector2<T2> &vec2, const T2 _z, const T2 _w) { assign(vec2.x, vec2.y, _z, _w); }
  template <typename T2> atVector4(const T2 _x, const atVector2<T2> &vec2, const T2 _w) { assign(_x, vec2.x, vec2.y, _w); }
  template <typename T2> atVector4(const T2 _x, const T2 _y, const atVector2<T2> &vec2) { assign(_x, _y, vec2.x, vec2.y); }
  template <typename T2> atVector4(const atVector3<T2> &vec3, const T2 _w) { assign(vec3.x, vec3.y, vec3.z, _w); }
  template <typename T2> atVector4(const T2 _x, const atVector3<T2> &vec3) { assign(_x, vec3.x, vec3.y, vec3.z); }

  static VecType<T> zero() { return VecType<T>(0, 0, 0, 0); }
  static VecType<T> one() { return VecType<T>(1, 1, 1, 1); }

  template <typename T2> void assign(const T2 _x, const T2 _y, const T2 _z, const T2 _w) { x = (T)_x; y = (T)_y; z = (T)_z; w = (T)_w; }
  template <typename T2> void assign(const VecType<T2> &rhs) { atVectorMath::Assign(this, rhs); }
  void assign(const T x, const T y) { assign<T>(x, y, z, w); }
  void assign(const VecType<T> &rhs) { assign<T>(rhs); }

  T& operator[](const int64_t index) { return data()[index]; }
  const T& operator[](const int64_t index) const { return data()[index]; }

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

  operator atMatrix<T, 4, 1>() const { return atMatrix<T, 4, 1>( { x ,y ,z ,w } ); }

  atVector2<T> xy() const { return atVector2<T>(x, y); }
  atVector2<T> xz() const { return atVector2<T>(x, z); }
  atVector2<T> xw() const { return atVector2<T>(x, w); }
  atVector2<T> yz() const { return atVector2<T>(y, z); }
  atVector2<T> yw() const { return atVector2<T>(y, w); }
  atVector2<T> zw() const { return atVector2<T>(z, w); }
  atVector3<T> xyz() const { return atVector3<T>(x, y, z); }
  atVector3<T> xzw() const { return atVector3<T>(x, z, w); }
  atVector3<T> xyw() const { return atVector3<T>(x, y, w); }
  atVector3<T> yzw() const { return atVector3<T>(y, z, w); }

  T Mag() const { return Mag(*this); }
  T Length() const { return Length(*this); }
  T Dot(const VecType<T> &rhs) const { return Dot(*this, rhs); }
  T Angle(const VecType<T> &rhs) const { return Angle(*this, rhs); }

  VecType<T> Normalize() const { return Normalize(*this); }

  static T Mag(const VecType<T> &rhs) { return atVectorMath::Mag(rhs); }
  static T Length(const VecType<T> &rhs) { return atVectorMath::Length(rhs); }
  static T Dot(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Dot(lhs, rhs); }
  static T Angle(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Angle(lhs, rhs); }

  static VecType<T> Normalize(const VecType<T> &rhs) { return atVectorMath::Normalize(rhs); }

  T* begin() { return data(); }
  T* end() { return data() + element_count; }
  const T* begin() const { return data(); }
  const T* end() const { return data() + element_count; }

  T* data() { return &x; }
  const T* data() const { return &x; }

  T x;
  T y;
  T z;
  T w;
};

#endif // atVector4_h__
