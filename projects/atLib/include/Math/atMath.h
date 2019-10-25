
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

#include "atMatrix.h"
#include "atReadStream.h"
#include "atWriteStream.h"
#include <math.h>

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

typedef atVector2<double> atVec2D;
typedef atVector3<double> atVec3D;
typedef atVector4<double> atVec4D;

typedef atMatrix4x4<double> atMat4D;
typedef atMatrix4x4<float> atMat4F;


template<> atTypeDesc atGetTypeDesc<atVector2<float>>();
template<> atTypeDesc atGetTypeDesc<atVector3<float>>();
template<> atTypeDesc atGetTypeDesc<atVector4<float>>();

template<> atTypeDesc atGetTypeDesc<atVector2<double>>();
template<> atTypeDesc atGetTypeDesc<atVector3<double>>();
template<> atTypeDesc atGetTypeDesc<atVector4<double>>();

template<> atTypeDesc atGetTypeDesc<atVector2<int32_t>>();
template<> atTypeDesc atGetTypeDesc<atVector3<int32_t>>();
template<> atTypeDesc atGetTypeDesc<atVector4<int32_t>>();

template<> atTypeDesc atGetTypeDesc<atVector2<int64_t>>();
template<> atTypeDesc atGetTypeDesc<atVector3<int64_t>>();
template<> atTypeDesc atGetTypeDesc<atVector4<int64_t>>();

template<> atTypeDesc atGetTypeDesc<atVector2<int8_t>>();
template<> atTypeDesc atGetTypeDesc<atVector3<int8_t>>();
template<> atTypeDesc atGetTypeDesc<atVector4<int8_t>>();

template<> atTypeDesc atGetTypeDesc<atVector2<int16_t>>();
template<> atTypeDesc atGetTypeDesc<atVector3<int16_t>>();
template<> atTypeDesc atGetTypeDesc<atVector4<int16_t>>();

template<> atTypeDesc atGetTypeDesc<atMatrix4x4<double>>();
template<> atTypeDesc atGetTypeDesc<atMatrix4x4<float>>();

template<typename T> inline T atClipNearZ();
template<typename T> inline T atClipFarZ();

template<typename T> inline T atMin(const T &a);
template<typename T> inline T atMax(const T &a);
template<typename T, typename T2, typename... Args> inline T atMin(const T &first, const T2 &second, Args ...args);
template<typename T, typename T2, typename... Args> inline T atMax(const T &first, const T2 &second, Args ...args);

template<typename T> inline T atSquare(const T &x);
template<typename T> inline T atSin(const T &rads);
template<typename T> inline T atCos(const T &rads);
template<typename T> inline T atTan(const T &rads);
template<typename T> inline T atASin(const T &rads);
template<typename T> inline T atACos(const T &rads);
template<typename T> inline T atATan(const T &rads);
template<typename T> inline T atATan2(const atVector2<T> &pos);
template<typename T> inline T atATan2(const T &y, const T &x);

template<typename T, typename T2, typename T3> inline T atClamp(const T &val, const T2 &min, const T3 &max);

template<typename T> inline atVector2<T> atQuadraticSolve(const T &a, const T &b, const T &c);
template<typename T> inline T atSigmoid(const T &val);
template<typename T> inline T atDerivative(const T &val, T (*func)(const T&), const double step = 0.000001);

template<typename T> inline T atMod(const T &a, const T &b);
template<> inline float atMod(const float &a, const float &b);
template<> inline double atMod(const double &a, const double &b);

template<typename T, typename T2> inline T atLerp(const T &a, const T &b, const T2 &time);

inline float atSqrt(const float &val);
inline double atSqrt(const double &val);
inline int64_t atSqrt(const int64_t &val);
inline int32_t atSqrt(const int32_t &val);
inline int16_t atSqrt(const int16_t &val);
inline int8_t atSqrt(const int8_t &val);

