
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

template<typename T> inline atVector3<T>::atVector3()
  : x(0)
  , y(0)
  , z(0)
{}

template<typename T> inline atVector3<T>::atVector3(T val)
  : x(val)
  , y(val)
  , z(val)
{}

template<typename T> inline atVector3<T>::atVector3(T _x, T _y, T _z)
  : x(_x)
  , y(_y)
  , z(_z)
{}

template<typename T> inline atVector3<T>::atVector3(const atVector3<T> &copy)
  : x(copy.x)
  , y(copy.y)
  , z(copy.z)
{}

template<typename T> inline atVector3<T>::atVector3(atVector3<T> &&move)
  : x(move.x)
  , y(move.y)
  , z(move.z)
{}

template<typename T> template<typename T2> inline atVector3<T>::atVector3(T2 val)
  : x((T)val)
  , y((T)val)
  , z((T)val)
{}

template<typename T> template<typename T2> inline atVector3<T>::atVector3(T2 _x, T2 _y, T2 _z)
  : x((T)_x)
  , y((T)_y)
  , z((T)_z)
{}

template<typename T> template<typename T2> inline atVector3<T>::atVector3(T2 _x, const atVector2<T2> &yz)
  : x((T)_x)
  , y((T)yz.x)
  , z((T)yz.y)
{}

template<typename T> template<typename T2> inline atVector3<T>::atVector3(const atVector2<T2> &xy, T2 _z)
  : x((T)xy.x)
  , y((T)xy.y)
  , z((T)_z)
{}

template<typename T> template<typename T2> inline atVector3<T>::atVector3(atVector3<T2> copy)
  : x((T)copy.x)
  , y((T)copy.y)
  , z((T)copy.z)
{}

template<typename T> inline const atVector3<T>& atVector3<T>::assign(T _x, T _y, T _z)
{
  x = _x;
  y = _y;
  z = _z;
  return *this;
}

