
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

#include "atFormat.h"
#include "atOpenGL.h"
#include "atDirectX.h"
#include "atDXInclude_Internal.h"

#define FMT_MASK_RGB    0xFFFF0000
#define FMT_MASK_DEPTH  0x0000FFFF

#ifdef atPLATFORM_WIN32

int64_t atFormat::DXGI(const atType &type, const int64_t &width)
{
  static int64_t fmtLookup[atType_Count][4] =
  {
    DXGI_FORMAT_R16_FLOAT                   , // 1 x 16-bit float
    DXGI_FORMAT_R16G16_FLOAT                , // 2 x 16-bit float
    DXGI_FORMAT_UNKNOWN                     , // 3 x 16-bit float
    DXGI_FORMAT_R16G16B16A16_FLOAT          , // 4 x 16-bit float

    DXGI_FORMAT_R32_FLOAT                   , // 1 x 32-bit float
    DXGI_FORMAT_R32G32_FLOAT     , // 2 x 32-bit float
    DXGI_FORMAT_R32G32B32_FLOAT             , // 3 x 32-bit float
    DXGI_FORMAT_R32G32B32A32_FLOAT          , // 4 x 32-bit float
    
    DXGI_FORMAT_UNKNOWN                     , // 1 x 64-bit float
    DXGI_FORMAT_UNKNOWN                     , // 2 x 64-bit float
    DXGI_FORMAT_UNKNOWN                     , // 3 x 64-bit float
    DXGI_FORMAT_UNKNOWN                     , // 4 x 64-bit float
    
    
    DXGI_FORMAT_R8_SINT                     , // 1 x 8-bit int
    DXGI_FORMAT_R8G8_SINT                   , // 2 x 8-bit int
    DXGI_FORMAT_UNKNOWN                     , // 3 x 8-bit int
    DXGI_FORMAT_R8G8B8A8_SINT               , // 4 x 8-bit int
    
    DXGI_FORMAT_R16_SINT                    , // 1 x 16-bit int
    DXGI_FORMAT_R16G16_SINT                 , // 2 x 16-bit int
    DXGI_FORMAT_UNKNOWN                     , // 3 x 16-bit int
    DXGI_FORMAT_R16G16B16A16_SINT           , // 4 x 16-bit int
    
    DXGI_FORMAT_R32_SINT                    , // 1 x 32-bit int
    DXGI_FORMAT_R32G32_SINT                 , // 2 x 32-bit int
    DXGI_FORMAT_R32G32B32_SINT              , // 3 x 32-bit int
    DXGI_FORMAT_R32G32B32A32_SINT           , // 4 x 32-bit int
    
    DXGI_FORMAT_UNKNOWN                     , // 1 x 64-bit int
    DXGI_FORMAT_UNKNOWN                     , // 2 x 64-bit int
    DXGI_FORMAT_UNKNOWN                     , // 3 x 64-bit int
    DXGI_FORMAT_UNKNOWN                     , // 4 x 64-bit int


    DXGI_FORMAT_R8_UINT                     , // 1 x 8-bit unsigned int
    DXGI_FORMAT_R8G8_UINT                   , // 2 x 8-bit unsigned int
    DXGI_FORMAT_UNKNOWN                     , // 3 x 8-bit unsigned int
    DXGI_FORMAT_R8G8B8A8_UINT               , // 4 x 8-bit unsigned int

    DXGI_FORMAT_R16_UINT                    , // 1 x 16-bit unsigned int
    DXGI_FORMAT_R16G16_UINT                 , // 2 x 16-bit unsigned int
    DXGI_FORMAT_UNKNOWN                     , // 3 x 16-bit unsigned int
    DXGI_FORMAT_R16G16B16A16_UINT           , // 4 x 16-bit unsigned int

    DXGI_FORMAT_R8G8B8A8_UNORM                    , // 1 x 32-bit unsigned int
    DXGI_FORMAT_R32G32_UINT                 , // 2 x 32-bit unsigned int
    DXGI_FORMAT_R32G32B32_UINT              , // 3 x 32-bit unsigned int
    DXGI_FORMAT_R32G32B32A32_UINT           , // 4 x 32-bit unsigned int
    
    DXGI_FORMAT_UNKNOWN                     , // 1 x 64-bit unsigned int
    DXGI_FORMAT_UNKNOWN                     , // 2 x 64-bit unsigned int
    DXGI_FORMAT_UNKNOWN                     , // 3 x 64-bit unsigned int
    DXGI_FORMAT_UNKNOWN                     , // 4 x 64-bit unsigned int
  };

  atAssert(width <= DXGIMaxWidth(), "Width must be less than 4!");
  int64_t format = fmtLookup[type][width-1] & 0x0000FFFF;
  atAssert(format != DXGI_FORMAT_UNKNOWN, "Unknown DXGI Format!");
  return format;
}

