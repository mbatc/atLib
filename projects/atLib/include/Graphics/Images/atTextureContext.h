
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

#ifndef atTextureContext_h__
#define atTextureContext_h__

#include "atGraphics.h"
#include "atImage.h"

class atTextureContext
{
public:
  atTextureContext(const atVector<atCol> &colour, const atVec2I &size, const int64_t sampleCount = 1); // Create a colour texture
  atTextureContext(const atVector<float> &depth, const atVec2I &size, const int64_t sampleCount = 1); // Create a depth texture
  atTextureContext(ID3D11Texture2D *pTexture, const bool genMipmaps = true, const int64_t sampleCount = 1);
  atTextureContext(const void *pData, const atVec2I &size, const bool depthTexture = false, const bool genMipmaps = true, const int64_t sampleCount = 1);
  atTextureContext(const atImage &file, const bool genMipmaps = true, const int64_t sampleCount = 1);
  atTextureContext(const atFilename &file, const bool genMipmaps = true, const int64_t sampleCount = 1);
  atTextureContext(const atTextureContext &copy);
  atTextureContext(atTextureContext &&move);
  ~atTextureContext();

  void Release();

  void UpdateTexture(const atImage &image, const bool genMipmaps = true, const int64_t sampleCount = 1);

  atVec2I Size() const;
  
  operator ID3D11Texture2D *();
  operator ID3D11ShaderResourceView *();
  operator ID3D11RenderTargetView *();
  operator ID3D11DepthStencilView *();
  operator ID3D11UnorderedAccessView *();
  operator ID3D11Texture2D **();
  operator ID3D11ShaderResourceView **();
  operator ID3D11RenderTargetView **();
  operator ID3D11DepthStencilView **();
  operator ID3D11UnorderedAccessView **();

  const atTextureContext& operator=(const atTextureContext &rhs);
  const atTextureContext& operator=(atTextureContext &&rhs);

protected:
  bool m_genMipmaps;
  bool m_depthTexture;
  int64_t m_sampleCount;

  ID3D11ShaderResourceView *m_pShaderView = nullptr;
  ID3D11RenderTargetView *m_pRenderView = nullptr;
  ID3D11DepthStencilView *m_pDepthView = nullptr;
  ID3D11UnorderedAccessView *m_pUAView = nullptr;
  ID3D11Texture2D *m_pTexture = nullptr;
};

#endif // atTextureContext_h__
