#include "atVector2.h"

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

template<typename T> inline atVector2<T>::atVector2()
  : x(0)
  , y(0)
{}

template<typename T> inline atVector2<T>::atVector2(T val)
  : x(val)
  , y(val)
{}

template<typename T> inline atVector2<T>::atVector2(T _x, T _y)
  : x(_x)
  , y(_y)
{}

template<typename T> inline atVector2<T>::atVector2(const atVector2<T>& copy)
  : x(copy.x)
  , y(copy.y)
{}

template<typename T> inline atVector2<T>::atVector2(atVector2<T>&& move)
  : x(move.x)
  , y(move.y)
{
  move = zero();
}

template<typename T> template<typename T2> inline atVector2<T>::atVector2(T2 val)
  : x((T)val)
  , y((T)val)
{}

template<typename T> template<typename T2> inline atVector2<T>::atVector2(T2 _x, T2 _y)
  : x((T)_x)
  , y((T)_y)
{}

template<typename T> template<typename T2> inline atVector2<T>::atVector2(atVector2<T2> copy)
  : x((T)copy.x)
  , y((T)copy.y)
{}

template<typename T> inline atVector2<T> atVector2<T>::zero()
{
  return atVector2<T>(0);
}

template<typename T> inline atVector2<T> atVector2<T>::one()
{
  return atVector2<T>(1);
}

template<typename T> inline const atVector2<T>& atVector2<T>::assign(T x, T y)
{
  this->x = x;
  this->y = y;
  return *this;
}

template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::assign(T2 x, T2 y)
{
  this->x = (T)x;
  this->y = (T)y;
  return *this;
}

template<typename T> inline const atVector2<T>& atVector2<T>::assign(const atVector2<T>& rhs)
{
  x = rhs.x;
  y = rhs.y;
  return *this;
}

template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::assign(const atVector2<T2>& rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  return *this;
}

template<typename T> inline atVector2<T> atVector2<T>::Normalize() const { return Normalize(*this); }

template<typename T> inline T atVector2<T>::Length(const atVector2<T> &rhs)
{
  return rhs.x * rhs.x + rhs.y * rhs.y;
}

template<typename T> inline T atVector2<T>::Dot(const atVector2<T> &lhs, const atVector2<T> &rhs)
{
  return  lhs.x * rhs.x + 
    lhs.y * rhs.y + 
    lhs.z * rhs.z;
}

template<typename T> inline T atVector2<T>::Angle(const atVector2<T> &lhs, const atVector2<T> &rhs)
{
  return atACos(Dot(lhs, rhs) / (lhs.Mag() * rhs.Mag()));
}

template<typename T> inline atVector2<T> atVector2<T>::Project(const atVector2<T>& vec, const atVector2<T>& to)
{
  return vec * to.Normalize();
}

template<typename T> inline atVector2<T> atVector2<T>::Reflect(const atVector2<T> &dir, const atVector2<T> &norm)
{
  return dir - norm * Dot(dir, norm) * 2;
}

template<typename T> inline atVector2<T> atVector2<T>::Max(const atVector2<T> &a, const atVector2<T> &b)
{
  return atVector3<T>(atMax(a.x, b.x), atMax(a.y, b.y), atMax(a.z, b.z));
}

template<typename T> inline atVector2<T> atVector2<T>::Min(const atVector2<T> &a, const atVector2<T> &b)
{
  return atVector3<T>(atMin(a.x, b.x), atMin(a.y, b.y), atMin(a.z, b.z));
}

template<typename T> inline atVector2<T> atVector2<T>::Clamp(const atVector2<T> &vec, const atVector2<T> &min, const atVector2<T> &max)
{
  return Max(min, Min(vec, max));
}

template<typename T> inline atVector2<T> atVector2<T>::Add(const atVector2<T>& rhs) { return *this + rhs; }
template<typename T> inline atVector2<T> atVector2<T>::Sub(const atVector2<T>& rhs) { return *this - rhs; }
template<typename T> inline atVector2<T> atVector2<T>::Mul(const atVector2<T>& rhs) { return *this * rhs; }
template<typename T> inline atVector2<T> atVector2<T>::Div(const atVector2<T>& rhs) { return *this / rhs; }

template<typename T> inline T atVector2<T>::Mag() const { return Mag(*this); }
template<typename T> inline T atVector2<T>::Length() const { return Length(*this); }
template<typename T> inline T atVector2<T>::Dot(const atVector2<T>& rhs) const { return Dot(*this, rhs); }
template<typename T> inline T atVector2<T>::Angle(const atVector2<T> &rhs) const { return Angle(*this, rhs); }
template<typename T> inline atVector2<T> atVector2<T>::Reflect(const atVector2<T>& norm) const { return Reflect(*this, norm); }
template<typename T> inline atVector2<T> atVector2<T>::Project(const atVector2<T>& to) const { return Project(*this, to); }
template<typename T> inline atVector2<T> atVector2<T>::Max(const atVector2<T> &b) const { return Max(*this, b); }
template<typename T> inline atVector2<T> atVector2<T>::Min(const atVector2<T> &b) const { return Min(*this, b); }
template<typename T> inline atVector2<T> atVector2<T>::Clamp(const atVector2<T> &min, const atVector2<T> &max) const { return Clamp(*this, min, max); }
template<typename T> inline T atVector2<T>::Mag(const atVector2<T>& rhs) { return atSqrt(Length(rhs)); }
template<typename T> inline atVector2<T> atVector2<T>::Normalize(const atVector2<T> &rhs) { return rhs / rhs.Mag(); }
template<typename T> inline const T* atVector2<T>::end() const { return data() + 2; }
template<typename T> inline const T* atVector2<T>::begin() const { return data(); }
template<typename T> inline T* atVector2<T>::end() { return data() + 2; }
template<typename T> inline T* atVector2<T>::begin() { return data(); }
template<typename T> inline T * atVector2<T>::data() { return &x; }
template<typename T> inline const T* atVector2<T>::data() const { return &x; }

