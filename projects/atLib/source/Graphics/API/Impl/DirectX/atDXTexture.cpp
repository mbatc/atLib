#include "atDXTexture.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atFormat.h"

static void* _CreateTex1D(const atVec2I &size, const bool &isDepthTex, const bool &genMipmaps, const int64_t &sampleCount, const int64_t &arraySize);
static void* _CreateTex2D(const atVec2I &size, const bool &isDepthTex, const bool &genMipmaps, const int64_t &sampleCount, const int64_t &arraySize);
static void* _CreateTex3D(const atVec2I &size, const bool &isDepthTex, const bool &genMipmaps, const int64_t &sampleCount, const int64_t &arraySize);
static void* _CreateSRView(void *pTexture, const bool &genMipmaps);
static void* _CreateDSView(void *pTexture);
static void* _CreateRTView(void *pTexture);
static void* _CreateUAView(void *pTexture);

atDXTexture::atDXTexture(const atVector<float> &image, const atVec2I &size, const atGFXTextureType &type, const int64_t &sampleCount) : atGFXTexInterface(image, size, type, sampleCount) {}
atDXTexture::atDXTexture(const atImage &image, const atGFXTextureType &type, const bool &genMipMaps, const int64_t &sampleCount) : atGFXTexInterface(image, type, genMipMaps, sampleCount) {}
atDXTexture::atDXTexture(const atVector<atVector<float>> &images, const atVec2I &size, const atGFXTextureType &type, const int64_t &sampleCount) : atGFXTexInterface(images, size, type, sampleCount) {}
atDXTexture::atDXTexture(const atVector<atImage> &images, const atVec2I &size, const atGFXTextureType &type, const bool &genMipMaps, const int64_t &sampleCount) : atGFXTexInterface(images, size, type, genMipMaps, sampleCount) {}
atDXTexture::atDXTexture(const atVec2I &size, const atGFXTextureType &type, const bool &isDepthTex, const bool &genMipMaps, const int64_t &sampleCount, const int64_t &arraySize) : atGFXTexInterface(size, type, isDepthTex, genMipMaps, sampleCount, arraySize) {}

bool atDXTexture::Delete()
{
  if (!GFXResource())
    return false;

  ID3D11Texture1D *pTex1D = (ID3D11Texture1D*)GFXResource();
  ID3D11Texture2D *pTex2D = (ID3D11Texture2D*)GFXResource();
  ID3D11Texture3D *pTex3D = (ID3D11Texture3D*)GFXResource();
  
  switch (m_type)
  {
  case atGFX_TT_1D:
    atDirectX::SafeRelease(pTex1D);
    break;
  case atGFX_TT_2D:
    atDirectX::SafeRelease(pTex2D);
    break;
  case atGFX_TT_3D:
    atDirectX::SafeRelease(pTex3D);
    break;
  }

  ID3D11ShaderResourceView *pSRV = (ID3D11ShaderResourceView*)m_pShaderView;
  ID3D11DepthStencilView *pDSV = (ID3D11DepthStencilView*)m_pDepthView;
  ID3D11RenderTargetView *pRTV = (ID3D11RenderTargetView*)m_pRenderView;
  atDirectX::SafeRelease(pSRV);
  atDirectX::SafeRelease(pDSV);
  atDirectX::SafeRelease(pRTV);

  m_pShaderView = nullptr;
  m_pDepthView = nullptr;
  m_pRenderView = nullptr;
  m_pResource = nullptr;
  return true;
}

