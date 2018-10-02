
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

#include "atHardwareTexture.h"
#include "atGraphics.h"
#include "atHashMap.h"
#include "atImage.h"

int64_t s_imageCounter = 0;
int64_t s_samplerCounter = 0;

atHashMap<int64_t, ID3D11ShaderResourceView*> s_image_store;
atHashMap<int64_t, ID3D11SamplerState*> s_sampler_store;
atHashMap<atFilename, int64_t> s_imageID;
atHashMap<D3D11_SAMPLER_DESC, int64_t> s_samplerID;

int64_t atHardwareTexture::CreateSampler(const int64_t filter, const atTexCoordMode uMode, const atTexCoordMode vMode, const atTexCoordMode wMode, const float mipLodBias, const atComparison compFunc, const atVec4F & borderCol, const float minLOD, const float maxLOD)
{
  D3D11_SAMPLER_DESC desc;
  desc.Filter = (D3D11_FILTER)filter;
  desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)uMode;
  desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)vMode;
  desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)wMode;
  desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)compFunc;
  desc.MaxLOD = maxLOD;
  desc.MinLOD = minLOD;
  desc.MipLODBias = mipLodBias;
  desc.MaxAnisotropy = 0;
  memcpy(desc.BorderColor, borderCol.data(), atGetTypeDesc(borderCol).size);

  int64_t *pID = s_samplerID.TryGet(desc);
  if (pID)
    return *pID;

  ID3D11SamplerState *pSampler = nullptr;
  atGraphics::GetDevice()->CreateSamplerState(&desc, &pSampler);
  s_sampler_store.Add(s_samplerCounter, pSampler);
  s_samplerID.Add(desc, s_samplerCounter);
  return s_samplerCounter++;
}

int64_t atHardwareTexture::UploadTexture(const atFilename &file)
{
  int64_t *pID = s_imageID.TryGet(file);
  if (pID)
    return *pID;

  ID3D11ShaderResourceView *pShaderRes = nullptr;
  atImage image(file);
  
  D3D11_SHADER_RESOURCE_VIEW_DESC desc;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
  desc.Texture2D.MipLevels = 1;
  desc.Texture2D.MostDetailedMip = 0;

  D3D11_TEXTURE2D_DESC texDesc;
  texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  texDesc.Height = (UINT)image.Height();
  texDesc.Width = (UINT)image.Width();
  texDesc.Usage = D3D11_USAGE_DEFAULT;
  texDesc.SampleDesc.Quality = 0;
  texDesc.SampleDesc.Count = 1;
  texDesc.CPUAccessFlags = 0;
  texDesc.ArraySize = 1;
  texDesc.MipLevels = 1;
  texDesc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA data;
  data.pSysMem = image.Pixels().data();
  data.SysMemPitch = (UINT)(image.Width() * sizeof(atCol));
  data.SysMemSlicePitch = 0;

  ID3D11Texture2D *pTexture = nullptr;
  atGraphics::GetDevice()->CreateTexture2D(&texDesc, &data, &pTexture);
  
  if (!pTexture)
    return AT_INVALID_ID;

  atGraphics::GetDevice()->CreateShaderResourceView(pTexture, &desc, &pShaderRes);
  pTexture->Release();
  pTexture = nullptr;

  s_image_store.Add(s_imageCounter, pShaderRes);
  s_imageID.Add(file, s_imageCounter);
  return s_imageCounter++;
}

void* atHardwareTexture::GetTexture(const int64_t id) { ID3D11ShaderResourceView **ppShaderRes = s_image_store.TryGet(id); return ppShaderRes ? *ppShaderRes : nullptr; }
void* atHardwareTexture::GetSampler(const int64_t id) { ID3D11SamplerState **ppSampler = s_sampler_store.TryGet(id); return ppSampler ? *ppSampler : nullptr; }

// A hacky way of releasing resources when the program ends
struct CleanupStruct
{
  ~CleanupStruct()
  {
    for (auto &kvp : s_image_store)
      atGraphics::SafeRelease(kvp.m_val);
    for (auto &kvp : s_sampler_store)
      atGraphics::SafeRelease(kvp.m_val);
  }
};
static CleanupStruct cleanup;