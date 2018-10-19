
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

#include "atRenderable.h"
#include "atShaderPool.h"
#include "atRenderState.h"
#include "atHardwareTexture.h"

atRenderable::atRenderable()
  : m_pVertBuffer(nullptr)
  , m_pIndexBuffer(nullptr)
  , m_nIndices(0)
  , m_nVerts(0)
  , m_shaderID(AT_INVALID_ID)
  , m_layoutID(AT_INVALID_ID)
{}

atRenderable::atRenderable(atRenderable &&move)
{
  m_pIndexBuffer = move.m_pIndexBuffer;
  m_pVertBuffer = move.m_pVertBuffer;
  m_nVerts = move.m_nVerts;
  m_nIndices = move.m_nIndices;
  m_shaderRound = move.m_shaderRound;
  m_layoutID = move.m_layoutID;
  m_layout = std::move(move.m_layout);
  m_resource = std::move(move.m_resource);
  m_shader = std::move(move.m_shader);
  m_shaderID = std::move(move.m_shaderID);
  move.m_layoutID = AT_INVALID_ID;
  move.m_shaderID = AT_INVALID_ID;
  move.m_shaderRound = AT_INVALID_ID;
  move.m_nIndices = 0;
  move.m_nVerts = 0;
  move.m_pIndexBuffer = nullptr;
  move.m_pVertBuffer = nullptr;
}

atRenderable::atRenderable(const atRenderable &copy)
  : m_pVertBuffer(nullptr)
  , m_pIndexBuffer(nullptr)
  , m_nIndices(0)
  , m_nVerts(0)
  , m_shaderID(AT_INVALID_ID)
  , m_layoutID(AT_INVALID_ID)
{
  for (auto &res : copy.m_resource)
    m_resource.Add(res.m_key, res.m_val);
  m_shader = copy.m_shader;
}

atRenderable::~atRenderable() { Clear(); }

bool atRenderable::Draw(const atRenderable_PrimitiveType type /*= atRPT_TriangleList*/)
{
  Rebuild();
  if (m_shaderID == AT_INVALID_ID || m_shaderRound != atShaderPool::ShaderRound())
  {
    m_shaderID = atShaderPool::GetShader(m_shader); 
    m_layoutID = atShaderPool::GetInputLayout(m_shaderID, m_layout);
  }

  if (m_shaderID == AT_INVALID_ID || m_layoutID == AT_INVALID_ID)
    return false;

  m_shaderRound = atShaderPool::ShaderRound();
    
  // Get Resources and Locations
  for (auto &kvp : m_resource)
  {
    if(kvp.m_val.id != AT_INVALID_ID)
      continue;

    switch (kvp.m_val.type)
    {
    case atRRT_Texture: kvp.m_val.id = atHardwareTexture::UploadTexture(atString((char*)kvp.m_val.data.data())); break;
    case atRRT_Sampler: kvp.m_val.id = atHardwareTexture::CreateSampler(); break;
    case atRRT_Variable: kvp.m_val.loc = atShaderPool::GetVariableLoc(m_shaderID, kvp.m_key); break;
    }
    if (kvp.m_val.id == AT_INVALID_ID) kvp.m_val.id = 0;
  }

  // Upload and Bind Resources
  for (auto &kvp : m_resource)
  {
    if (kvp.m_val.id == AT_INVALID_ID)
      continue;

    switch (kvp.m_val.type)
    {
    case atRRT_Texture: atShaderPool::SetVariable(m_shaderID, kvp.m_key, (void*)atHardwareTexture::GetTexture(kvp.m_val.id), 0); break;
    case atRRT_Sampler: atShaderPool::SetVariable(m_shaderID, kvp.m_key, (void*)atHardwareTexture::GetSampler(kvp.m_val.id), 0); break;
    case atRRT_Variable: atShaderPool::SetVariable(m_shaderID, kvp.m_val.loc, kvp.m_val.data.data(), kvp.m_val.data.size()); break;
    }
  }

  // Bind Shader to DX Context
  atRenderState::BindShader(m_shaderID);
  atShaderPool::BindInputLayout(m_layoutID);

  // Bind Vertex Buffer and Index Buffer (if not null) then Draw
  UINT offset = 0;
  UINT stride = (UINT)m_stride;
  atGraphics::GetContext()->IASetVertexBuffers(0, 1, &m_pVertBuffer, &stride, &offset);
  switch (type)
  {
  case atRPT_TriangleList: atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
  case atRPT_LineList: atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); break;
  case atRPT_PointList: atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
  case atRPT_TriangleListAdj: atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ); break;
  case atRPT_LineListAdj: atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ); break;
  case atRPT_TriangleStrip: atGraphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
  }

  bool indexed = type == atRPT_TriangleList || type == atRPT_LineList || type == atRPT_PointList;
  if (m_pIndexBuffer && indexed)
  {
    atGraphics::GetContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    atGraphics::GetContext()->DrawIndexed((UINT)m_nIndices, 0, 0);
  }
  else
    atGraphics::GetContext()->Draw((UINT)m_nVerts, 0);
  return true;
}

