
#ifdef atPLATFORM_WIN32

#include "atDirectXState.h"
#include "atRenderState.h"
#include "atDXTexture.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atDXInclude_Internal.h"
#include "atWindow.h"
#include "atFormat.h"

#define _GetContext()        ((ID3D11DeviceContext*)    GetContext())
#define _GetDevice()         ((ID3D11Device*)           GetDevice())
#define _GetSwapChain()      ((IDXGISwapChain*)         GetSwapChain())
#define _GetBackbuffer()     ((ID3D11RenderTargetView*) GetBackbuffer())
#define _GetDepthBuffer()    ((ID3D11DepthStencilView*) GetDepthBuffer())
#define _GetOutputDisplay()  ((IDXGIOutput*)            GetOutputDisplay())
#define _GetFactory()        ((IDXGIFactory*)           GetFactory())
#define _GetDisplayAdapter() ((IDXGIAdapter*)           GetDisplayAdapter())

struct __atDirectXImpl
{
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

#define m_pDX ((__atDirectXImpl*)this->m_pImpl)

void* atDirectX::GetContext()
{
  if (!m_pDX->m_pContext)
    CreateDeviceAndContext();
  return m_pDX->m_pContext;
}

void* atDirectX::GetDevice()
{
  if (!m_pDX->m_pDevice)
    CreateDeviceAndContext();
  return m_pDX->m_pDevice;
}

void* atDirectX::GetDisplayAdapter()
{
  if (!m_pDX->m_pAdapter)
    CreateAdapter();
  return m_pDX->m_pAdapter;
}

atDirectX::atDirectX(atWindow *pWindow, const bool &vsyncEnabled)
{
  m_pImpl = atNew<__atDirectXImpl>();

  memset(m_pDX->m_adapterDesc, 0, sizeof(m_pDX->m_adapterDesc));
  m_pDX->m_featureList[0] = D3D_FEATURE_LEVEL_11_0;
  m_pDX->m_featureList[1] = D3D_FEATURE_LEVEL_11_1;
  m_pDX->m_featureList[2] = D3D_FEATURE_LEVEL_10_0;
  m_pDX->m_featureList[3] = D3D_FEATURE_LEVEL_10_1;
  m_pDX->m_featureList[4] = D3D_FEATURE_LEVEL_9_1;
  m_pDX->m_featureList[5] = D3D_FEATURE_LEVEL_9_2;
  m_pDX->m_featureList[6] = D3D_FEATURE_LEVEL_9_1;
  m_pDX->m_gfxMemory = 0;
  m_pDX->m_flags = 0;
  m_pDX->m_refresh.den = 1;
  m_pDX->m_refresh.num = 0;
  m_pDX->m_refresh.defDen = 1;
  m_pDX->m_refresh.defNum = 0;
  m_pDX->m_refresh.vsync = vsyncEnabled;
  m_pState = atNew<atDirectXState>();

  CreateSwapChain(pWindow, vsyncEnabled);
}

atDirectX::~atDirectX()
{
  atDelete(m_pDX);
  m_pImpl = nullptr;
}

void atDirectX::Swap() { m_pDX->m_pSwapChain->Present(GetVsyncEnabled(), 0); }

void atDirectX::Resize(const atVec2I &size)
{
  SafeRelease(m_pDX->m_pBackBuffer);
  SafeRelease(m_pDX->m_pDepthBuffer);
  m_pDX->m_pSwapChain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
  CreateBackBufferView();
  CreateDepthBuffer(size);
  _GetContext()->OMSetRenderTargets(1, &m_pDX->m_pBackBuffer, m_pDX->m_pDepthBuffer);
}

void atDirectX::ClearColour(const atVec4F &colour) { m_pDX->m_pContext->ClearRenderTargetView(m_pDX->m_pBackBuffer, &colour[0]); }
void atDirectX::ClearDepth(const float &depth) { m_pDX->m_pContext->ClearDepthStencilView(m_pDX->m_pDepthBuffer, D3D11_CLEAR_DEPTH, depth, 0xFF); }
void atDirectX::ClearStencil() { m_pDX->m_pContext->ClearDepthStencilView(m_pDX->m_pDepthBuffer, D3D11_CLEAR_STENCIL, 0, 0xFF); }
void atDirectX::SetWindowed(const bool &windowed) { m_pDX->m_pSwapChain->SetFullscreenState(!windowed, nullptr); }

void* atDirectX::GetOutputDisplay()
{
  if (!m_pDX->m_pOutputDisplay)
    CreateOutput();
  return m_pDX->m_pOutputDisplay;
}

void* atDirectX::GetFactory()
{
  if (!m_pDX->m_pFactory)
    CreateFactory();
  return m_pDX->m_pFactory;
}

int64_t atDirectX::GFXMemorySize()
{
  GetOutputDisplay();
  return m_pDX->m_gfxMemory;
}

void atDirectX::Shutdown()
{
  SafeRelease(m_pDX->m_pAdapter);
  SafeRelease(m_pDX->m_pFactory);
  SafeRelease(m_pDX->m_pOutputDisplay);
  SafeRelease(m_pDX->m_pDevice);
  SafeRelease(m_pDX->m_pContext);
}

atGraphicsAPI atDirectX::API() { return atGfxApi_DirectX; }

void atDirectX::CreateSwapChain(atWindow *pWindow, const bool &vsyncEnabled)
{
  if (m_pDX->m_pDepthBuffer && m_pDX->m_pSwapChain)
    return;

  SafeRelease(m_pDX->m_pSwapChain);
  SafeRelease(m_pDX->m_pDepthBuffer);

  DXGI_SWAP_CHAIN_DESC desc;
  memset(&desc, 0, sizeof(desc));
  desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.BufferDesc.RefreshRate.Numerator = (UINT)atDirectX::RefreshRateNumerator(vsyncEnabled);
  desc.BufferDesc.RefreshRate.Denominator = (UINT)atDirectX::RefreshRateDenominator(vsyncEnabled);
  desc.BufferDesc.Width = pWindow->Size().x;
  desc.BufferDesc.Height = pWindow->Size().y;
  desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.OutputWindow = pWindow->Handle();
  desc.BufferCount = 1;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.Windowed = pWindow->IsWindowed();
  desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  if (FAILED(_GetFactory()->CreateSwapChain(_GetDevice(), &desc, &m_pDX->m_pSwapChain)))
  {
    m_pDX->m_pSwapChain = nullptr;
    return;
  }

  CreateBackBufferView();
  CreateDepthBuffer(pWindow->Size());
  _GetContext()->OMSetRenderTargets(1, &m_pDX->m_pBackBuffer, m_pDX->m_pDepthBuffer);
}

void atDirectX::CreateDeviceAndContext()
{
  if (m_pDX->m_pDevice || m_pDX->m_pContext || !GetDisplayAdapter())
    return;

#ifdef _DEBUG
  m_flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, nullptr, m_pDX->m_flags, m_pDX->m_featureList, sizeof(m_pDX->m_featureList) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION,
    &m_pDX->m_pDevice, &m_pDX->m_featureLevel, &m_pDX->m_pContext);
}

