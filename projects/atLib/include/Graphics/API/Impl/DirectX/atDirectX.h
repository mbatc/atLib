#ifndef atDirectX_h__
#define atDirectX_h__

#include "atString.h"

#include <d3d.h>
#include <dxgi.h>
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

class atDirectX
{
public:
  template<typename T> static void SafeRelease(T *&pBuffer) { if (pBuffer) pBuffer->Release(); pBuffer = nullptr; }

  static IDXGIOutput* GetOutputDisplay();
  static IDXGIFactory* GetFactory();
  static ID3D11Device* GetDevice();
  static IDXGIAdapter* GetDisplayAdapter();
  static ID3D11DeviceContext* GetContext();
  static atString GetAdapterDescription();
  static D3D_FEATURE_LEVEL GetFeatureLevel();
  static DWORD GetCreateFlags();

  static int64_t RefreshRateNumerator(const bool vsync);
  static int64_t RefreshRateDenominator(const bool vsync);
  
  static int64_t GFXMemorySize();

  static void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0);
  static void Draw(int64_t nVerts, int64_t startLocation = 0);

  static void Shutdown();

protected:
  static void CreateDeviceAndContext();
  static void CreateOutput();
  static void CreateAdapter();
  static void CreateFactory();
  
  static ID3D11Device *m_pDevice;
  static IDXGIAdapter *m_pAdapter;
  static IDXGIFactory *m_pFactory;
  static IDXGIOutput *m_pOutputDisplay;
  static ID3D11DeviceContext *m_pContext;
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

#endif // atDirectX_h__
