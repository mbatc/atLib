#ifdef atPLATFORM_WIN32

#include "atGraphics.h"
#include "atDXShader.h"
#include "atDirectX.h"
#include "atHashSet.h"
#include "atDXInclude_Internal.h"

static const char *_shaderStrID[atPS_Count] =
{
  "vs_5_0", // atPS_Vertex
  "ps_5_0", // atPS_Fragment
  "gs_5_0", // atPS_Geometry
  "cs_5_0", // atPS_Compute
  "ds_5_0", // atPS_Domain
  "hs_5_0"  // atPS_Hull
};

static bool _GetTypeDesc(const D3D11_SIGNATURE_PARAMETER_DESC &shderDesc, atTypeDesc *pDesc);
static bool _GetTypeDesc(const D3D11_SHADER_TYPE_DESC &shderDesc, atTypeDesc *pDesc);
static void _BindConstantBuffer(const atPipelineStage &stage, atDXBuffer *pBuffer, int64_t slot);
static void _BindSampler(const atPipelineStage &stage, atDXSampler *pBuffer, int64_t slot);
static void _BindTexture(const atPipelineStage &stage, atDXTexture *pBuffer, int64_t slot);

bool atDXShader::BindTexture(const atString &name, atGFXTexInterface *pTexture)
{
  if (pTexture->API() != API())
    return false;

  for (TextureDesc &desc : m_textures)
    if (desc.name == name)
    {
      desc.pTex = (atDXTexture*)pTexture;
      _BindTexture(Stage(), desc.pTex, desc.slot);
      return true;
    }

  return false;
}

bool atDXShader::BindSampler(const atString &name, atGFXSamplerInterface *pSampler)
{
  if (pSampler->API() != API())
    return false;

  for (SamplerDesc &desc : m_samplers)
    if (desc.name == name)
    {
      desc.pSampler = (atDXSampler*)pSampler;
      _BindSampler(Stage(), desc.pSampler, desc.slot);
      return true;
    }

  return false;
}

bool atDXShader::SetUniform(const atString &name, const void *pData, const atTypeDesc &info)
{
  VarDesc *pVar = nullptr;
  ConstBufferDesc *pBuffer = FindVarBuffer(name, &pVar);
  if (!pBuffer)
    return false;
  if (atTypeCast(pBuffer->data.data() + pVar->offset, pData, pVar->desc, info))
  {
    pBuffer->updated = true;
    return true;
  }

  return false;
}

atVector<atString> atDXShader::Textures() const
{
  atVector<atString> names;
  for (const TextureDesc &desc : m_textures)
    names.push_back(desc.name);
  return names;
}

atVector<atString> atDXShader::Samplers() const
{
  atVector<atString> names;
  for (const SamplerDesc &desc : m_samplers)
    names.push_back(desc.name);
  return names;
}

atVector<atString> atDXShader::Uniforms() const
{
  atVector<atString> names;
  for (const ConstBufferDesc &desc : m_buffers)
    for (const VarDesc &var : desc.vars)
      names.push_back(var.name);
  return names;
}

const atVector<uint8_t>& atDXShader::ByteCode() const { return m_byteCode; }

bool atDXShader::Delete()
{
  ID3D11DeviceChild *pResource = (ID3D11DeviceChild*)m_pResource;
  atDirectX::SafeRelease(pResource);
  m_pResource = nullptr;
  return true;
}

bool atDXShader::Bind()
{
  if (!Upload())
    return false;

  atDirectX *pDX = (atDirectX*)atGraphics::GetCtx();
  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();

  switch (Stage())
  {
  case atPS_Vertex: pCtx->VSSetShader((ID3D11VertexShader*)m_pResource, 0, 0); break;
  case atPS_Fragment: pCtx->PSSetShader((ID3D11PixelShader*)m_pResource, 0, 0); break;
  case atPS_Geometry: pCtx->GSSetShader((ID3D11GeometryShader*)m_pResource, 0, 0); break;
  case atPS_Compute: pCtx->CSSetShader((ID3D11ComputeShader*)m_pResource, 0, 0); break;
  case atPS_Domain: pCtx->DSSetShader((ID3D11DomainShader*)m_pResource, 0, 0); break;
  case atPS_Hull: pCtx->HSSetShader((ID3D11HullShader*)m_pResource, 0, 0); break;
  default: return false;
  }

  for (ConstBufferDesc &desc : m_buffers) _BindConstantBuffer(Stage(), desc.pBuffer, desc.slot);
  for (TextureDesc &desc : m_textures) _BindTexture(Stage(), desc.pTex, desc.slot);
  for (SamplerDesc &desc : m_samplers) _BindSampler(Stage(), desc.pSampler, desc.slot);
  return true;
}