void atDirectX::CreateOutput()
{
  if (m_pDX->m_pOutputDisplay || !GetDisplayAdapter())
    return;

  if (FAILED(m_pDX->m_pAdapter->EnumOutputs(0, &m_pDX->m_pOutputDisplay)))
  {
    m_pDX->m_pOutputDisplay = nullptr;
    return;
  }

  UINT numModes;
  if (FAILED(m_pDX->m_pOutputDisplay->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
    return;

  m_pDX->m_displayModeList.resize(numModes);
  if (FAILED(m_pDX->m_pOutputDisplay->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, m_pDX->m_displayModeList.data())))
    return;

  for (int64_t i = 0; i < numModes; ++i)
    if (m_pDX->m_displayModeList[i].Width == (UINT)GetSystemMetrics(SM_CXSCREEN) && m_pDX->m_displayModeList[i].Height == (UINT)GetSystemMetrics(SM_CYSCREEN))
    {
      m_pDX->m_refresh.den = m_pDX->m_displayModeList[i].RefreshRate.Denominator;
      m_pDX->m_refresh.num = m_pDX->m_displayModeList[i].RefreshRate.Numerator;
    }

  DXGI_ADAPTER_DESC dxgiDesc;
  if (FAILED(m_pDX->m_pAdapter->GetDesc(&dxgiDesc)))
    return;

  m_pDX->m_gfxMemory = dxgiDesc.DedicatedVideoMemory;
  size_t charsConverted = 0;

  if (wcstombs_s(&charsConverted, m_pDX->m_adapterDesc, dxgiDesc.Description, 128))
    return;
}

void atDirectX::CreateAdapter()
{
  if (m_pDX->m_pAdapter || !GetFactory())
    return;

  if (FAILED(m_pDX->m_pFactory->EnumAdapters(0, &m_pDX->m_pAdapter)))
    m_pDX->m_pAdapter = nullptr;
}

void atDirectX::CreateFactory()
{
  if (m_pDX->m_pFactory)
    return;

  if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pDX->m_pFactory)))
    m_pDX->m_pFactory = nullptr;
}

