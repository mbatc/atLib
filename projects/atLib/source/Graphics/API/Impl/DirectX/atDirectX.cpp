#ifdef atPLATFORM_WIN32

#include "atDirectXState.h"
#include "atRenderState.h"
#include "atDXTexture.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atWindow.h"
#include "atFormat.h"

ID3D11DeviceContext* atDirectX::GetContext()
{
  if (!m_pContext)
    CreateDeviceAndContext();
  return m_pContext;
}

ID3D11Device* atDirectX::GetDevice()
{
  if (!m_pDevice)
    CreateDeviceAndContext();
  return m_pDevice;
}

IDXGIAdapter* atDirectX::GetDisplayAdapter()
{
  if (!m_pAdapter)
    CreateAdapter();
  return m_pAdapter;
}

atDirectX::atDirectX(atWindow *pWindow, const bool &vsyncEnabled)
{
  memset(m_adapterDesc, 0, sizeof(m_adapterDesc));
  m_featureList[0] = D3D_FEATURE_LEVEL_11_0;
  m_featureList[1] = D3D_FEATURE_LEVEL_11_1;
  m_featureList[2] = D3D_FEATURE_LEVEL_10_0;
  m_featureList[3] = D3D_FEATURE_LEVEL_10_1;
  m_featureList[4] = D3D_FEATURE_LEVEL_9_1;
  m_featureList[5] = D3D_FEATURE_LEVEL_9_2;
  m_featureList[6] = D3D_FEATURE_LEVEL_9_1;
  m_gfxMemory = 0;
  m_flags = 0;
  m_refresh.den = 1;
  m_refresh.num = 0;
  m_refresh.defDen = 1;
  m_refresh.defNum = 0;
  m_refresh.vsync = vsyncEnabled;
  m_pState = atNew<atDirectXState>();

  CreateSwapChain(pWindow, vsyncEnabled);
}

IDXGIOutput* atDirectX::GetOutputDisplay()
{
  if (!m_pOutputDisplay)
    CreateOutput();
  return m_pOutputDisplay;
}

IDXGIFactory* atDirectX::GetFactory()
{
  if (!m_pFactory)
    CreateFactory();
  return m_pFactory;
}

int64_t atDirectX::GFXMemorySize()
{
  GetOutputDisplay();
  return m_gfxMemory;
}

void atDirectX::Shutdown()
{
  SafeRelease(m_pAdapter);
  SafeRelease(m_pFactory);
  SafeRelease(m_pOutputDisplay);
  SafeRelease(m_pDevice);
  SafeRelease(m_pContext);
}

void atDirectX::CreateSwapChain(atWindow *pWindow, const bool &vsyncEnabled)
{
  if (m_pDepthBuffer && m_pSwapChain)
    return;

  SafeRelease(m_pSwapChain);
  SafeRelease(m_pDepthBuffer);

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

  if (FAILED(GetFactory()->CreateSwapChain(GetDevice(), &desc, &m_pSwapChain)))
  {
    m_pSwapChain = nullptr;
    return;
  }

  CreateBackBufferView();
  CreateDepthBuffer(pWindow->Size());
  GetContext()->OMSetRenderTargets(1, &m_pBackBuffer, m_pDepthBuffer);
}

void atDirectX::CreateDeviceAndContext()
{
  if (m_pDevice || m_pContext || !GetDisplayAdapter())
    return;

#ifdef _DEBUG
  m_flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, nullptr, m_flags, m_featureList, sizeof(m_featureList) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION,
    &m_pDevice, &m_featureLevel, &m_pContext);
}

void atDirectX::CreateOutput()
{
  if (m_pOutputDisplay || !GetDisplayAdapter())
    return;

  if (FAILED(m_pAdapter->EnumOutputs(0, &m_pOutputDisplay)))
  {
    m_pOutputDisplay = nullptr;
    return;
  }

  UINT numModes;
  if (FAILED(m_pOutputDisplay->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
    return;

  m_displayModeList.resize(numModes);
  if (FAILED(m_pOutputDisplay->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, m_displayModeList.data())))
    return;

  for (int64_t i = 0; i < numModes; ++i)
    if (m_displayModeList[i].Width == (UINT)GetSystemMetrics(SM_CXSCREEN) && m_displayModeList[i].Height == (UINT)GetSystemMetrics(SM_CYSCREEN))
    {
      m_refresh.den = m_displayModeList[i].RefreshRate.Denominator;
      m_refresh.num = m_displayModeList[i].RefreshRate.Numerator;
    }

  DXGI_ADAPTER_DESC dxgiDesc;
  if (FAILED(m_pAdapter->GetDesc(&dxgiDesc)))
    return;

  m_gfxMemory = dxgiDesc.DedicatedVideoMemory;
  size_t charsConverted = 0;

  if (wcstombs_s(&charsConverted, m_adapterDesc, dxgiDesc.Description, 128))
    return;
}

void atDirectX::CreateAdapter()
{
  if (m_pAdapter || !GetFactory())
    return;

  if (FAILED(m_pFactory->EnumAdapters(0, &m_pAdapter)))
    m_pAdapter = nullptr;
}

void atDirectX::CreateFactory()
{
  if (m_pFactory)
    return;

  if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pFactory)))
    m_pFactory = nullptr;
}

