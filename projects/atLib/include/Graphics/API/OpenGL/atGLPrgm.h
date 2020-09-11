#ifndef atGLPrgm_h__
#define atGLPrgm_h__

#include "atProgram.h"
#include "atTexture.h"
#include "atHashMap.h"

class atGLPrgm : public atProgram
{
public:
  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  bool BindIndices(atGPUBuffer *pBuffer) override;
  bool BindAttribute(const atString &name, atGPUBuffer *pBuffer) override;
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
    atTextureType type = atTexture_None;
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

  atVector<VarDesc> m_uniforms;
  atVector<TexDesc> m_textures;
  atVector<AttributeDesc> m_attributes;

  atHashMap<atString, int64_t> m_uniformLookup;
  atHashMap<atString, int64_t> m_textureLookup;
  atHashMap<atString, int64_t> m_attributeLookup;
};

#endif // atGLPrgm_h__