bool atRenderable::Rebuild()
{
  bool vbInvalid = false;
  bool ibInvalid = false;

  int64_t vbCount = 0;
  int64_t ibCount = 0;

  for (auto &kvp : m_resource)
    if (kvp.m_val.type == atRRT_VertexData)
    {
      vbInvalid |= kvp.m_val.id == AT_INVALID_ID;
      ++vbCount;
    }
    else if (kvp.m_val.type == atRRT_Indices) 
    {
      ibInvalid |= kvp.m_val.id == AT_INVALID_ID;
      ++ibCount;
    }

  bool res = true;
  if (vbInvalid || vbCount == 0)
  {
    atGraphics::SafeRelease(m_pVertBuffer);

    m_layout.clear();
    int64_t nVerts = 0;
    for (auto &kvp : m_resource)
    {
      if (kvp.m_val.type != atRRT_VertexData)
        continue;

      kvp.m_val.id = 0;
      m_layout.push_back(VertexData{ kvp.m_key, kvp.m_val.desc });
      atAssert(kvp.m_val.count == nVerts || nVerts == 0, "All per vertex vectors must be of the same size()!");
      nVerts = kvp.m_val.count;
    }

    m_stride = 0;
    for (const VertexData &data : m_layout)
      m_stride += data.desc.size;
    atVector<uint8_t> rawVert(m_stride * nVerts, 0);

    int64_t offset = 0;
    for (int64_t i = 0; i < nVerts; ++i)
      for (const VertexData &data : m_layout)
      {
        Resource &res = m_resource.Get(data.semantic);
        memcpy(rawVert.data() + offset, res.data.data() + i * data.desc.size, (size_t)data.desc.size);
        offset += data.desc.size;
      }

    // Create D3D11 Vertex Buffer
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)rawVert.size();
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = rawVert.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    m_nVerts = nVerts;
    res &= SUCCEEDED(atGraphics::GetDevice()->CreateBuffer(&desc, &data, &m_pVertBuffer));
  }

  if (ibInvalid || ibCount == 0)
  {
    atGraphics::SafeRelease(m_pIndexBuffer);

    Resource *pIndices = nullptr;
    for (auto &kvp : m_resource)
      if (kvp.m_val.type == atRRT_Indices)
      {
        pIndices = &kvp.m_val;
        pIndices->id = 0;
        break;
      }

    if (pIndices)
    {
      // Create D3D11 Index Buffer
      D3D11_BUFFER_DESC desc;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.ByteWidth = (UINT)pIndices->data.size();
      desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
      desc.CPUAccessFlags = 0;
      desc.MiscFlags = 0;

      D3D11_SUBRESOURCE_DATA data;
      data.pSysMem = pIndices->data.data();
      data.SysMemPitch = 0;
      data.SysMemSlicePitch = 0;

      m_nIndices = pIndices->count;
      res &= SUCCEEDED(atGraphics::GetDevice()->CreateBuffer(&desc, &data, &m_pIndexBuffer));
    }
  }

  return res;
}

void atRenderable::Clear()
{
  atGraphics::SafeRelease(m_pIndexBuffer);
  atGraphics::SafeRelease(m_pVertBuffer);
  m_layoutID = AT_INVALID_ID;
  m_shaderID = AT_INVALID_ID;
  m_nIndices = 0;
  m_nVerts = 0;
  m_resource.Clear();
}

atRenderable::Resource& atRenderable::GetResource(const atString &name)
{
  Resource *pRes = m_resource.TryGet(name);
  if (pRes)
    return *pRes;
  m_resource.Add(name);
  return m_resource.Get(name);
}

void atRenderable::FreeResource(const atString &name)
{
  Resource *pRes = m_resource.TryGet(name);
  if (!pRes)
    return;
  m_resource.Remove(name);
}

void atRenderable::SetShader(const atString &name) { m_shader = name; m_layoutID = -1; }
