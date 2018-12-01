
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

#include "atFormat.h"
#include "atGraphics.h"

// enum atType
// {
//   atType_Float16,
//   atType_Float32,
//   atType_Float64,
// 
//   atType_Int8,
//   atType_Int16,
//   atType_Int32,
//   atType_Int64,
// 
//   atType_Uint8,
//   atType_Uint16,
//   atType_Uint32,
//   atType_Uint64,
//   atType_Count,
// };

#define FMT_MASK_RGB    0xFFFF0000
#define FMT_MASK_DEPTH  0x0000FFFF

static int64_t s_dxgiFormatLookup[atType_Count][4] = 
{ 
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16_FLOAT                   , // 1 x 16-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16G16_FLOAT                , // 2 x 16-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 3 x 16-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16G16B16A16_FLOAT          , // 4 x 16-bit float

  ((int64_t)DXGI_FORMAT_D32_FLOAT << 32) | DXGI_FORMAT_R32_FLOAT                   , // 1 x 32-bit float
  ((int64_t)DXGI_FORMAT_D32_FLOAT_S8X24_UINT << 32) | DXGI_FORMAT_R32G32_FLOAT     , // 2 x 32-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R32G32B32_FLOAT             , // 3 x 32-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R32G32B32A32_FLOAT          , // 4 x 32-bit float

  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 1 x 64-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 2 x 64-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 3 x 64-bit float
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 4 x 64-bit float


  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R8_SINT                     , // 1 x 8-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R8G8_SINT                   , // 2 x 8-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 3 x 8-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R8G8B8A8_SINT               , // 4 x 8-bit int

  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16_SINT                    , // 1 x 16-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16G16_SINT                 , // 2 x 16-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 3 x 16-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16G16B16A16_SINT           , // 4 x 16-bit int

  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R32_SINT                    , // 1 x 32-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R32G32_SINT                 , // 2 x 32-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R32G32B32_SINT              , // 3 x 32-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R32G32B32A32_SINT           , // 4 x 32-bit int

  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 1 x 64-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 2 x 64-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 3 x 64-bit int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 4 x 64-bit int


  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R8_UINT                     , // 1 x 8-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R8G8_UINT                   , // 2 x 8-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_UNKNOWN                     , // 3 x 8-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R8G8B8A8_UINT               , // 4 x 8-bit unsigned int

  ((int64_t)DXGI_FORMAT_D16_UNORM << 32) | DXGI_FORMAT_R16_UINT                    , // 1 x 16-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R16G16_UINT                 , // 2 x 16-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_UNKNOWN                     , // 3 x 16-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R16G16B16A16_UINT           , // 4 x 16-bit unsigned int

  ((int64_t)DXGI_FORMAT_D32_FLOAT << 32) | DXGI_FORMAT_R32_UINT                    , // 1 x 32-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R32G32_UINT                 , // 2 x 32-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_R32G32B32_UINT              , // 3 x 32-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_R32G32B32A32_UINT           , // 4 x 32-bit unsigned int

  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_UNKNOWN                     , // 1 x 64-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_UNKNOWN                     , // 2 x 64-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32) | DXGI_FORMAT_UNKNOWN                     , // 3 x 64-bit unsigned int
  ((int64_t)DXGI_FORMAT_UNKNOWN << 32)   | DXGI_FORMAT_UNKNOWN                     , // 4 x 64-bit unsigned int
};

int64_t atFormat::DXGI(const atType type, const int64_t width)
{
  atAssert(width <= DXGIMaxWidth(), "Width must be less than 4!");
  int64_t format = s_dxgiFormatLookup[type][width-1] & 0x0000FFFF;
  atAssert(format != DXGI_FORMAT_UNKNOWN, "Unknown DXGI Format!");
  return format;
}

int64_t atFormat::DXGIDepth(const atType type) { return DXGI(type, 1) >> sizeof(int32_t); }
