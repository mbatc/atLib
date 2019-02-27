
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

#ifndef _atVector2_h__
#define _atVector2_h__

#include "atVectorMath.h"
#include "atMatrix.h"

template <typename T> class atVector2
{
public:
  template <typename ValType> using VecType = atVector2<ValType>;
  static const int64_t element_count = 2;

  atVector2() : x(0), y(0) {}
  atVector2(const T _x, const T _y) : x(_x), y(_y) {}
  atVector2(const VecType<T> &copy) : x(copy.x), y (copy.y) {}

  atVector2(VecType<T> &&move)
  {
    memcpy(data(), move.data(), sizeof(T) * 2);
    memset(move.data(), 0, sizeof(T) * 2);
  }

  template <typename T2> atVector2(const T2 val) : x((T)val), y((T)val) {}
  template <typename T2> atVector2(const T2 _x, const T2 _y) : x((T)_x), y((T)_y) {}
  template <typename T2> atVector2(const VecType<T2> &copy) : x((T)copy.x), y((T)copy.y) {}
  template <typename T2> atVector2(const std::initializer_list<T2> &list)
  {
    for (int64_t i = 0; i < element_count && i < (int64_t)list.size(); ++i)
      data()[i] = (T)*(list.begin() + i);
  }

  static VecType<T> zero() { return VecType<T>(0, 0); }
  static VecType<T> one() { return VecType<T>(1, 1); }

  template <typename T2> void assign(const T2 _x, const T2 _y) { x = (T)_x; y = (T)_y; }
  template <typename T2> void assign(const VecType<T2> &rhs) { atVectorMath::Assign(this, rhs); }
  void assign(const T _x, const T _y) { assign<T>(_x, _y); }
  void assign(const VecType<T> &rhs) { assign<T>(rhs); }

  T& operator[](const int64_t index) { return data()[index]; }
  const T& operator[](const int64_t index) const { return data()[index]; }

  VecType<T> operator-() const { return atVectorMath::Subtract(VecType<T>::zero(), *this); }
  VecType<T> operator+(const VecType<T> &rhs) const { return atVectorMath::Add(*this, rhs); }
  VecType<T> operator-(const VecType<T> &rhs) const { return atVectorMath::Subtract(*this, rhs); }
  VecType<T> operator*(const VecType<T> &rhs) const { return atVectorMath::Multiply(*this, rhs); }
  VecType<T> operator/(const VecType<T> &rhs) const { return atVectorMath::Divide(*this, rhs); }
  VecType<T> operator%(const VecType<T> &rhs) const { return{ atMod(x, rhs.x), atMod(y, rhs.y) }; }
  VecType<T> operator+=(const VecType<T> &rhs) { *this = *this + rhs; return *this; }
  VecType<T> operator-=(const VecType<T> &rhs) { *this = *this - rhs; return *this; }
  VecType<T> operator*=(const VecType<T> &rhs) { *this = *this * rhs; return *this; }
  VecType<T> operator/=(const VecType<T> &rhs) { *this = *this / rhs; return *this; }
  VecType<T> operator%=(const VecType<T> &rhs) { *this = *this % rhs; return *this; }
  VecType<T> operator=(const VecType<T> &rhs) { return atVectorMath::Assign(this, rhs); }
  bool operator==(const VecType<T> &rhs) const { return atVectorMath::Equals(*this, rhs); }
  bool operator!=(const VecType<T> &rhs) const { return atVectorMath::NotEqual(*this, rhs); }

  template <typename T2> VecType<T> operator+(const VecType<T2> &rhs) const { return atVectorMath::Add(*this, rhs); }
  template <typename T2> VecType<T> operator-(const VecType<T2> &rhs) const { return atVectorMath::Subtract(*this, rhs); }
  template <typename T2> VecType<T> operator*(const VecType<T2> &rhs) const { return atVectorMath::Multiply(*this, rhs); }
  template <typename T2> VecType<T> operator/(const VecType<T2> &rhs) const { return atVectorMath::Divide(*this, rhs); }
  template <typename T2> VecType<T> operator%(const VecType<T2> &rhs) const { return{ atMod(x, (T)rhs.x), atMod(y, (T)rhs.y) }; }
  template <typename T2> VecType<T> operator+=(const VecType<T2> &rhs) { *this = *this + rhs; return *this; }
  template <typename T2> VecType<T> operator-=(const VecType<T2> &rhs) { *this = *this - rhs; return *this; }
  template <typename T2> VecType<T> operator*=(const VecType<T2> &rhs) { *this = *this * rhs; return *this; }
  template <typename T2> VecType<T> operator/=(const VecType<T2> &rhs) { *this = *this / rhs; return *this; }
  template <typename T2> VecType<T> operator%=(const VecType<T2> &rhs) { *this = *this % rhs; return *this; }
  template <typename T2> VecType<T> operator=(const VecType<T2> &rhs) { return atVectorMath::Assign(this, rhs); }
  template <typename T2> bool operator==(const VecType<T2> &rhs) const { return atVectorMath::Equals(*this, rhs); }
  template <typename T2> bool operator!=(const VecType<T2> &rhs) const { return atVectorMath::NotEqual(*this, rhs); }
  
  template <typename T2> VecType<T> operator+(const T2 &rhs) const { return atVectorMath::Add(*this, rhs); }
  template <typename T2> VecType<T> operator-(const T2 &rhs) const { return atVectorMath::Subtract(*this, rhs); }
  template <typename T2> VecType<T> operator*(const T2 &rhs) const { return atVectorMath::Multiply(*this, rhs); }
  template <typename T2> VecType<T> operator/(const T2 &rhs) const { return atVectorMath::Divide(*this, rhs); }
  template <typename T2> VecType<T> operator%(const T2 &rhs) const { return{ atMod(x, (T)rhs), atMod(y, (T)rhs) }; }
  template <typename T2> VecType<T> operator+=(const T2 &rhs) { *this = *this + rhs; return *this; }
  template <typename T2> VecType<T> operator-=(const T2 &rhs) { *this = *this - rhs; return *this; }
  template <typename T2> VecType<T> operator*=(const T2 &rhs) { *this = *this * rhs; return *this; }
  template <typename T2> VecType<T> operator/=(const T2 &rhs) { *this = *this / rhs; return *this; }
  template <typename T2> VecType<T> operator%=(const T2 &rhs) { *this = *this % rhs; return *this; }
  template <typename T2> VecType<T> operator=(const T2 &rhs) { return atVectorMath::Assign(this, rhs); }
  template <typename T2> bool operator==(const T2 &rhs) const { return atVectorMath::Equals(*this, rhs); }
  template <typename T2> bool operator!=(const T2 &rhs) const { return atVectorMath::NotEqual(*this, rhs); }

  operator atMatrix<T, 2, 1>() const { return atMatrix<T, 2, 1>({ x, y }); }

  VecType<T> Add(const VecType<T> &rhs) { return *this = atVectorMath::Add(*this, rhs); }
  VecType<T> Sub(const VecType<T> &rhs) { return *this = atVectorMath::Subtract(*this, rhs); }
  VecType<T> Mul(const VecType<T> &rhs) { return *this = atVectorMath::Multiply(*this, rhs); }
  VecType<T> Div(const VecType<T> &rhs) { return *this = atVectorMath::Divide(*this, rhs); }

  T Mag() const { return Mag(*this); }
  T Length() const { return Length(*this); }
  T Dot(const VecType<T> &rhs) const { return Dot(*this, rhs); }
  T Angle(const VecType<T> &rhs) const { return Angle(*this, rhs); }

  VecType<T> Normalize() const { return Normalize(*this); }
  VecType<T> Reflect(const VecType<T> &norm) { return Reflect(*this, norm); }

  static T Mag(const VecType<T> &rhs) { return atVectorMath::Mag(rhs); }
  static T Length(const VecType<T> &rhs) { return atVectorMath::Length(rhs); }
  static T Dot(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Dot(lhs, rhs); }
  static T Angle(const VecType<T> &lhs, const VecType<T> &rhs) { return atVectorMath::Angle(lhs, rhs); }

  static VecType<T> Reflect(const VecType<T> &dir, const VecType<T> &norm) { return atVectorMath::Reflect(dir, norm); }
  static VecType<T> Normalize(const VecType<T> &rhs) { return atVectorMath::Normalize(rhs); }
  
  T* begin() { return data(); }
  T* end() { return data() + element_count; }
  const T* begin() const { return data(); }
  const T* end() const { return data() + element_count; }

  T* data() { return &x; }
  const T* data() const { return &x; }
  
  T x;
  T y;
};

#endif
