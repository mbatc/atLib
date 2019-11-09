
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

#ifndef _atTypes_h__
#define _atTypes_h__

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <tuple>

#define AT_INVALID_ID (int64_t)-1
#define AT_INVALID_INDEX (int64_t)-1
#define AT_INVALID_LOC (int64_t)-1

void atUnused();
template<typename T, typename... Args> void atUnused(const T &first, Args... args) { (void)first; atUnused(args...); }

enum atType
{
  atType_Float16,
  atType_Float32,
  atType_Float64,

  atType_Int8,
  atType_Int16,
  atType_Int32,
  atType_Int64,

  atType_Uint8,
  atType_Uint16,
  atType_Uint32,
  atType_Uint64,

  atType_Count,
  atType_Unknown
};

// Return the size in bytes of a specific type
template<typename T> int64_t atSize() { return sizeof(T); };
template<typename T> int64_t atSize(const T& val) { return sizeof(val); }; 
template<> int64_t atSize(const atType &val);

struct atTypeDesc
{
  atTypeDesc(const atType type = atType_Unknown, const int64_t width = 1, const int64_t count = 1)
    : type(type)
    , count(count)
    , width(width)
    , size(atSize(type))
  {}

  atType type = atType_Unknown;
  int64_t width = 1;
  int64_t count = 1;
  int64_t size = 1;

  bool operator==(const atTypeDesc &o) const;
  bool operator!=(const atTypeDesc &o) const;
};

// Return an enum representing the type
template<typename T> atType atGetType() { return atType_Unknown; }
template<> atType atGetType<bool>();
template<> atType atGetType<char>();
template<> atType atGetType<wchar_t>();
template<> atType atGetType<int8_t>();
template<> atType atGetType<int16_t>();
template<> atType atGetType<int32_t>();
template<> atType atGetType<int64_t>();
template<> atType atGetType<uint8_t>();
template<> atType atGetType<uint16_t>();
template<> atType atGetType<uint32_t>();
template<> atType atGetType<uint64_t>();
template<> atType atGetType<float>();
template<> atType atGetType<double>();

// Return a struct containing the atType enum and count of that type
// Specialized Templates implemented in atMath.h for atVector2/3/4 and atMatrix

template<typename T> atTypeDesc atGetTypeDesc() { return atTypeDesc(atGetType<T>()); }
template<typename T> atTypeDesc atGetTypeDesc(const T &unused) { atUnused(unused); return atGetTypeDesc<T>(); }

// Type case helpers
template<typename T1, typename T2> bool __atTypeCast(T1 *pDst, const T2 *pSrc, const int64_t &count);
template<typename T> bool __atTypeCast(T *pDst, const T *pSrc, const int64_t &count);
template<typename T> bool __atTypeCast(void *pDst, const T *pSrc, const atType &dstType, const int64_t &count);

// Cast types that can be expressed by an atTypeDesc
bool atTypeCast(void *pDst, const void *pSrc, const atTypeDesc &dstType, const atTypeDesc &srcType);

#include "atTypes.inl"

#endif