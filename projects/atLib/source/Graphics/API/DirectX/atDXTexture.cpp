#ifdef atPLATFORM_WIN32

#include "atDXTexture.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atFormat.h"
#include "atDXInclude_Internal.h"

static void* _CreateTex1D(const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize);
static void* _CreateTex2D(const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize);
static void* _CreateTex3D(const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize);
static void* _CreateSRView(void *pTexture);
static void* _CreateDSView(void *pTexture);
static void* _CreateRTView(void *pTexture);
static void* _CreateUAView(void *pTexture);

atDXTexture::atDXTexture(const atTextureType &type, const atVector<float> &image, const atVec2I &size, const int64_t &sampleCount) : atTexture(type, image, size, sampleCount) {}
atDXTexture::atDXTexture(const atTextureType &type, const atImage &image, const int64_t &sampleCount) : atTexture(type, image, sampleCount) {}
atDXTexture::atDXTexture(const atTextureType &type, const atVector<atVector<float>> &images, const atVec2I &size, const int64_t &sampleCount) : atTexture(type, images, size, sampleCount) {}
atDXTexture::atDXTexture(const atTextureType &type, const atVector<atImage> &images, const atVec2I &size, const int64_t &sampleCount) : atTexture(type, images, size, sampleCount) {}
atDXTexture::atDXTexture(const atTextureType &type) : atTexture(type) {}
atDXTexture::atDXTexture(const atTextureType &type, const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize) : atTexture(type, size, isDepthTex, sampleCount, arraySize) {}

