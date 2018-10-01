
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

#define AT_INVALID_ID -1
#define AT_INVALID_INDEX -1
#define AT_INVALID_LOC -1

void atUnused();
template <typename T, typename... Args> void atUnused(const T &first, Args... args) { (void)first; atUnused(args...); }

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
template <typename T> int64_t atSize() { return sizeof(T); };
template <typename T> int64_t atSize(const T& val) { return sizeof(val); };

struct atTypeDesc
{
  atTypeDesc(const atType type, const int64_t count = 1)
    : type(type)
    , count(count)
    , size(atSize(type) * count)
  {}

  atType type;
  int64_t count;
  int64_t size;
};

// Return an enum representing the type
template <typename T> atType atGetType() { return atType_Unknown; }
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
template <typename T> atTypeDesc atGetTypeDesc() { return{ atGetType<T>(), 1}; }
template <typename T> atTypeDesc atGetTypeDesc(const T &unused) { atUnused(unused); return atGetTypeDesc<T>(); }

#endif