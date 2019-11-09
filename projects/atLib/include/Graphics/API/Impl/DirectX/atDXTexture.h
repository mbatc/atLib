#ifndef atDXTexture_h__
#define atDXTexture_h__

#include "atGFXTexInterface.h"

class atDXTexture : public atGFXTexInterface
{
public:
  atDXTexture(const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const bool &isDepthTex = false, const bool &genMipMaps = true, const int64_t &sampleCount = 1, const int64_t &arraySize = 1);
  atDXTexture(const atImage &image, const atGFXTextureType &type = atGFX_TT_2D, const bool &genMipMaps = true, const int64_t &sampleCount = 1);
  atDXTexture(const atVector<float> &image, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const int64_t &sampleCount = 1);
  atDXTexture(const atVector<atImage> &images, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const bool &genMipMaps = true, const int64_t &sampleCount = 1);
  atDXTexture(const atVector<atVector<float>> &images, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const int64_t &sampleCount = 1);

  atGraphicsAPI API() override { return atGfxApi_DirectX; }
  bool Delete() override;
  bool Upload() override;

  void* ShaderView();
  void* RenderView();
  void* DepthView();

protected:
  void *m_pShaderView;
  void *m_pRenderView;
  void *m_pDepthView;
};

#endif // atDXTexture_h__