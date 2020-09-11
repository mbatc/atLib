#ifndef atDXShader_h__
#define atDXShader_h__

#ifdef atPLATFORM_WIN32

#include "atHashMap.h"
#include "atDXBuffer.h"
#include "atDXSampler.h"
#include "atDXTexture.h"
#include "atShader.h"

class atDXShader : public atShader
{
  friend class atDXPrgm;

public:
  atDXShader(const atPipelineStage &stage) : atShader(stage) {}

  bool Delete() override;
  bool Upload() override; // Compiles shader
  bool Bind() override;

  atGraphicsAPI API() override { return atGfxApi_DirectX; }

protected:
  struct UniformLocation
  {
    int64_t bufferIdx = -1;
    int64_t variableIdx = -1;
  };

  void UpdateConstantBuffers();

  bool BindTexture(const int64_t &index, atTexture *pTexture);
  bool BindSampler(const int64_t &index, atSampler *pSampler);
  bool SetUniform(const int64_t &bufferIdx, const int64_t &varIdx, const void *pData, const atTypeDesc &info);

  int64_t AttributeCount() const;
  const atString &AttributeName(const int64_t &index) const;
  const atString &AttributeFullName(const int64_t &index) const;
  const atTypeDesc &AttributeInfo(const int64_t &index) const;
  const int64_t &AttributeIndex(const int64_t &index) const;
  const int64_t &AttributeSlot(const int64_t &index) const;

  const atVector<uint8_t> &ByteCode() const;

  atVector<atString> Textures() const;
  atVector<atString> Samplers() const;
  atVector<atKeyValue<atString, UniformLocation>> Uniforms() const;

  struct VarDesc
  {
    atString name;
    int64_t offset;
    atTypeDesc desc;
  };

  struct ConstBufferDesc
  {
    atString name;
    atVector<uint8_t> data;
    atVector<VarDesc> vars;

    int64_t slot = 0;
    bool updated = false;
    atDXBuffer *pBuffer;
  };

  struct SamplerDesc
  {
    atString name;
    int64_t slot;
    atDXSampler *pSampler;
  };

  struct TextureDesc
  {
    atString name;
    int64_t slot;
    atDXTexture *pTex;
  };

  struct AttributeDesc
  {
    atString name;
    atString fullName;
    atTypeDesc desc;
    int64_t idx;
    int64_t slot;
  };

  bool Reflect(void *pBlob);
  void SetBufferSlot(const atString &name, const int64_t &slot);
  void SetTextureSlot(const atString &name, const int64_t &slot);
  void SetSamplerSlot(const atString &name, const int64_t &slot);
  
  ConstBufferDesc* FindVarBuffer(const atString &name, VarDesc **pVar = nullptr);

  atVector<ConstBufferDesc> m_buffers;
  atVector<TextureDesc> m_textures;
  atVector<SamplerDesc> m_samplers;
  atVector<AttributeDesc> m_attributes;
  atVector<uint8_t> m_byteCode;
};

#endif

#endif // atDXShader_h__
