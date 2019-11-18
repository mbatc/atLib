#include "atRenderable.h"

template<typename T> void _SetNamedItem(atVector<atKeyValue<atString, T>> *pVec, const atString &name, const T &value)
{
  for (auto &kvp : *pVec)
    if (kvp.m_key == name)
    {
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

bool atRenderable::Draw(const bool &drawIndexed, const atGFX_PrimitiveType &primType)
{
  if (!Upload())
    return false;

  if (!m_pPrgm->Bind())
    return false;

  for (auto &uniform : m_uniforms)
    m_pPrgm->SetUniform(uniform.m_key, uniform.m_val.data.data(), uniform.m_val.desc);
  for (auto &attribute : m_attributes)
    m_pPrgm->BindAttribute(attribute.m_key, attribute.m_val.get());
  for (auto &texture : m_textures)
    m_pPrgm->BindTexture(texture.m_key, texture.m_val.get());
  for (auto &sampler : m_samplers)
    m_pPrgm->BindSampler(sampler.m_key, sampler.m_val.get());

  return m_pPrgm->Draw(drawIndexed, primType);
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

std::shared_ptr<atGFXTexInterface> atRenderable::GetTexture(const atString &name)
{
  for (const atKeyValue<atString, std::shared_ptr<atGFXTexInterface>> &kvp : m_textures)
    if (kvp.m_key == name)
      return kvp.m_val;
  return nullptr;
}

std::shared_ptr<atGFXSamplerInterface> atRenderable::GetSampler(const atString &name)
{
  for (const atKeyValue<atString, std::shared_ptr<atGFXSamplerInterface>> &kvp : m_samplers)
    if (kvp.m_key == name)
      return kvp.m_val;
  return nullptr;
}

std::shared_ptr<atGFXBufferInterface> atRenderable::GetAttribute(const atString &name)
{
  for (const atKeyValue<atString, std::shared_ptr<atGFXBufferInterface>> &kvp : m_attributes)
    if (kvp.m_key == name)
      return kvp.m_val;
  return nullptr;
}

std::shared_ptr<atGFXPrgmInterface> atRenderable::GetProgram() { return m_pPrgm; }
void atRenderable::SetProgram(const std::shared_ptr<atGFXPrgmInterface> &pProgram) { m_pPrgm = pProgram; }
void atRenderable::SetTexture(const atString &name, const std::shared_ptr<atGFXTexInterface> &pTexture) { _SetNamedItem(&m_textures, name, pTexture); }
void atRenderable::SetSampler(const atString &name, const std::shared_ptr<atGFXSamplerInterface> &pSampler) { _SetNamedItem(&m_samplers, name, pSampler); }
void atRenderable::SetAttribute(const atString &name, const std::shared_ptr<atGFXBufferInterface> &pAttribute) { _SetNamedItem(&m_attributes, name, pAttribute); }