int64_t atFormat::DXGIDepth(const atType &type)
{
  static int64_t fmtLookup[atType_Count][4] =
  {
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_D16_UNORM,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_D32_FLOAT,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_UNKNOWN
  };

  int64_t format = fmtLookup[type][0] & 0x0000FFFF;
  atAssert(format != DXGI_FORMAT_UNKNOWN, "Unknown DXGI Format!");
  return format;
}

#else

int64_t atFormat::DXGI(const atType &, const int64_t &) { return 0; }
int64_t atFormat::DXGIDepth(const atType &) { return 0; }

#endif

int64_t atFormat::GL(const atType &type, const int64_t &width)
{
  static int64_t fmtLookup[atType_Count][4] =
  {
    GL_RED, // 1 x 16-bit float
    GL_RG, // 2 x 16-bit float
    GL_RGB, // 3 x 16-bit float
    GL_RGBA, // 4 x 16-bit float
    GL_RED, // 1 x 32-bit float
    GL_RG, // 2 x 32-bit float
    GL_RGB, // 3 x 32-bit float
    GL_RGBA, // 4 x 32-bit float
    GL_NONE, // 1 x 64-bit float
    GL_NONE, // 2 x 64-bit float
    GL_NONE, // 3 x 64-bit float
    GL_NONE, // 4 x 64-bit float
    GL_RED, // 1 x 8-bit int
    GL_RG, // 2 x 8-bit int
    GL_RGB, // 3 x 8-bit int
    GL_RGBA, // 4 x 8-bit int
    GL_RED_INTEGER, // 1 x 16-bit int
    GL_RG_INTEGER, // 2 x 16-bit int
    GL_RGB_INTEGER, // 3 x 16-bit int
    GL_RGBA_INTEGER, // 4 x 16-bit int
    GL_RED_INTEGER, // 1 x 32-bit int
    GL_RG_INTEGER, // 2 x 32-bit int
    GL_RGB_INTEGER, // 3 x 32-bit int
    GL_RGBA_INTEGER, // 4 x 32-bit int
    GL_NONE, // 1 x 64-bit int
    GL_NONE, // 2 x 64-bit int
    GL_NONE, // 3 x 64-bit int
    GL_NONE, // 4 x 64-bit int
    GL_RED, // 1 x 8-bit unsigned int
    GL_RG, // 2 x 8-bit unsigned int
    GL_RGB, // 3 x 8-bit unsigned int
    GL_RGBA, // 4 x 8-bit unsigned int
    GL_RED_INTEGER, // 1 x 16-bit unsigned int
    GL_RG_INTEGER, // 2 x 16-bit unsigned int
    GL_RGB_INTEGER, // 3 x 16-bit unsigned int
    GL_RGBA_INTEGER, // 4 x 16-bit unsigned int
    GL_RED_INTEGER, // 1 x 32-bit unsigned int
    GL_RG_INTEGER, // 2 x 32-bit unsigned int
    GL_RGB_INTEGER, // 3 x 32-bit unsigned int
    GL_RGBA_INTEGER, // 4 x 32-bit unsigned int
    GL_RED_INTEGER, // 1 x 64-bit unsigned int
    GL_RG_INTEGER, // 2 x 64-bit unsigned int
    GL_RGB_INTEGER, // 3 x 64-bit unsigned int
    GL_RGBA_INTEGER, // 4 x 64-bit unsigned int
  };

  atAssert(width <= GLMaxWidth(), "Width must be less than 4!");
  int64_t format = fmtLookup[type][width - 1] & 0x0000FFFF;
  atAssert(format != GL_NONE, "Unknown DXGI Format!");
  return format;
}

