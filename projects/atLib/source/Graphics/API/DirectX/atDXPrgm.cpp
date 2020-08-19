#ifdef atPLATFORM_WIN32

#include "atDXShader.h"
#include "atGraphics.h"
#include "atDirectX.h"
#include "atDXPrgm.h"
#include "atFormat.h"
#include "atDXInclude_Internal.h"

static atDXPrgm *_pBoundProgram = nullptr;

bool atDXPrgm::Bind()
{
  if (!Upload() || !BindStages() || (m_pIndices && m_pIndices->Desc().type != atType_Uint16 && m_pIndices->Desc().type != atType_Uint32))
    return false;

  BindLayout();
  BindIndices();
  for (int64_t i = 0; i < m_inputs.size(); ++i)
    BindInput(i);

  _pBoundProgram = this;
  return true;
}

void atDXPrgm::BindInput(const int64_t &loc)
{
  if (!m_inputs[loc])
    return;

  atDXShader *pVertShader = (atDXShader*)m_pStages[atPS_Vertex];
  if (!pVertShader)
    return;

  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
  UINT slot = (UINT)pVertShader->AttributeSlot(loc);
  UINT offset = 0;
  UINT stride = (UINT)(m_inputs[loc]->Size() / atMax(1ll, m_inputs[loc]->Count()));
  ID3D11Buffer *pBuffer = (ID3D11Buffer*)m_inputs[loc]->NativeResource();
  pCtx->IASetVertexBuffers(slot, 1, &pBuffer, &stride, &offset);
}

bool atDXPrgm::BindAttribute(const atString &name, atGPUBuffer *pBuffer)
{
  return BindAttribute(FindAttribute(name), pBuffer);
}

bool atDXPrgm::BindIndices(atGPUBuffer *pBuffer)
{
  if (pBuffer->API() != API())
    return false;

  atShader* pVertShader = m_pStages[atPS_Vertex];
  if (!pVertShader)
    return false;

  m_pIndices = (atDXBuffer*)pBuffer;
  if (IsBound())
    BindIndices();

  return true;
}

bool atDXPrgm::BindTexture(const atString &name, atTexture *pTexture) { return BindTexture(FindTexture(name), pTexture); }
bool atDXPrgm::BindSampler(const atString &name, atSampler *pSampler) { return BindSampler(FindSampler(name), pSampler); }
bool atDXPrgm::SetUniform(const atString &name, const void *pData, const atTypeDesc &info) { return SetUniform(FindUniform(name), pData, info); }

int64_t atDXPrgm::FindUniform(const atString &name) const { return m_uniformLookup.GetOr(name, AT_INVALID_INDEX); }
int64_t atDXPrgm::FindAttribute(const atString &name) const { return m_attributeLookup.GetOr(name, AT_INVALID_INDEX); }
int64_t atDXPrgm::FindTexture(const atString &name) const { return m_textureLookup.GetOr(name, AT_INVALID_INDEX); }
int64_t atDXPrgm::FindSampler(const atString &name) const { return m_samplerLookup.GetOr(name, AT_INVALID_INDEX); }

bool atDXPrgm::BindAttribute(const int64_t &index, atGPUBuffer *pBuffer)
{
  if (pBuffer->API() != API() || index < 0 || index >= m_attributeLookup.Size())
    return false;

  atDXShader *pVertShader = (atDXShader *)m_pStages[atPS_Vertex];
  if (!pVertShader)
    return false;
  int64_t loc = m_attributeLoc[index];
  m_inputs[loc] = (atDXBuffer *)pBuffer;
  if (IsBound())
    BindInput(loc);
  return true;
}

bool atDXPrgm::BindTexture(const int64_t &index, atTexture *pTexture)
{
  if (pTexture->API() != API() || index < 0 || index >= m_textureLoc.size())
    return false;

  atKeyValue<int64_t, int64_t> &loc = m_textureLoc[index];
  ((atDXShader *)m_pStages[loc.m_key])->BindTexture(loc.m_val, pTexture);
  return false;
}

bool atDXPrgm::BindSampler(const int64_t &index, atSampler *pSampler)
{
  if (pSampler->API() != API() || index < 0 || index >= m_samplerLoc.size())
    return false;

  atKeyValue<int64_t, int64_t> &loc = m_samplerLoc[index];
  ((atDXShader *)m_pStages[loc.m_key])->BindSampler(loc.m_val, pSampler);
  return false;
}

