#include "atRenderable.h"

template<typename T> void _SetNamedItem(atVector<atKeyValue<atString, T>> *pVec, const atString &name, const T &value)
{
  for (auto &kvp : *pVec)
    if (kvp.m_key == name)
    {
      atGraphics *pCtx = atGraphics::GetCurrent();
      if (kvp.m_val)
        pCtx->Release(kvp.m_val);
      kvp.m_val = value;
      return;
    }

  pVec->push_back({ name, value });
}

atRenderable::atRenderable(atRenderable &&o)
  : m_pPrgm(std::move(o.m_pPrgm))
  , m_samplers(std::move(o.m_samplers))
  , m_attributes(std::move(o.m_attributes))
  , m_textures(std::move(o.m_textures))
  , m_uniforms(std::move(o.m_uniforms))
{}

atRenderable::atRenderable(const atRenderable &o)
  : m_pPrgm(o.m_pPrgm)
  , m_samplers(o.m_samplers)
  , m_attributes(o.m_attributes)
  , m_textures(o.m_textures)
  , m_uniforms(o.m_uniforms)
{}

bool atRenderable::Draw(const bool &drawIndexed, const atGFX_PrimitiveType &primType, int64_t count, int64_t offset, int64_t baseVertIdx)
{
  if (!Upload())
    return false;

  if (!m_pPrgm->Bind())
    return false;

  for (auto &uniform : m_uniforms)
    m_pPrgm->SetUniform(uniform.m_key, uniform.m_val.data.data(), uniform.m_val.desc);

  for (auto &attribute : m_attributes)
    if (attribute.m_val->Type() == atBT_VertexData)
      m_pPrgm->BindAttribute(attribute.m_key, attribute.m_val);
    else
      m_pPrgm->BindIndices(attribute.m_val);

  for (auto &texture : m_textures)
    m_pPrgm->BindTexture(texture.m_key, texture.m_val);
  for (auto &sampler : m_samplers)
    m_pPrgm->BindSampler(sampler.m_key, sampler.m_val);

  return m_pPrgm->Draw(drawIndexed, primType, count, offset, baseVertIdx);
}

bool atRenderable::Upload()
{
  for (auto &attribute : m_attributes)
    attribute.m_val->Upload();
  for (auto &texture : m_textures)
    texture.m_val->Upload();
  for (auto &sampler : m_samplers)
    sampler.m_val->Upload();
  
  m_pPrgm->Upload();
  return true;
}

void atRenderable::SetUniform(const atString &name, Uniform &&value)
{
  for (auto &kvp : m_uniforms)
    if (kvp.m_key == name)
    {
      kvp.m_val = std::move(value);
      return;
    }

  m_uniforms.push_back({ name, std::move(value) });
}

bool atRenderable::GetUniform(const atString &name, atVector<uint8_t> *pData, atTypeDesc *pInfo)
{
  for (const atKeyValue<atString, Uniform> &kvp : m_uniforms)
    if (kvp.m_key == name)
    {
      if (pData) *pData = kvp.m_val.data;
      if (pInfo) *pInfo = kvp.m_val.desc;
      return true;
    }

  return false;
}

atTexture* atRenderable::GetTexture(const atString &name)
{
  for (const atKeyValue<atString, atTexture*> &kvp : m_textures)
    if (kvp.m_key == name)
      return kvp.m_val;
  return nullptr;
}

atSampler* atRenderable::GetSampler(const atString &name)
{
  for (const atKeyValue<atString, atSampler*> &kvp : m_samplers)
    if (kvp.m_key == name)
      return kvp.m_val;
  return nullptr;
}

atGPUBuffer* atRenderable::GetAttribute(const atString &name)
{
  for (const atKeyValue<atString, atGPUBuffer*> &kvp : m_attributes)
    if (kvp.m_key == name)
      return kvp.m_val;
  return nullptr;
}

atProgram* atRenderable::GetProgram() { return m_pPrgm; }

void atRenderable::SetProgram(atProgram* pProgram)
{
  atGraphics *pCtx = atGraphics::GetCurrent();
  pProgram->AddReference();
  if (m_pPrgm)
    pCtx->Release(m_pPrgm);
  m_pPrgm = pProgram;
}

void atRenderable::SetTexture(const atString &name, atTexture* pTexture)
{
  if (!pTexture)
    return;

  pTexture->AddReference();
  _SetNamedItem(&m_textures, name, pTexture);
}

void atRenderable::SetSampler(const atString &name, atSampler* pSampler)
{
  if (!pSampler)
    return;

  pSampler->AddReference();
  _SetNamedItem(&m_samplers, name, pSampler);
}

void atRenderable::SetAttribute(const atString &name, atGPUBuffer* pAttribute)
{
  if (!pAttribute)
    return;

  pAttribute->AddReference();
  _SetNamedItem(&m_attributes, name, pAttribute);
}
