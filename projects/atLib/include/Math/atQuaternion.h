#ifndef atQuaternion_h__
#define atQuaternion_h__

#include "atMatrix.h"

template<typename T> class atQuaternion
{
public:
  atQuaternion(const T &x, const T &y, const T &z, const T &w);
  atQuaternion(const atVector3<T> &axis, const T &angle);
  atQuaternion(const T &eulerX, const T &eulerY, const T &eulerZ);
  atQuaternion(const atMatrix4x4<T> &rotation);

  void Set(const T &x, const T &y, const T &z, const T &w);
  void Set(const atVector3<T> &axis, const T &angle);
  void Set(const T &eulerX, const T &eulerY, const T &eulerZ);
  void Set(const atMatrix4x4<T> &rotation);

  static atQuaternion<T> zero();
  static atQuaternion<T> Identity();

  static atQuaternion<T> Normalize(const atQuaternion<T> &quat);
  static atQuaternion<T> Inverse(const atQuaternion<T> &quat);
  static atQuaternion<T> Mul(const atQuaternion<T> &lhs, const atVector3<T> &rhs);
  static atQuaternion<T> Mul(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);
  static atQuaternion<T> Mul(const atQuaternion<T> &lhs, const T &rhs);
  static atQuaternion<T> Add(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);
  static atQuaternion<T> Sub(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);
  static atQuaternion<T> Div(const atQuaternion<T> &lhs, const T &rhs);

  static T AngleTo(const atQuaternion<T> &from, const atQuaternion<T> &to);
  static T Angle(const atQuaternion<T> &quat);
  static T Length(const atQuaternion<T> &quat);
  static T Mag(const atQuaternion<T> &quat);
  static T Dot(const atQuaternion<T> &lhs, const atQuaternion<T> &rhs);

  static atVector3<T> Rotate(const atQuaternion<T> &quat, const atVector3<T> &vec);
  static atVector3<T> Axis(const atQuaternion<T> &quat);
  static atVector3<T> EulerAngles(const atQuaternion<T> &quat);

  atQuaternion<T> Slerp(const atQuaternion &to, const T &factor) const;
  atQuaternion<T> Normalize() const;
  atQuaternion<T> Inverse() const;
  atQuaternion<T> Mul(const atVector3<T> &rhs) const;
  atQuaternion<T> Mul(const atQuaternion<T> &rhs) const;
  atQuaternion<T> Mul(const T &rhs) const;
  atQuaternion<T> Add(const atQuaternion<T> &rhs);
  atQuaternion<T> Sub(const atQuaternion<T> &rhs);
  atQuaternion<T> Div(const T &rhs);

  T AngleTo(const atQuaternion<T> &to);
  T Angle() const;
  T Length() const;
  T Mag() const;
  T Dot(const atQuaternion<T> &rhs) const;

  atVector3<T> EulerAngles() const;
  atVector3<T> Axis() const;
  atVector3<T> Rotate(const atVector3<T> &vec) const;

  bool operator==(const atQuaternion<T> &rhs) const;
  bool operator!=(const atQuaternion<T> &rhs) const;

  atVector3<T> operator*(const atVector3<T> &rhs) const;

  atQuaternion<T> operator*(const atQuaternion<T> &rhs) const;
  atQuaternion<T> operator-(const atQuaternion<T> &rhs) const;
  atQuaternion<T> operator+(const atQuaternion<T> &rhs) const;
  atQuaternion<T> operator*(const T &rhs) const;
  atQuaternion<T> operator/(const T &rhs) const;
  atQuaternion<T> operator-() const;

  const atQuaternion<T>& operator=(const atQuaternion<T> &rhs);
  const atQuaternion<T>& operator*=(const atQuaternion<T> &rhs);
  const atQuaternion<T>& operator*=(const T &rhs);
  
  template<typename T2> const atQuaternion<T>& operator=(const atQuaternion<T2> &rhs);
  template<typename T2> const atQuaternion<T>& operator*=(const atQuaternion<T2> &rhs);
  template<typename T2> const atQuaternion<T>& operator*=(const T2 &rhs);

  template<typename T2> atQuaternion<T> operator*(const atQuaternion<T2> &rhs) const;
  template<typename T2> atQuaternion<T> operator-(const atQuaternion<T2> &rhs) const;
  template<typename T2> atQuaternion<T> operator+(const atQuaternion<T2> &rhs) const;
  template<typename T2> atQuaternion<T> operator*(const T2 &rhs) const;
  template<typename T2> atQuaternion<T> operator/(const T2 &rhs) const;

  const T& at(int64_t index) const;
  const T& operator[](int64_t index) const;
  T& at(int64_t index);
  T& operator[](int64_t index);

  const T* begin() const;
  const T* end() const;
  T* begin();
  T* end();

  T x;
  T y;
  T z;
  T w;
};

#include "atQuaternion.inl"
#endif // atQuaternion_h__