bool atDXTexture::Delete()
{
  if (!NativeResource())
    return false;

  ID3D11Texture1D *pTex1D = (ID3D11Texture1D*)NativeResource();
  ID3D11Texture2D *pTex2D = (ID3D11Texture2D*)NativeResource();
  ID3D11Texture3D *pTex3D = (ID3D11Texture3D*)NativeResource();
  
  switch (m_type)
  {
  case atTexture_1D:
    atDirectX::SafeRelease(pTex1D);
    break;
  case atTexture_2D:
    atDirectX::SafeRelease(pTex2D);
    break;
  case atTexture_3D:
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
  m_genMipmaps = false;
  return true;
}

bool atDXTexture::Upload()
{
  if (m_size.x == 0 || m_size.y == 0)
    return false;

  if (NativeResource())
    return true;

  switch (m_type)
  {
  case atTexture_1D: m_pResource = _CreateTex1D(m_size, m_isDepthTex, m_sampleCount, m_layerCount); break;
  case atTexture_2D: m_pResource = _CreateTex2D(m_size, m_isDepthTex, m_sampleCount, m_layerCount); break;
  case atTexture_3D: m_pResource = _CreateTex3D(m_size, m_isDepthTex, m_sampleCount, m_layerCount); break;
  }

  if (!m_pResource)
    return false;

  if (m_pixels.size())
  {
    int64_t bytesPerImage = m_pixels.size() / m_layerCount / m_size.y;
    for (int64_t i = 0; i < m_layerCount; ++i)
      ((ID3D11DeviceContext*)((atDirectX*)atGraphics::GetCurrent())->GetContext())->UpdateSubresource((ID3D11Resource*)m_pResource, (UINT)i, nullptr, m_pixels.data() + bytesPerImage * i, (UINT)bytesPerImage, 0);
  }

  if (m_isDepthTex)
  {
    m_pDepthView = _CreateDSView(m_pResource);
  }
  else
  {
    m_pShaderView = _CreateSRView(m_pResource);
    m_pRenderView = _CreateRTView(m_pResource);

    if (m_genMipmaps)
      GenerateMipmaps();
    m_genMipmaps = false;
  }

  return true;
}

void atDXTexture::GenerateMipmaps()
{
  if (m_pShaderView)
  {
    atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
    ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
    pCtx->GenerateMips((ID3D11ShaderResourceView*)m_pShaderView);
  }
  else
  { // Gen mipmaps later when the texture is uploaded
    m_genMipmaps = true;
  }
}

void* atDXTexture::ShaderView() { return m_pShaderView; }
void* atDXTexture::RenderView() { return m_pRenderView; }
void* atDXTexture::DepthView() { return m_pDepthView; }

static void* _CreateTex1D(const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize)
{
  D3D11_TEXTURE1D_DESC texDesc = { 0 };
  texDesc.BindFlags = isDepthTex ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(isDepthTex ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Width = (UINT)size.x * size.y;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.ArraySize = 1;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = isDepthTex ? 0 : D3D11_RESOURCE_MISC_GENERATE_MIPS; // Allow mipmap generation if this is a colour texture
  ID3D11Texture1D *pTex = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  pDev->CreateTexture1D(&texDesc, nullptr, &pTex);
  return pTex;
}

static void* _CreateTex2D(const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize)
{
  D3D11_TEXTURE2D_DESC texDesc = { 0 };
  texDesc.BindFlags = isDepthTex ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(isDepthTex ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Height = (UINT)size.y;
  texDesc.Width = (UINT)size.x;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.SampleDesc.Quality = 0;
  texDesc.SampleDesc.Count = (UINT)sampleCount;
  texDesc.ArraySize = (UINT)arraySize;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = isDepthTex ? 0 : D3D11_RESOURCE_MISC_GENERATE_MIPS; // Allow mipmap generation if this is a colour texture
  ID3D11Texture2D *pTex = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  pDev->CreateTexture2D(&texDesc, nullptr, &pTex);
  return pTex;
}

static void* _CreateTex3D(const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize)
{
  D3D11_TEXTURE3D_DESC texDesc = { 0 };
  texDesc.BindFlags = isDepthTex ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
  texDesc.Format = DXGI_FORMAT(isDepthTex ? atFormat::DXGIDepth(atType_Float32) : atFormat::DXGI(atType_Uint32, 1));
  texDesc.Height = (UINT)size.y;
  texDesc.Width = (UINT)size.x;
  texDesc.Depth = (UINT)arraySize;
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.MipLevels = sampleCount > 1 ? 1 : 0;
  texDesc.MiscFlags = isDepthTex ? 0 : D3D11_RESOURCE_MISC_GENERATE_MIPS; // Allow mipmap generation if this is a colour texture
  ID3D11Texture3D *pTex = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  pDev->CreateTexture3D(&texDesc, nullptr, &pTex);
  return pTex;
}

static void *_CreateSRView(void *pTexture)
{
  ID3D11ShaderResourceView *pView = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetDevice();
  pDev->CreateShaderResourceView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

static void *_CreateDSView(void *pTexture)
{
  ID3D11DepthStencilView *pView = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  pDev->CreateDepthStencilView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

static void *_CreateRTView(void *pTexture)
{
  ID3D11RenderTargetView *pView = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  pDev->CreateRenderTargetView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

static void *_CreateUAView(void *pTexture)
{
  ID3D11UnorderedAccessView *pView = nullptr;
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  pDev->CreateUnorderedAccessView((ID3D11Resource*)pTexture, nullptr, &pView);
  return pView;
}

#else

atDXTexture::atDXTexture(const atVector<float> &image, const atVec2I &size, const atTextureType &type, const int64_t &sampleCount) : atTexture(image, size, type, sampleCount) { atRelFail("DirectX is only supported on Windows platforms."); }
atDXTexture::atDXTexture(const atImage &image, const atTextureType &type, const bool &genMipMaps, const int64_t &sampleCount) : atTexture(image, type, genMipMaps, sampleCount) { atRelFail("DirectX is only supported on Windows platforms."); }
atDXTexture::atDXTexture(const atVector<atVector<float>> &images, const atVec2I &size, const atTextureType &type, const int64_t &sampleCount) : atTexture(images, size, type, sampleCount) { atRelFail("DirectX is only supported on Windows platforms."); }
atDXTexture::atDXTexture(const atVector<atImage> &images, const atVec2I &size, const atTextureType &type, const bool &genMipMaps, const int64_t &sampleCount) : atTexture(images, size, type, genMipMaps, sampleCount) { atRelFail("DirectX is only supported on Windows platforms."); }
atDXTexture::atDXTexture(const atVec2I &size, const atTextureType &type, const bool &isDepthTex, const bool &genMipMaps, const int64_t &sampleCount, const int64_t &arraySize) : atTexture(size, type, isDepthTex, genMipMaps, sampleCount, arraySize) { atRelFail("DirectX is only supported on Windows platforms."); }

bool atDXTexture::Delete()
{
  atRelFail("DirectX is only supported on Windows platforms.");
  return false;
}

bool atDXTexture::Upload()
{
  atRelFail("DirectX is only supported on Windows platforms.");
  return false;
}

void* atDXTexture::ShaderView() { atRelFail("DirectX is only supported on Windows platforms."); return nullptr; }
void* atDXTexture::RenderView() { atRelFail("DirectX is only supported on Windows platforms."); return nullptr; }
void* atDXTexture::DepthView() { atRelFail("DirectX is only supported on Windows platforms."); return nullptr; }

#endif