void atDXShader::UpdateConstantBuffers()
{
  if (m_pResource)
  { // Try update the constant buffers
    for (ConstBufferDesc &cb : m_buffers)
      if (cb.updated)
      {
        cb.pBuffer->Set(cb.data);
        cb.updated = false;
      }
  }
}

bool atDXShader::Upload()
{
  atDirectX *pDX = (atDirectX*)atGraphics::GetCtx();
  if (m_pResource)
  {
    UpdateConstantBuffers();
    return true;
  }

  if (m_src.length() == 0 || !pDX->GetDevice())
    return false;

  ID3D10Blob *pShaderBlob = nullptr;
  ID3D10Blob *pError = nullptr;
  if (FAILED(D3DCompile(m_src.c_str(), m_src.length(), NULL, NULL, NULL, "main", _shaderStrID[Stage()], D3D10_SHADER_ENABLE_STRICTNESS, 0, &pShaderBlob, &pError)))
  {
    atFail((char*)pError->GetBufferPointer());
    return false;
  }

  ID3D11Device *pDev = (ID3D11Device*)pDX->GetDevice();
  switch (Stage())
  {
  case atPS_Vertex:   pDev->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11VertexShader**)&m_pResource); break;
  case atPS_Fragment: pDev->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11PixelShader**)&m_pResource); break;
  case atPS_Geometry: pDev->CreateGeometryShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11GeometryShader**)&m_pResource); break;
  case atPS_Hull:     pDev->CreateHullShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11HullShader**)&m_pResource); break;
  case atPS_Domain:   pDev->CreateDomainShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11DomainShader**)&m_pResource); break;
  case atPS_Compute:  pDev->CreateComputeShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), NULL, (ID3D11ComputeShader**)&m_pResource); break;
  }

  Reflect(pShaderBlob);
  m_byteCode.resize(pShaderBlob->GetBufferSize());
  memcpy(m_byteCode.data(), pShaderBlob->GetBufferPointer(), (size_t)pShaderBlob->GetBufferSize());
  atDirectX::SafeRelease(pShaderBlob);
  return m_pResource != nullptr;
}

