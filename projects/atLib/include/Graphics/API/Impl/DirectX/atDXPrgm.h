#ifndef atDXPrgm_h__
#define atDXPrgm_h__

#include "atGFXPrgmInterface.h"
#include "atDXBuffer.h"
#include "atPtr.h"

class atDXPrgm : public atGFXPrgmInterface
{
public:
  bool Bind() override;
  bool Upload() override;
  bool Delete() override;
 
  bool IsBound();

  bool BindAttribute(const atString &name, atGFXBufferInterface *pBuffer) override;
  bool BindIndices(atGFXBufferInterface *pBuffer) override;
  bool BindTexture(const atString &name, atGFXTexInterface *pTexture) override;
  bool BindSampler(const atString &name, atGFXSamplerInterface *pSampler) override;

  bool SetUniform(const atString &name, const void *pData, const atTypeDesc &info) override;
  bool HasUniform(const atString &name) override;
  bool Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const int64_t &elementCount = -1, const int64_t &elementOffset = 0) override;

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
};

#endif // atDXPrgm_h__
