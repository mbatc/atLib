#include "atDirectX.h"
#include "atRenderState.h"

atVector<DXGI_MODE_DESC> atDirectX::m_displayModeList;
ID3D11DeviceContext *atDirectX::m_pContext = nullptr;
IDXGIOutput *atDirectX::m_pOutputDisplay = nullptr;
IDXGIAdapter *atDirectX::m_pAdapter = nullptr;
ID3D11Device *atDirectX::m_pDevice = nullptr;
IDXGIFactory *atDirectX::m_pFactory;
int64_t atDirectX::m_gfxMemory = 0;
char atDirectX::m_adapterDesc[128] = { 0 };
DWORD atDirectX::m_flags = 0;
D3D_FEATURE_LEVEL atDirectX::m_featureLevel;
D3D_FEATURE_LEVEL atDirectX::m_featureList[] = {
  D3D_FEATURE_LEVEL_11_0,
  D3D_FEATURE_LEVEL_10_0,
  D3D_FEATURE_LEVEL_10_1,
  D3D_FEATURE_LEVEL_9_1,
  D3D_FEATURE_LEVEL_9_2,
  D3D_FEATURE_LEVEL_9_1 };

int64_t atDirectX::RefreshRate::den = 1;
int64_t atDirectX::RefreshRate::num = 0;
int64_t atDirectX::RefreshRate::defDen = 1;
int64_t atDirectX::RefreshRate::defNum = 0;

ID3D11DeviceContext* atDirectX::GetContext()
{
  if (!m_pContext)
    CreateDeviceAndContext();
  return m_pContext;
}

atString atDirectX::GetAdapterDescription() { return m_adapterDesc; }

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
      RefreshRate::den = m_displayModeList[i].RefreshRate.Denominator;
      RefreshRate::num = m_displayModeList[i].RefreshRate.Numerator;
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

int64_t atDirectX::RefreshRateNumerator(const bool vsync) { GetOutputDisplay(); return vsync ? RefreshRate::num : RefreshRate::defNum; }
int64_t atDirectX::RefreshRateDenominator(const bool vsync) { GetOutputDisplay(); return vsync ? RefreshRate::den : RefreshRate::defDen; }
DWORD atDirectX::GetCreateFlags() { return m_flags; }
D3D_FEATURE_LEVEL atDirectX::GetFeatureLevel() { GetDevice(); return m_featureLevel; }

bool operator==(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_SAMPLER_DESC)) == 0; }
bool operator==(const D3D11_BLEND_DESC &lhs, const D3D11_BLEND_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_BLEND_DESC)) == 0; }
bool operator==(const D3D11_RASTERIZER_DESC &lhs, const D3D11_RASTERIZER_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_RASTERIZER_DESC)) == 0; }
bool operator==(const D3D11_DEPTH_STENCIL_DESC &lhs, const D3D11_DEPTH_STENCIL_DESC &rhs) { return memcmp(&lhs, &rhs, sizeof(D3D11_DEPTH_STENCIL_DESC)) == 0; }
bool operator!=(const D3D11_SAMPLER_DESC &lhs, const D3D11_SAMPLER_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_BLEND_DESC &lhs, const D3D11_BLEND_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_RASTERIZER_DESC &lhs, const D3D11_RASTERIZER_DESC &rhs) { return !(lhs == rhs); }
bool operator!=(const D3D11_DEPTH_STENCIL_DESC &lhs, const D3D11_DEPTH_STENCIL_DESC &rhs) { return !(lhs == rhs); }