bool atDXShader::Reflect(void *pBlob)
{
  ID3D10Blob *pShaderBlob = (ID3D10Blob*)pBlob;
  ID3D11ShaderReflection *pReflection = nullptr;
  if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflection)))
    return false;

  m_attributes.clear();
  m_buffers.clear();
  m_textures.clear();
  m_samplers.clear();

  D3D11_SHADER_DESC desc;
  pReflection->GetDesc(&desc);
  for (int64_t cbIdx = 0; cbIdx < desc.ConstantBuffers; ++cbIdx)
  {
    D3D11_SHADER_BUFFER_DESC bufDesc;
    ID3D11ShaderReflectionConstantBuffer *pBuffer = pReflection->GetConstantBufferByIndex((UINT)cbIdx);
    pBuffer->GetDesc(&bufDesc);

    ConstBufferDesc newBuffer;
    newBuffer.name = bufDesc.Name;
    newBuffer.vars.reserve(bufDesc.Variables);

    for (int64_t varIdx = 0; varIdx < bufDesc.Variables; ++varIdx)
    {
      D3D11_SHADER_VARIABLE_DESC varDesc;
      ID3D11ShaderReflectionVariable *pVariable = pBuffer->GetVariableByIndex((UINT)varIdx);
      pVariable->GetDesc(&varDesc);

      D3D11_SHADER_TYPE_DESC typeDesc;
      ID3D11ShaderReflectionType *pType = pVariable->GetType();
      pType->GetDesc(&typeDesc);

      VarDesc var;
      var.name = varDesc.Name;
      var.offset = varDesc.StartOffset;
      if (!_GetTypeDesc(typeDesc, &var.desc))
        continue; // This is a variable
      newBuffer.vars.emplace_back(std::move(var));
    }

    newBuffer.data.resize(bufDesc.Size);
    newBuffer.updated = true;
    newBuffer.pBuffer = atNew<atDXBuffer>(newBuffer.data, atBT_ShaderData);
    m_buffers.push_back(std::move(newBuffer));
  }

  for (int64_t inputIdx = 0; inputIdx < desc.InputParameters; ++inputIdx)
  {
    D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
    pReflection->GetInputParameterDesc((UINT)inputIdx, &inputDesc);
    AttributeDesc attrDesc;
    if (!_GetTypeDesc(inputDesc, &attrDesc.desc))
      continue;
    attrDesc.name = inputDesc.SemanticName;
    attrDesc.fullName = inputDesc.SemanticName;
    attrDesc.slot = inputDesc.Register;
    attrDesc.idx = inputDesc.SemanticIndex;
    m_attributes.emplace_back(std::move(attrDesc));
  }

  atHashSet<atString> attrNames;
  for (int64_t i = m_attributes.size() - 1; i >= 0; --i)
  {
    if (m_attributes[i].idx > 0)
      attrNames.TryAdd(m_attributes[i].name);
    if (attrNames.Contains(m_attributes[i].name))
      m_attributes[i].fullName = m_attributes[i].name + atString(m_attributes[i].idx);
  }

  for (int64_t resIdx = 0; resIdx < desc.BoundResources; ++resIdx)
  {
    D3D11_SHADER_INPUT_BIND_DESC resDesc;
    pReflection->GetResourceBindingDesc((UINT)resIdx, &resDesc);

    switch (resDesc.Type)
    {
    case D3D10_SIT_CBUFFER: SetBufferSlot(resDesc.Name, resDesc.BindPoint); break;
    case D3D10_SIT_TEXTURE: SetTextureSlot(resDesc.Name, resDesc.BindPoint); break;
    case D3D10_SIT_SAMPLER: SetSamplerSlot(resDesc.Name, resDesc.BindPoint); break;
    }
  }

  atDirectX::SafeRelease(pReflection);
  return true;
}

void atDXShader::SetBufferSlot(const atString &name, const int64_t &slot)
{
  for (ConstBufferDesc &cb : m_buffers)
    if (cb.name == name)
    {
      cb.slot = slot;
      return;
    }
}

void atDXShader::SetTextureSlot(const atString &name, const int64_t &slot)
{
  for (TextureDesc &td : m_textures)
    if (td.name == name)
    {
      td.slot = slot;
      return;
    }

  TextureDesc newTex;
  newTex.name = name;
  newTex.slot = slot;
  newTex.pTex = nullptr;
  m_textures.push_back(newTex);
}

void atDXShader::SetSamplerSlot(const atString &name, const int64_t &slot)
{
  for (SamplerDesc &sd : m_samplers)
    if (sd.name == name)
    {
      sd.slot = slot;
      return;
    }

  SamplerDesc newSampler;
  newSampler.name = name;
  newSampler.slot = slot;
  newSampler.pSampler = nullptr;
  m_samplers.push_back(newSampler);
}

atDXShader::ConstBufferDesc* atDXShader::FindVarBuffer(const atString &name, VarDesc **pVar)
{
  for (ConstBufferDesc &desc : m_buffers)
    for (VarDesc &var : desc.vars)
      if (var.name == name)
      {
        if (pVar)
          *pVar = &var;
        return &desc;
      }

  return nullptr;
}

