
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

#include <d3d.h>
#include <dxgi.h>
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

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
  static IDXGIOutput* GetOutputDisplay();
  static IDXGIFactory* GetFactory();
  static ID3D11Device* GetDevice();
  static IDXGIAdapter* GetDisplayAdapter();
  static ID3D11DeviceContext* GetContext();
  static atString GetAdapterDescription();
  static D3D_FEATURE_LEVEL GetFeatureLevel();
  static DWORD GetCreateFlags();

  template <class T> static void SafeRelease(T &ref);
  static int64_t GFXMemorySize();

  static int64_t RefreshRateNumerator(const bool vsync = false);
  static int64_t RefreshRateDenominator(const bool vsync = false);

  static bool BindShaderResource(const atShaderType shader, const atShader_ResourceType resType, const int64_t slot, void *pResource);

  static void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0);
  static void Draw(int64_t nVerts, int64_t startLocation = 0);

  // Should be called before exiting any application using atGraphics
  static void Shutdown();

protected:
  static void CreateDeviceAndContext();
  static void CreateOutput();
  static void CreateAdapter();
  static void CreateFactory();

  static ID3D11Device *m_pDevice;
  static ID3D11DeviceContext *m_pContext;
  static IDXGIAdapter *m_pAdapter;
  static IDXGIOutput *m_pOutputDisplay;
  static IDXGIFactory *m_pFactory;
  static atVector<DXGI_MODE_DESC> m_displayModeList;

  static DWORD m_flags;
  static int64_t m_gfxMemory;
  static D3D_FEATURE_LEVEL m_featureList[];
  static D3D_FEATURE_LEVEL m_featureLevel;
  static char m_adapterDesc[128];
  struct RefreshRate
  {
    static int64_t num;
    static int64_t den;
    static int64_t defNum;
    static int64_t defDen;
  };
};

template<class T> inline void atGraphics::SafeRelease(T &ref)
{
  if (!ref)
    return;
  ref->Release();
  ref = nullptr;
}

// ----------------------------------------------------------------------------------------
// DirectX Type Description Comparison Functions

bool operator==(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs);
bool operator==(const D3D11_BLEND_DESC & lhs, const D3D11_BLEND_DESC & rhs);
bool operator==(const D3D11_RASTERIZER_DESC & lhs, const D3D11_RASTERIZER_DESC & rhs);
bool operator==(const D3D11_DEPTH_STENCIL_DESC & lhs, const D3D11_DEPTH_STENCIL_DESC & rhs);

bool operator!=(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs);
bool operator!=(const D3D11_BLEND_DESC & lhs, const D3D11_BLEND_DESC & rhs);
bool operator!=(const D3D11_RASTERIZER_DESC & lhs, const D3D11_RASTERIZER_DESC & rhs);
bool operator!=(const D3D11_DEPTH_STENCIL_DESC & lhs, const D3D11_DEPTH_STENCIL_DESC & rhs);

atTrivialStreamRead(D3D11_BLEND_DESC);
atTrivialStreamRead(D3D11_SAMPLER_DESC);
atTrivialStreamRead(D3D11_RASTERIZER_DESC);
atTrivialStreamRead(D3D11_DEPTH_STENCIL_DESC);

atTrivialStreamWrite(D3D11_BLEND_DESC);
atTrivialStreamWrite(D3D11_SAMPLER_DESC);
atTrivialStreamWrite(D3D11_RASTERIZER_DESC);
atTrivialStreamWrite(D3D11_DEPTH_STENCIL_DESC);

// ----------------------------------------------------------------------------------------
#endif // _atGraphics_h__
