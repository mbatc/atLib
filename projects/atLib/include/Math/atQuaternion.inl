
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

#include "atLimits.h"

template<typename T> inline atQuaternion<T>::atQuaternion(const atVector3<T> &axis, const T &angle) { Set(axis, angle); }
template<typename T> inline atQuaternion<T>::atQuaternion(const T &eulerX, const T &eulerY, const T &eulerZ) { Set(eulerX, eulerY, eulerZ); }
template<typename T> inline atQuaternion<T>::atQuaternion(const atMatrix4x4<T> &rotation) { Set(rotation); }

template<typename T> inline atQuaternion<T>::atQuaternion()
  : x(0)
  , y(0)
  , z(0)
  , w(1)
{}

template<typename T> inline atQuaternion<T>::atQuaternion(const T &x, const T &y, const T &z, const T &w)
  : x(x)
  , y(y)
  , z(z)
  , w(w)
{}

template<typename T> inline void atQuaternion<T>::Set(const T &_x, const T &_y, const T &_z, const T &_w)
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
}

template<typename T> inline void atQuaternion<T>::Set(const atVector3<T> &axis, const T &angle)
{
  T dist = axis.Mag();
  T s = atSin(angle * T(0.5)) / dist;
  Set(axis.x * s, axis.y * s, axis.z * s, atCos(angle * T(0.5)));
}

template<typename T> inline void atQuaternion<T>::Set(const T &eulerX, const T &eulerY, const T &eulerZ)
{
  double cy = cos(eulerY * 0.5);
  double sy = sin(eulerY * 0.5);
  double cp = cos(eulerX * 0.5);
  double sp = sin(eulerX * 0.5);
  double cr = cos(eulerZ * 0.5);
  double sr = sin(eulerZ * 0.5);

  w = cy * cp * cr + sy * sp * sr;
  x = cy * cp * sr - sy * sp * cr;
  y = sy * cp * sr + cy * sp * cr;
  z = sy * cp * cr - cy * sp * sr;
}

