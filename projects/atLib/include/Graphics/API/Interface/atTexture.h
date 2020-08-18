#ifndef atTexture_h__
#define atTexture_h__

#include "atImage.h"
#include "atGFXResource.h"

class atTexture : public atGFXResource
{
public:
  atTexture(const atTextureType &type);
  atTexture(const atTextureType &type, const atVec2I &size, const bool &isDepthTex = false, const int64_t &sampleCount = 1, const int64_t &arraySize = 1);
  atTexture(const atTextureType &type, const atImage &image, const int64_t &sampleCount = 1);
  atTexture(const atTextureType &type, const atVector<float> &image, const atVec2I &size, const int64_t &sampleCount = 1);
  atTexture(const atTextureType &type, const atVector<atImage> &images, const atVec2I &size, const int64_t &sampleCount = 1);
  atTexture(const atTextureType &type, const atVector<atVector<float>> &images, const atVec2I &size, const int64_t &sampleCount = 1);
  ~atTexture();

  void Set(const atImage &image);
  void Set(const atVector<float> &image, const atVec2I &size);
  void Set(const atVector<atImage> &images);
  void Set(const atVector<atVector<float>> &images, const atVec2I &size);
  virtual void GenerateMipmaps() = 0;

  const int64_t& GetSampleCount() const;
  void SetSampleCount(const int64_t &sampleCount);

  const atVec2I& Size() const;
  const atTextureType &Type() const;

  bool Download(atImage *pImage = nullptr, atVec2I *pSize = nullptr);
  bool Download(atVector<float> *pImage = nullptr, atVec2I *pSize = nullptr);

protected:
  atVec2I m_size;
  atTextureType m_type;

  bool m_genMipmaps;
  bool m_isDepthTex;

  int64_t m_sampleCount;
  int64_t m_layerCount;
  atVector<uint8_t> m_pixels;
};

#endif // atGFXTexInterface_h__