template<typename T> inline T& atVector2<T>::operator[](int64_t index) { return data()[index]; }
template<typename T> inline const T& atVector2<T>::operator[](int64_t index) const { return data()[index]; }
template<typename T> inline atVector2<T> atVector2<T>::operator-() const { return atVector2<T>(-x, -y); }
template<typename T> inline atVector2<T> atVector2<T>::operator+(const atVector2<T>& rhs) const { return atVector2<T>(x + rhs.x, y + rhs.y); }
template<typename T> inline atVector2<T> atVector2<T>::operator-(const atVector2<T>& rhs) const { return atVector2<T>(x - rhs.x, y - rhs.y); }
template<typename T> inline atVector2<T> atVector2<T>::operator*(const atVector2<T>& rhs) const { return atVector2<T>(x * rhs.x, y * rhs.y); }
template<typename T> inline atVector2<T> atVector2<T>::operator/(const atVector2<T>& rhs) const { return atVector2<T>(x / rhs.x, y / rhs.y); }
template<typename T> inline atVector2<T> atVector2<T>::operator%(const atVector2<T>& rhs) const { return atVector2<T>(atMod(x, rhs.x), atMod(y, rhs.y)); }
template<typename T> inline const atVector2<T>& atVector2<T>::operator+=(const atVector2<T>& rhs) { return assign(*this + rhs); }
template<typename T> inline const atVector2<T>& atVector2<T>::operator-=(const atVector2<T>& rhs) { return assign(*this - rhs); }
template<typename T> inline const atVector2<T>& atVector2<T>::operator*=(const atVector2<T>& rhs) { return assign(*this * rhs); }
template<typename T> inline const atVector2<T>& atVector2<T>::operator/=(const atVector2<T>& rhs) { return assign(*this / rhs); }
template<typename T> inline const atVector2<T>& atVector2<T>::operator%=(const atVector2<T>& rhs) { return assign(*this % rhs); }
template<typename T> inline const atVector2<T>& atVector2<T>::operator=(const atVector2<T>& rhs) { return assign(rhs); }
template<typename T> inline bool atVector2<T>::operator==(const atVector2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
template<typename T> inline bool atVector2<T>::operator!=(const atVector2<T>& rhs) const { return !(*this == rhs); }

template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator+(const atVector2<T2>& rhs) const { return atVector2<T>(x + (T)rhs.x, y + (T)rhs.y); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator-(const atVector2<T2>& rhs) const { return atVector2<T>(x - (T)rhs.x, y - (T)rhs.y); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator*(const atVector2<T2>& rhs) const { return atVector2<T>(x * (T)rhs.x, y * (T)rhs.y); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator/(const atVector2<T2>& rhs) const { return atVector2<T>(x / (T)rhs.x, y / (T)rhs.y); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator%(const atVector2<T2>& rhs) const { return atVector2<T>(atMod(x, (T)rhs.x), atMod(y, (T)rhs.y)); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator+=(const atVector2<T2>& rhs) { return assign(*this + rhs); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator-=(const atVector2<T2>& rhs) { return assign(*this - rhs); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator*=(const atVector2<T2>& rhs) { return assign(*this * rhs); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator/=(const atVector2<T2>& rhs) { return assign(*this / rhs); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator%=(const atVector2<T2>& rhs) { return assign(*this % rhs); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator=(const atVector2<T2>& rhs) { return assign(rhs); }

template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator+(const T2 &rhs) const { return atVector2<T>(x + (T)rhs, y + (T)rhs); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator-(const T2 &rhs) const { return atVector2<T>(x - (T)rhs, y - (T)rhs); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator*(const T2 &rhs) const { return atVector2<T>(x * (T)rhs, y * (T)rhs); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator/(const T2 &rhs) const { return atVector2<T>(x / (T)rhs, y / (T)rhs); }
template<typename T> template<typename T2> inline atVector2<T> atVector2<T>::operator%(const T2 &rhs) const { return atVector2<T>(atMod(x, (T)rhs), atMod(y, (T)rhs)); }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator+=(const T2 &rhs) { *this = *this + rhs; return *this; }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator-=(const T2 &rhs) { *this = *this - rhs; return *this; }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator*=(const T2 &rhs) { *this = *this * rhs; return *this; }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator/=(const T2 &rhs) { *this = *this / rhs; return *this; }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator%=(const T2 &rhs) { *this = *this % rhs; return *this; }
template<typename T> template<typename T2> inline const atVector2<T>& atVector2<T>::operator=(const T2 &rhs) { return assign((T)rhs, (T)rhs); }