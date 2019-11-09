#include "atGFXTexInterface.h"

atGFXTexInterface::atGFXTexInterface(const atImage &image, const atGFXTextureType &type, const bool &genMipMaps, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(genMipMaps)
  , m_type(type)
  , m_isDepthTex(false)
{
  Set(image);
}

atGFXTexInterface::atGFXTexInterface(const atVector<float> &image, const atVec2I &size, const atGFXTextureType &type, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(true)
{
  Set(image, size);
}

atGFXTexInterface::atGFXTexInterface(const atVec2I &size, const atGFXTextureType &type, const bool &isDepthTex, const bool &genMipMaps, const int64_t &sampleCount, const int64_t &arraySize)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(genMipMaps)
  , m_type(type)
  , m_isDepthTex(isDepthTex)
  , m_layerCount(arraySize)
{
  m_size = size;
}

atGFXTexInterface::atGFXTexInterface(const atVector<atImage> &images, const atVec2I &size, const atGFXTextureType &type, const bool &genMipMaps, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps()
  , m_type(type)
  , m_isDepthTex(false)
{
  Set(images);
}

atGFXTexInterface::atGFXTexInterface(const atVector<atVector<float>> &images, const atVec2I &size, const atGFXTextureType &type, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(true)
{
  Set(images, size);
}

atGFXTexInterface::~atGFXTexInterface() {}

void atGFXTexInterface::Set(const atImage &image)
{
  Delete();
  m_size = image.Size();
  m_layerCount = 1;

  int64_t nBytes = m_size.x * m_size.y * sizeof(atCol);
  m_pixels.resize(nBytes);
  memcpy(m_pixels.data(), image.Pixels().data(), nBytes);
}

void atGFXTexInterface::Set(const atVector<float> &image, const atVec2I &size)
{
  Delete();
  m_size = size;
  m_layerCount = 1;
  atRelAssert(image.size() == m_size.x * m_size.y, "Not enough pixels to match size");
  int64_t nBytes = m_size.x * m_size.y * sizeof(atCol);
  m_pixels.resize(nBytes);
  memcpy(m_pixels.data(), image.data(), nBytes);
}

void atGFXTexInterface::Set(const atVector<atImage> &images)
{
  Delete();
  if (images.size() == 0)
    return;

  m_size = images[0].Size();
  int64_t nBytes = m_size.x * m_size.y * sizeof(atCol);
  m_pixels.resize(nBytes * images.size());
  for (int64_t i = 0; i < images.size(); ++i)
  {
    const atImage &img = images[i];
    atRelAssert(m_size == img.Size(), "Textures in this array must be of the same size");
    memcpy(m_pixels.data() + nBytes * i, img.Pixels().data(), nBytes);
  }

  m_layerCount = images.size();
}

void atGFXTexInterface::Set(const atVector<atVector<float>> &images, const atVec2I &size)
{
  Delete();
  if (images.size() == 0)
    return;

  m_size = size;
  int64_t nPixels = m_size.x * m_size.y;
  int64_t nBytes = nPixels * sizeof(float);
  m_pixels.resize(sizeof(float) * images.size() * size.x * size.y);
  for (int64_t i = 0; i < images.size(); ++i)
  {
    const atVector<float> &img = images[i];
    atRelAssert(img.size() == nPixels, "Textures in this array must be of the same size");
    memcpy(m_pixels.data() + nBytes * i, img.data(), nBytes);
  }

  m_layerCount = images.size();
}

const atVec2I& atGFXTexInterface::Size() const { return m_size; }