#include "atQuaternion.h" // atQuaternion relies on some of the above functions

typedef atQuaternion<double> atQuatD;
typedef atQuaternion<float> atQuatF;

template<> atTypeDesc atGetTypeDesc<atQuaternion<double>>();
template<> atTypeDesc atGetTypeDesc<atQuaternion<float>>();

template<typename T> inline atMatrix4x4<T> atMatrixProjection(const T aspect, const T FOV, const T nearPlane, const T farPlane, const T zClipNear = atClipNearZ<T>(), const T zClipFar = atClipFarZ<T>());
template<typename T> inline atMatrix4x4<T> atMatrixOrtho(const T width, const T height, const T nearPlane, const T farPlane);
template<typename T> inline atMatrix4x4<T> atMatrixOrtho(const T left, const T right, const T top, const T bottom, const T nearPlane, const T farPlane);
template<typename T> inline atMatrix4x4<T> atMatrixRotationX(const T rads);
template<typename T> inline atMatrix4x4<T> atMatrixRotationY(const T rads);
template<typename T> inline atMatrix4x4<T> atMatrixRotationZ(const T rads);
template<typename T> inline atMatrix4x4<T> atMatrixRotation(const atVector3<T> &axis, T rads);
template<typename T> inline atMatrix4x4<T> atMatrixRotation(const atQuaternion<T> &quat);
template<typename T> inline atMatrix4x4<T> atMatrixYawPitchRoll(const T yaw, const T pitch, const T roll);
template<typename T> inline atMatrix4x4<T> atMatrixYawPitchRoll(const atVector3<T> &ypr);
template<typename T> inline atMatrix4x4<T> atMatrixTranslation(const atVector3<T> &translation);
template<typename T> inline atMatrix4x4<T> atMatrixScale(const atVector3<T> &scale);
template<typename T> inline atMatrix4x4<T> atMatrixScale(const atVector4<T> &scale);
template<typename T> inline atMatrix4x4<T> atMatrixScaleUniform(const T &scale);
template<typename T> inline void atMatrixDecompose(const atMatrix4x4<T> &mat, atVector3<T> *pTranslation, atVector3<T> *pRotation, atVector3<T> *pScale);
template<typename T> inline atVector3<T> atMatrixExtractTranslation(const atMatrix4x4<T> &mat);
template<typename T> inline atVector3<T> atMatrixExtractRotation(const atMatrix4x4<T> &mat);
template<typename T> inline atVector3<T> atMatrixExtractScale(const atMatrix4x4<T> &mat);

template<typename T> inline atVector4<T> operator*(const T &lhs, const atVector4<T> &rhs);
template<typename T> inline atVector3<T> operator*(const T &lhs, const atVector3<T> &rhs);
template<typename T> inline atVector2<T> operator*(const T &lhs, const atVector2<T> &rhs);

template<typename T> inline atVector4<T> operator/(const T &lhs, const atVector4<T> &rhs);
template<typename T> inline atVector3<T> operator/(const T &lhs, const atVector3<T> &rhs);
template<typename T> inline atVector2<T> operator/(const T &lhs, const atVector2<T> &rhs);

template<typename T> atTrivialStreamWrite(atVector2<T>)
template<typename T> atTrivialStreamWrite(atVector3<T>)
template<typename T> atTrivialStreamWrite(atVector4<T>)
template<typename T> atTrivialStreamWrite(atMatrix4x4<T>)

template<typename T> atTrivialStreamRead(atVector2<T>)
template<typename T> atTrivialStreamRead(atVector3<T>)
template<typename T> atTrivialStreamRead(atVector4<T>)
template<typename T> atTrivialStreamRead(atMatrix4x4<T>)

template<typename T> atTrivialStreamWrite(atQuaternion<T>)
template<typename T> atTrivialStreamRead(atQuaternion<T>)

#include "atMath.inl"
#endif // _atMath_h__
