#include "atGLTexture.h"
#include "atOpenGL.h"
#include "atFormat.h"

atGLTexture::atGLTexture(const atVec2I &size, const atGFXTextureType &type, const bool &isDepthTex, const bool &genMipMaps, const int64_t &sampleCount, const int64_t &arraySize) : atGFXTexInterface(size, type, isDepthTex, genMipMaps, sampleCount, arraySize) {}
atGLTexture::atGLTexture(const atImage &image, const atGFXTextureType &type, const bool &genMipMaps, const int64_t &sampleCount) : atGFXTexInterface(image, type, genMipMaps, sampleCount) {}
atGLTexture::atGLTexture(const atVector<float> &image, const atVec2I &size, const atGFXTextureType &type, const int64_t &sampleCount) : atGFXTexInterface(image, size, type, sampleCount) {}
atGLTexture::atGLTexture(const atVector<atImage> &images, const atVec2I &size, const atGFXTextureType &type, const bool &genMipMaps, const int64_t &sampleCount) : atGFXTexInterface(images, size, type, genMipMaps, sampleCount) {}
atGLTexture::atGLTexture(const atVector<atVector<float>> &images, const atVec2I &size, const atGFXTextureType &type, const int64_t &sampleCount) : atGFXTexInterface(images, size, type, sampleCount) {}

int64_t atGLTexture::Target()
{
  switch (m_type)
  {
  case atGFX_TT_1D: return m_layerCount > 1 ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
  case atGFX_TT_2D: return m_layerCount > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
  case atGFX_TT_3D: return GL_TEXTURE_3D;
  }

  return GL_TEXTURE_2D;
}

bool atGLTexture::Bind()
{
  if (!GFXResource())
    return false;

  glBindTexture((GLenum)Target(), (uint32_t)(int64_t)GFXResource());
  return true;
}

bool atGLTexture::Upload()
{
  if (GFXResource())
    return true;

  uint32_t texID = 0;
  GLenum target = (GLenum)Target();
  glCreateTextures(target, 1, &texID);
  m_pResource = (void*)(int64_t)texID;

  if (!m_pResource)
    return false;

  glBindTexture(target, texID);
  GLenum glPxlFmt = (GLenum)(m_isDepthTex ? atFormat::GLDepth(atType_Float32) : atFormat::GL(atType_Uint8, 4));
  GLenum glPxlType = (GLenum)(m_isDepthTex ? atFormat::GLType(atType_Float32) : atFormat::GLType(atType_Uint8));
  GLint glInternalFmt = (GLint)(m_isDepthTex ? atFormat::GLDepthInternal(atType_Float32) : atFormat::GLInternal(atType_Uint8, 4));
  
  switch (m_type)
  {
  case atGFX_TT_1D:
    for (int64_t layer = 0; layer < m_layerCount; ++layer)
      glTexImage1D(target, (GLint)layer, glInternalFmt, (GLsizei)m_size.x * m_size.y, 0, glPxlFmt, glPxlType, m_pixels.data());
    break;
  case atGFX_TT_2D:
    for (int64_t layer = 0; layer < m_layerCount; ++layer)
      glTexImage2D(target, (GLint)layer, GL_RGBA8, (GLsizei)m_size.x, (GLsizei)m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels.data());
    break;
  case atGFX_TT_3D:
    glTexImage3D(target, 0, glInternalFmt, (GLsizei)m_size.x, (GLsizei)m_size.y, (GLsizei)m_layerCount, 0, glPxlFmt, glPxlType, m_pixels.data());
    break;
  }
  
  if (m_genMipmaps)
    glGenerateMipmap(target);

#ifdef _DEBUG
  atString name = (m_isDepthTex ? "Depth " : "Color ") + atString("Texture");
  glObjectLabel(GL_TEXTURE, texID, (GLsizei)name.length(), name.c_str());
#endif

  return true;
}

bool atGLTexture::Delete()
{
  if (!GFXResource())
    return false;

  uint32_t glID = (uint32_t)(int64_t)GFXResource();
  glDeleteTextures(1, &glID);
  m_pResource = nullptr;
  return true;
}


