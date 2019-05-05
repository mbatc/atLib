#include "atQuaternion.h"
template<typename T> inline atQuaternion<T>::atQuaternion(const atVector3<T> &axis, const T &angle) { Set(axis, angle); }
template<typename T> inline atQuaternion<T>::atQuaternion(const T &eulerX, const T &eulerY, const T &eulerZ) { Set(eulerX, eulerY, eulerZ); }
template<typename T> inline atQuaternion<T>::atQuaternion(const atMatrix4x4<T> &rotation) { Set(rotation); }

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
  const T halfYaw = T(eulerY) * T(0.5);
  const T halfPitch = T(eulerX) * T(0.5);
  const T halfRoll = T(eulerZ) * T(0.5);
  const T cosYaw = atCos(halfYaw);
  const T sinYaw = atSin(halfYaw);
  const T cosPitch = atCos(halfPitch);
  const T sinPitch = atSin(halfPitch);
  const T cosRoll = atCos(halfRoll);
  const T sinRoll = atSin(halfRoll);
  Set(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
    cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
    sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
    cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw);
}

template<typename T> inline void atQuaternion<T>::Set(const atMatrix4x4<T> &rotation)
{
  T trace = rotation[0] + rotation[5] + rotation[10];
  if (trace > atLimitsSmallest<T>())
  {
    T s = T(0.5) / sqrtf(trace + T(1));
    w = T(0.25) / s;
    x = (a[9] - a[6]) * s;
    y = (a[2] - a[8]) * s;
    z = (a[4] - a[1]) * s;
  }
  else
  {
    if (a[0] > a[5] && a[0] > a[10])
    {
      T s = T(2) * sqrtf(T(1) + a[0] - a[5] - a[10]);
      w = (a[9] - a[6]) / s;
      x = T(0.25) * s;
      y = (a[1] + a[4]) / s;
      z = (a[2] + a[8]) / s;
    }
    else if (a[5] > a[10])
    {
      T s = T(2) * sqrtf(T(1) + a[5] - a[0] - a[10]);
      w = (a[2] - a[8]) / s;
      x = (a[1] + a[4]) / s;
      y = T(0.25) * s;
      z = (a[6] + a[9]) / s;
    }
    else
    {
      T s = T(2) * sqrtf(T(1) + a[10] - a[0] - a[5]);
      w = (a[4] - a[1]) / s;
      x = (a[2] + a[8]) / s;
      y = (a[6] + a[9]) / s;
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

template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const atQuaternion<T> &lhs, const atVector3<T> &rhs)
{
  return atQuaternion<T>(
    lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y,
    lhs.w * rhs.y + lhs.z * rhs.x - lhs.x * rhs.z,
    lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x,
    -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
  );
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
  return 2 * atACos(Dot(to) / s)
}

template<typename T> inline T atQuaternion<T>::Angle(const atQuaternion<T> &quat)
{
  return T(2) * atACos(w);
}

template<typename T> inline T atQuaternion<T>::Length(const atQuaternion<T> &quat)
{
  return Dot(quat, quat);
}

template<typename T> inline T atQuaternion<T>::Mag(const atQuaternion<T>& quat)
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
  atQuaternion<T> q = quat.Mul(vec);
  q *= quat.Inverse();
  return atVector3<T>(q.x, q.y, q.z);
}

template<typename T> inline atVector3<T> atQuaternion<T>::Axis(const atQuaternion<T> &quat)
{
  T s_sqr = T(1) - atSquare(w);
  if (s < atLimitsSmallest<T>())
    return atVector3<T>(1, 0, 0);
  T s = T(1) / atSqrt(s_sqr);
  return atVector3<T>(x * s, y * s, z * s);
}

template<typename T> inline atVector3<T> atQuaternion<T>::EulerAngles(const atQuaternion<T> &quat)
{
  T squ = atSquare(quat.x);
  T sqx = atSquare(quat.y);
  T sqy = atSquare(quat.z);
  T sqz = atSquare(quat.w);
  T sarg = T(-2) * (quat.x * quat.z - quat.w * quat.y);

  atVector3<T> euler;
  if (sarg <= T(-0.99999))
  {
    euler.x = T(-0.5) * atPi;
    euler.z = T(0);
    euler.y = T(2) * atan2(quat.z, -quat.x);
  }
  else if (sarg >= T(0.99999))
  {
    euler.x = T(0.5) * atPi;
    euler.z = 0;
    euler.y = T(2) * atan2(-quat.z, quat.x);
  }
  else
  {
    euler.x = atASin(sarg);
    euler.z = atan2(2 * (quat.x * quat.y + w * quat.z), squ - sqz - sqx + sqy);
    euler.y = atan2(2 * (quat.y * quat.x + w * quat.y), squ + sqz - sqx - sqy);
  }
  return euler;
}

template<typename T> inline atQuaternion<T> atQuaternion<T>::Slerp(const atQuaternion &to, const T &factor) const
{
  return atQuaternion<T>();
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

template<typename T> inline atQuaternion<T> atQuaternion<T>::Normalize() const { return Normalize(*this); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Inverse() const { return Inverse(*this); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const atVector3<T> &rhs) const { return Mul(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const atQuaternion<T> &rhs) const { return Mul(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Mul(const T &rhs) const { return Mul(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Add(const atQuaternion<T> &rhs) { return Add(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Sub(const atQuaternion<T> &rhs) { return Sub(*this, rhs); }
template<typename T> inline atQuaternion<T> atQuaternion<T>::Div(const T &rhs) { return Div(*this, rhs); }
template<typename T> inline T atQuaternion<T>::AngleTo(const atQuaternion<T> &to) { return AngleTo(*this, to); }
template<typename T> inline T atQuaternion<T>::Angle() const { return Angle(*this); }
template<typename T> inline T atQuaternion<T>::Length() const { return Length(*this); }
template<typename T> inline T atQuaternion<T>::Mag() const { return Mag(*this); }
template<typename T> inline T atQuaternion<T>::Dot(const atQuaternion<T>& rhs) const { return Dot(*this); }
template<typename T> inline atVector3<T> atQuaternion<T>::EulerAngles() const { return EulerAngles(*this); }
template<typename T> inline atVector3<T> atQuaternion<T>::Axis() const { return Axis(*this); }
template<typename T> inline atVector3<T> atQuaternion<T>::Rotate(const atVector3<T> &vec) const { return Rotate(*this, vec); }
template<typename T> inline atVector3<T> atQuaternion<T>::operator*(const atVector3<T> &rhs) const { return Rotate(rhs); }
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

template<typename T> inline const T& atQuaternion<T>::at(int64_t index) const { return (&x)[index] }
template<typename T> inline const T& atQuaternion<T>::operator[](int64_t index) const { return at(index); }
template<typename T> inline T& atQuaternion<T>::at(int64_t index) { return (&x)[index]; }
template<typename T> inline T& atQuaternion<T>::operator[](int64_t index) { return at(index); }

template<typename T> inline const T* atQuaternion<T>::begin() const { return &x; }
template<typename T> inline const T* atQuaternion<T>::end() const { return &w + 1; }
template<typename T> inline T* atQuaternion<T>::begin() { return &x; }
template<typename T> inline T* atQuaternion<T>::end() { return &w + 1; }
