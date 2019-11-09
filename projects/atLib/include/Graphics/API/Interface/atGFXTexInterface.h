#ifndef atGFXTexInterface_h__
#define atGFXTexInterface_h__

#include "atImage.h"
#include "atGFXResource.h"

enum atGFXTextureType
{
  atGFX_TT_1D,
  atGFX_TT_2D,
  atGFX_TT_3D,
  atGFX_TT_CubeMap,
  atGFX_TT_Count,
};

class atGFXTexInterface : public atGFXResource
{
public:
  atGFXTexInterface(const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const bool &isDepthTex = false, const bool &genMipMaps = true, const int64_t &sampleCount = 1, const int64_t &arraySize = 1);
  atGFXTexInterface(const atImage &image, const atGFXTextureType &type = atGFX_TT_2D, const bool &genMipMaps = true, const int64_t &sampleCount = 1);
  atGFXTexInterface(const atVector<float> &image, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const int64_t &sampleCount = 1);
  atGFXTexInterface(const atVector<atImage> &images, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const bool &genMipMaps = true, const int64_t &sampleCount = 1);
  atGFXTexInterface(const atVector<atVector<float>> &images, const atVec2I &size, const atGFXTextureType &type = atGFX_TT_2D, const int64_t &sampleCount = 1);
  ~atGFXTexInterface();

  void Set(const atImage &image);
  void Set(const atVector<float> &image, const atVec2I &size);
  void Set(const atVector<atImage> &images);
  void Set(const atVector<atVector<float>> &images, const atVec2I &size);

  const atVec2I& Size() const;

protected:
  atVec2I m_size;
  atGFXTextureType m_type;

  bool m_genMipmaps;
  bool m_isDepthTex;
  int64_t m_sampleCount;
  int64_t m_layerCount;

  atVector<uint8_t> m_pixels;
  // void *m_pPixels;
  // int64_t m_bufSize;
};

#endif // atGFXTexInterface_h__