int64_t atFormat::GLInternal(const atType &type, const int64_t &width)
{
  static int64_t fmtLookup[atType_Count][4] =
  {
    GL_R16F, // 1 x 16-bit float
    GL_RG16F, // 2 x 16-bit float
    GL_RGB16F, // 3 x 16-bit float
    GL_RGBA16F, // 4 x 16-bit float
    GL_R32F, // 1 x 32-bit float
    GL_RG32F, // 2 x 32-bit float
    GL_RGB32F, // 3 x 32-bit float
    GL_RGBA32F, // 4 x 32-bit float
    GL_NONE, // 1 x 64-bit float
    GL_NONE, // 2 x 64-bit float
    GL_NONE, // 3 x 64-bit float
    GL_NONE, // 4 x 64-bit float
    GL_R8_SNORM, // 1 x 8-bit int
    GL_RG8_SNORM, // 2 x 8-bit int
    GL_RGB8_SNORM, // 3 x 8-bit int
    GL_RGBA8_SNORM, // 4 x 8-bit int
    GL_R16I, // 1 x 16-bit int
    GL_RG16I, // 2 x 16-bit int
    GL_RGB16I, // 3 x 16-bit int
    GL_RGBA16I, // 4 x 16-bit int
    GL_R32I, // 1 x 32-bit int
    GL_RG32I, // 2 x 32-bit int
    GL_RGB32I, // 3 x 32-bit int
    GL_RGBA32I, // 4 x 32-bit 
    GL_NONE, // 1 x 64-bit int
    GL_NONE, // 2 x 64-bit int
    GL_NONE, // 3 x 64-bit int
    GL_NONE, // 4 x 64-bit int
    GL_R8UI, // 1 x 8-bit unsigned int
    GL_RG8UI, // 2 x 8-bit unsigned int
    GL_RGB8UI, // 3 x 8-bit unsigned int
    GL_RGBA8UI, // 4 x 8-bit unsigned int
    GL_R16UI, // 1 x 16-bit unsigned int
    GL_RG16UI, // 2 x 16-bit unsigned int
    GL_RGB16UI, // 3 x 16-bit unsigned int
    GL_RGBA16UI, // 4 x 16-bit unsigned int
    GL_R32UI, // 1 x 32-bit unsigned int
    GL_RG32UI, // 2 x 32-bit unsigned int
    GL_RGB32UI, // 3 x 32-bit unsigned int
    GL_RGBA32UI, // 4 x 32-bit unsigned int
    GL_NONE, // 1 x 64-bit unsigned int
    GL_NONE, // 2 x 64-bit unsigned int
    GL_NONE, // 3 x 64-bit unsigned int
    GL_NONE, // 4 x 64-bit unsigned int
  };

  atAssert(width <= GLMaxWidth(), "Width must be less than 4!");
  int64_t format = fmtLookup[type][width - 1] & 0x0000FFFF;
  atAssert(format != GL_NONE, "Unknown DXGI Format!");
  return format;
}

int64_t atFormat::GLType(const atType &type)
{
  switch (type)
  {
  case atType_Float16: return GL_HALF_FLOAT;
  case atType_Float32: return GL_FLOAT;
  case atType_Float64: return GL_NONE;
  case atType_Int8:    return GL_BYTE;
  case atType_Int16:   return GL_SHORT;
  case atType_Int32:   return GL_INT;
  case atType_Int64:   return GL_NONE;
  case atType_Uint8:   return GL_UNSIGNED_BYTE;
  case atType_Uint16:  return GL_UNSIGNED_SHORT;
  case atType_Uint32:  return GL_UNSIGNED_INT;
  case atType_Uint64:  return GL_NONE;
  }

  return GL_NONE;
}

