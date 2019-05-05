
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

template<typename T> inline atVector4<T>::atVector4()
  : x(0)
  , y(0)
  , z(0)
  , w(0)
{}

template<typename T> inline atVector4<T>::atVector4(T val)
  : x(val)
  , y(val)
  , z(val)
  , w(val)
{}

template<typename T> inline atVector4<T>::atVector4(T _x, T _y, T _z, T _w)
  : x(_x)
  , y(_y)
  , z(_z)
  , w(_w)
{}

template<typename T> inline atVector4<T>::atVector4(const atVector3<T>& xyz, T _w)
  : x(xyz.x)
  , y(xyz.y)
  , z(xyz.z)
  , w(_w)
{}

template<typename T> inline atVector4<T>::atVector4(T _x, const atVector3<T>& yzw)
  : x(_x)
  , y(yzw.x)
  , z(yzw.y)
  , w(yzw.z)
{}

template<typename T> inline atVector4<T>::atVector4(T _x, T _y, const atVector2<T>& zw)
  : x(_x)
  , y(_y)
  , z(zw.x)
  , w(zw.y)
{}

template<typename T> inline atVector4<T>::atVector4(T _x, const atVector2<T>& yz, T _w)
  : x(_x)
  , y(yz.x)
  , z(yz.y)
  , w(_w)
{}

template<typename T> inline atVector4<T>::atVector4(const atVector2<T>& xy, T _z, T _w)
  : x(xy.x)
  , y(xy.y)
  , z(_z)
  , w(_w)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(T2 val)
  : x((T)val)
  , y((T)val)
  , z((T)val)
  , w((T)val)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(T2 _x, T2 _y, T2 _z, T2 _w)
  : x((T)_x)
  , y((T)_y)
  , z((T)_z)
  , w((T)_w)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(const atVector3<T2>& xyz, T2 _w)
  : x((T)xyz.x)
  , y((T)xyz.y)
  , z((T)xyz.z)
  , w((T)_w)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(T2 _x, const atVector3<T2>& yzw)
  : x((T)_x)
  , y((T)yzw.x)
  , z((T)yzw.y)
  , w((T)yzw.z)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(T2 _x, T2 _y, const atVector2<T2>& zw)
  : x((T)_x)
  , y((T)_y)
  , z((T)zw.x)
  , w((T)zw.y)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(T2 _x, const atVector2<T2>& yz, T2 _w)
  : x((T)_x)
  , y((T)yz.x)
  , z((T)yz.y)
  , w((T)_w)
{}


template<typename T> template<typename T2> inline atVector4<T>::atVector4(const atVector2<T2>& xy, T2 _z, T2 _w)
  : x((T)xy.x)
  , y((T)xy.y)
  , z((T)_z)
  , w((T)_w)
{}

template<typename T> template<typename T2> inline atVector4<T>::atVector4(atVector4<T2> copy)
  : x((T)copy.x)
  , y((T)copy.y)
  , z((T)copy.z)
  , w((T)copy.w)
{}

template<typename T> inline atVector4<T>::atVector4(const atVector2<T>& xy, const atVector2<T>& yw)
  : x(xy.x)
  , y(xy.y)
  , z(yw.x)
  , w(yw.y)
{}

template<typename T> inline atVector4<T>::atVector4(const atVector4<T>& copy)
  : x(copy.x)
  , y(copy.y)
  , z(copy.z)
  , w(copy.w)
{}

template<typename T> inline atVector4<T>::atVector4(atVector4<T>&& move)
  : x(move.x)
  , y(move.y)
  , z(move.z)
  , w(move.w)
{
  move = zero();
}

template<typename T> inline const atVector4<T>& atVector4<T>::assign(T _x, T _y, T _z, T _w)
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
  return *this;
}

template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::assign(T2 _x, T2 _y, T2 _z, T2 _w)
{
  x = (T)_x;
  y = (T)_y;
  z = (T)_z;
  w = (T)_w;
  return *this;
}

template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::assign(const atVector4<T2>& rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  z = (T)rhs.z;
  w = (T)rhs.w;
  return *this;
}

template<typename T> inline const atVector4<T>& atVector4<T>::assign(const atVector4<T>& rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  z = (T)rhs.z;
  w = (T)rhs.w;
  return *this;
}

template<typename T> inline T atVector4<T>::Dot(const atVector4<T> &lhs, const atVector4<T> &rhs)
{
  return lhs.x * rhs.x +
    lhs.y * rhs.y +
    lhs.z * rhs.z +
    lhs.w * rhs.w;
}

