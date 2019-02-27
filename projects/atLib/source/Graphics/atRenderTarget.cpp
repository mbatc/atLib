
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

#include "atRenderTarget.h"
#include "atWindow.h"
#include "atRenderState.h"
#include "atHardwareTexture.h"

atRenderTarget::~atRenderTarget() { Destroy(); }

atRenderTarget::atRenderTarget(const int64_t colorTex, const int64_t depthTex) 
  : m_vsync(false)
  , m_pWindow(nullptr)
  , m_pSwapChain(nullptr)
  , m_windowed(true)
  , m_colorTexID(colorTex)
  , m_depthTexID(depthTex)
{}

atRenderTarget::atRenderTarget(atWindow *pWindow, const atVec2I &size, const bool vsync, const bool windowed)
  : m_vsync(vsync)
  , m_size(size)
  , m_pWindow(pWindow)
  , m_pSwapChain(nullptr)
  , m_windowed(windowed)
  , m_colorTexID(AT_INVALID_ID)
  , m_depthTexID(AT_INVALID_ID)
  , m_dirty(true)
{}

void atRenderTarget::Clear(const atVec4F &color, const float depth)
{
  GetColourTexID();
  GetDepthTexID();
  atTextureContext *pColour = atHardwareTexture::GetTexture(m_colorTexID);
  atTextureContext *pDepth = atHardwareTexture::GetTexture(m_depthTexID);
  if(pColour)
    atGraphics::GetContext()->ClearRenderTargetView(*pColour, &color[0]);
  if(pDepth)
    atGraphics::GetContext()->ClearDepthStencilView(*pDepth, D3D11_CLEAR_DEPTH, depth, 0);
}

int64_t atRenderTarget::GetDepthTexID()
{
  if (m_depthTexID == AT_INVALID_ID && m_pWindow)
    m_depthTexID = atHardwareTexture::UploadDepthTexture(nullptr, m_size, 4);
  return m_depthTexID;
}

int64_t atRenderTarget::GetColourTexID()
{
  if (m_colorTexID == AT_INVALID_ID && m_pWindow)
    GetWindowRenderTarget();
  return m_colorTexID;
}

bool atRenderTarget::Resize()
{
  if (m_pWindow)
  {
    if (m_size == m_pWindow->Size() && m_windowed == m_pWindow->IsWindowed())
      return false;
    Destroy();

    m_size = m_pWindow->Size();
    m_windowed = m_pWindow->IsWindowed();
    return true;
  }
  return false;
}

IDXGISwapChain* atRenderTarget::GetSwapChain()
{
  if (!m_pSwapChain)
    CreateSwapChain();
  return m_pSwapChain;
}

void atRenderTarget::CreateSwapChain()
{
  if (!m_pWindow)
    return;

  DXGI_SWAP_CHAIN_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.BufferDesc.RefreshRate.Numerator = (UINT)atGraphics::RefreshRateNumerator(m_vsync);
  desc.BufferDesc.RefreshRate.Denominator = (UINT)atGraphics::RefreshRateDenominator(m_vsync);
  desc.BufferDesc.Width = m_size.x;
  desc.BufferDesc.Height = m_size.y;
  desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  desc.SampleDesc.Count = 4;
  desc.SampleDesc.Quality = 0;
  desc.OutputWindow = m_pWindow->Handle();
  desc.BufferCount = 1;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.Windowed = m_pWindow->IsWindowed();
  desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  desc.Flags = 0;
  
  if (FAILED(atGraphics::GetFactory()->CreateSwapChain(atGraphics::GetDevice(), &desc, &m_pSwapChain)))
    m_pSwapChain = nullptr;
}

void atRenderTarget::GetWindowRenderTarget()
{
  if (m_colorTexID != AT_INVALID_ID || !GetSwapChain() || !atGraphics::GetDevice())
    return;

  ID3D11Texture2D *pBackbuffer = nullptr;
  if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackbuffer)))
    return;
  m_colorTexID = atHardwareTexture::AddTexture(pBackbuffer, false);
}

void atRenderTarget::Destroy()
{
  atGraphics::SafeRelease(m_pSwapChain);
  atHardwareTexture::DeleteTexture(m_colorTexID);
  atHardwareTexture::DeleteTexture(m_depthTexID);
  m_colorTexID = AT_INVALID_ID;
  m_depthTexID = AT_INVALID_ID;
  m_dirty = true;
}

bool atRenderTarget::Dirty()
{
  bool res = m_dirty;
  m_dirty = false;
  return res;
}

void atRenderTarget::Swap() { m_pSwapChain->Present(m_vsync ? 1 : 0, 0);  }