bool atDXPrgm::SetUniform(const int64_t &index, const void *pData, const atTypeDesc &info)
{
  if (index < 0 || index >= m_uniformLoc.size())
    return false;

  atKeyValue<int64_t, atDXShader::UniformLocation> &loc = m_uniformLoc[index];
  ((atDXShader *)m_pStages[loc.m_key])->SetUniform(loc.m_val.bufferIdx, loc.m_val.variableIdx, pData, info);
  return true;
}

bool atDXPrgm::HasUniform(const atString &name) const { return FindUniform(name) != -1; }

bool atDXPrgm::Upload()
{
  if (!(m_pStages[atPS_Vertex] && m_pStages[atPS_Fragment]))
    return false; // Must have at least a vertex and fragment shader

  if (m_pLayout && !ShouldReload())
    return true;

  Delete(); // Delete previously compiled shader

  bool success = true;
  for (atShader* pStage : m_pStages)
    if (pStage)
      success &= pStage->Upload();

  // Create lookups for the uniforms, textures and samplers
  for (int64_t shaderIdx = 0; shaderIdx < atArraySize(m_pStages); ++shaderIdx)
  {
    atDXShader *pShader = (atDXShader*)m_pStages[shaderIdx];
    if (!pShader)
      continue;

    for (const atKeyValue<atString, atDXShader::UniformLocation> &loc : pShader->Uniforms())
    {
      m_uniformLookup.Add(loc.m_key, m_uniformLoc.size());
      m_uniformLoc.emplace_back(shaderIdx, loc.m_val);
    }

    atVector<atString> texNames = pShader->Textures();
    for (int64_t texIdx = 0; texIdx < texNames.size(); ++texIdx)
    {
      m_textureLookup.Add(texNames[texIdx], m_textureLoc.size());
      m_textureLoc.emplace_back(shaderIdx, texIdx);
    }

    atVector<atString> samplerNames = pShader->Samplers();
    for (int64_t samplerIdx = 0; samplerIdx < samplerNames.size(); ++samplerIdx)
    {
      m_samplerLookup.Add(samplerNames[samplerIdx], m_samplerLoc.size());
      m_samplerLoc.emplace_back(shaderIdx, samplerIdx);
    }
  }

  // Create lookups for the uniforms, textures and samplers
  for (int64_t shaderIdx = 0; shaderIdx < atArraySize(m_pStages); ++shaderIdx)
  {
    atDXShader *pShader = (atDXShader*)m_pStages[shaderIdx];
    if (!pShader)
      continue;

    for (const atKeyValue<atString, atDXShader::UniformLocation> &loc : pShader->Uniforms())
    {
      m_uniformLookup.Add(loc.m_key, m_uniformLoc.size());
      m_uniformLoc.emplace_back(shaderIdx, loc.m_val);
    }

    atVector<atString> texNames = pShader->Textures();
    for (int64_t texIdx = 0; texIdx < texNames.size(); ++texIdx)
    {
      m_textureLookup.Add(texNames[texIdx], m_textureLoc.size());
      m_textureLoc.emplace_back(shaderIdx, texIdx);
    }

    atVector<atString> samplerNames = pShader->Samplers();
    for (int64_t samplerIdx = 0; samplerIdx < samplerNames.size(); ++samplerIdx)
    {
      m_samplerLookup.Add(samplerNames[samplerIdx], m_samplerLoc.size());
      m_samplerLoc.emplace_back(shaderIdx, samplerIdx);
    }
  }

  atDXShader *pShader = (atDXShader*)m_pStages[atPS_Vertex];
  atVector<D3D11_INPUT_ELEMENT_DESC> d3dDesc;
  for (int64_t i = 0; i < pShader->AttributeCount(); ++i)
  {
    D3D11_INPUT_ELEMENT_DESC element;
    atTypeDesc typeDesc = pShader->AttributeInfo(i);
    element.SemanticName = pShader->AttributeName(i).c_str();
    element.SemanticIndex = (UINT)pShader->AttributeIndex(i);
    element.Format = (DXGI_FORMAT)atFormat::DXGI(typeDesc.type, typeDesc.width);
    element.InputSlot = (UINT)pShader->AttributeSlot(i);
    element.AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
    element.InstanceDataStepRate = 0;
    element.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
    d3dDesc.push_back(element);

    m_attributeLookup.Add(element.SemanticName, m_attributeLoc.size());
    m_attributeLoc.push_back(i);
  }
  m_inputs.clear();
  m_inputs.resize(d3dDesc.size(), 0);

  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11InputLayout *pLayout = nullptr;
  ID3D11Device *pCtx = (ID3D11Device*)pDX->GetDevice();
  if (FAILED(pCtx->CreateInputLayout(d3dDesc.data(), (UINT)d3dDesc.size(), pShader->ByteCode().data(), (size_t)pShader->ByteCode().size(), &pLayout)))
    return false;

  m_pLayout = pLayout;
  m_shaderRound = m_globalShaderRound;
  return true;
}

