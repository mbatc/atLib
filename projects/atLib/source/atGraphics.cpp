
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

#include "atGraphics.h"
#include <stdlib.h>

atVector<DXGI_MODE_DESC> atGraphics::m_displayModeList;
ID3D11DeviceContext *atGraphics::m_pContext = nullptr;
ID3D11Device *atGraphics::m_pDevice = nullptr;
IDXGIAdapter *atGraphics::m_pAdapter = nullptr;
IDXGIOutput *atGraphics::m_pOutputDisplay = nullptr;
IDXGIFactory *atGraphics::m_pFactory;
int64_t atGraphics::m_gfxMemory = 0;
char atGraphics::m_adapterDesc[128] = { 0 };
DWORD atGraphics::m_flags = 0;
D3D_FEATURE_LEVEL atGraphics::m_featureLevel;
D3D_FEATURE_LEVEL atGraphics::m_featureList[] = {  
  D3D_FEATURE_LEVEL_11_0,
  D3D_FEATURE_LEVEL_10_0, 
  D3D_FEATURE_LEVEL_10_1,
  D3D_FEATURE_LEVEL_9_1,
  D3D_FEATURE_LEVEL_9_2,
  D3D_FEATURE_LEVEL_9_1 };

int64_t atGraphics::RefreshRate::den = 1;
int64_t atGraphics::RefreshRate::num = 0;
int64_t atGraphics::RefreshRate::defDen = 1;
int64_t atGraphics::RefreshRate::defNum = 0;

ID3D11DeviceContext* atGraphics::GetContext()
{
  if (!m_pContext)
    CreateDeviceAndContext();
  return m_pContext;
}

atString atGraphics::GetAdapterDescription() { return m_adapterDesc; }

ID3D11Device* atGraphics::GetDevice()
{
  if (!m_pDevice)
    CreateDeviceAndContext();
  return m_pDevice;
}

IDXGIAdapter* atGraphics::GetDisplayAdapter()
{
  if (!m_pAdapter)
    CreateAdapter();
  return m_pAdapter;
}

IDXGIOutput* atGraphics::GetOutputDisplay()
{
  if (!m_pOutputDisplay)
    CreateOutput();
  return m_pOutputDisplay;
}

IDXGIFactory * atGraphics::GetFactory()
{
  if (!m_pFactory)
    CreateFactory();
  return m_pFactory;
}

int64_t atGraphics::GFXMemorySize()
{
  GetOutputDisplay();
  return m_gfxMemory;
}

void atGraphics::Shutdown()
{
  SafeRelease(m_pAdapter);
  SafeRelease(m_pFactory);
  SafeRelease(m_pOutputDisplay);
  SafeRelease(m_pDevice);
  SafeRelease(m_pContext);
}

void atGraphics::CreateDeviceAndContext()
{
  if (m_pDevice || m_pContext || !GetDisplayAdapter())
    return;
#ifdef _DEBUG
  m_flags = D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, nullptr, m_flags, m_featureList, sizeof(m_featureList) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, 
    &m_pDevice, &m_featureLevel, &m_pContext);
}

void atGraphics::CreateOutput()
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

void atGraphics::CreateAdapter()
{
  if (m_pAdapter || !GetFactory())
    return;
  if (FAILED(m_pFactory->EnumAdapters(0, &m_pAdapter)))
    m_pAdapter = nullptr;
}

void atGraphics::CreateFactory()
{
  if (m_pFactory)
    return;
  if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pFactory)))
    m_pFactory = nullptr;
}

bool atGraphics::BindShaderResource(const atShaderType shader, const atShader_ResourceType resType, const int64_t slot, void * pResource)
{
  if (slot < 0)
    return false;

  ID3D11Buffer *pBuffer;
  ID3D11SamplerState *pSampler;
  ID3D11ShaderResourceView *pTexture;
  switch (resType)
  {
  case atSRT_Buffer:
    pBuffer = (ID3D11Buffer*)pResource;
    switch (shader)
    {
    case atST_Vertex: m_pContext->VSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Pixel: m_pContext->PSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Domain: m_pContext->DSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Hull: m_pContext->HSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Compute: m_pContext->CSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Geometry: m_pContext->GSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    default: return false;
    }
    break;
  case atSRT_Sampler:
    pSampler = (ID3D11SamplerState*)pResource;
    switch (shader)
    {
    case atST_Vertex: m_pContext->VSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Pixel: m_pContext->PSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Domain: m_pContext->DSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Hull: m_pContext->HSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Compute: m_pContext->CSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Geometry: m_pContext->GSSetSamplers((UINT)slot, 1, &pSampler); break;
    default: return false;
    }
    break;
  case atSRT_Texture:
    pTexture = (ID3D11ShaderResourceView*)pResource;
    switch (shader)
    {
    case atST_Vertex: m_pContext->VSSetShaderResources((UINT)slot, 1, &pTexture); break;
    case atST_Pixel: m_pContext->PSSetShaderResources((UINT)slot, 1, &pTexture); break;
    case atST_Domain: m_pContext->DSSetShaderResources((UINT)slot, 1, &pTexture); break;
    case atST_Hull: m_pContext->HSSetShaderResources((UINT)slot, 1, &pTexture); break;
    case atST_Compute: m_pContext->CSSetShaderResources((UINT)slot, 1, &pTexture); break;
    case atST_Geometry: m_pContext->GSSetShaderResources((UINT)slot, 1, &pTexture); break;
    default: return false;
    }
    break;
  default: return false;
  }
  return true;
}

int64_t atGraphics::RefreshRateNumerator(const bool vsync) { GetOutputDisplay(); return vsync ? RefreshRate::num : RefreshRate::defNum; }
int64_t atGraphics::RefreshRateDenominator(const bool vsync) { GetOutputDisplay(); return vsync ? RefreshRate::den : RefreshRate::defDen; }
DWORD atGraphics::GetCreateFlags() { return m_flags; }
D3D_FEATURE_LEVEL atGraphics::GetFeatureLevel() { GetDevice(); return m_featureLevel; }