bool atDXTexture::Upload()
{
  if (m_size.x == 0 || m_size.y == 0)
    return false;

  if (GFXResource())
    return true;

  switch (m_type)
  {
  case atGFX_TT_1D: m_pResource = _CreateTex1D(m_size, m_isDepthTex, m_genMipmaps, m_sampleCount, m_layerCount); break;
  case atGFX_TT_2D: m_pResource = _CreateTex2D(m_size, m_isDepthTex, m_genMipmaps, m_sampleCount, m_layerCount); break;
  case atGFX_TT_3D: m_pResource = _CreateTex3D(m_size, m_isDepthTex, m_genMipmaps, m_sampleCount, m_layerCount); break;
  }

  if (!m_pResource)
    return false;

  if (m_pixels.size())
  {
    int64_t bytesPerImage = m_pixels.size() / m_layerCount / m_size.y;
    for (int64_t i = 0; i < m_layerCount; ++i)
      ((atDirectX*)atGraphics::GetCtx())->GetContext()->UpdateSubresource((ID3D11Resource*)m_pResource, (UINT)i, nullptr, m_pixels.data() + bytesPerImage * i, (UINT)bytesPerImage, 0);
  }

  if (m_isDepthTex)
  {
    m_pDepthView = _CreateDSView(m_pResource);
  }
  else
  {
    m_pShaderView = _CreateSRView(m_pResource, m_genMipmaps);
    m_pRenderView = _CreateRTView(m_pResource);
  }

  return true;
}

void* atDXTexture::ShaderView() { return m_pShaderView; }
void* atDXTexture::RenderView() { return m_pRenderView; }
void* atDXTexture::DepthView() { return m_pDepthView; }

static void* _CreateTex1D(const atVec2I &size, const bool &isDepthTex, const bool &genMipmaps, const int64_t &sampleCount, const int64_t &arraySize)
{
  D3D11_TEXTURE1D_DESC texDesc{};
  texDesc.BindFlags = isDepthTex ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(isDepthTex ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Width = (UINT)size.x * size.y;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.ArraySize = 1;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS * genMipmaps;
  ID3D11Texture1D *pTex = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateTexture1D(&texDesc, nullptr, &pTex);
  return pTex;
}

static void* _CreateTex2D(const atVec2I &size, const bool &isDepthTex, const bool &genMipmaps, const int64_t &sampleCount, const int64_t &arraySize)
{
  D3D11_TEXTURE2D_DESC texDesc{};
  texDesc.BindFlags = isDepthTex ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(isDepthTex ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Height = (UINT)size.y;
  texDesc.Width = (UINT)size.x;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.SampleDesc.Quality = 0;
  texDesc.SampleDesc.Count = (UINT)sampleCount;
  texDesc.ArraySize = (UINT)arraySize;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS * genMipmaps;
  ID3D11Texture2D *pTex = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateTexture2D(&texDesc, nullptr, &pTex);
  return pTex;
}

static void* _CreateTex3D(const atVec2I &size, const bool &isDepthTex, const bool &genMipmaps, const int64_t &sampleCount, const int64_t &arraySize)
{
  D3D11_TEXTURE3D_DESC texDesc{};
  texDesc.BindFlags = isDepthTex ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(isDepthTex ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Height = (UINT)size.y;
  texDesc.Width = (UINT)size.x;
  texDesc.Depth = (UINT)arraySize;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS * genMipmaps;
  ID3D11Texture3D *pTex = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateTexture3D(&texDesc, nullptr, &pTex);
  return pTex;
}

static void *_CreateSRView(void *pTexture, const bool &genMipmaps)
{
  ID3D11ShaderResourceView *pView = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateShaderResourceView((ID3D11Resource*)pTexture, nullptr, &pView);
  if (genMipmaps)
    ((atDirectX*)atGraphics::GetCtx())->GetContext()->GenerateMips(pView);
  return pView;
}

static void *_CreateDSView(void *pTexture)
{
  ID3D11DepthStencilView *pView = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateDepthStencilView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

static void *_CreateRTView(void *pTexture)
{
  ID3D11RenderTargetView *pView = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateRenderTargetView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

static void *_CreateUAView(void *pTexture)
{
  ID3D11UnorderedAccessView *pView = nullptr;
  ((atDirectX*)atGraphics::GetCtx())->GetDevice()->CreateUnorderedAccessView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