void atDirectX::CreateBackBufferView()
{
  ID3D11Texture2D *pBBTex = nullptr;
  if (FAILED(m_pDX->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBBTex)))
    pBBTex = nullptr;

  _GetDevice()->CreateRenderTargetView(pBBTex, nullptr, &m_pDX->m_pBackBuffer);
  pBBTex->Release();
}

void atDirectX::CreateDepthBuffer(const atVec2I &size)
{
  if (m_pDX->m_pDepthBuffer)
    SafeRelease(m_pDX->m_pDepthBuffer);

  D3D11_TEXTURE2D_DESC texDesc{};
  texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  texDesc.Format = (DXGI_FORMAT)atFormat::DXGIDepth(atType_Float32);
  texDesc.Height = (UINT)size.y;
  texDesc.Width = (UINT)size.x;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.SampleDesc.Quality = 0;
  texDesc.SampleDesc.Count = 1;
  texDesc.ArraySize = 1;
  texDesc.MipLevels = 0;
  texDesc.MiscFlags = 0;
  ID3D11Texture2D *pDepthTex = nullptr;
  _GetDevice()->CreateTexture2D(&texDesc, nullptr, &pDepthTex);
  _GetDevice()->CreateDepthStencilView(pDepthTex, nullptr, &m_pDX->m_pDepthBuffer);
  SafeRelease(pDepthTex);
}

void atDirectX::DrawIndexed(int64_t nIndices, int64_t startLocation, int64_t baseVertIndex, const atGFX_PrimitiveType &primType, const atType &indicesType)
{
  atUnused(indicesType);

  atRenderState::Bind();
  D3D11_PRIMITIVE_TOPOLOGY topology;
  switch (primType)
  {
  case atGFX_PT_PointList: topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
  case atGFX_PT_LineList: topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
  case atGFX_PT_TriangleList: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
  case atGFX_PT_TriangleStrip: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
  case atGFX_PT_LineListAdj: topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ; break;
  case atGFX_PT_TriangleListAdj: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ; break;
  }

  _GetContext()->IASetPrimitiveTopology(topology);
  _GetContext()->DrawIndexed((UINT)nIndices, (UINT)startLocation, (UINT)baseVertIndex);
}

void atDirectX::Draw(int64_t nVerts, int64_t startLocation, const atGFX_PrimitiveType &primType)
{
  atRenderState::Bind();
  _GetContext()->Draw((UINT)nVerts, (UINT)startLocation);
}

int64_t atDirectX::RefreshRateNumerator(const bool vsync)
{
  GetOutputDisplay();
  return vsync ? m_pDX->m_refresh.num : m_pDX->m_refresh.defNum;
}

int64_t atDirectX::RefreshRateDenominator(const bool vsync)
{
  GetOutputDisplay();
  return vsync ? m_pDX->m_refresh.den : m_pDX->m_refresh.defDen;
}

int64_t atDirectX::GetFeatureLevel()
{
  GetDevice();
  return (int64_t)m_pDX->m_featureLevel;
}

int64_t atDirectX::GetCreateFlags() { return m_pDX->m_flags; }
bool atDirectX::GetVsyncEnabled() const { return m_pDX->m_refresh.vsync; }
atString atDirectX::GetAdapterDescription() { return m_pDX->m_adapterDesc; }
atGFXContextState* atDirectX::GetState() { return m_pState; }
void* atDirectX::GetSwapChain() { return m_pDX->m_pSwapChain; }
void* atDirectX::GetBackbuffer() { return m_pDX->m_pBackBuffer; }
void* atDirectX::GetDepthBuffer() { return m_pDX->m_pDepthBuffer; }

#endif
