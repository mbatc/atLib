
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

#include "atTextureContext.h"
#include "atImage.h"
#include "atFormat.h"

static ID3D11ShaderResourceView *_CreateSRView(ID3D11Texture2D *pTexture, const bool genMipmaps, const int64_t sampleCount)
{
  ID3D11ShaderResourceView *pView = nullptr;

  D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
  desc.Texture2D.MipLevels = genMipmaps ? -1 : 1;
  desc.Texture2D.MostDetailedMip = 0;
  desc.ViewDimension = sampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  atGraphics::GetDevice()->CreateShaderResourceView(pTexture, &desc, &pView);
  if(genMipmaps)
    atGraphics::GetContext()->GenerateMips(pView);
  return pView;
}

static ID3D11DepthStencilView *_CreateDSView(ID3D11Texture2D *pTexture, const int64_t sampleCount)
{
  ID3D11DepthStencilView *pView = nullptr;
  D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
  desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  desc.ViewDimension = sampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipSlice = 0;
  desc.Flags = 0;

  atGraphics::GetDevice()->CreateDepthStencilView(pTexture, &desc, &pView);
  return pView;
}

static ID3D11RenderTargetView *_CreateRTView(ID3D11Texture2D *pTexture)
{
  ID3D11RenderTargetView *pView = nullptr;
  atGraphics::GetDevice()->CreateRenderTargetView(pTexture, nullptr, &pView);
  return pView;
}

static ID3D11UnorderedAccessView *_CreateUAView(ID3D11Texture2D *pTexture)
{
  ID3D11UnorderedAccessView *pView = nullptr;
  atGraphics::GetDevice()->CreateUnorderedAccessView(pTexture, nullptr, &pView);
  return pView;
}

atTextureContext::atTextureContext(ID3D11Texture2D *pTexture, const bool genMipmaps, const int64_t sampleCount) : m_pTexture(pTexture), m_genMipmaps(genMipmaps) {}

atTextureContext::atTextureContext(const atFilename &file, const bool genMipmaps, const int64_t sampleCount) : atTextureContext(atImage(file), genMipmaps) {}

atTextureContext::atTextureContext(const atImage &image, const bool genMipmaps, const int64_t sampleCount) : atTextureContext(image.Pixels().data(), image.Size(), false, genMipmaps) {}

atTextureContext::atTextureContext(const atVector<float> &depth, const atVec2I &size, const int64_t sampleCount) : atTextureContext(depth.data(), size, true) {}

