#ifndef atGLPrgm_h__
#define atGLPrgm_h__

#include "atGFXPrgmInterface.h"
#include "atGFXTexInterface.h"
#include "atHashMap.h"

class atGLPrgm : public atGFXPrgmInterface
{
public:
  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  bool BindIndices(atGFXBufferInterface *pBuffer) override;
  bool BindAttribute(const atString &name, atGFXBufferInterface *pBuffer) override;
  bool BindTexture(const atString &name, atGFXTexInterface *pTexture) override;
  bool BindSampler(const atString &name, atGFXSamplerInterface *pSampler) override;

  bool SetUniform(const atString &name, const void *pData, const atTypeDesc &info) override;
  bool HasUniform(const atString &name) override;

  bool Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType = atGFX_PT_TriangleList, const int64_t &elementCount = -1, const int64_t &elementOffset = 0) override;

  bool Delete() override;
  bool Upload() override;
  bool Bind() override;

protected:
  void Reflect();

  struct TexDesc
  {
    int64_t unit = -1;
    int64_t glLoc = 0;
    atString name = "";
    atGFXTextureType type = atGFX_TT_None;
  };

  struct VarDesc
  {
    int64_t glLoc = 0;
    atString name = "";
    atTypeDesc desc = atGetTypeDesc<void>();
  };

  struct AttributeDesc
  {
    int64_t glLoc = 0;
    atString name = "";
    atTypeDesc desc = atGetTypeDesc<void>();
  };

  int64_t m_vao = 0;
  int64_t m_activeTextures = 0;
  int64_t m_indexCount = INT64_MAX;
  int64_t m_vertexCount = INT64_MAX;
  atType m_indicesType = atType_Unknown;

  atHashMap<atString, VarDesc> m_uniforms;
  atHashMap<atString, TexDesc> m_textures;
  atHashMap<atString, AttributeDesc> m_attributes;
};

#endif // atGLPrgm_h__
