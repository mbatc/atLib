#ifndef atGLTexture_h__
#define atGLTexture_h__

#include "atTexture.h"

class atGLTexture : public atTexture
{
public:
  atGLTexture(const atTextureType &type);
  atGLTexture(const atTextureType &type, const atVec2I &size, const bool &isDepthTex = false, const int64_t &sampleCount = 1, const int64_t &arraySize = 1);
  atGLTexture(const atTextureType &type, const atImage &image, const int64_t &sampleCount = 1);
  atGLTexture(const atTextureType &type, const atVector<float> &image, const atVec2I &size, const int64_t &sampleCount = 1);
  atGLTexture(const atTextureType &type, const atVector<atImage> &images, const atVec2I &size, const int64_t &sampleCount = 1);
  atGLTexture(const atTextureType &type, const atVector<atVector<float>> &images, const atVec2I &size, const int64_t &sampleCount = 1);

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  int64_t Target();

  bool Bind() override;
  bool Upload() override;
  bool Delete() override;
  void GenerateMipmaps() override;
};

#endif // atGLTexture_h__
