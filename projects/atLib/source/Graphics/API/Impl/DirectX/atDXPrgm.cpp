#include "atDXPrgm.h"
#include "atDirectX.h"
#include "atDXShader.h"
#include "atFormat.h"

static atDXPrgm *_pBoundProgram = nullptr;

bool atDXPrgm::Bind()
{
  if (!Upload() || !BindStages() || (m_pIndices && m_pIndices->Desc().type != atType_Uint16 && m_pIndices->Desc().type != atType_Float32))
    return false;

  BindLayout();
  BindIndices();
  for (int64_t i = 0; i < m_inputs.size(); ++i)
    BindInput(i);

  _pBoundProgram = this;
  return true;
}

void atDXPrgm::BindInput(const int64_t &i)
{
  if (!m_inputs[i])
    return;

  atDXShader *pVertShader = (atDXShader*)m_pStages[atPS_Vertex];
  if (!pVertShader)
    return;

  UINT slot = (UINT)pVertShader->AttributeSlot(i);
  UINT offset = 0;
  UINT stride = (UINT)(m_inputs[i]->Size() / m_inputs[i]->Count());
  ID3D11Buffer *pBuffer = (ID3D11Buffer*)m_inputs[i]->GFXResource();
  atDirectX::GetContext()->IASetVertexBuffers(slot, 1, &pBuffer, &stride, &offset);
}

bool atDXPrgm::BindAttribute(const atString &name, atGFXBufferInterface *pBuffer)
{
  if (pBuffer->API() != API())
    return false;

  atDXShader *pVertShader = (atDXShader*)m_pStages[atPS_Vertex];
  if (!pVertShader)
    return false;

  for (int64_t i = 0; i < pVertShader->AttributeCount(); ++i)
    if (pVertShader->AttributeFullName(i) == name)
    {
      m_inputs[i] = (atDXBuffer*)pBuffer;
      if (IsBound())
        BindInput(i);
      return true;
    }

  return false;
}

bool atDXPrgm::BindIndices(atGFXBufferInterface *pBuffer)
{
  if (pBuffer->API() != API())
    return false;

  atGFXShaderInterface *pVertShader = m_pStages[atPS_Vertex];
  if (!pVertShader)
    return false;

  m_pIndices = (atDXBuffer*)pBuffer;
  if (IsBound())
    BindIndices();

  return true;
}

bool atDXPrgm::BindTexture(const atString &name, atGFXTexInterface *pTexture)
{
  for (atGFXShaderInterface *pShader : m_pStages)
    if (((atDXShader*)pShader)->BindTexture(name, pTexture))
      return true;
  return false;
}

bool atDXPrgm::BindSampler(const atString &name, atGFXSamplerInterface *pSampler)
{
  for (atGFXShaderInterface *pShader : m_pStages)
    if (((atDXShader*)pShader)->BindSampler(name, pSampler))
      return true;
  return false;
}

bool atDXPrgm::SetUniform(const atString &name, const void *pData, const atTypeDesc &info)
{
  for (atGFXShaderInterface *pShader : m_pStages)
    if (((atDXShader*)pShader)->SetUniform(name, pData, info))
      return true;
  return false;
}

bool atDXPrgm::HasUniform(const atString &name)
{
  for (atGFXShaderInterface *pShader : m_pStages)
    if (((atDXShader*)pShader)->HasUniform(name))
      return true;
  return false;
}

bool atDXPrgm::Upload()
{
  if (!(m_pStages[atPS_Vertex] && m_pStages[atPS_Fragment]))
    return false; // Must have at least a vertex and fragment shader

  if (m_pLayout)
    return true;

  bool success = true;
  for (atGFXShaderInterface *pStage : m_pStages)
    success &= !pStage || pStage->Upload();

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
  }
  m_inputs.clear();
  m_inputs.resize(d3dDesc.size(), 0);

  ID3D11InputLayout *pLayout = nullptr;
  if (FAILED(atDirectX::GetDevice()->CreateInputLayout(d3dDesc.data(), (UINT)d3dDesc.size(), pShader->ByteCode().data(), (size_t)pShader->ByteCode().size(), &pLayout)))
    return false;

  m_pLayout = pLayout;
  return true;
}

bool atDXPrgm::Delete()
{
  ID3D11InputLayout *pLayout = (ID3D11InputLayout*)m_pLayout;
  atDirectX::SafeRelease(pLayout);
  m_inputs.clear();
  return true;
}

bool atDXPrgm::Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType, const int64_t &elementCount, const int64_t &elementOffset)
{
  switch (primType)
  {
  case atGFX_PT_LineList: atDirectX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); break;
  case atGFX_PT_PointList: atDirectX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
  case atGFX_PT_LineListAdj: atDirectX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ); break;
  case atGFX_PT_TriangleList: atDirectX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
  case atGFX_PT_TriangleStrip: atDirectX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
  case atGFX_PT_TriangleListAdj: atDirectX::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ); break;
  }

  if (m_pIndices && indexedMode)
    atDirectX::DrawIndexed(elementCount < 0 ? m_pIndices->Count() : elementCount, elementOffset);
  else
    atDirectX::Draw(elementCount < 0 ? m_inputs[0]->Count() : elementCount, elementOffset);

  return true;
}

void atDXPrgm::BindLayout()
{
  if (!m_pLayout)
    return;

  atDirectX::GetContext()->IASetInputLayout((ID3D11InputLayout*)m_pLayout);
}

void atDXPrgm::BindIndices()
{
  if (!m_pIndices)
    return;

  atDirectX::GetContext()->IASetIndexBuffer((ID3D11Buffer*)m_pIndices->GFXResource(), m_pIndices->Desc().type == atType_Uint16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
}

bool atDXPrgm::IsBound() { return _pBoundProgram == this; }
