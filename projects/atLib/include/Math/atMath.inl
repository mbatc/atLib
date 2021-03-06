#include "atMath.h"

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

template<typename T> inline T atATan2(const T &y, const T &x)
{
  if (x == y == 0) return 0;
  const double rad = sqrt(x * x + y * y);
  return 2 * atATan(x <= 0 ? (rad - x) / y : y / (rad + y));
}

template<typename T> inline T atSquare(const T &x) { return x * x; }
template<typename T> inline T atSin(const T &rads) { return (T)sin(rads); }
template<typename T> inline T atCos(const T &rads) { return (T)cos(rads); }
template<typename T> inline T atTan(const T &rads) { return (T)tan(rads); }
template<typename T> inline T atASin(const T &rads) { return (T)asin(rads); }
template<typename T> inline T atACos(const T &rads) { return (T)acos(rads); }
template<typename T> inline T atATan(const T &rads) { return (T)atan(rads); }
template<typename T> inline T atATan2(const atVector2<T> &pos) { return atATan2(pos.x, pos.y); }
template<typename T, typename T2> inline T atLerp(const T &a, const T &b, const T2 &time) { return a + (b - a) * (time); }
template<typename T, typename T2, typename T3> inline T atClamp(const T &val, const T2 &min, const T3 &max) { return atMin(max, atMax(min, val)); }

template<typename T> inline atVector2<T> atQuadraticSolve(const T &a, const T &b, const T &c)
{
  T val = (T)sqrt(b * b - 4 * a * c);
  T ac_2 = 2 * a * c;
  return atVector2<T>((-b - val) / ac_2, (-b + val) / ac_2);
}

template<typename InT, typename OutT> inline OutT atDerivative(const InT &val, const std::function<OutT(InT)> &func, const InT step)
{
  return OutT(func(val + step) - func(val - step)) / (step * 2);
}

template<typename InT, typename OutT> inline OutT atNthDerivative(const InT &val, const std::function<OutT(InT)> &func, const int64_t &n, const InT step)
{
  if (n <= 0)
    return atDerivative(val, func, step);
  OutT rhs = atNthDerivative(val + step, func, n - 1, step);
  OutT lhs = atNthDerivative(val - step, func, n - 1, step);
  return OutT(rhs - lhs) / (step * 2);
}

template<typename T> inline T atCatmullRomSpline(const T &p1, const T &p2, const T &p3, const T &p4, const T &t, const T &s)
{
  atMatrixNxM<T> cubics(4, 1, { t * t * t, t * t, t, 1 });
  atMatrixNxM<T> coeffs(4, 4,
  {
    -s, 2 - s, s - 2, s,
    2 * s, s - 3, 3 - 2 * s, -s,
    -s, 0, s, 0,
    0, 1, 0, 0
  });

  atMatrixNxM<T> points(1, 4, { p1, p2, p3, p4 });

  return (cubics * coeffs * points)[0];
}

template<typename T> inline T atSigmoid(const T &val) { return 1 / (1 + exp(-val)); }

template<typename T> inline T atSigmoidApprox(const T &val)
{
  static T sigLookup[] = {
    T(0),
    atSigmoid(T(-5)),
    atSigmoid(T(-4)),
    atSigmoid(T(-3)),
    atSigmoid(T(-2)),
    atSigmoid(T(-1)),
    atSigmoid(T(0)),
    atSigmoid(T(1)),
    atSigmoid(T(2)),
    atSigmoid(T(3)),
    atSigmoid(T(4)),
    atSigmoid(T(5)),
    T(1)
  };
  static int64_t lookupSize = atArraySize(sigLookup);
  static int64_t sigMid = lookupSize / 2;

  T offset = val + sigMid;
  int64_t lower = atClamp(int64_t(offset), 0, lookupSize - 1);
  int64_t upper = atClamp(int64_t(offset + 1), 0, lookupSize - 1);
  return atLerp(sigLookup[lower], sigLookup[upper], (offset - lower));
}

template<typename T> inline T atMod(const T &a, const T &b) { return a % b; }

