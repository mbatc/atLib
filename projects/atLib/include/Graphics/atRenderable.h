#ifndef atRenderable_h__
#define atRenderable_h__

#include "atShader.h"
#include "atGPUBuffer.h"
#include "atProgram.h"
#include "atTexture.h"
#include "atKeyValue.h"
#include <memory>

class atRenderable
{
public:
  atRenderable() = default;
  atRenderable(atRenderable &&o);
  atRenderable(const atRenderable &o);

  bool Draw(const bool &drawIndexed, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, int64_t count = -1, int64_t offset = 0, int64_t baseVertIdx = 0);
  bool Upload();

  void SetProgram(atProgram *pProgram);
  void SetTexture(const atString &name, atTexture *pTexture);
  void SetSampler(const atString &name, atSampler *pSampler);
  void SetAttribute(const atString &name, atGPUBuffer *pAttribute);

  void ClearAttribute(const atString &name);
  void ClearTexture(const atString &name);
  void ClearSampler(const atString &name);
  void ClearUniform(const atString &name);

  bool GetUniform(const atString &name, atVector<uint8_t> *pData = nullptr, atTypeDesc *pInfo = nullptr);

  atProgram* GetProgram();
  atTexture* GetTexture(const atString &name);
  atGPUBuffer* GetAttribute(const atString &name);
  atSampler* GetSampler(const atString &name);

  template<typename T> void SetUniform(const atString &name, const T &value);
  template<typename T> void SetUniform(const atString &name, const atVector<T> &value);

  void Clear();

protected:
  struct Uniform
  {
    atVector<uint8_t> data;
    atTypeDesc desc;
  };

  void SetUniform(const atString &name, Uniform &&value);

  atProgram* m_pPrgm = nullptr;
  atVector<atKeyValue<atString, Uniform>> m_uniforms;
  atVector<atKeyValue<atString, atTexture*>> m_textures;
  atVector<atKeyValue<atString, atGPUBuffer*>> m_attributes;
  atVector<atKeyValue<atString, atSampler*>> m_samplers;
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