int64_t atFormat::GLDepth(const atType &type)
{
  static int64_t fmtLookup[atType_Count][4] =
  {
    GL_DEPTH_COMPONENT, // 1 x 16-bit float
    GL_NONE, // 2 x 16-bit float
    GL_DEPTH_COMPONENT, // 3 x 16-bit float
    GL_NONE, // 4 x 16-bit float
    GL_DEPTH_COMPONENT, // 1 x 32-bit float
    GL_NONE, // 2 x 32-bit float
    GL_NONE, // 3 x 32-bit float
    GL_NONE, // 4 x 32-bit float
    GL_NONE, // 1 x 64-bit float
    GL_NONE, // 2 x 64-bit float
    GL_NONE, // 3 x 64-bit float
    GL_NONE, // 4 x 64-bit float
    GL_NONE, // 1 x 8-bit int
    GL_NONE, // 2 x 8-bit int
    GL_NONE, // 3 x 8-bit int
    GL_NONE, // 4 x 8-bit int
    GL_NONE, // 1 x 16-bit int
    GL_NONE, // 2 x 16-bit int
    GL_NONE, // 3 x 16-bit int
    GL_NONE, // 4 x 16-bit int
    GL_NONE, // 1 x 32-bit int
    GL_NONE, // 2 x 32-bit int
    GL_NONE, // 3 x 32-bit int
    GL_NONE, // 4 x 32-bit int
    GL_NONE, // 1 x 64-bit int
    GL_NONE, // 2 x 64-bit int
    GL_NONE, // 3 x 64-bit int
    GL_NONE, // 4 x 64-bit int
    GL_NONE, // 1 x 8-bit unsigned int
    GL_NONE, // 2 x 8-bit unsigned int
    GL_NONE, // 3 x 8-bit unsigned int
    GL_NONE, // 4 x 8-bit unsigned int
    GL_NONE, // 1 x 16-bit unsigned int
    GL_NONE, // 2 x 16-bit unsigned int
    GL_NONE, // 3 x 16-bit unsigned int
    GL_NONE, // 4 x 16-bit unsigned int
    GL_NONE, // 1 x 32-bit unsigned int
    GL_NONE, // 2 x 32-bit unsigned int
    GL_NONE, // 3 x 32-bit unsigned int
    GL_NONE, // 4 x 32-bit unsigned int
    GL_NONE, // 1 x 64-bit unsigned int
    GL_NONE, // 2 x 64-bit unsigned int
    GL_NONE, // 3 x 64-bit unsigned int
    GL_NONE, // 4 x 64-bit unsigned int
  };

  int64_t format = fmtLookup[type][0] >> 32;
  atAssert(format != GL_NONE, "Unknown GL Format!");
  return format;
}

int64_t atFormat::GLDepthInternal(const atType &type)
{
  static int64_t fmtLookup[atType_Count][4] =
  {
    GL_DEPTH_COMPONENT16, // 1 x 16-bit float
    GL_NONE, // 2 x 16-bit float
    GL_DEPTH_COMPONENT24, // 3 x 16-bit float
    GL_NONE, // 4 x 16-bit float
    GL_DEPTH_COMPONENT32, // 1 x 32-bit float
    GL_NONE, // 2 x 32-bit float
    GL_NONE, // 3 x 32-bit float
    GL_NONE, // 4 x 32-bit float
    GL_NONE, // 1 x 64-bit float
    GL_NONE, // 2 x 64-bit float
    GL_NONE, // 3 x 64-bit float
    GL_NONE, // 4 x 64-bit float
    GL_NONE, // 1 x 8-bit int
    GL_NONE, // 2 x 8-bit int
    GL_NONE, // 3 x 8-bit int
    GL_NONE, // 4 x 8-bit int
    GL_NONE, // 1 x 16-bit int
    GL_NONE, // 2 x 16-bit int
    GL_NONE, // 3 x 16-bit int
    GL_NONE, // 4 x 16-bit int
    GL_NONE, // 1 x 32-bit int
    GL_NONE, // 2 x 32-bit int
    GL_NONE, // 3 x 32-bit int
    GL_NONE, // 4 x 32-bit int
    GL_NONE, // 1 x 64-bit int
    GL_NONE, // 2 x 64-bit int
    GL_NONE, // 3 x 64-bit int
    GL_NONE, // 4 x 64-bit int
    GL_NONE, // 1 x 8-bit unsigned int
    GL_NONE, // 2 x 8-bit unsigned int
    GL_NONE, // 3 x 8-bit unsigned int
    GL_NONE, // 4 x 8-bit unsigned int
    GL_NONE, // 1 x 16-bit unsigned int
    GL_NONE, // 2 x 16-bit unsigned int
    GL_NONE, // 3 x 16-bit unsigned int
    GL_NONE, // 4 x 16-bit unsigned int
    GL_NONE, // 1 x 32-bit unsigned int
    GL_NONE, // 2 x 32-bit unsigned int
    GL_NONE, // 3 x 32-bit unsigned int
    GL_NONE, // 4 x 32-bit unsigned int
    GL_NONE, // 1 x 64-bit unsigned int
    GL_NONE, // 2 x 64-bit unsigned int
    GL_NONE, // 3 x 64-bit unsigned int
    GL_NONE, // 4 x 64-bit unsigned int
  };

  int64_t format = fmtLookup[type][0] >> 32;
  atAssert(format != GL_NONE, "Unknown DXGI Format!");
  return format;
}