bool atDXPrgm::Delete()
{
  if (!m_pLayout)
    return false;

  // Delete input layout
  ID3D11InputLayout *pLayout = (ID3D11InputLayout*)m_pLayout;
  atDirectX::SafeRelease(pLayout);

  // Delete sub shaders
  for (atShader *pShader : m_pStages)
    pShader->Delete();

  // Clear inputs
  m_inputs.clear();
  m_pLayout = nullptr;
  m_attributeLoc.clear();
  m_textureLoc.clear();
  m_samplerLoc.clear();
  m_uniformLoc.clear();
  m_uniformLookup.Clear();
  m_textureLookup.Clear();
  m_samplerLookup.Clear();
  m_attributeLookup.Clear();
  return true;
}

bool atDXPrgm::Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType, const int64_t &elementCount, const int64_t &elementOffset, const int64_t &baseVtxIdx)
{
  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();

  for (auto &pShader : m_pStages)
  {
    if (!pShader)
      continue;

    atDXShader *pDXShader = (atDXShader*)pShader;
    pDXShader->UpdateConstantBuffers();
  }

  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();

  switch (primType)
  {
  case atGFX_PT_LineList: pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); break;
  case atGFX_PT_PointList: pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
  case atGFX_PT_LineListAdj: pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ); break;
  case atGFX_PT_TriangleList: pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
  case atGFX_PT_TriangleStrip: pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
  case atGFX_PT_TriangleListAdj: pCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ); break;
  }

  if (m_pIndices && indexedMode)
    pDX->DrawIndexed(elementCount < 0 ? m_pIndices->Count() : elementCount, elementOffset, baseVtxIdx);
  else
    pDX->Draw(elementCount < 0 ? m_inputs[0]->Count() : elementCount, elementOffset);

  return true;
}

void atDXPrgm::BindLayout()
{
  if (!m_pLayout)
    return;

  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
  pCtx->IASetInputLayout((ID3D11InputLayout*)m_pLayout);
}

void atDXPrgm::BindIndices()
{
  if (!m_pIndices)
    return;

  atDirectX *pDX = (atDirectX*)atGraphics::GetCurrent();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
  pCtx->IASetIndexBuffer((ID3D11Buffer*)m_pIndices->NativeResource(), m_pIndices->Desc().type == atType_Uint16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
}

bool atDXPrgm::IsBound() { return _pBoundProgram == this; }

#else

bool atDXPrgm::Bind() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
void atDXPrgm::BindInput(const int64_t &) { atRelAssert("DirectX is only supported on Windows platforms."); }
bool atDXPrgm::BindAttribute(const atString &, atGPUBuffer *) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::BindIndices(atGPUBuffer *) { atRelAssert("DirectX is only supported on Windows platforms."); return true; }
bool atDXPrgm::BindTexture(const atString &, atTexture *) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::BindSampler(const atString &, atSampler *) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::SetUniform(const atString &, const void *, const atTypeDesc &) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::HasUniform(const atString &) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::Upload() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::Delete() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXPrgm::Draw(const bool &, const atGFX_PrimitiveType &, const int64_t &, const int64_t &, const int64_t &) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
void atDXPrgm::BindLayout() { atRelAssert("DirectX is only supported on Windows platforms."); }
void atDXPrgm::BindIndices() { atRelAssert("DirectX is only supported on Windows platforms."); }
bool atDXPrgm::IsBound() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }

#endif
