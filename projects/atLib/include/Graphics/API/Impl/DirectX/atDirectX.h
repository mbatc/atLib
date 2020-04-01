#ifndef atDirectX_h__
#define atDirectX_h__

#ifdef atPLATFORM_WINDOWS

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

class atWindow;
class atDirectXState;

class atDirectX
{
public:
  atDirectX(atWindow *pWindow, const bool &vsyncEnabled = true);
  
  atDirectXState* GetState();
  IDXGISwapChain* GetSwapChain();
  ID3D11RenderTargetView* GetBackbuffer();
  ID3D11DepthStencilView* GetDepthBuffer();
  IDXGIOutput* GetOutputDisplay();
  IDXGIFactory* GetFactory();
  ID3D11Device* GetDevice();
  IDXGIAdapter* GetDisplayAdapter();
  ID3D11DeviceContext* GetContext();
  atString GetAdapterDescription();
  D3D_FEATURE_LEVEL GetFeatureLevel();
  DWORD GetCreateFlags();

  void ResizeSwapChain(const atVec2I &size);

  bool GetVsyncEnabled() const;
  int64_t RefreshRateNumerator(const bool vsync);
  int64_t RefreshRateDenominator(const bool vsync);
  
  int64_t GFXMemorySize();

  void DrawIndexed(int64_t nIndices, int64_t startLocation = 0, int64_t baseVertIndex = 0);
  void Draw(int64_t nVerts, int64_t startLocation = 0);

  void Shutdown();

  template<typename T> static void SafeRelease(T *&pBuffer) { if (pBuffer) pBuffer->Release(); pBuffer = nullptr; }

protected:
  void CreateSwapChain(atWindow *pWindow, const bool &vsyncEnabled);
  void CreateDeviceAndContext();
  void CreateOutput();
  void CreateAdapter();
  void CreateFactory();
  void CreateBackBufferView();
  void CreateDepthBuffer(const atVec2I &size);

  atDirectXState *m_pState = nullptr;
  ID3D11Device *m_pDevice = nullptr;
  IDXGISwapChain *m_pSwapChain = nullptr;
  IDXGIAdapter *m_pAdapter = nullptr;
  IDXGIFactory *m_pFactory = nullptr;
  IDXGIOutput *m_pOutputDisplay = nullptr;
  ID3D11DeviceContext *m_pContext = nullptr;
  ID3D11RenderTargetView *m_pBackBuffer = nullptr;
  ID3D11DepthStencilView *m_pDepthBuffer = nullptr;

  atVector<DXGI_MODE_DESC> m_displayModeList;
  DWORD m_flags = 0;
  int64_t m_gfxMemory = 0;
  D3D_FEATURE_LEVEL m_featureList[7];
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_9_1;
  char m_adapterDesc[128];
  
  struct RefreshRate
  {
    int64_t num = 0;
    int64_t den = 0;
    int64_t defNum = 0;
    int64_t defDen = 0;
    bool vsync = false;
  } m_refresh;
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

#endif

#endif // atDirectX_h__
