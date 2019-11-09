#include "atNewRenderableCore.h"

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

bool atNewRenderableCore::Draw(const bool &drawIndexed, const atGFX_PrimitiveType &primType)
{
  if (!Upload())
    return false;

  if (!m_pProgam->Bind())
    return false;
  for (auto &uniform : m_uniforms)      m_pProgam->SetUniform(uniform.m_key, uniform.m_val.data.data(), uniform.m_val.desc);
  m_pProgam->Upload(); // upload updated uniforms

  for (auto &attribute : m_attributes)  m_pProgam->BindAttribute(attribute.m_key, attribute.m_val);
  for (auto &texture : m_textures)      m_pProgam->BindTexture(texture.m_key, texture.m_val);
  for (auto &sampler : m_samplers)      m_pProgam->BindSampler(sampler.m_key, sampler.m_val);

  return m_pProgam->Draw(drawIndexed, primType);
}

bool atNewRenderableCore::Upload()
{
  for (auto &attribute : m_attributes)  attribute.m_val->Upload();
  for (auto &texture : m_textures)      texture.m_val->Upload();
  for (auto &sampler : m_samplers)      sampler.m_val->Upload();
  return true;
}

void atNewRenderableCore::SetUniform(const atString &name, Uniform &&value)
{
  for (auto &kvp : m_uniforms)
    if (kvp.m_key == name)
    {
      kvp.m_val = std::move(value);
      return;
    }
  m_uniforms.push_back({ name, std::move(value) });
}

void atNewRenderableCore::SetAttribute(const atString &name, atGFXBufferInterface *pAttribute) { _SetNamedItem(&m_attributes, name, pAttribute); }
void atNewRenderableCore::SetTexture(const atString &name, atGFXTexInterface *pTexture) { _SetNamedItem(&m_textures, name, pTexture); }
void atNewRenderableCore::SetSampler(const atString &name, atGFXSamplerInterface *pSampler) { _SetNamedItem(&m_samplers, name, pSampler); }
void atNewRenderableCore::SetProgram(atGFXPrgmInterface *pProgram) { m_pProgam = pProgram; }
