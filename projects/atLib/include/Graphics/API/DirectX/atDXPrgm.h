#ifndef atDXPrgm_h__
#define atDXPrgm_h__

#include "atProgram.h"
#include "atDXBuffer.h"
#include "atDXShader.h"

class atDXPrgm : public atProgram
{
public:
  bool Bind() override;
  bool Upload() override;
  bool Delete() override;
 
  bool IsBound();

  bool BindAttribute(const atString &name, atGPUBuffer *pBuffer) override;
  bool BindIndices(atGPUBuffer *pBuffer) override;
  bool BindTexture(const atString &name, atTexture *pTexture) override;
  bool BindSampler(const atString &name, atSampler *pSampler) override;

  bool SetUniform(const atString &name, const void *pData, const atTypeDesc &info) override;
  bool HasUniform(const atString &name) const override;

  int64_t FindUniform(const atString &index) const override;
  int64_t FindAttribute(const atString &index) const override;
  int64_t FindTexture(const atString &index) const override;
  int64_t FindSampler(const atString &index) const override;

  bool BindAttribute(const int64_t &index, atGPUBuffer *pBuffer) override;
  bool BindTexture(const int64_t &index, atTexture *pTexture) override;
  bool BindSampler(const int64_t &index, atSampler *pSampler) override;
  bool SetUniform(const int64_t &index, const void *pData, const atTypeDesc &info) override;

  bool Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const int64_t &elementCount = -1, const int64_t &elementOffset = 0, const int64_t &baseVtxIdx = 0) override;

  atGraphicsAPI API() override { return atGfxApi_DirectX; }

protected:
  void BindInput(const int64_t &i);
  void BindLayout();
  void BindIndices();

  // Index buffer input
  atDXBuffer *m_pIndices = nullptr;

  // Vertex buffer input
  void *m_pLayout = nullptr;
  atVector<atDXBuffer*> m_inputs;

  atVector<int64_t> m_attributeLoc;
  atVector<atKeyValue<int64_t, int64_t>> m_textureLoc;
  atVector<atKeyValue<int64_t, int64_t>> m_samplerLoc;
  atVector<atKeyValue<int64_t, atDXShader::UniformLocation>> m_uniformLoc;

  atHashMap<atString, int64_t> m_uniformLookup;
  atHashMap<atString, int64_t> m_textureLookup;
  atHashMap<atString, int64_t> m_samplerLookup;
  atHashMap<atString, int64_t> m_attributeLookup;
};

#endif // atDXPrgm_h__