bool atDXShader::HasUniform(const atString &name) { return FindVarBuffer(name) != nullptr; }
int64_t atDXShader::AttributeCount() const { return m_attributes.size(); }
const atString& atDXShader::AttributeName(const int64_t &index) const { return m_attributes[index].name; }
const atString& atDXShader::AttributeFullName(const int64_t &index) const { return m_attributes[index].fullName; }
const atTypeDesc& atDXShader::AttributeInfo(const int64_t &index) const { return m_attributes[index].desc; }
const int64_t& atDXShader::AttributeIndex(const int64_t &index) const { return m_attributes[index].idx; }
const int64_t& atDXShader::AttributeSlot(const int64_t &index) const { return m_attributes[index].slot; }

static bool _GetTypeDesc(const D3D11_SHADER_TYPE_DESC &shderDesc, atTypeDesc *pDesc)
{
  switch (shderDesc.Type)
  {
  case D3D_SVT_BOOL: *pDesc = atGetTypeDesc<bool>(); break;
  case D3D_SVT_INT: *pDesc = atGetTypeDesc<int32_t>(); break;
  case D3D_SVT_FLOAT: *pDesc = atGetTypeDesc<float>(); break;
  case D3D_SVT_UINT: *pDesc = atGetTypeDesc<uint32_t>(); break;
  case D3D_SVT_UINT8: *pDesc = atGetTypeDesc<uint8_t>(); break;
  case D3D11_SVT_DOUBLE: *pDesc = atGetTypeDesc<double>(); break;
  default: return false;
  }

  pDesc->count = (shderDesc.Rows * shderDesc.Columns) * atMax(shderDesc.Elements, 1);
  return true;
}

static bool _GetTypeDesc(const D3D11_SIGNATURE_PARAMETER_DESC &shderDesc, atTypeDesc *pDesc)
{
  switch (shderDesc.ComponentType)
  {
  case D3D_REGISTER_COMPONENT_UINT32: *pDesc = atGetTypeDesc<uint32_t>(); break;
  case D3D_REGISTER_COMPONENT_SINT32: *pDesc = atGetTypeDesc<int32_t>(); break;
  case D3D_REGISTER_COMPONENT_FLOAT32: *pDesc = atGetTypeDesc<float>(); break;
  case D3D_REGISTER_COMPONENT_UNKNOWN: *pDesc = atGetTypeDesc<void>(); break;
  default: return false;
  }

  pDesc->width = atNumBitsOn(shderDesc.Mask);
  return true;
}

static void _BindConstantBuffer(const atPipelineStage &stage, atDXBuffer *pBuffer, int64_t slot)
{
  bool validBuffer = pBuffer && pBuffer->Upload();
  atDirectX *pDX = (atDirectX*)atGraphics::GetCtx();

  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
  ID3D11Buffer *pDxBuf = validBuffer ? (ID3D11Buffer*)pBuffer->GFXResource() : nullptr;
  switch (stage)
  {
  case atPS_Vertex: pCtx->VSSetConstantBuffers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Fragment: pCtx->PSSetConstantBuffers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Domain: pCtx->DSSetConstantBuffers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Hull: pCtx->HSSetConstantBuffers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Compute: pCtx->CSSetConstantBuffers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Geometry: pCtx->GSSetConstantBuffers((UINT)slot, 1, &pDxBuf); break;
  }
}

static void _BindSampler(const atPipelineStage &stage, atDXSampler *pBuffer, int64_t slot)
{
  bool validBuffer = pBuffer && pBuffer->Upload();
  atDirectX *pDX = (atDirectX*)atGraphics::GetCtx();

  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
  ID3D11SamplerState *pDxBuf = validBuffer ? (ID3D11SamplerState*)pBuffer->GFXResource() : nullptr;
  switch (stage)
  {
  case atPS_Vertex: pCtx->VSSetSamplers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Fragment: pCtx->PSSetSamplers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Domain: pCtx->DSSetSamplers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Hull: pCtx->HSSetSamplers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Compute: pCtx->CSSetSamplers((UINT)slot, 1, &pDxBuf); break;
  case atPS_Geometry: pCtx->GSSetSamplers((UINT)slot, 1, &pDxBuf); break;
  }
}

