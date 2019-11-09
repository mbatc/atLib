#ifndef atNewRenderableCore_h__
#define atNewRenderableCore_h__

#include "atGFXShaderInterface.h"
#include "atGFXBufferInterface.h"
#include "atGFXPrgmInterface.h"
#include "atGFXTexInterface.h"
#include "atKeyValue.h"

class atNewRenderableCore
{
public:
  bool Draw(const bool &drawIndexed, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList);
  bool Upload();

  void SetAttribute(const atString &name, atGFXBufferInterface *pAttribute);
  void SetTexture(const atString &name, atGFXTexInterface *pTexture);
  void SetSampler(const atString &name, atGFXSamplerInterface *pSampler);
  void SetProgram(atGFXPrgmInterface *pProgram);

  template<typename T> void SetUniform(const atString &name, const T &value);
  template<typename T> void SetUniform(const atString &name, const atVector<T> &value);

protected:
  struct Uniform
  {
    atVector<uint8_t> data;
    atTypeDesc desc;
  };

  void SetUniform(const atString &name, Uniform &&value);

  atGFXPrgmInterface *m_pProgam = nullptr;
  atVector<atKeyValue<atString, Uniform>> m_uniforms;
  atVector<atKeyValue<atString, atGFXBufferInterface*>> m_attributes;
  atVector<atKeyValue<atString, atGFXTexInterface*>> m_textures;
  atVector<atKeyValue<atString, atGFXSamplerInterface*>> m_samplers;
};

template<typename T> void atNewRenderableCore::SetUniform(const atString &name, const T &value)
{
  atMemoryWriter writer;
  writer.Write(value);
  SetUniform(name, { writer.m_data, atGetTypeDesc(value) });
}

template<typename T> void atNewRenderableCore::SetUniform(const atString &name, const atVector<T> &value)
{
  atMemoryWriter writer;
  writer.Write(value.data(), value.size());
  SetUniform(name, { writer.m_data, atGetTypeDesc(value) });
}

#endif // atNewRenderableCore_h__
