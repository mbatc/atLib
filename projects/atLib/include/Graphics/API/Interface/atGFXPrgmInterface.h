#ifndef atGFXPrgmInterface_h__
#define atGFXPrgmInterface_h__

#include "atGFXResource.h"
#include "atGFXShaderInterface.h"
#include "atGFXBufferInterface.h"

enum atGFX_PrimitiveType
{
  atGFX_PT_PointList,
  atGFX_PT_LineList,
  atGFX_PT_TriangleList,
  atGFX_PT_TriangleStrip,
  atGFX_PT_PointListAdj,
  atGFX_PT_LineListAdj,
  atGFX_PT_TriangleListAdj,
};

class atGFXPrgmInterface : public atGFXResource
{
public:
  atGFXPrgmInterface();

  void Reload();
  bool SetStage(atGFXShaderInterface *pShader);

  virtual bool BindAttribute(const atString &name, atGFXBufferInterface *pBuffer) { return false; }
  virtual bool BindIndices(atGFXBufferInterface *pBuffer) { return false; }
  virtual bool BindTexture(const atString &name, atGFXTexInterface *pTexture) { return false; }
  virtual bool BindSampler(const atString &name, atGFXSamplerInterface *pSampler) { return false; }
  virtual bool SetUniform(const atString &name, const void *pData, const atTypeDesc &info) { return false; }
  virtual bool HasUniform(const atString &name) { return false; }
  virtual bool Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const int64_t &elementCount = -1, const int64_t &elementOffset = 0) { return false; }

  template<typename T> bool SetUniform(const atString &name, const T &value);
  template<typename T> bool SetUniform(const atString &name, const atVector<T> &value);

protected:
  bool BindStages()
  {
    bool success = true;
    for (int64_t i = 0; i < atPS_Count; ++i)
      success &= !m_pStages[i] || m_pStages[i]->Bind();
    return success;
  }

  atGFXShaderInterface *m_pStages[atPS_Count];
};

template<typename T> inline bool atGFXPrgmInterface::SetUniform(const atString &name, const T &value) { SetUniform(name, (const void*)&value, atGetTypeDesc(value)); }
template<typename T> inline bool atGFXPrgmInterface::SetUniform(const atString &name, const atVector<T> &value) { SetUniform(name, (const void*)value.data(), atGetTypeDesc(value)); }

#endif // atGFXPrgmInterface_h__
