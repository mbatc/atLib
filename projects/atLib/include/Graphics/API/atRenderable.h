#ifndef atRenderable_h__
#define atRenderable_h__

#include "atGFXShaderInterface.h"
#include "atGFXBufferInterface.h"
#include "atGFXPrgmInterface.h"
#include "atGFXTexInterface.h"
#include "atKeyValue.h"
#include "atPtr.h"
#include <memory>

class atRenderable
{
public:
  atRenderable() = default;
  atRenderable(atRenderable &&o);
  atRenderable(const atRenderable &o);

  bool Draw(const bool &drawIndexed, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList);
  bool Upload();

  void SetProgram(const std::shared_ptr<atGFXPrgmInterface> &pProgram);
  void SetTexture(const atString &name, const std::shared_ptr<atGFXTexInterface> &pTexture);
  void SetSampler(const atString &name, const std::shared_ptr<atGFXSamplerInterface> &pSampler);
  void SetAttribute(const atString &name, const std::shared_ptr<atGFXBufferInterface> &pAttribute);

  bool GetUniform(const atString &name, atVector<uint8_t> *pData = nullptr, atTypeDesc *pInfo = nullptr);

  std::shared_ptr<atGFXPrgmInterface> GetProgram();
  std::shared_ptr<atGFXTexInterface> GetTexture(const atString &name);
  std::shared_ptr<atGFXBufferInterface> GetAttribute(const atString &name);
  std::shared_ptr<atGFXSamplerInterface> GetSampler(const atString &name);

  template<typename T> void SetUniform(const atString &name, const T &value);
  template<typename T> void SetUniform(const atString &name, const atVector<T> &value);

protected:
  struct Uniform
  {
    atVector<uint8_t> data;
    atTypeDesc desc;
  };

  void SetUniform(const atString &name, Uniform &&value);

  std::shared_ptr<atGFXPrgmInterface> m_pPrgm;
  atVector<atKeyValue<atString, Uniform>> m_uniforms;
  atVector<atKeyValue<atString, std::shared_ptr<atGFXTexInterface>>> m_textures;
  atVector<atKeyValue<atString, std::shared_ptr<atGFXBufferInterface>>> m_attributes;
  atVector<atKeyValue<atString, std::shared_ptr<atGFXSamplerInterface>>> m_samplers;
};

template<typename T> void atRenderable::SetUniform(const atString &name, const T &value)
{
  atMemoryWriter writer;
  writer.Write(value);
  SetUniform(name, { writer.m_data, atGetTypeDesc(value) });
}

template<typename T> void atRenderable::SetUniform(const atString &name, const atVector<T> &value)
{
  atMemoryWriter writer;
  writer.Write(value.data(), value.size());
  SetUniform(name, { writer.m_data, atGetTypeDesc(value) });
}

#endif // atRenderable_h__