template<typename T> inline atMatrix4x4<T> atMatrixProjection(const T aspect, const T FOV, const T nearPlane, const T farPlane, const T clipSpaceNearZ, const T clipSpaceFarZ)
{
  const T A = (clipSpaceFarZ * farPlane - clipSpaceNearZ * nearPlane) / (nearPlane - farPlane);
  const T B = (clipSpaceFarZ - clipSpaceNearZ) * farPlane * nearPlane / (nearPlane - farPlane);
  const T yScale = 1.0 / atTan(FOV / 2);
  return

  atMatrix4x4<T>(
    yScale / aspect, 0,       0,     0,
    0,               yScale,  0,     0,
    0,               0,       A,     B,
    0,               0,      -1,     0
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixOrtho(const T width, const T height, const T nearPlane, const T farPlane) { return atMatrixOrtho<T>((T)0, width, (T)0, height, nearPlane, farPlane); }

template<typename T> inline atMatrix4x4<T> atMatrixOrtho(const T left, const T right, const T top, const T bottom, const T nearPlane, const T farPlane)
{
  return

  atMatrix4x4<T>(
    (T)2 / (right - left),                0,                                0,                                               0,
    0,                                 (T)2 / (top - bottom),               0,                                               0,
    0,                                    0,                                2 / (farPlane - nearPlane),                      0,
    -(right + left) / (right - left),    -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), 1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixRotationX(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return

  atMatrix4x4<T>(
    1, 0, 0,  0,
    0, c, -s, 0,
    0, s, c,  0,
    0, 0, 0,  1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixRotationY(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return

  atMatrix4x4<T>(
    c,  0, s, 0,
    0,  1, 0, 0,
    -s, 0, c, 0,
    0,  0, 0, 1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixRotationZ(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return

  atMatrix4x4<T>(
    c, -s, 0, 0,
    s, c,  0, 0,
    0, 0,  1, 0,
    0, 0,  0, 1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixRotation(const atVector3<T> &axis, T rads)
{
  T c = atCos(rads);
  T s = atSin(rads);
  return

  atMatrix4x4<T>(
    c + atSquare(axis.x) * (1 - c),         axis.x * axis.y * (1 - c) - axis.z * s, axis.x * axis.z * (1 - c) + axis.y * s, 0,
    axis.y * axis.x * (1 - c) + axis.z * s, c + atSquare(axis.y) * (1 - c),         axis.y * axis.z * (1 - c) - axis.x * s, 0,
    axis.z * axis.x * (1 - c) - axis.y * s, axis.z * axis.y * (1 - c) + axis.x * s, c + atSquare(axis.z) * (1 - c),         0,
    0,                                      0,                                      0,                                      1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixRotation(const atQuaternion<T> &quat)
{
  const atQuaternion<T> &q = quat; // for shorter notation
  const T d = q.Length();
  if (d < atLimitsSmallest<T>())
    return atMatrix4x4<T>::Identity();

  T s = T(2) / d;
  T xs = q.x * s, ys = q.y * s, zs = q.z * s;
  T wx = q.w * xs, wy = q.w * ys, wz = q.w * zs;
  T xx = q.x * xs, xy = q.x * ys, xz = q.x * zs;
  T yy = q.y * ys, yz = q.y * zs, zz = q.z * zs;

  return
    
  atMatrix4x4<T>(
    T(1) - (yy + zz), xy - wz,          xz + wy,          0,
    xy + wz,          T(1) - (xx + zz), yz - wx,          0,
    xz - wy,          yz + wx,          T(1) - (xx + yy), 0,
    0,                0,                0,                1);
}

template<typename T> inline atMatrix4x4<T> atMatrixTranslation(const atVector3<T> &translation)
{
  return 

  atMatrix4x4<T>(
    1, 0, 0, translation.x,
    0, 1, 0, translation.y,
    0, 0, 1, translation.z,
    0, 0, 0, 1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixScale(const atVector3<T> &scale)
{
  return

  atMatrix4x4<T>(
    scale.x, 0,       0,       0,
    0,       scale.y, 0,       0,
    0,       0,       scale.z, 0,
    0,       0,       0,       1
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixScale(const atVector4<T> &scale)
{
  return

  atMatrix4x4<T>(
    scale.x, 0,       0,       0,
    0,       scale.y, 0,       0,
    0,       0,       scale.z, 0,
    0,       0,       0,       scale.w
  );
}

template<typename T> inline atMatrix4x4<T> atMatrixScaleUniform(const T &scale)
{
  return

  atMatrix4x4<T>(
    1, 0, 0, 0, 
    0, 1, 0, 0, 
    0, 0, 1, 0, 
    0, 0, 0, (T)1 / scale
  );
}

template<typename T> inline void atMatrixDecompose(const atMatrix4x4<T> &mat, atVector3<T> *pTranslation, atVector3<T> *pRotation, atVector3<T> *pScale)
{
  if (pTranslation) *pTranslation = atMatrixExtractTranslation(mat);
  if (pRotation) *pRotation = atMatrixExtractRotation(mat);
  if (pScale) *pScale = atMatrixExtractScale(mat);
}

template<typename T> inline atVector3<T> atMatrixExtractRotation(const atMatrix4x4<T> &mat) { return atMatrixExtractOrientation(mat).Angle(); }

template<typename T> inline atVector3<T> atMatrixExtractOrientation(const atMatrix4x4<T> &mat) { return atQuaternion(mat).EulerAngles(); }

template<typename T> inline atVector3<T> atMatrixExtractTranslation(const atMatrix4x4<T> &mat) { return atVector3<T>(mat[3], mat[7], mat[11]); }

template<typename T> inline atVector3<T> atMatrixExtractScale(const atMatrix4x4<T> &mat) 
{
  return atVector3<T>(
    atVector3<T>(mat[0], mat[4], mat[12]).Mag(), 
    atVector3<T>(mat[1], mat[5], mat[13]).Mag(), 
    atVector3<T>(mat[2], mat[6], mat[14]).Mag());
}

template<typename T> inline T atNumBitsOn(const T &val)
{
  static uint8_t lookup[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
  if (val == 0)
    return 0;
  uint8_t addr = uint8_t(val & 0x0F);
  return lookup[addr] + atNumBitsOn(val >> 4);
}

template<typename T> inline T atToIndex(const atVector2<T> &coord, const atVector2<T> &dims) { return coord.x + coord.y * dims.x; }
template<typename T> inline T atToIndex(const atVector3<T> &coord, const atVector3<T> &dims) { return coord.x + coord.y * dims.x + coord.z * dims.x * dims.y; }
template<typename T> inline T atToIndex(const atVector4<T> &coord, const atVector4<T> &dims) { return coord.x + coord.y * dims.x + coord.z * dims.x * dims.y + coord.w * coord.x * coord.y * coord.z; }
template<typename T> inline atVector2<T> atFromIndex(const T &index, const atVector2<T> &dims) { return atVector2<T>(atMod(index, dims.x), index / dims.x); }
template<typename T> inline atVector3<T> atFromIndex(const T &index, const atVector3<T> &dims) { return atVector3<T>(atMod(index, dims.x), atMod(index, dims.x * dims.y) / dims.x, index / (dims.x * dims.y)); }
template<typename T> inline atVector4<T> atFromIndex(const T &index, const atVector4<T> &dims) { return atVector3<T>(atMod(index, dims.x), atMod(index, dims.x * dims.y) / dims.x, atMod(index, dims.x * dims.y * dims.z) / (dims.x * dims.y), index / (dims.x * dims.y * dims.z)); }

template<typename T> inline atVector4<T> operator*(const T &lhs, const atVector4<T>& rhs) { return rhs * lhs; }

template<typename T> inline atVector3<T> operator*(const T &lhs, const atVector3<T>& rhs) { return rhs * lhs; }

template<typename T> inline atVector2<T> operator*(const T &lhs, const atVector2<T>& rhs) { return rhs * lhs; }

template<typename T> inline atVector4<T> operator/(const T &lhs, const atVector4<T>& rhs) { return atVector4<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }

template<typename T> inline atVector3<T> operator/(const T &lhs, const atVector3<T>& rhs) { return atVector3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }

template<typename T> inline atVector2<T> operator/(const T &lhs, const atVector2<T>& rhs) { return atVector2<T>(lhs / rhs.x, lhs / rhs.y); }

template<typename T> inline atMatrix4x4<T> atMatrixYawPitchRoll(const T yaw, const T pitch, const T roll) { return atMatrixRotationY(yaw) * atMatrixRotationX(pitch) * atMatrixRotationZ(roll); }

template<typename T> inline atMatrix4x4<T> atMatrixYawPitchRoll(const atVector3<T> &ypr) { return atMatrixYawPitchRoll(ypr.x, ypr.y, ypr.z); }

inline float atSqrt(const float &val) { return sqrtf(val); }

inline double atSqrt(const double &val) { return sqrtl(val); }

inline int64_t atSqrt(const int64_t &val) { return (int64_t)sqrt((double)val); }

inline int32_t atSqrt(const int32_t &val) { return (int32_t)sqrt((double)val); }

inline int16_t atSqrt(const int16_t &val) { return (int16_t)sqrt((double)val); }

inline int8_t atSqrt(const int8_t &val) { return (int8_t)sqrt((double)val); }

#ifdef ATLIB_DIRECTX
template<typename T> inline
T atClipNearZ()
{
  return (T)0;
}

template<typename T> inline
T atClipFarZ()
{
  return (T)1;
}

#elif ATLIB_OPENGL

template<typename T> inline
T atClipNearZ()
{
  return (T)-1;
}

template<typename T> inline
T atClipFarZ()
{
  return (T)1;
}

#endif