template<typename T> inline atVector2<T> atVector4<T>::xy() const { return atVector2<T>(x, y); }
template<typename T> inline atVector2<T> atVector4<T>::xz() const { return atVector2<T>(x, z); }
template<typename T> inline atVector2<T> atVector4<T>::xw() const { return atVector2<T>(x, w); }
template<typename T> inline atVector2<T> atVector4<T>::yz() const { return atVector2<T>(y, z); }
template<typename T> inline atVector2<T> atVector4<T>::yw() const { return atVector2<T>(y, w); }
template<typename T> inline atVector2<T> atVector4<T>::zw() const { return atVector2<T>(z, w); }
template<typename T> inline atVector3<T> atVector4<T>::xyz() const { return atVector3<T>(x, y, z); }
template<typename T> inline atVector3<T> atVector4<T>::xzw() const { return atVector3<T>(x, z, w); }
template<typename T> inline atVector3<T> atVector4<T>::xyw() const { return atVector3<T>(x, y, w); }
template<typename T> inline atVector3<T> atVector4<T>::yzw() const { return atVector3<T>(y, z, w); }

template<typename T> inline atVector4<T> atVector4<T>::Add(const atVector4<T>& rhs) const { return *this + rhs; }
template<typename T> inline atVector4<T> atVector4<T>::Sub(const atVector4<T>& rhs) const { return *this - rhs; }
template<typename T> inline atVector4<T> atVector4<T>::Mul(const atVector4<T>& rhs) const { return *this * rhs; }
template<typename T> inline atVector4<T> atVector4<T>::Div(const atVector4<T>& rhs) const { return *this / rhs; }

template<typename T> inline T atVector4<T>::Mag() const { return Mag(*this); }
template<typename T> inline T atVector4<T>::Length() const { return Length(*this); }
template<typename T> inline T atVector4<T>::Dot(const atVector4<T> &rhs) const { return Dot(*this, rhs); }
template<typename T> inline T atVector4<T>::Angle(const atVector4<T> &rhs) const { return Angle(*this, rhs); }
template<typename T> inline atVector4<T> atVector4<T>::Normalize() const { return Normalize(*this); }
template<typename T> inline atVector4<T> atVector4<T>::Reflect(const atVector4<T>& norm) const { return Reflect(*this, norm); }
template<typename T> inline atVector4<T> atVector4<T>::Project(const atVector4<T>& to) const { return Project(*this, to); }

template<typename T> inline T atVector4<T>::Mag(const atVector4<T>& rhs) { return atSqrt(Length(rhs)); }
template<typename T> inline T atVector4<T>::Length(const atVector4<T>& rhs) { return Dot(rhs, rhs); }
template<typename T> inline T atVector4<T>::Angle(const atVector4<T> &lhs, const atVector4<T> &rhs) { return atACos(Dot(lhs, rhs) / (Mag(lhs) * Mag(rhs))); }
template<typename T> inline atVector4<T> atVector4<T>::Reflect(const atVector4<T>& dir, const atVector4<T>& norm) { return dir - norm * Dot(dir, norm) * 2; }
template<typename T> inline atVector4<T> atVector4<T>::Normalize(const atVector4<T>& rhs) { return rhs / Mag(rhs); }
template<typename T> inline atVector4<T> atVector4<T>::Project(const atVector4<T>& vec, const atVector4<T>& to) { return vec * to.Normalize(); }
template<typename T> inline atVector4<T> atVector4<T>::zero() { return atVector4<T>((T)0); }
template<typename T> inline atVector4<T> atVector4<T>::one() { return atVector4<T>((T)1); }