template<typename T> inline void atQuaternion<T>::Set(const atMatrix4x4<T> &rotation)
{
  T trace = rotation[0] + rotation[5] + rotation[10];
  if (trace > atLimitsSmallest<T>())
  {
    T s = T(0.5) / atSqrt(trace + T(1));
    w = T(0.25) / s;
    x = (rotation[9] - rotation[6]) * s;
    y = (rotation[2] - rotation[8]) * s;
    z = (rotation[4] - rotation[1]) * s;
  }
  else
  {
    if (rotation[0] > rotation[5] && rotation[0] > rotation[10])
    {
      T s = T(2) * atSqrt(T(1) + rotation[0] - rotation[5] - rotation[10]);
      w = (rotation[9] - rotation[6]) / s;
      x = T(0.25) * s;
      y = (rotation[1] + rotation[4]) / s;
      z = (rotation[2] + rotation[8]) / s;
    }
    else if (rotation[5] > rotation[10])
    {
      T s = T(2) * atSqrt(T(1) + rotation[5] - rotation[0] - rotation[10]);
      w = (rotation[2] - rotation[8]) / s;
      x = (rotation[1] + rotation[4]) / s;
      y = T(0.25) * s;
      z = (rotation[6] + rotation[9]) / s;
    }
    else
    {
      T s = T(2) * atSqrt(T(1) + rotation[10] - rotation[0] - rotation[5]);
      w = (rotation[4] - rotation[1]) / s;
      x = (rotation[2] + rotation[8]) / s;
      y = (rotation[6] + rotation[9]) / s;
      z = T(0.25) * s;
    }
  }
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::zero()
{
  return atQuaternion<T>(0, 0, 0, 0);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Identity()
{
  return atQuaternion<T>(0, 0, 0, 1);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Normalize(const atQuaternion<T> &quat)
{
  return quat / Mag(quat);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Inverse(const atQuaternion<T> &quat)
{
  return atQuaternion<T>(-quat.x, -quat.y, -quat.z, quat.w);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs)
{
  return atQuaternion<T>(
    lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
    lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
    lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
    lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
  );
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const atQuaternion<T> &lhs, const T &rhs)
{
  return atQuaternion<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Add(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs)
{
  return atQuaternion<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Sub(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs)
{
  return atQuaternion<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Div(const atQuaternion<T> &lhs, const T &rhs)
{
  T reciprocal = T(1) / rhs;
  return Mul(lhs, reciprocal);
}

template<typename T> inline T atQuaternion<T>::AngleTo(const atQuaternion<T> &from, const atQuaternion<T> &to)
{
  T s = atSqrt(from.Length() * to.Length());
  if (abs(s) < atLimitsSmallest<T>())
    return 0;
  return 2 * atACos(Dot(to) / s);
}

template<typename T> inline T atQuaternion<T>::Angle(const atQuaternion<T> &quat)
{
  return T(2) * atACos(quat.w);
}

template<typename T> inline T atQuaternion<T>::Length(const atQuaternion<T> &quat)
{
  return Dot(quat, quat);
}

template<typename T> inline T atQuaternion<T>::Mag(const atQuaternion<T> &quat)
{
  return atSqrt(Length(quat));
}

template<typename T> inline T atQuaternion<T>::Dot(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs)
{
  return 
    lhs.x * rhs.x + 
    lhs.y * rhs.y + 
    lhs.z * rhs.z + 
    lhs.w * rhs.w;
}

template<typename T> inline atVector3<T> atQuaternion<T>::Rotate(const atQuaternion<T> &quat, const atVector3<T> &vec)
{
  atQuaternion<T> q = quat * vec;
  q *= quat.Inverse();
  return atVector3<T>(q.x, q.y, q.z);
}

template<typename T> inline atVector3<T> atQuaternion<T>::Axis(const atQuaternion<T> &quat)
{
  T s_sqr = T(1) - atSquare(quat.w);
  T s = T(1) / atSqrt(s_sqr);
  if (s < atLimitsSmallest<T>())
    return atVector3<T>(1, 0, 0);
  return atVector3<T>(quat.x * s, quat.y * s, quat.z * s);
}

template<typename T> inline atVector3<T> atQuaternion<T>::EulerAngles(const atQuaternion<T> &quat)
{
  atVector3<T> euler;
  // roll (x-axis rotation)
  double sinr_cosp = +2.0 * (quat.w * quat.x + quat.y * quat.z);
  double cosr_cosp = +1.0 - 2.0 * (quat.x * quat.x + quat.y * quat.y);
  euler.z = atan2(sinr_cosp, cosr_cosp);

  // pitch (y-axis rotation)
  double sinp = +2.0 * (quat.w * quat.y - quat.z * quat.x);
  if (fabs(sinp) >= 1)
    euler.x = copysign(atPi_2, sinp); // use 90 degrees if out of range
  else
    euler.x = asin(sinp);

  // yaw (z-axis rotation)
  double siny_cosp = +2.0 * (quat.w * quat.z + quat.x * quat.y);
  double cosy_cosp = +1.0 - 2.0 * (quat.y * quat.y + quat.z * quat.z);
  euler.y = atan2(siny_cosp, cosy_cosp);
  return euler;
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Slerp(const atQuaternion<T> &to, const T &factor) const
{
  atQuaternion<T> result = *this;

  T mag = atSqrt(Length() * to.Length());
  if (mag > atLimitsSmallest<T>())
  {
    const T product = Dot(to) / mag;
    const T absProduct = abs(product);

    if (absProduct < T(1) - atLimitsSmallest<T>())
    {
      const T theta = atACos(absProduct);
      const T d = atSin(theta);
      if (d > 0)
      {
        T sign = product < 0 ? -1 : 1;
        T s0 = atSin((T(1) - factor) * theta) / d;
        T s1 = atSin(sign * factor * theta) / d;
        result.Set(
          x * s0 + to.x * s1,
          y * s0 + to.y * s1,
          z * s0 + to.z * s1,
          w * s0 + to.w * s1
        );
      }
    }
  }
  return result;
}

template<typename T> inline bool atQuaternion<T>::operator==(const atQuaternion<T> &rhs) const
{
  return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

template<typename T> inline bool atQuaternion<T>::operator!=(const atQuaternion<T>& rhs) const
{
  return !(*this == rhs);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::operator-() const
{
  return atQuaternion<T>(-x, -y, -z, -w);
}

template<typename T> inline const atQuaternion<T>& atQuaternion<T>::operator=(const atQuaternion<T> &rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
  return *this;
}

template<typename T> template<typename T2> inline const atQuaternion<T>& atQuaternion<T>::operator=(const atQuaternion<T2> &rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  z = (T)rhs.z;
  w = (T)rhs.w;
  return *this;
}

template<typename T> inline atQuaternion<T> operator*(const atVector3<T> &lhs, const atQuaternion<T> &rhs)
{
  return atQuaternion<T>(
    +lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
    +lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
    +lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
    -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

template<typename T> inline atQuaternion<T> operator*(const atQuaternion<T> &lhs, const atVector3<T> &rhs)
{
  return atQuaternion<T>(
    lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y,
    lhs.w * rhs.y + lhs.z * rhs.x - lhs.x * rhs.z,
    lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x,
    -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Normalize() const { return Normalize(*this); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Inverse() const { return Inverse(*this); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const atQuaternion<T> &rhs) const { return Mul(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const T &rhs) const { return Mul(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Add(const atQuaternion<T> &rhs) const { return Add(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Sub(const atQuaternion<T> &rhs) const { return Sub(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Div(const T &rhs) const { return Div(*this, rhs); }
template<typename T> inline T atQuaternion<T>::AngleTo(const atQuaternion<T> &to) const { return AngleTo(*this, to); }
template<typename T> inline T atQuaternion<T>::Angle() const { return Angle(*this); }
template<typename T> inline T atQuaternion<T>::Length() const { return Length(*this); }
template<typename T> inline T atQuaternion<T>::Mag() const { return Mag(*this); }
template<typename T> inline T atQuaternion<T>::Dot(const atQuaternion<T>& rhs) const { return Dot(*this, rhs); }
template<typename T> inline atVector3<T> atQuaternion<T>::EulerAngles() const { return EulerAngles(*this); }
template<typename T> inline atVector3<T> atQuaternion<T>::Axis() const { return Axis(*this); }
template<typename T> inline atVector3<T> atQuaternion<T>::Rotate(const atVector3<T> &vec) const { return Rotate(*this, vec); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::operator*(const atQuaternion<T> &rhs) const { return Mul(rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::operator-(const atQuaternion<T> &rhs) const { return Sub(rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::operator+(const atQuaternion<T> &rhs) const { return Add(rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::operator*(const T &rhs) const { return Mul(rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::operator/(const T &rhs) const { return Div(rhs); }
template<typename T> inline const atQuaternion<T>& atQuaternion<T>::operator*=(const atQuaternion<T> &rhs) { return *this = Mul(rhs); }
template<typename T> inline const atQuaternion<T>& atQuaternion<T>::operator*=(const T &rhs) { return *this = Mul(rhs); }
template<typename T> template<typename T2> inline atQuaternion<T> atQuaternion<T>::operator*(const atQuaternion<T2> &rhs) const { return Mul(atQuaternion<T>((T)rhs.x, (T)rhs.y, (T)rhs.z, (T)rhs.w)); }
template<typename T> template<typename T2> inline atQuaternion<T> atQuaternion<T>::operator-(const atQuaternion<T2> &rhs) const { return Sub(atQuaternion<T>((T)rhs.x, (T)rhs.y, (T)rhs.z, (T)rhs.w)); }
template<typename T> template<typename T2> inline atQuaternion<T> atQuaternion<T>::operator+(const atQuaternion<T2> &rhs) const { return Add(atQuaternion<T>((T)rhs.x, (T)rhs.y, (T)rhs.z, (T)rhs.w)); }
template<typename T> template<typename T2> inline atQuaternion<T> atQuaternion<T>::operator*(const T2 &rhs) const { return Mul((T)rhs); }
template<typename T> template<typename T2> inline atQuaternion<T> atQuaternion<T>::operator/(const T2 &rhs) const { return Div((T)rhs); }
template<typename T> template<typename T2> inline const atQuaternion<T>& atQuaternion<T>::operator*=(const atQuaternion<T2> &rhs) { return *this = Mul(atQuaternion<T>((T)rhs.x, (T)rhs.y, (T)rhs.z, (T)rhs.w)); }
template<typename T> template<typename T2> inline const atQuaternion<T>& atQuaternion<T>::operator*=(const T2 &rhs) { return *this = Mul((T)rhs); }

template<typename T> inline const T& atQuaternion<T>::at(int64_t index) const { return (&x)[index]; }
template<typename T> inline const T& atQuaternion<T>::operator[](int64_t index) const { return at(index); }
template<typename T> inline T& atQuaternion<T>::at(int64_t index) { return (&x)[index]; }
template<typename T> inline T& atQuaternion<T>::operator[](int64_t index) { return at(index); }

template<typename T> inline const T* atQuaternion<T>::begin() const { return &x; }
template<typename T> inline const T* atQuaternion<T>::end() const { return &w + 1; }
template<typename T> inline T* atQuaternion<T>::begin() { return &x; }
template<typename T> inline T* atQuaternion<T>::end() { return &w + 1; }
