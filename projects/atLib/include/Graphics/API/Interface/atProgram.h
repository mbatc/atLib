#ifndef atProgram_h__
#define atProgram_h__

#include <memory>
#include "atGFXResource.h"
#include "atShader.h"
#include "atGPUBuffer.h"

class atProgram : public atGFXResource
{
public:
  atProgram();

  // Force all shader programs to be reloaded
  static void ReloadAll();

  void Reload();
  bool SetStage(atShader *pShader);
  bool SetStage(const atString &source, const atPipelineStage &stage);
  bool SetStageFile(const atString &path, const atPipelineStage &stage);

  virtual bool BindIndices(atGPUBuffer *pBuffer) = 0;
  virtual bool BindAttribute(const atString &name, atGPUBuffer *pBuffer) = 0;
  virtual bool BindTexture(const atString &name, atTexture *pTexture) = 0;
  virtual bool BindSampler(const atString &name, atSampler *pSampler) = 0;
  virtual bool SetUniform(const atString &name, const void *pData, const atTypeDesc &info) = 0;
  virtual bool HasUniform(const atString &name) const = 0;

  virtual int64_t FindUniform(const atString &index) const = 0;
  virtual int64_t FindAttribute(const atString &index) const = 0;
  virtual int64_t FindTexture(const atString &index) const = 0;
  virtual int64_t FindSampler(const atString &index) const = 0;

  virtual bool BindAttribute(const int64_t &index, atGPUBuffer *pBuffer) = 0;
  virtual bool BindTexture(const int64_t &index, atTexture *pTexture) = 0;
  virtual bool BindSampler(const int64_t &index, atSampler *pSampler) = 0;
  virtual bool SetUniform(const int64_t &index, const void *pData, const atTypeDesc &info) = 0;

  virtual bool Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const int64_t &elementCount = -1, const int64_t &elementOffset = 0, const int64_t &baseVtxIdx = 0) = 0;

  template<typename T> bool SetUniform(const atString &name, const T &value);
  template<typename T> bool SetUniform(const atString &name, const atVector<T> &value);

protected:
  bool ShouldReload() const;

  bool BindStages()
  {
    bool success = true;
    for (int64_t i = 0; i < atPS_Count; ++i)
      success &= !m_pStages[i] || m_pStages[i]->Bind();
    return success;
  }

  atShader* m_pStages[atPS_Count] = { 0 };

  int64_t m_shaderRound = -1;

  static int64_t m_globalShaderRound;

  int64_t m_shaderRound = -1;

  static int64_t m_globalShaderRound;
};

template<typename T> inline bool atProgram::SetUniform(const atString &name, const T &value) { return SetUniform(name, (const void*)&value, atGetTypeDesc(value)); }
template<typename T> inline bool atProgram::SetUniform(const atString &name, const atVector<T> &value) { return SetUniform(name, (const void*)value.data(), atGetTypeDesc(value)); }

#endif // atGFXPrgmInterface_h__
