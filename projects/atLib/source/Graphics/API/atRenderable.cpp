#include "atRenderable.h"
#include "atDXTexture.h"
#include "atDXPrgm.h"
#include "atDXShader.h"
#include "atFile.h"
#include "atAssetDirs.h"

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

atRenderable::atRenderable(atRenderable &&o) { *this = std::move(o); }

atRenderable::atRenderable(const atRenderable &o) { *this = o; }

atRenderable& atRenderable::operator=(atRenderable &&o)
{
  m_pPrgm = std::move(o.m_pPrgm);
  m_samplers = std::move(o.m_samplers);
  m_attributes = std::move(o.m_attributes);
  m_textures = std::move(o.m_textures);
  m_uniforms = std::move(o.m_uniforms);
  return *this;
}

atRenderable& atRenderable::operator=(const atRenderable &o)
{
  m_pPrgm = o.m_pPrgm;
  m_samplers = o.m_samplers;
  m_attributes = o.m_attributes;
  m_textures = o.m_textures;
  m_uniforms = o.m_uniforms;
  return *this;
}

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

void atRenderable::SetMaterial(const atMaterial &material)
{
  for (int64_t i = 0; i < material.LayerCount(); ++i)
  {
    for (const atString &name : material.Textures())
    {
      atMaterialProperty prop = atMaterial::GetProperty(name);
      if (!material.HasTexture(prop, i) || prop == atMP_Unknown)
        continue;

      std::shared_ptr<atDXTexture> tex = std::make_shared<atDXTexture>(atImage(atFilename(material.GetTexture(prop, i))));

      if (atMaterial::textureMapping[prop].length() > 0)
        SetTexture(atMaterial::textureMapping[prop] + atString(i), tex);
    }

    for (const atString &name : material.Colours())
    {
      atMaterialProperty prop = atMaterial::GetProperty(name);
      if (!material.HasColour(name, i) || prop == atMP_Unknown)
        continue;

      if (atMaterial::colourMapping[prop].length() > 0)
        SetUniform(atMaterial::colourMapping[prop] + atString(i), atVec4F(material.GetColour(prop, i)));
    }

    for (const atString &name : material.Values())
    {
      atMaterialProperty prop = atMaterial::GetProperty(name);
      if (!material.HasValue(name, i) || prop == atMP_Unknown)
        continue;

      if (atMaterial::valueMapping[prop].length() > 0)
        SetUniform(atMaterial::valueMapping[prop] + atString(i), float(material.GetValue(prop, i)));
    }

    std::shared_ptr<atDXPrgm> prgm = std::make_shared<atDXPrgm>();
    prgm->SetStage(std::make_shared<atDXShader>(atFile::ReadText(atAssetDirs::Shaders() + "/" + material.GetShader(atPS_Vertex)), atPS_Vertex));
    prgm->SetStage(std::make_shared<atDXShader>(atFile::ReadText(atAssetDirs::Shaders() + "/" + material.GetShader(atPS_Fragment)), atPS_Fragment));
    SetProgram(prgm);
  }
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
