#ifndef atDXTexture_h__
#define atDXTexture_h__

#ifdef atPLATFORM_WIN32

#include "atTexture.h"

class atDXTexture : public atTexture
{
public:
  atDXTexture(const atTextureType &type);
  atDXTexture(const atTextureType &type, const atVec2I &size, const bool &isDepthTex = false, const int64_t &sampleCount = 1, const int64_t &arraySize = 1);
  atDXTexture(const atTextureType &type, const atImage &image, const int64_t &sampleCount = 1);
  atDXTexture(const atTextureType &type, const atVector<float> &image, const atVec2I &size, const int64_t &sampleCount = 1);
  atDXTexture(const atTextureType &type, const atVector<atImage> &images, const atVec2I &size, const int64_t &sampleCount = 1);
  atDXTexture(const atTextureType &type, const atVector<atVector<float>> &images, const atVec2I &size, const int64_t &sampleCount = 1);

  atGraphicsAPI API() override { return atGfxApi_DirectX; }
  bool Delete() override;
  bool Upload() override;
  void GenerateMipmaps() override;

  void* ShaderView();
  void* RenderView();
  void* DepthView();

protected:
  void *m_pShaderView = nullptr;
  void *m_pRenderView = nullptr;
  void *m_pDepthView = nullptr;
};

#endif
#endif // atDXTexture_h__