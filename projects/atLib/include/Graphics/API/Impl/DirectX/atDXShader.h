#ifndef atDXShader_h__
#define atDXShader_h__

#include "atPtr.h"
#include "atHashMap.h"
#include "atDXBuffer.h"
#include "atDXSampler.h"
#include "atDXTexture.h"
#include "atGFXShaderInterface.h"

class atDXShader : public atGFXShaderInterface
{
public:
  atDXShader(const atString &src, const atPipelineStage &stage) : atGFXShaderInterface(src, stage) {}

  bool BindTexture(const atString &name, atGFXTexInterface *pTexture);
  bool BindSampler(const atString &name, atGFXSamplerInterface *pSampler);
  bool SetUniform(const atString &name, const void *pData, const atTypeDesc &info);
  bool HasUniform(const atString &name);

  atVector<atString> Textures() const;
  atVector<atString> Samplers() const;
  atVector<atString> Uniforms() const;

  const atVector<uint8_t>& ByteCode() const;
  
  int64_t AttributeCount() const;
  const atString& AttributeName(const int64_t &index) const;
  const atString& AttributeFullName(const int64_t &index) const;
  const atTypeDesc& AttributeInfo(const int64_t &index) const;
  const int64_t& AttributeIndex(const int64_t &index) const;
  const int64_t& AttributeSlot(const int64_t &index) const;

  bool Delete() override;
  bool Upload() override; // Compiles shader
  bool Bind() override;

  atGraphicsAPI API() override { return atGfxApi_DirectX; }

  void UpdateConstantBuffers();

protected:
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

#endif // atDXShader_h__
