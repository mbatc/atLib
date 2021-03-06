#include "atGLTexture.h"
#include "atOpenGL.h"
#include "atFormat.h"

atGLTexture::atGLTexture(const atTextureType &type) : atTexture(type) {}

atGLTexture::atGLTexture(const atTextureType &type, const atVec2I &size, const bool &isDepthTex, const int64_t &sampleCount, const int64_t &arraySize) : atTexture(type, size, isDepthTex, sampleCount, arraySize) {}
atGLTexture::atGLTexture(const atTextureType &type, const atImage &image, const int64_t &sampleCount) : atTexture(type, image, sampleCount) {}
atGLTexture::atGLTexture(const atTextureType &type, const atVector<float> &image, const atVec2I &size, const int64_t &sampleCount) : atTexture(type, image, size, sampleCount) {}
atGLTexture::atGLTexture(const atTextureType &type, const atVector<atImage> &images, const atVec2I &size, const int64_t &sampleCount) : atTexture(type, images, size, sampleCount) {}
atGLTexture::atGLTexture(const atTextureType &type, const atVector<atVector<float>> &images, const atVec2I &size, const int64_t &sampleCount) : atTexture(type, images, size, sampleCount) {}

int64_t atGLTexture::Target()
{
  switch (m_type)
  {
  case atTexture_1D: return m_layerCount > 1 ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D;
  case atTexture_2D: return m_layerCount > 1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
  case atTexture_3D: return GL_TEXTURE_3D;
  }

  return GL_TEXTURE_2D;
}

bool atGLTexture::Bind()
{
  if (!NativeResource())
    return false;

  glBindTexture((GLenum)Target(), (uint32_t)(int64_t)NativeResource());
  return true;
}

bool atGLTexture::Upload()
{
  if (NativeResource())
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
  case atTexture_1D:
    for (int64_t layer = 0; layer < m_layerCount; ++layer)
      glTexImage1D(target, (GLint)layer, glInternalFmt, (GLsizei)m_size.x * m_size.y, 0, glPxlFmt, glPxlType, m_pixels.data());
    break;
  case atTexture_2D:
    for (int64_t layer = 0; layer < m_layerCount; ++layer)
      glTexImage2D(target, (GLint)layer, GL_RGBA, (GLsizei)m_size.x, (GLsizei)m_size.y, 0, glPxlFmt, glPxlType, m_pixels.data());
    break;
  case atTexture_3D:
    glTexImage3D(target, 0, glInternalFmt, (GLsizei)m_size.x, (GLsizei)m_size.y, (GLsizei)m_layerCount, 0, glPxlFmt, glPxlType, m_pixels.data());
    break;
  }

  GenerateMipmaps();
  m_genMipmaps = false;

#ifdef _DEBUG
  atString name = (m_isDepthTex ? "Depth " : "Color ") + atString("Texture");
  glObjectLabel(GL_TEXTURE, texID, (GLsizei)name.length(), name.c_str());
#endif

  return true;
}

bool atGLTexture::Delete()
{
  if (!NativeResource())
    return false;

  uint32_t glID = (uint32_t)(int64_t)NativeResource();
  glDeleteTextures(1, &glID);
  m_pResource = nullptr;
  m_genMipmaps = false;
  return true;
}

void atGLTexture::GenerateMipmaps()
{
  if (NativeResource() == 0)
  {
    m_genMipmaps = true;
  }
  else
  {
    uint32_t glID = (uint32_t)(int64_t)NativeResource();
    GLenum target = (GLenum)Target();
    glBindTexture(target, glID);
    glGenerateMipmap(target);
  }
}


