
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

atRenderTarget::~atRenderTarget() { Destroy(); }

atRenderTarget::atRenderTarget(const atWindow *pWindow, const atVec2I &size, const bool vsync, const bool windowed)
  : m_vsync(vsync)
  , m_size(size)
  , m_pWindow(pWindow)
  , m_pDepthStencilBuffer(nullptr)
  , m_pDepthStencilView(nullptr)
  , m_pRenderTarget(nullptr)
  , m_pSwapChain(nullptr)
  , m_windowed(windowed)
{}

void atRenderTarget::Bind()
{
  Resize();
  GetDepthStencilView();
  GetRenderTarget();
  GetSwapChain();
  UINT vpCount = 0;
  atGraphics::GetContext()->RSGetViewports(&vpCount, nullptr);
  if (vpCount == 0)
  {
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0.f;
    vp.TopLeftY = 0.f;
    vp.Height = (float)m_size.y;
    vp.Width = (float)m_size.x;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    atGraphics::GetContext()->RSSetViewports(1, &vp);
  }
  atGraphics::GetContext()->OMSetRenderTargets(1, &m_pRenderTarget, m_pDepthStencilView);
}

void atRenderTarget::Clear(const atVec4F &color)
{
  atGraphics::GetContext()->ClearRenderTargetView(m_pRenderTarget, &color[0]);
  atGraphics::GetContext()->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

bool atRenderTarget::Resize()
{
  if (m_size == m_pWindow->Size() && m_windowed == m_pWindow->IsWindowed())
    return false;
  Destroy();

  m_size = m_pWindow->Size();
  m_windowed = m_pWindow->IsWindowed();
  return true;
}

ID3D11DepthStencilView* atRenderTarget::GetDepthStencilView()
{
  if (!m_pDepthStencilView)
    CreateDepthStencil();
  return m_pDepthStencilView;
}

ID3D11RenderTargetView* atRenderTarget::GetRenderTarget()
{
  if (!m_pRenderTarget)
    CreateRenderTarget();
  return m_pRenderTarget;
}

ID3D11Texture2D* atRenderTarget::GetDepthStencilBuffer()
{
  if (!m_pDepthStencilBuffer)
    CreateDepthStencil();
  return m_pDepthStencilBuffer;
}

IDXGISwapChain* atRenderTarget::GetSwapChain()
{
  if (!m_pSwapChain)
    CreateSwapChain();
  return m_pSwapChain;
}

void atRenderTarget::CreateSwapChain()
{
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
  desc.OutputWindow = m_pWindow->GetHandle();
  desc.BufferCount = 1;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.Windowed = m_pWindow->IsWindowed();
  desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  desc.Flags = 0;
  
  if (FAILED(atGraphics::GetFactory()->CreateSwapChain(atGraphics::GetDevice(), &desc, &m_pSwapChain)))
    m_pSwapChain = nullptr;
}

void atRenderTarget::CreateDepthStencil()
{
  if (m_pDepthStencilBuffer || !atGraphics::GetDevice())
    return;

  {
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = m_size.x;
    desc.Height = m_size.y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 4;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    if (FAILED(atGraphics::GetDevice()->CreateTexture2D(&desc, NULL, &m_pDepthStencilBuffer)))
      return;
  }
  
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC desc;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    desc.Texture2D.MipSlice = 0;
    desc.Flags = 0;
    if (FAILED(atGraphics::GetDevice()->CreateDepthStencilView(m_pDepthStencilBuffer, &desc, &m_pDepthStencilView)))
      return;
  }
}

void atRenderTarget::CreateRenderTarget()
{
  if (m_pRenderTarget || !GetSwapChain() || !atGraphics::GetDevice())
    return;

  ID3D11Texture2D *pBackbuffer = nullptr;
  if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackbuffer)))
    return;

  if (FAILED(atGraphics::GetDevice()->CreateRenderTargetView(pBackbuffer, NULL, &m_pRenderTarget)))
    return;
  atGraphics::SafeRelease(pBackbuffer);
}

void atRenderTarget::Destroy()
{
  atGraphics::SafeRelease(m_pSwapChain);
  atGraphics::SafeRelease(m_pDepthStencilView);
  atGraphics::SafeRelease(m_pRenderTarget);
  atGraphics::SafeRelease(m_pDepthStencilBuffer);
}

void atRenderTarget::Swap() { m_pSwapChain->Present(m_vsync ? 1 : 0, 0); }
