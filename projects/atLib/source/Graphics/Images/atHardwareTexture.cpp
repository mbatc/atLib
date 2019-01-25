
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
#include "atTextureContext.h"

int64_t s_imageCounter = 0;
int64_t s_samplerCounter = 0;

atHashMap<int64_t, atTextureContext*> s_textures;
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
  memcpy(desc.BorderColor, borderCol.data(), (size_t)atGetTypeDesc(borderCol).size);

  int64_t *pID = s_samplerID.TryGet(desc);
  if (pID)
    return *pID;

  ID3D11SamplerState *pSampler = nullptr;
  atGraphics::GetDevice()->CreateSamplerState(&desc, &pSampler);
  s_sampler_store.Add(s_samplerCounter, pSampler);
  s_samplerID.Add(desc, s_samplerCounter);
  return s_samplerCounter++;
}

int64_t atHardwareTexture::AddTexture(ID3D11Texture2D *pTexture, const bool genMipmaps, const int64_t sampleCount)
{
  s_textures.Add(s_imageCounter, atNew<atTextureContext>(pTexture, genMipmaps, sampleCount));
  return s_imageCounter++;
}

int64_t atHardwareTexture::UploadTexture(const atFilename &file, const bool genMipmaps, const int64_t sampleCount)
{
  int64_t *pID = s_imageID.TryGet(file);
  if (pID)
    return *pID;
  s_imageID.Add(file, s_imageCounter);
  return UploadTexture(atImage(file), genMipmaps, sampleCount);
}

int64_t atHardwareTexture::UploadTexture(const atImage &image, const bool genMipmaps, const int64_t sampleCount)
{
  s_textures.Add(s_imageCounter, atNew<atTextureContext>(image, genMipmaps, sampleCount));
  return s_imageCounter++;
}

bool atHardwareTexture::UpdateTexture(const int64_t id, const atImage &image, const bool genMipmaps, const int64_t sampleCount)
{
  atTextureContext **ppTex = s_textures.TryGet(id);
  if(!ppTex)
    return false;
  (*ppTex)->UpdateTexture(image, genMipmaps, sampleCount);
  return true;
}

int64_t atHardwareTexture::UploadDepthTexture(const float *pPixels, const atVec2I &size, const int64_t sampleCount)
{
  s_textures.Add(s_imageCounter, atNew<atTextureContext>(pPixels, size, true, false, sampleCount));
  return s_imageCounter++;
}

bool atHardwareTexture::UpdateDepthTexture(const int64_t id, const float *pPixels, const atVec2I &size, const int64_t sampleCount)
{
  atTextureContext **ppTex = s_textures.TryGet(id);
  if (!ppTex)
    return false;
  (**ppTex) = atTextureContext(pPixels, size, true, false, sampleCount);
  return true;
}

void atHardwareTexture::DeleteTexture(const int64_t id)
{
  atTextureContext **pTex = s_textures.TryGet(id);
  if (!pTex)
    return;
  atGraphics::SafeRelease(*pTex);
  s_textures.Remove(id);
}

void atHardwareTexture::DeleteSampler(const int64_t id)
{
  ID3D11SamplerState **ppSample = s_sampler_store.TryGet(id);
  if (!ppSample)
    return;
  atGraphics::SafeRelease(*ppSample);
  s_sampler_store.Remove(id);
}

atTextureContext* atHardwareTexture::GetTexture(const int64_t id) { atTextureContext **ppTexture = s_textures.TryGet(id); return ppTexture ? *ppTexture : nullptr; }
void* atHardwareTexture::GetSampler(const int64_t id) { ID3D11SamplerState **ppSampler = s_sampler_store.TryGet(id); return ppSampler ? *ppSampler : nullptr; }
int64_t atHardwareTexture::UploadTexture(const atCol *pPixels, const atVec2I &size, const bool genMipmaps, const int64_t sampleCount) { return UploadTexture(atImage(pPixels, size), genMipmaps, sampleCount); }

// A hacky way of releasing resources when the program ends
struct CleanupStruct
{
  ~CleanupStruct()
  {
    s_textures.Clear();
    for (auto &kvp : s_sampler_store)
      atGraphics::SafeRelease(kvp.m_val);
  }
};

static CleanupStruct cleanup;