#include "atTexture.h"

atTexture::atTexture(const atTextureType &type, const atImage &image, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(false)
{
  Set(image);
}

atTexture::atTexture(const atTextureType &type, const atVector<float> &image, const atVec2I &size, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(true)
{
  Set(image, size);
}

atTexture::atTexture(const atTextureType &type)
  : m_type(type)
  , m_isDepthTex(false)
  , m_genMipmaps(false)
  , m_sampleCount(1)
{}

atTexture::atTexture(const atTextureType &type, const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(isDepthTex)
  , m_layerCount(arraySize)
{
  m_size = size;
}

atTexture::atTexture(const atTextureType &type, const atVector<atImage> &images, const atVec2I &size, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(false)
{
  Set(images);
}

atTexture::atTexture(const atTextureType &type, const atVector<atVector<float>> &images, const atVec2I &size, const int64_t &sampleCount)
  : m_sampleCount(sampleCount)
  , m_genMipmaps(false)
  , m_type(type)
  , m_isDepthTex(true)
{
  Set(images, size);
}

atTexture::~atTexture() {}

void atTexture::Set(const atImage &image)
{
  Delete();
  m_size = image.Size();
  m_layerCount = 1;

  int64_t nBytes = m_size.x * m_size.y * sizeof(atCol);
  m_pixels.resize(nBytes);
  memcpy(m_pixels.data(), image.Pixels().data(), nBytes);
  m_isDepthTex = false;
}

void atTexture::Set(const atVector<float> &image, const atVec2I &size)
{
  Delete();
  m_size = size;
  m_layerCount = 1;
  atRelAssert(image.size() == m_size.x * m_size.y, "Not enough pixels to match size");
  int64_t nBytes = m_size.x * m_size.y * sizeof(atCol);
  m_pixels.resize(nBytes);
  memcpy(m_pixels.data(), image.data(), nBytes);
  m_isDepthTex = true;
}

void atTexture::Set(const atVector<atImage> &images)
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
  m_isDepthTex = false;
}

void atTexture::Set(const atVector<atVector<float>> &images, const atVec2I &size)
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
  m_isDepthTex = true;
}

const atVec2I& atTexture::Size() const { return m_size; }
const atTextureType& atTexture::Type() const { return m_type; }
