
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

#ifndef _atGraphics_h__
#define _atGraphics_h__

#include "atString.h"
#include "atDirectX.h"
#include "atOpenGL.h"

enum atShaderType
{
  atST_Vertex,
  atST_Pixel,
  atST_Compute,
  atST_Geometry,
  atST_Domain,
  atST_Hull
};

enum atShader_ResourceType
{
  atSRT_Texture,
  atSRT_Sampler,
  atSRT_Buffer
};

class atGraphics
{
public:
  static bool BindShaderResource(const atShaderType shader, const atShader_ResourceType resType, const int64_t slot, void *pResource);
  static bool CreateBuffer(ID3D11Buffer **ppBuffer, void *pData, int64_t size, int64_t binding, int64_t usage = D3D11_USAGE_DEFAULT, int64_t cpuAccess = 0);

  // Should be called before exiting any application using atGraphics
  static void Shutdown();
};

template<typename T> inline void atGraphics::SafeRelease(T &ref)
{
  if (!ref)
    return;
  ref->Release();
  ref = nullptr;
}

#endif // _atGraphics_h__