template<typename T> inline const atVector3<T>& atVector3<T>::assign(const atVector3<T>& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::assign(T2 _x, T2 _y, T2 _z)
{
  x = (T)_x;
  y = (T)_y;
  z = (T)_z;
  return *this;
}

template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::assign(const atVector3<T2>& rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  z = (T)rhs.z;
  return *this;
}

template<typename T> inline T atVector3<T>::Length(const atVector3<T> &rhs)
{
  return rhs.x * rhs.x +
    rhs.y * rhs.y +
    rhs.z * rhs.z;
}

template<typename T> inline T atVector3<T>::Dot(const atVector3<T> &lhs, const atVector3<T> &rhs)
{
  return lhs.x * rhs.x +
    lhs.y * rhs.y +
    lhs.z * rhs.z;
}

template<typename T> inline atVector3<T> atVector3<T>::Cross(const atVector3<T> &lhs, const atVector3<T> &rhs)
{
  return  atVector3<T>(
    lhs[1] * rhs[2] - lhs[2] * rhs[1],
    lhs[2] * rhs[0] - lhs[0] * rhs[2],
    lhs[0] * rhs[1] - lhs[1] * rhs[0]
    );
}

template<typename T> inline atVector3<T> atVector3<T>::Reflect(const atVector3<T> &dir, const atVector3<T> &norm)
{
  return dir - norm * Dot(dir, norm) * 2;
}

template<typename T> inline atVector3<T> atVector3<T>::Project(const atVector3<T>& vec, const atVector3<T>& to)
{
  double toMag = to.Mag();
  return vec.Dot(to) / toMag * (to / toMag);
}

template<typename T> inline atVector3<T> atVector3<T>::Max(const atVector3<T> &a, const atVector3<T> &b)
{
  return atVector3<T>(atMax(a.x, b.x), atMax(a.y, b.y), atMax(a.z, b.z));
}

template<typename T> inline atVector3<T> atVector3<T>::Min(const atVector3<T> &a, const atVector3<T> &b)
{
  return atVector3<T>(atMin(a.x, b.x), atMin(a.y, b.y), atMin(a.z, b.z));
}

template<typename T> inline atVector3<T> atVector3<T>::Clamp(const atVector3<T> &vec, const atVector3<T> &min, const atVector3<T> &max)
{
  return Max(min, Min(vec, max));
}

template<typename T> inline atVector3<T>::atVector3(T _x, const atVector2<T>& yz) : atVector3(_x, yz.x, yz.y) {}
template<typename T> inline atVector3<T>::atVector3(const atVector2<T>& xy, T _z) : atVector3(xy.x, xy.y, _z) {}
template<typename T> inline T atVector3<T>::Mag() const { return Mag(*this); }
template<typename T> inline T atVector3<T>::Length() const { return Length(*this); }
template<typename T> inline T atVector3<T>::Dot(const atVector3<T> &rhs) const { return Dot(*this, rhs); }
template<typename T> inline T atVector3<T>::Angle(const atVector3<T> &rhs) const { return Angle(*this, rhs); }
template<typename T> inline atVector3<T> atVector3<T>::Add(const atVector3<T> &rhs) { return *this + rhs; }
template<typename T> inline atVector3<T> atVector3<T>::Sub(const atVector3<T> &rhs) { return *this - rhs; }
template<typename T> inline atVector3<T> atVector3<T>::Mul(const atVector3<T> &rhs) { return *this * rhs; }
template<typename T> inline atVector3<T> atVector3<T>::Div(const atVector3<T> &rhs) { return *this / rhs; }
template<typename T> inline atVector3<T> atVector3<T>::Normalize() const { return Normalize(*this); }
template<typename T> inline atVector3<T> atVector3<T>::Reflect(const atVector3<T> &norm) const { return Reflect(*this, norm); }
template<typename T> inline atVector3<T> atVector3<T>::Cross(const atVector3<T> &rhs) const { return Cross(*this, rhs); }
template<typename T> inline atVector3<T> atVector3<T>::Project(const atVector3<T>& to) const { return Project(*this, to); }
template<typename T> inline atVector3<T> atVector3<T>::Max(const atVector3<T> &b) const { return Max(*this, b); }
template<typename T> inline atVector3<T> atVector3<T>::Min(const atVector3<T> &b) const { return Min(*this, b); }
template<typename T> inline atVector3<T> atVector3<T>::Clamp(const atVector3<T> &min, const atVector3<T> &max) const { return Clamp(*this, min, max); }
template<typename T> inline T atVector3<T>::Mag(const atVector3<T> &rhs) { return atSqrt(Length(rhs)); }
template<typename T> inline T atVector3<T>::Angle(const atVector3<T> &lhs, const atVector3<T> &rhs) { return atACos(Dot(lhs, rhs) / (Mag(lhs) * Mag(rhs))); }
template<typename T> inline atVector3<T> atVector3<T>::Normalize(const atVector3<T> &rhs) { return rhs / Mag(rhs); }
template<typename T> inline atVector3<T> atVector3<T>::zero() { return atVector3<T>(0); }
template<typename T> inline atVector3<T> atVector3<T>::one() { return atVector3<T>(1); }
template<typename T> inline atVector2<T> atVector3<T>::xy() const { return atVector2<T>(x, y); }
template<typename T> inline atVector2<T> atVector3<T>::xz() const { return atVector2<T>(x, z); }
template<typename T> inline atVector2<T> atVector3<T>::yz() const { return atVector2<T>(y, z); }

template<typename T> inline T& atVector3<T>::operator[](int64_t index) { return data()[index]; }
template<typename T> inline const T & atVector3<T>::operator[](int64_t index) const { return data()[index]; }
template<typename T> inline atVector3<T> atVector3<T>::operator-() const { return atVector3<T>(-x, -y, -z); }
template<typename T> inline atVector3<T> atVector3<T>::operator+(const atVector3<T>& rhs) const { return atVector3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }
template<typename T> inline atVector3<T> atVector3<T>::operator-(const atVector3<T>& rhs) const { return atVector3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
template<typename T> inline atVector3<T> atVector3<T>::operator*(const atVector3<T>& rhs) const { return atVector3<T>(x * rhs.x, y * rhs.y, z * rhs.z); }
template<typename T> inline atVector3<T> atVector3<T>::operator/(const atVector3<T>& rhs) const { return atVector3<T>(x / rhs.x, y / rhs.y, z / rhs.z); }
template<typename T> inline atVector3<T> atVector3<T>::operator%(const atVector3<T>& rhs) const { return atVector3<T>(atMod(x, rhs.x), atMod(y, rhs.y), atMod(z, rhs.z)); }
template<typename T> inline const atVector3<T>& atVector3<T>::operator+=(const atVector3<T>& rhs) { return assign(*this + rhs); }
template<typename T> inline const atVector3<T>& atVector3<T>::operator-=(const atVector3<T>& rhs) { return assign(*this - rhs); }
template<typename T> inline const atVector3<T>& atVector3<T>::operator*=(const atVector3<T>& rhs) { return assign(*this * rhs); }
template<typename T> inline const atVector3<T>& atVector3<T>::operator/=(const atVector3<T>& rhs) { return assign(*this / rhs); }
template<typename T> inline const atVector3<T>& atVector3<T>::operator%=(const atVector3<T>& rhs) { return assign(*this % rhs); }
template<typename T> inline const atVector3<T>& atVector3<T>::operator=(const atVector3<T>& rhs) { return assign(rhs); }
template<typename T> inline bool atVector3<T>::operator==(const atVector3<T>& rhs) const { return memcmp(this, &rhs, sizeof(atVector3<T>)) == 0; }
template<typename T> inline bool atVector3<T>::operator!=(const atVector3<T>& rhs) const { return !(*this == rhs); }

template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator+(const T2 &rhs) const { return atVector3<T>(x + (T)rhs, y + (T)rhs, z + (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator-(const T2 &rhs) const { return atVector3<T>(x - (T)rhs, y - (T)rhs, z - (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator*(const T2 &rhs) const { return atVector3<T>(x * (T)rhs, y * (T)rhs, z * (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator/(const T2 &rhs) const { return atVector3<T>(x / (T)rhs, y / (T)rhs, z / (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator%(const T2 &rhs) const { return atVector3<T>(atMod(x, (T)rhs), atMod(y, (T)rhs), atMod(z, (T)rhs)); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator+=(const T2 &rhs) { return assign(*this + rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator-=(const T2 &rhs) { return assign(*this - rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator*=(const T2 &rhs) { return assign(*this * rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator/=(const T2 &rhs) { return assign(*this / rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator%=(const T2 &rhs) { return assign(*this % rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator=(const T2 &rhs) { return assign(rhs); }

template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator+(const atVector3<T2> &rhs) const { return atVector3<T>(x + (T)rhs, y + (T)rhs, z + (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator-(const atVector3<T2> &rhs) const { return atVector3<T>(x - (T)rhs, y - (T)rhs, z - (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator*(const atVector3<T2> &rhs) const { return atVector3<T>(x * (T)rhs, y * (T)rhs, z * (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator/(const atVector3<T2> &rhs) const { return atVector3<T>(x / (T)rhs, y / (T)rhs, z / (T)rhs); }
template<typename T> template<typename T2> inline atVector3<T> atVector3<T>::operator%(const atVector3<T2> &rhs) const { return atVector3<T>(atMod(x, (T)rhs), atMod(y, (T)rhs), atMod(z, (T)rhs)); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator+=(const atVector3<T2> &rhs) { return assign(*this + rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator-=(const atVector3<T2> &rhs) { return assign(*this - rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator*=(const atVector3<T2> &rhs) { return assign(*this * rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator/=(const atVector3<T2> &rhs) { return assign(*this / rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator%=(const atVector3<T2> &rhs) { return assign(*this % rhs); }
template<typename T> template<typename T2> inline const atVector3<T>& atVector3<T>::operator=(const atVector3<T2> &rhs) { return assign(rhs); }

template<typename T> inline const T* atVector3<T>::end() const { return data() + 3; }
template<typename T> inline const T* atVector3<T>::begin() const { return data(); }
template<typename T> inline const T* atVector3<T>::data() const { return &x; }
template<typename T> inline T* atVector3<T>::end() { return data() + 3; }
template<typename T> inline T* atVector3<T>::begin() { return data(); }
template<typename T> inline T* atVector3<T>::data() { return &x; }