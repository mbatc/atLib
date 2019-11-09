
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
#include "atTextureContext.h"
#include "atRenderState.h"
#include <stdlib.h>

void atGraphics::Shutdown()
{
  atDirectX::Shutdown();
}

bool atGraphics::BindShaderResource(const atShaderType shader, const atShader_ResourceType resType, const int64_t slot, void *pResource)
{
  if (slot < 0 || !pResource)
    return false;

  ID3D11Buffer *pBuffer;
  atTextureContext *pTexture;
  ID3D11SamplerState *pSampler;
  ID3D11DeviceContext *pContext = atDirectX::GetContext();
  switch (resType)
  {
  case atSRT_Buffer:
    pBuffer = (ID3D11Buffer*)pResource;
    switch (shader)
    {
    case atST_Vertex: pContext->VSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Pixel: pContext->PSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Domain: pContext->DSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Hull: pContext->HSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Compute: pContext->CSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    case atST_Geometry: pContext->GSSetConstantBuffers((UINT)slot, 1, &pBuffer); break;
    default: return false;
    }
    break;
  case atSRT_Sampler:
    pSampler = (ID3D11SamplerState*)pResource;
    switch (shader)
    {
    case atST_Vertex: pContext->VSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Pixel: pContext->PSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Domain: pContext->DSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Hull: pContext->HSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Compute: pContext->CSSetSamplers((UINT)slot, 1, &pSampler); break;
    case atST_Geometry: pContext->GSSetSamplers((UINT)slot, 1, &pSampler); break;
    default: return false;
    }
    break;
  case atSRT_Texture:
    pTexture = (atTextureContext*)pResource;
    switch (shader)
    {
    case atST_Vertex: pContext->VSSetShaderResources((UINT)slot, 1, *pTexture); break;
    case atST_Pixel: pContext->PSSetShaderResources((UINT)slot, 1, *pTexture); break;
    case atST_Domain: pContext->DSSetShaderResources((UINT)slot, 1, *pTexture); break;
    case atST_Hull: pContext->HSSetShaderResources((UINT)slot, 1, *pTexture); break;
    case atST_Compute: pContext->CSSetShaderResources((UINT)slot, 1, *pTexture); break;
    case atST_Geometry: pContext->GSSetShaderResources((UINT)slot, 1, *pTexture); break;
    default: return false;
    }
    break;
  default: return false;
  }
  return true;
}

bool atGraphics::CreateBuffer(ID3D11Buffer **ppBuffer, void *pData, int64_t size, int64_t binding, int64_t usage, int64_t cpuAccess)
{
  if (!ppBuffer)
    return false;
  atDirectX::SafeRelease(*ppBuffer);
  D3D11_BUFFER_DESC desc = { 0 };
  desc.Usage = (D3D11_USAGE)usage;
  desc.BindFlags = (UINT)binding;
  desc.ByteWidth = (UINT)size;
  desc.CPUAccessFlags = (UINT)cpuAccess;

  D3D11_SUBRESOURCE_DATA data = { 0 };
  data.pSysMem = pData;
  data.SysMemPitch = 0;
  data.SysMemSlicePitch = 0;
  return !FAILED(atDirectX::GetDevice()->CreateBuffer(&desc, pData ? &data : nullptr, ppBuffer));
}