static void _BindTexture(const atPipelineStage &stage, atDXTexture *pBuffer, int64_t slot)
{
  bool validBuffer = pBuffer && pBuffer->Upload();
  atDirectX *pDX = (atDirectX*)atGraphics::GetCtx();

  ID3D11DeviceContext *pCtx = (ID3D11DeviceContext*)pDX->GetContext();
  ID3D11ShaderResourceView *pDxBuf = validBuffer ? (ID3D11ShaderResourceView*)pBuffer->ShaderView() : nullptr;
  switch (stage)
  {
  case atPS_Vertex: pCtx->VSSetShaderResources((UINT)slot, 1, &pDxBuf); break;
  case atPS_Fragment: pCtx->PSSetShaderResources((UINT)slot, 1, &pDxBuf); break;
  case atPS_Domain: pCtx->DSSetShaderResources((UINT)slot, 1, &pDxBuf); break;
  case atPS_Hull: pCtx->HSSetShaderResources((UINT)slot, 1, &pDxBuf); break;
  case atPS_Compute: pCtx->CSSetShaderResources((UINT)slot, 1, &pDxBuf); break;
  case atPS_Geometry: pCtx->GSSetShaderResources((UINT)slot, 1, &pDxBuf); break;
  }
}

#else

// Used for functions that return const& of 
static atString _unused_str;
static int64_t _unused_i64 = 0;
static atTypeDesc _unused_typeDesc;
static atVector<uint8_t> _unused_vec_ui8;

bool atDXShader::BindTexture(const atString &, atGFXTexInterface *) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXShader::BindSampler(const atString &, atGFXSamplerInterface *) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXShader::SetUniform(const atString &, const void *, const atTypeDesc &) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }

atVector<atString> atDXShader::Textures() const { atRelAssert("DirectX is only supported on Windows platforms."); return{}; }
atVector<atString> atDXShader::Samplers() const { atRelAssert("DirectX is only supported on Windows platforms."); return{}; }
atVector<atString> atDXShader::Uniforms() const { atRelAssert("DirectX is only supported on Windows platforms."); return{}; }
const atVector<uint8_t>& atDXShader::ByteCode() const { atRelAssert("DirectX is only supported on Windows platforms."); return _unused_vec_ui8; }

bool atDXShader::Delete() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXShader::Bind() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }

void atDXShader::UpdateConstantBuffers() { atRelAssert("DirectX is only supported on Windows platforms."); }

bool atDXShader::Upload() { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
bool atDXShader::Reflect(void *) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }

void atDXShader::SetBufferSlot(const atString &, const int64_t &) { atRelAssert("DirectX is only supported on Windows platforms."); }
void atDXShader::SetTextureSlot(const atString &, const int64_t &) { atRelAssert("DirectX is only supported on Windows platforms."); }
void atDXShader::SetSamplerSlot(const atString &, const int64_t &) { atRelAssert("DirectX is only supported on Windows platforms."); }

atDXShader::ConstBufferDesc* atDXShader::FindVarBuffer(const atString &, VarDesc **) { atRelAssert("DirectX is only supported on Windows platforms."); return nullptr; }

bool atDXShader::HasUniform(const atString &) { atRelAssert("DirectX is only supported on Windows platforms."); return false; }
int64_t atDXShader::AttributeCount() const { atRelAssert("DirectX is only supported on Windows platforms."); return 0; }
const atString& atDXShader::AttributeName(const int64_t &) const { atRelAssert("DirectX is only supported on Windows platforms."); return _unused_str; }
const atString& atDXShader::AttributeFullName(const int64_t &) const { atRelAssert("DirectX is only supported on Windows platforms."); return _unused_str; }
const atTypeDesc& atDXShader::AttributeInfo(const int64_t &) const { atRelAssert("DirectX is only supported on Windows platforms."); return _unused_typeDesc; }
const int64_t& atDXShader::AttributeIndex(const int64_t &) const { atRelAssert("DirectX is only supported on Windows platforms."); return _unused_i64; }
const int64_t& atDXShader::AttributeSlot(const int64_t &) const { atRelAssert("DirectX is only supported on Windows platforms."); return _unused_i64; }

#endif