void atDirectX::CreateBackBufferView()
{
  ID3D11Texture2D *pBBTex = nullptr;
  if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBBTex)))
    pBBTex = nullptr;

  GetDevice()->CreateRenderTargetView(pBBTex, nullptr, &m_pBackBuffer);
  pBBTex->Release();
}

void atDirectX::CreateDepthBuffer(const atVec2I &size)
{
  if (m_pDepthBuffer)
    SafeRelease(m_pDepthBuffer);
  
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
  GetDevice()->CreateTexture2D(&texDesc, nullptr, &pDepthTex);
  GetDevice()->CreateDepthStencilView(pDepthTex, nullptr, &m_pDepthBuffer);
  SafeRelease(pDepthTex);
}

void atDirectX::DrawIndexed(int64_t nIndices, int64_t startLocation, int64_t baseVertIndex)
{
  atRenderState::Bind();
  GetContext()->DrawIndexed((UINT)nIndices, (UINT)startLocation, (UINT)baseVertIndex);
}

void atDirectX::Draw(int64_t nVerts, int64_t startLocation)
{
  atRenderState::Bind();
  GetContext()->Draw((UINT)nVerts, (UINT)startLocation);
}

int64_t atDirectX::RefreshRateNumerator(const bool vsync)
{
  GetOutputDisplay();
  return vsync ? m_refresh.num : m_refresh.defNum;
}

int64_t atDirectX::RefreshRateDenominator(const bool vsync)
{
  GetOutputDisplay();
  return vsync ? m_refresh.den : m_refresh.defDen;
}

D3D_FEATURE_LEVEL atDirectX::GetFeatureLevel()
{
  GetDevice();
  return m_featureLevel;
}

void atDirectX::ResizeSwapChain(const atVec2I &size)
{
  SafeRelease(m_pBackBuffer);
  SafeRelease(m_pDepthBuffer);
  m_pSwapChain->ResizeBuffers(0, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
  CreateBackBufferView();
  CreateDepthBuffer(size);
  GetContext()->OMSetRenderTargets(1, &m_pBackBuffer, m_pDepthBuffer);
}

DWORD atDirectX::GetCreateFlags() { return m_flags; }
bool atDirectX::GetVsyncEnabled() const { return m_refresh.vsync; }
atString atDirectX::GetAdapterDescription() { return m_adapterDesc; }
atDirectXState* atDirectX::GetState() { return m_pState; }
IDXGISwapChain* atDirectX::GetSwapChain() { return m_pSwapChain; }
ID3D11RenderTargetView* atDirectX::GetBackbuffer() { return m_pBackBuffer; }
ID3D11DepthStencilView* atDirectX::GetDepthBuffer() { return m_pDepthBuffer; }

bool operator==(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_SAMPLER_DESC)) == 0; }
bool operator==(const D3D11_BLEND_DESC &lhs, const D3D11_BLEND_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_BLEND_DESC)) == 0; }
bool operator==(const D3D11_RASTERIZER_DESC &lhs, const D3D11_RASTERIZER_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_RASTERIZER_DESC)) == 0; }
bool operator==(const D3D11_DEPTH_STENCIL_DESC &lhs, const D3D11_DEPTH_STENCIL_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_DEPTH_STENCIL_DESC)) == 0; }
bool operator!=(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_BLEND_DESC &lhs, const D3D11_BLEND_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_RASTERIZER_DESC &lhs, const D3D11_RASTERIZER_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_DEPTH_STENCIL_DESC &lhs, const D3D11_DEPTH_STENCIL_DESC &rhs) { return !(lhs == rhs); }

#endif