atTextureContext::atTextureContext(const void *pData, const atVec2I &size, const bool depthTexture, const bool genMipmaps, const int64_t sampleCount)
  : m_genMipmaps(genMipmaps && !depthTexture)
  , m_depthTexture(depthTexture)
  , m_sampleCount(sampleCount)
{
  D3D11_TEXTURE2D_DESC texDesc{};
  texDesc.BindFlags = m_depthTexture ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(m_depthTexture ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Height = (UINT)size.y;
  texDesc.Width = (UINT)size.x;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.SampleDesc.Quality = 0;
  texDesc.SampleDesc.Count = (UINT)sampleCount;
  texDesc.ArraySize = 1;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS * m_genMipmaps;

  atGraphics::GetDevice()->CreateTexture2D(&texDesc, nullptr, &m_pTexture);
  if (pData)
    atGraphics::GetContext()->UpdateSubresource(m_pTexture, 0, nullptr, pData, (UINT)(size.x * (m_depthTexture ? sizeof(float) : sizeof(atCol))), 0);
}


atTextureContext::atTextureContext(const atTextureContext &copy)
{
  m_genMipmaps = copy.m_genMipmaps;
  m_pTexture = copy.m_pTexture;
  m_pUAView = copy.m_pUAView;
  m_pDepthView = copy.m_pDepthView;
  m_pRenderView = copy.m_pRenderView;
  m_pShaderView = copy.m_pShaderView;
  m_sampleCount = copy.m_sampleCount;
  m_depthTexture = copy.m_depthTexture;
  if (m_pTexture) m_pTexture->AddRef();
  if (m_pUAView) m_pUAView->AddRef();
  if (m_pDepthView) m_pDepthView->AddRef();
  if (m_pRenderView) m_pRenderView->AddRef();
  if (m_pShaderView) m_pShaderView->AddRef();
}

atTextureContext::atTextureContext(atTextureContext &&move) : m_genMipmaps(move.m_genMipmaps)
{
  m_pTexture = move.m_pTexture;
  m_pUAView = move.m_pUAView;
  m_pDepthView = move.m_pDepthView;
  m_pRenderView = move.m_pRenderView;
  m_pShaderView = move.m_pShaderView;
  m_sampleCount = move.m_sampleCount;
  m_depthTexture = move.m_depthTexture;
  move.m_sampleCount = 1;
  move.m_depthTexture = false;
  move.m_pTexture = nullptr;
  move.m_pUAView = nullptr;
  move.m_pDepthView = nullptr;
  move.m_pRenderView = nullptr;
  move.m_pShaderView = nullptr;
}

const atTextureContext &atTextureContext::operator=(const atTextureContext &rhs)
{
  Release();
  m_pTexture = rhs.m_pTexture;
  m_pUAView = rhs.m_pUAView;
  m_pDepthView = rhs.m_pDepthView;
  m_pRenderView = rhs.m_pRenderView;
  m_pShaderView = rhs.m_pShaderView;
  m_genMipmaps = rhs.m_genMipmaps;
  m_sampleCount = rhs.m_sampleCount;
  m_depthTexture = rhs.m_depthTexture;
  if (m_pTexture) m_pTexture->AddRef();
  if (m_pUAView) m_pUAView->AddRef();
  if (m_pDepthView) m_pDepthView->AddRef();
  if (m_pRenderView) m_pRenderView->AddRef();
  if (m_pShaderView) m_pShaderView->AddRef();
  return *this;
}

const atTextureContext &atTextureContext::operator=(atTextureContext &&rhs)
{
  Release();
  m_pTexture = rhs.m_pTexture;
  m_pUAView = rhs.m_pUAView;
  m_pDepthView = rhs.m_pDepthView;
  m_pRenderView = rhs.m_pRenderView;
  m_pShaderView = rhs.m_pShaderView;
  m_genMipmaps = rhs.m_genMipmaps;
  m_sampleCount = rhs.m_sampleCount;
  m_depthTexture = rhs.m_depthTexture;
  rhs.m_sampleCount = 1;
  rhs.m_depthTexture = false;
  rhs.m_pTexture = nullptr;
  rhs.m_pUAView = nullptr;
  rhs.m_pDepthView = nullptr;
  rhs.m_pRenderView = nullptr;
  rhs.m_pShaderView = nullptr;
  return *this;
}

void atTextureContext::Release()
{
  atGraphics::SafeRelease(m_pTexture);
  atGraphics::SafeRelease(m_pDepthView);
  atGraphics::SafeRelease(m_pRenderView);
  atGraphics::SafeRelease(m_pShaderView);
  atGraphics::SafeRelease(m_pUAView);
}

atTextureContext::operator ID3D11ShaderResourceView*()
{
  if (!m_pShaderView)
    m_pShaderView = _CreateSRView(m_pTexture, m_genMipmaps, m_sampleCount);
  return m_pShaderView;
}

atTextureContext::operator ID3D11RenderTargetView*()
{
  if (!m_pRenderView)
    m_pRenderView = _CreateRTView(m_pTexture);
  return m_pRenderView;
}

atTextureContext::operator ID3D11DepthStencilView*()
{
  if (!m_pDepthView)
    m_pDepthView = _CreateDSView(m_pTexture, m_sampleCount);
  return m_pDepthView;
}

atTextureContext::operator ID3D11UnorderedAccessView*()
{
  if (!m_pUAView)
    m_pUAView = _CreateUAView(m_pTexture);
  return m_pUAView;
}

atTextureContext::operator ID3D11ShaderResourceView**()
{
  if (!m_pShaderView)
    m_pShaderView = _CreateSRView(m_pTexture, m_genMipmaps, m_sampleCount);
  return &m_pShaderView;
}

atTextureContext::operator ID3D11RenderTargetView**()
{
  if (!m_pRenderView)
    m_pRenderView = _CreateRTView(m_pTexture);
  return &m_pRenderView;
}

atTextureContext::operator ID3D11DepthStencilView**()
{
  if (!m_pDepthView)
    m_pDepthView = _CreateDSView(m_pTexture, m_sampleCount);
  return &m_pDepthView;
}

atTextureContext::operator ID3D11UnorderedAccessView**()
{
  if (!m_pUAView)
    m_pUAView = _CreateUAView(m_pTexture);
  return &m_pUAView;
}

void atTextureContext::UpdateTexture(const atImage &image, const bool genMipaps, const int64_t sampleCount) { *this = atTextureContext(image, genMipaps, sampleCount); }

atTextureContext::~atTextureContext() { Release(); }
atTextureContext::operator ID3D11Texture2D*() { return m_pTexture; }
atTextureContext::operator ID3D11Texture2D**() { return &m_pTexture; }
