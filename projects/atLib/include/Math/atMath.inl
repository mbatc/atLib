#include "atMath.h"

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

template<typename T> inline T atATan2(const T y, const T x)
{
  if (x == y == 0) return 0;
  const double rad = sqrt(x * x + y * y);
  return 2 * atATan(x <= 0 ? (rad - x) / y : y / (rad + y));
}

template<typename T> inline atVector2<T> atQuadraticSolve(const T a, const T b, const T c)
{
  T val = (T)sqrt(b * b - 4 * a * c);
  T ac_2 = 2 * a * c;
  return atVector2<T>((-b - val) / ac_2, (-b + val) / ac_2);
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixProjection(const T aspect, const T FOV, const T nearPlane, const T farPlane)
{
  const T f_fn = farPlane / (farPlane - nearPlane);
  const T yScale = 1.0 / atTan(FOV / 2);
  return
  {
    yScale / aspect, 0,       0,     0,
    0,               yScale,  0,     0,
    0,               0,      -f_fn, -nearPlane * f_fn,
    0,               0,      -1,     0
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixOrtho(const T width, const T height, const T nearPlane, const T farPlane) { return atMatrixOrtho<T>((T)0, width, (T)0, height, nearPlane, farPlane); }

template<typename T> inline atMatrix<T, 4, 4> atMatrixOrtho(const T left, const T right, const T top, const T bottom, const T nearPlane, const T farPlane)
{
  return
  {
    (T)2 / (right - left),                0,                                0,                                               0,
    0,                                 (T)2 / (top - bottom),               0,                                               0,
    0,                                    0,                                2 / (farPlane - nearPlane),                      0,
    -(right + left) / (right - left),    -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), 1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixRotationX(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return

  {
    1, 0, 0,  0,
    0, c, -s, 0,
    0, s, c,  0,
    0, 0, 0,  1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixRotationY(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return

  {
    c,  0, s, 0,
    0,  1, 0, 0,
    -s, 0, c, 0,
    0,  0, 0, 1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixRotationZ(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return

  {
    c, -s, 0, 0,
    s, c,  0, 0,
    0, 0,  1, 0,
    0, 0,  0, 1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixRotation(const atVector3<T> &axis, const T rads)
{
  T c = atCos(rads);
  T s = atSin(rads);
  return

  {
    c + atSquare(axis.x) * (1 - c),         axis.x * axis.y * (1 - c) - axis.z * s, axis.x * axis.z * (1 - c) + axis.y * s, 0,
    axis.y * axis.x * (1 - c) + axis.z * s, c + atSquare(axis.y) * (1 - c),         axis.y * axis.z * (1 - c) - axis.x * s, 0,
    axis.z * axis.x * (1 - c) - axis.y * s, axis.z * axis.y * (1 - c) + axis.x * s, c + atSquare(axis.z) * (1 - c),         0,
    0,                                      0,                                      0,                                      1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixTranslation(const atVector3<T> &translation)
{
  return 

  {
    1, 0, 0, translation.x,
    0, 1, 0, translation.y,
    0, 0, 1, translation.z,
    0, 0, 0, 1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixScale(const atVector3<T> &scale)
{
  return

  {
    scale.x, 0,       0,       0,
    0,       scale.y, 0,       0,
    0,       0,       scale.z, 0,
    0,       0,       0,       1
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixScale(const atVector4<T> &scale)
{
  return

  {
    scale.x, 0,       0,       0,
    0,       scale.y, 0,       0,
    0,       0,       scale.z, 0,
    0,       0,       0,       scale.w
  };
}

template<typename T> inline atMatrix<T, 4, 4> atMatrixScaleUniform(const T &scale)
{
  return

  {
    1, 0, 0, 0, 
    0, 1, 0, 0, 
    0, 0, 1, 0, 
    0, 0, 0, (T)1 / scale
  };
}

template<typename T> inline void atMatrixDecompose(const atMatrix<T, 4, 4> &mat, atVector3<T>* pTranslation, atVector3<T>* pRotation, atVector3<T>* pScale)
{
  if (pTranslation) *pTranslation = atMatrixExtractTranslation(mat);
  if (pRotation) *pRotation = atMatrixExtractRotation(mat);
  if (pScale) *pScale = atMatrixExtractScale(mat);
}

template<typename T> inline atVector3<T> atMatrixExtractRotation(const atMatrix<T, 4, 4> &mat) { return 0; }

template<typename T> inline atVector3<T> atMatrixExtractTranslation(const atMatrix<T, 4, 4> &mat) { return atVector3<T>(mat[3], mat[7], mat[11]); }

template<typename T> inline atVector3<T> atMatrixExtractScale(const atMatrix<T, 4, 4> &mat) 
{
  return atVector3<T>(
    atVector3<T>(mat[0], mat[4], mat[12]).Mag(), 
    atVector3<T>(mat[1], mat[5], mat[13]).Mag(), 
    atVector3<T>(mat[2], mat[6], mat[14]).Mag());
}

template<typename T> inline atVector4<T> operator*(const atMatrix<T, 4, 4> &lhs, const atVector4<T> &rhs)
{
  atMatrix<T, 1, 4> mat = atMatrix<T, 4, 1>(rhs).Mul(lhs);
  return atVector4<T>(mat[0], mat[1], mat[2], mat[3]);
}

template<typename T> inline atVector3<T> operator*(const atMatrix<T, 4, 4> &lhs, const atVector3<T> &rhs)
{
  atMatrix<T, 1, 4> mat = atMatrix<T, 4, 1>(atVector4<T>(rhs, 1.0)).Mul(lhs);
  return atVector3<T>(mat[0], mat[1], mat[2]);
}

template<typename T> inline atVector3<T> operator*(const atMatrix<T, 3, 3> &lhs, const atVector3<T> &rhs)
{
  atMatrix<T, 1, 3> mat = atMatrix<T, 3, 1>(rhs).Mul(lhs);
  return atVector4<T>(mat[0], mat[1], mat[2]);
}

template<typename T> inline atVector2<T> operator*(const atMatrix<T, 2, 2> &lhs, const atVector2<T> &rhs)
{
  atMatrix<T, 1, 2> mat = atMatrix<T, 2, 1>(rhs).Mul(lhs);
  return atVector4<T>(mat[0], mat[1]);
}

template<typename T> inline atVector4<T> operator*(const T &lhs, const atVector4<T>& rhs) { return rhs * lhs; }
template<typename T> inline atVector3<T> operator*(const T &lhs, const atVector3<T>& rhs) { return rhs * lhs; }
template<typename T> inline atVector2<T> operator*(const T &lhs, const atVector2<T>& rhs) { return rhs * lhs; }
template<typename T> inline atVector4<T> operator/(const T &lhs, const atVector4<T>& rhs) { return atVector4<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }
template<typename T> inline atVector3<T> operator/(const T &lhs, const atVector3<T>& rhs) { return atVector3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
template<typename T> inline atVector2<T> operator/(const T &lhs, const atVector2<T>& rhs) { return atVector2<T>(lhs / rhs.x, lhs / rhs.y); }
template<typename T> inline atMatrix<T, 4, 4> atMatrixYawPitchRoll(const T yaw, const T pitch, const T roll) { return atMatrixRotationY(yaw) * atMatrixRotationX(pitch) * atMatrixRotationZ(roll); }
