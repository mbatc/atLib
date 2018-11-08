
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

#ifndef atHardwareTexture_h__
#define atHardwareTexture_h__

#include "atFilename.h"

enum atTexCoordMode
{
  atTCM_Wrap = 1,           // D3D11_TEXTURE_ADDRESS_WRAP
  atTCM_Mirror = 2,         // D3D11_TEXTURE_ADDRESS_MIRROR
  atTCM_MirrorOnce = 5,     // D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
  atTCM_Clamp = 3,          // D3D11_TEXTURE_ADDRESS_CLAMP
  atTCM_Border = 4          // D3D11_TEXTURE_ADDRESS_BORDER
};

enum atComparison
{
  atComp_Equal = 3,         // D3D11_COMPARISON_EQUAL
  atComp_NotEqual = 6,      // D3D11_COMPARISON_NOT_EQUAL
  atComp_Greater = 5,       // D3D11_COMPARISON_GREATER
  atComp_Less = 2,          // D3D11_COMPARISON_LESS
  atComp_GreaterEqual = 7,  // D3D11_COMPARISON_GREATER_EQUAL
  atComp_LessEqual = 4,     // D3D11_COMPARISON_LESS_EQUAL
  atComp_Always = 8,        // D3D11_COMPARISON_ALWAYS
  atComp_Never = 1,         // D3D11_COMPARISON_NEVER
};

class atHardwareTexture
{
public:
  atHardwareTexture() = delete;

  static int64_t CreateSampler(const int64_t filter = 21 /*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/, const atTexCoordMode uMode = atTCM_Wrap, const atTexCoordMode vMode = atTCM_Wrap, const atTexCoordMode wMode = atTCM_Wrap, const float mipLodBias = 0, const atComparison compFunc = atComp_Never, const atVec4F &borderCol = { 0,0,0,0 }, const float minLOD = 0.f, const float maxLOD = FLT_MAX);
  static int64_t UploadTexture(const atFilename &file);

  static void* GetTexture(const int64_t id);
  static void* GetSampler(const int64_t id);

  static void DeleteTexture(const int64_t id);
  static void DeleteSampler(const int64_t id);
};

#endif // atHardwareTexture_h__
