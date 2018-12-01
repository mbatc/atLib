
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

#ifndef _atMath_h__
#define _atMath_h__

#include "atTypes.h"
#include "atMatrix.h"
#include "atVector4.h"
#include "atReadStream.h"
#include "atWriteStream.h"

#define atE 2.71828182845904523536   // e
#define atLog2e 1.44269504088896340736   // log2(e)
#define atLog10e 0.434294481903251827651  // log10(e)
#define atLn2 0.693147180559945309417  // ln(2)
#define atLn10  2.30258509299404568402   // ln(10)
#define atPi 3.14159265358979323846   // pi
#define atPi_2 1.57079632679489661923   // pi/2
#define atPi_4 0.785398163397448309616  // pi/4
#define at1_PI 0.318309886183790671538  // 1/pi
#define at2_PI 0.636619772367581343076  // 2/pi
#define atSqrtPi 1.12837916709551257390   // 2/sqrt(pi)
#define atSqrt2 1.41421356237309504880   // sqrt(2)
#define atSqrt1_2 0.707106781186547524401  // 1/sqrt(2)
#define atDegs2Rads(x) ((double)x * 0.017453292519943295769)
#define atRads2Degs(x) ((double)x * 57.29577951308232087679)
#define atDegs2Radsf(x) ((float)x * 0.017453292519943295769f)
#define atRads2Degsf(x) ((float)x * 57.29577951308232087679f)

typedef atVector2<int32_t> atVec2I;
typedef atVector3<int32_t> atVec3I;
typedef atVector4<int32_t> atVec4I;

typedef atVector2<int64_t> atVec2I64;
typedef atVector3<int64_t> atVec3I64;
typedef atVector4<int64_t> atVec4I64;

typedef atVector2<float> atVec2F;
typedef atVector3<float> atVec3F;
typedef atVector4<float> atVec4F;

typedef atVector2<double> atVec2F64;
typedef atVector3<double> atVec3F64;
typedef atVector4<double> atVec4F64;

typedef atMatrix<double, 2, 2> atMat2D;
typedef atMatrix<double, 3, 3> atMat3D;
typedef atMatrix<double, 4, 4> atMat4D;

typedef atMatrix<float, 2, 2> atMat2;
typedef atMatrix<float, 3, 3> atMat3;
typedef atMatrix<float, 4, 4> atMat4;

template <> atTypeDesc atGetTypeDesc<atVector2<float>>();
template <> atTypeDesc atGetTypeDesc<atVector3<float>>();
template <> atTypeDesc atGetTypeDesc<atVector4<float>>();

template <> atTypeDesc atGetTypeDesc<atVector2<double>>();
template <> atTypeDesc atGetTypeDesc<atVector3<double>>();
template <> atTypeDesc atGetTypeDesc<atVector4<double>>();

template <> atTypeDesc atGetTypeDesc<atVector2<int32_t>>();
template <> atTypeDesc atGetTypeDesc<atVector3<int32_t>>();
template <> atTypeDesc atGetTypeDesc<atVector4<int32_t>>();

template <> atTypeDesc atGetTypeDesc<atVector2<int64_t>>();
template <> atTypeDesc atGetTypeDesc<atVector3<int64_t>>();
template <> atTypeDesc atGetTypeDesc<atVector4<int64_t>>();

template <> atTypeDesc atGetTypeDesc<atVector2<int8_t>>();
template <> atTypeDesc atGetTypeDesc<atVector3<int8_t>>();
template <> atTypeDesc atGetTypeDesc<atVector4<int8_t>>();

template <> atTypeDesc atGetTypeDesc<atVector2<int16_t>>();
template <> atTypeDesc atGetTypeDesc<atVector3<int16_t>>();
template <> atTypeDesc atGetTypeDesc<atVector4<int16_t>>();

template <> atTypeDesc atGetTypeDesc<atMatrix<double, 2, 2>>();
template <> atTypeDesc atGetTypeDesc<atMatrix<double, 3, 3>>();
template <> atTypeDesc atGetTypeDesc<atMatrix<double, 4, 4>>();