template<typename T> inline T& atVector4<T>::operator[](int64_t index) { return data()[index]; }
template<typename T> inline const T& atVector4<T>::operator[](int64_t index) const { return data()[index]; }
template<typename T> inline atVector4<T> atVector4<T>::operator-() const { return atVector4<T>(-x, -y, -z, -w); }
template<typename T> inline atVector4<T> atVector4<T>::operator+(const atVector4<T>& rhs) const { return atVector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
template<typename T> inline atVector4<T> atVector4<T>::operator-(const atVector4<T>& rhs) const { return atVector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
template<typename T> inline atVector4<T> atVector4<T>::operator*(const atVector4<T>& rhs) const { return atVector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
template<typename T> inline atVector4<T> atVector4<T>::operator/(const atVector4<T>& rhs) const { return atVector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
template<typename T> inline atVector4<T> atVector4<T>::operator%(const atVector4<T>& rhs) const { return atVector4<T>(x % rhs.x, y % rhs.y, z % rhs.z, w % rhs.w); }
template<typename T> inline const atVector4<T>& atVector4<T>::operator+=(const atVector4<T>& rhs) { return assign(*this + rhs); }
template<typename T> inline const atVector4<T>& atVector4<T>::operator-=(const atVector4<T>& rhs) { return assign(*this - rhs); }
template<typename T> inline const atVector4<T>& atVector4<T>::operator*=(const atVector4<T>& rhs) { return assign(*this * rhs); }
template<typename T> inline const atVector4<T>& atVector4<T>::operator/=(const atVector4<T>& rhs) { return assign(*this / rhs); }
template<typename T> inline const atVector4<T>& atVector4<T>::operator%=(const atVector4<T>& rhs) { return assign(atMod(x, rhs.x), atMod(y, rhs.y), atMod(z, rhs.z), atMod(w, rhs.w)); }
template<typename T> inline const atVector4<T>& atVector4<T>::operator=(const atVector4<T>& rhs) { return assign(rhs); }
template<typename T> inline bool atVector4<T>::operator==(const atVector4<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
template<typename T> inline bool atVector4<T>::operator!=(const atVector4<T>& rhs) const { return !(*this == rhs); }

template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator+(const atVector4<T2>& rhs) const { return atVector4<T>(x + (T)rhs.x, y + (T)rhs.y, z + (T)rhs.z, w + (T)rhs.w); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator-(const atVector4<T2>& rhs) const { return atVector4<T>(x - (T)rhs.x, y - (T)rhs.y, z - (T)rhs.z, w - (T)rhs.w); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator*(const atVector4<T2>& rhs) const { return atVector4<T>(x * (T)rhs.x, y * (T)rhs.y, z * (T)rhs.z, w * (T)rhs.w); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator/(const atVector4<T2>& rhs) const { return atVector4<T>(x / (T)rhs.x, y / (T)rhs.y, z / (T)rhs.z, w / (T)rhs.w); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator%(const atVector4<T2>& rhs) const { return atVector4<T>(x % (T)rhs.x, y % (T)rhs.y, z % (T)rhs.z, w % (T)rhs.w); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator+=(const atVector4<T2>& rhs) { return assign(*this + rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator-=(const atVector4<T2>& rhs) { return assign(*this - rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator*=(const atVector4<T2>& rhs) { return assign(*this * rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator/=(const atVector4<T2>& rhs) { return assign(*this / rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator%=(const atVector4<T2>& rhs) { return assign(atMod(x, (T)rhs.x), atMod(y, (T)rhs.y), atMod(z, (T)rhs.z), atMod(w, (T)rhs.w)); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator=(const atVector4<T2>& rhs) { return assign(rhs); }

template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator+(const T2 &rhs) const { return atVector4<T>(x + (T)rhs, y + (T)rhs, z + (T)rhs, w + (T)rhs); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator-(const T2 &rhs) const { return atVector4<T>(x - (T)rhs, y - (T)rhs, z - (T)rhs, w - (T)rhs); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator*(const T2 &rhs) const { return atVector4<T>(x * (T)rhs, y * (T)rhs, z * (T)rhs, w * (T)rhs); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator/(const T2 &rhs) const { return atVector4<T>(x / (T)rhs, y / (T)rhs, z / (T)rhs, w / (T)rhs); }
template<typename T> template<typename T2> inline atVector4<T> atVector4<T>::operator%(const T2 &rhs) const { return atVector4<T>(atMod(x, (T)rhs), atMod(y, (T)rhs), atMod(z (T)rhs), atMod(w, (T)rhs)); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator+=(const T2 &rhs) { return assign(*this + rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator-=(const T2 &rhs) { return assign(*this - rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator*=(const T2 &rhs) { return assign(*this * rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator/=(const T2 &rhs) { return assign(*this / rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator%=(const T2 &rhs) { return assign(*this % rhs); }
template<typename T> template<typename T2> inline const atVector4<T>& atVector4<T>::operator=(const T2 &rhs) { return assign((T)rhs, (T)rhs, (T)rhs, (T)rhs); }

template<typename T> inline const T* atVector4<T>::end() const { return data() + 4; }
template<typename T> inline const T* atVector4<T>::begin() const { return data(); }
template<typename T> inline const T* atVector4<T>::data() const { return &x; }
template<typename T> inline T* atVector4<T>::end() { return data() + 4; }
template<typename T> inline T* atVector4<T>::begin() { return data(); }
template<typename T> inline T* atVector4<T>::data() { return &x; }

