#ifndef atGLTexture_h__
#define atGLTexture_h__

#include "atGFXTexInterface.h"

class atGLTexture : public atGFXTexInterface
{
public:
  atGLTexture(const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const bool &isDepthTex = false, const bool &genMipMaps = true, const int64_t &sampleCount = 1, const int64_t &arraySize = 1);
  atGLTexture(const atImage &image, const atGFXTextureType &type = atGFX_TT_2D, const bool &genMipMaps = true, const int64_t &sampleCount = 1);
  atGLTexture(const atVector<float> &image, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const int64_t &sampleCount = 1);
  atGLTexture(const atVector<atImage> &images, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const bool &genMipMaps = true, const int64_t &sampleCount = 1);
  atGLTexture(const atVector<atVector<float>> &images, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const int64_t &sampleCount = 1);

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  int64_t Target();

  bool Bind() override;
  bool Upload() override;
  bool Delete() override;
};

#endif // atGLTexture_h__