template <> atTypeDesc atGetTypeDesc<atMatrix<float, 2, 2>>();
template <> atTypeDesc atGetTypeDesc<atMatrix<float, 3, 3>>();
template <> atTypeDesc atGetTypeDesc<atMatrix<float, 4, 4>>();

template <typename T> T atMin(const T &a) { return a; }
template <typename T> T atMax(const T &a) { return a; }
template <typename T, typename T2, typename... Args> T atMin(const T &first, const T2 &second, Args ...args) { T argMin = (T)atMin(second, Args...); return (argMin < first) ? argMin : first; }
template <typename T, typename T2, typename... Args> T atMax(const T &first, const T2 &second, Args ...args) { T argMin = (T)atMax(second, Args...); return (argMin > first) ? argMin : first; }

template <typename T> T atSquare(const T x) { return x * x; }
template <typename T> T atSin(const T rads) { return sin(rads); }
template <typename T> T atCos(const T rads) { return cos(rads); }
template <typename T> T atTan(const T rads) { return tan(rads); }
template <typename T> T atASin(const T rads) { return asin(rads); }
template <typename T> T atACos(const T rads) { return acos(rads); }
template <typename T> T atATan(const T rads) { return atan(rads); }
template <typename T> T atATan2(const atVector2<T> pos) { return atATan2(pos.x, pos.y); }
template <typename T> T atATan2(const T y, const T x);

template <typename T> atMatrix<T, 4, 4> atMatrixProjection(const T aspect, const T FOV, const T nearPlane, const T farPlane);
template <typename T> atMatrix<T, 4, 4> atMatrixOrtho(const T width, const T height, const T nearPlane, const T farPlane);
template <typename T> atMatrix<T, 4, 4> atMatrixOrtho(const T left, const T right, const T top, const T bottom, const T nearPlane, const T farPlane);
template <typename T> atMatrix<T, 4, 4> atMatrixRotationX(const T rads);
template <typename T> atMatrix<T, 4, 4> atMatrixRotationY(const T rads);
template <typename T> atMatrix<T, 4, 4> atMatrixRotationZ(const T rads);
template <typename T> atMatrix<T, 4, 4> atMatrixRotation(const atVector3<T> &axis, T rads);
template <typename T> atMatrix<T, 4, 4> atMatrixYawPitchRoll(const T yaw, const T pitch, const T roll);
template <typename T> atMatrix<T, 4, 4> atMatrixTranslation(const atVector3<T> &translation);
template <typename T> atMatrix<T, 4, 4> atMatrixScale(const atVector3<T> &scale);
template <typename T> atMatrix<T, 4, 4> atMatrixScale(const atVector4<T> &scale);
template <typename T> atMatrix<T, 4, 4> atMatrixScaleUniform(const T &scale);

template <typename T> atVector4<T> operator*(const atMatrix<T, 4, 4> &lhs, const atVector4<T> &rhs);
template <typename T> atVector3<T> operator*(const atMatrix<T, 4, 4> &lhs, const atVector3<T> &rhs);
template <typename T> atVector3<T> operator*(const atMatrix<T, 3, 3> &lhs, const atVector3<T> &rhs);
template <typename T> atVector2<T> operator*(const atMatrix<T, 2, 2> &lhs, const atVector2<T> &rhs);

template <typename T> atVector4<T> operator*(const T &lhs, const atVector4<T> &rhs);
template <typename T> atVector3<T> operator*(const T &lhs, const atVector3<T> &rhs);
template <typename T> atVector2<T> operator*(const T &lhs, const atVector2<T> &rhs);

template <typename T> atVector4<T> operator/(const T &lhs, const atVector4<T> &rhs);
template <typename T> atVector3<T> operator/(const T &lhs, const atVector3<T> &rhs);
template <typename T> atVector2<T> operator/(const T &lhs, const atVector2<T> &rhs);

template <typename T> atTrivialStreamWrite(atVector2<T>)
template <typename T> atTrivialStreamWrite(atVector3<T>)
template <typename T> atTrivialStreamWrite(atVector4<T>)

template <typename T> atTrivialStreamRead(atVector2<T>)
template <typename T> atTrivialStreamRead(atVector3<T>)
template <typename T> atTrivialStreamRead(atVector4<T>)

#include "atMath.inl"
#endif // _atMath_h__
