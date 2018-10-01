
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

  move.m_nIndices = 0;
  move.m_nVerts = 0;
  move.m_pIndexBuffer = nullptr;
  move.m_pVertBuffer = nullptr;
}

atRenderable::~atRenderable() { Clear(); }

bool atRenderable::Draw(const atMat4 &mvp, const atRenderable_PrimitiveType type /*= atRPT_TriangleList*/)
{
  if (!m_pVertBuffer || !m_pIndexBuffer)
    if (!Rebuild())
      return false;

  if (m_shaderID == AT_INVALID_ID || m_shaderRound != atShaderPool::ShaderRound())
  {
    m_shaderID = atShaderPool::GetShader(m_shader); 
    m_layoutID = atShaderPool::GetInputLayout(m_shaderID, m_layout);
  }

  if (m_shaderID == AT_INVALID_ID || m_layoutID == AT_INVALID_ID)
    return false;

  m_shaderRound = atShaderPool::ShaderRound();

  UINT offset = 0;
  UINT stride = (UINT)m_stride;
  
  for (auto &kvp : m_textures)
    atShaderPool::SetVariable(m_shaderID, kvp.m_key, (void*)atHardwareTexture::GetResource(kvp.m_val), 0);
  atShaderPool::SetVariable(m_shaderID, "mvp", (void*)mvp.m_data, atGetTypeDesc<float>().size * 16);

  atRenderState::BindShader(m_shaderID);
  atShaderPool::BindInputLayout(m_layoutID);
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
  m_layout.clear();
  int64_t nVerts = 0;
  for (auto &kvp : m_positions)
    if (kvp.m_val.size() > 0)
    {
      m_layout.push_back(VertexData{ kvp.m_key, atGetTypeDesc(kvp.m_val[0]) });
      atAssert(kvp.m_val.size() == nVerts || nVerts == 0, "All per vertex vectors must be of the same size()!");
      nVerts = kvp.m_val.size();
    }

  for (auto &kvp : m_normals)
    if (kvp.m_val.size() > 0)
    {
      m_layout.push_back(VertexData{ kvp.m_key, atGetTypeDesc(kvp.m_val[0]) });
      atAssert(kvp.m_val.size() == nVerts || nVerts == 0, "All per vertex vectors must be of the same size()!");
      nVerts = kvp.m_val.size();
    }

  for (auto &kvp : m_colors)
    if (kvp.m_val.size() > 0)
    {
      m_layout.push_back(VertexData{ kvp.m_key, atGetTypeDesc(kvp.m_val[0]) });
      atAssert(kvp.m_val.size() == nVerts || nVerts == 0, "All per vertex vectors must be of the same size()!");
      nVerts = kvp.m_val.size();
    }

  for (auto &kvp : m_texCoords)
    if (kvp.m_val.size() > 0)
    {
      m_layout.push_back(VertexData{ kvp.m_key, atGetTypeDesc(kvp.m_val[0]) });
      atAssert(kvp.m_val.size() == nVerts || nVerts == 0, "All per vertex vectors must be of the same size()!");
      nVerts = kvp.m_val.size();
    }

  m_stride = 0;
  for (const VertexData &data : m_layout)
    m_stride += data.desc.size;
  atVector<uint8_t> rawVert(m_stride * nVerts, 0);

  int64_t offset = 0;
  for (int64_t i = 0; i < nVerts; ++i)
    for (const VertexData &data : m_layout)
    {
      atVector<atVec3F> *pPos = m_positions.TryGet(data.semantic);
      atVector<atVec4F> *pCol = m_colors.TryGet(data.semantic);
      atVector<atVec3F> *pNorm = m_normals.TryGet(data.semantic);
      atVector<atVec2F> *pCoord = m_texCoords.TryGet(data.semantic);

      if (pPos) memcpy(rawVert.data() + offset, (pPos->data() + i)->data(), (size_t)data.desc.size);
      if (pCol) memcpy(rawVert.data() + offset, (pCol->data() + i)->data(), (size_t)data.desc.size);
      if (pNorm) memcpy(rawVert.data() + offset, (pNorm->data() + i)->data(), (size_t)data.desc.size);
      if (pCoord) memcpy(rawVert.data() + offset, (pCoord->data() + i)->data(), (size_t)data.desc.size);
      offset += data.desc.size;
    }

  atGraphics::SafeRelease(m_pVertBuffer);
  atGraphics::SafeRelease(m_pIndexBuffer);

  bool res = true;
  { // Create D3D11 Vertex Buffer
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

  { // Create D3D11 Index Buffer
    D3D11_BUFFER_DESC desc;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = (UINT)(sizeof(uint32_t) * m_indices.size());
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = m_indices.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    m_nIndices = m_indices.size();
    res &= SUCCEEDED(atGraphics::GetDevice()->CreateBuffer(&desc, &data, &m_pIndexBuffer));
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

  m_texCoords.Clear();
  m_positions.Clear();
  m_indices.clear();
  m_normals.Clear();
  m_colors.Clear();
}

void atRenderable::SetShader(const atString &name) { m_shader = name; m_layoutID = -1; }
