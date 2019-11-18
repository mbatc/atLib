
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

#include "atTypes.h"

template<> int64_t atSize(const atType &val)
{
  switch (val)
  {
  case atType_Float32: return 4;
  case atType_Float64: return 8;
  case atType_Int8: return 1;
  case atType_Int16: return 2;
  case atType_Int32: return 4;
  case atType_Int64: return 8;
  case atType_Uint8: return 1;
  case atType_Uint16: return 2;
  case atType_Uint32: return 4;
  case atType_Uint64: return 8;
  default: return 0;
  }
}

template<> atType atGetType<bool>() { return atType_Uint8; }
template<> atType atGetType<char>() { return atType_Uint8; }
template<> atType atGetType<wchar_t>() { return atType_Uint16; }

template<> atType atGetType<int8_t>() { return atType_Int8; }
template<> atType atGetType<int16_t>() { return atType_Int16; }
template<> atType atGetType<int32_t>() { return atType_Int32; }
template<> atType atGetType<int64_t>() { return atType_Int64; }
template<> atType atGetType<uint8_t>() { return atType_Uint8; }
template<> atType atGetType<uint16_t>() { return atType_Uint16; }
template<> atType atGetType<uint32_t>() { return atType_Uint32; }
template<> atType atGetType<uint64_t>() { return atType_Uint64; }

template<> atType atGetType<float>() { return atType_Float32; }
template<> atType atGetType<double>() { return atType_Float64; }

void atUnused() {}

bool atTypeDesc::operator==(const atTypeDesc &o) const { return type == o.type && size == o.size && count == o.count; }
bool atTypeDesc::operator!=(const atTypeDesc &o) const { return !(o == *this); }

bool atTypeCast(void *pDst, const void *pSrc, const atTypeDesc &dstType, const atTypeDesc &srcType)
{
  int64_t dstCount = dstType.width * dstType.count;
  int64_t srcCount = srcType.width * srcType.count;
  int64_t count = dstCount < srcCount ? dstCount : srcCount;

  if (dstType.type == srcType.type)
  {
    memcpy(pDst, pSrc, count * srcType.size);
    return true;
  }

  switch (srcType.type)
  {
  case atType_Float16: return false;
  case atType_Float32: return __atTypeCast(pDst, (float*)pSrc, dstType.type, count);
  case atType_Float64: return __atTypeCast(pDst, (double*)pSrc, dstType.type, count);
  case atType_Int8: return __atTypeCast(pDst, (int8_t*)pSrc, dstType.type, count);
  case atType_Int16: return __atTypeCast(pDst, (int16_t*)pSrc, dstType.type, count);
  case atType_Int32: return __atTypeCast(pDst, (int32_t*)pSrc, dstType.type, count);
  case atType_Int64: return __atTypeCast(pDst, (int64_t*)pSrc, dstType.type, count);
  case atType_Uint8: return __atTypeCast(pDst, (uint8_t*)pSrc, dstType.type, count);
  case atType_Uint16: return __atTypeCast(pDst, (uint16_t*)pSrc, dstType.type, count);
  case atType_Uint32: return __atTypeCast(pDst, (uint32_t*)pSrc, dstType.type, count);
  case atType_Uint64: return __atTypeCast(pDst, (uint64_t*)pSrc, dstType.type, count);
  }
  return false;
}
