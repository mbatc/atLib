#include "atGLRenderTarget.h"
#include "atGLTexture.h"
#include "atOpenGL.h"

static void _AttachBuffer(atTexture *pTex, const int64_t &slot, const int64_t &mipLayer, const bool &colorBuf = true)
{
  atGLTexture *pGLTex = (atGLTexture*)pTex;
  uint32_t texID = (uint32_t)(int64_t)pTex->NativeResource();
  GLint texTarget = (GLint)pGLTex->Target();

  GLenum attachment = GLenum(colorBuf ? GL_COLOR_ATTACHMENT0 + slot : GL_DEPTH_ATTACHMENT);

  switch (pGLTex->Type())
  {
  case atTexture_1D:
    glFramebufferTexture1D(texTarget, attachment, texTarget, texID, (GLint)mipLayer);
    break;
  case atTexture_2D:
    glFramebufferTexture2D(texTarget, attachment, texTarget, texID, (GLint)mipLayer);
    break;
  case atTexture_3D:
    glFramebufferTexture3D(texTarget, attachment, texTarget, texID, (GLint)mipLayer, 0);
    break;
  }
}

bool atGLRenderTarget::Bind()
{
  if (!Upload())
    return false;

  uint32_t fbo = (uint32_t)(int64_t)NativeResource();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  for (Attachment &attach : m_color)
    _AttachBuffer(attach.pTex, attach.slot, attach.mipLayer);
  if (m_depth.pTex)
    _AttachBuffer(m_depth.pTex, m_depth.slot, m_depth.mipLayer, false);  
  return true;
}

bool atGLRenderTarget::Delete()
{
  m_color.clear();
  DetachDepth();

  uint32_t fbo = (uint32_t)(int64_t)NativeResource();
  glDeleteFramebuffers(1, &fbo);
  return true;
}

bool atGLRenderTarget::Upload()
{
  if (NativeResource())
    return true;

  uint32_t fbo = 0;
  glGenFramebuffers(1, &fbo);
  m_pResource = (void*)(int64_t)fbo;
  if (!m_pResource)
    return false;

  for (Attachment &attach : m_color)
    attach.pTex->Upload();

  if (m_depth.pTex)
    m_depth.pTex->Upload();

  return true;
}

bool atGLRenderTarget::Clear(const atVec4F &color, const float &depth)
{
  Bind();
  glClearColor(color.x, color.y, color.z, color.w);
  glClearDepth(depth);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  return true;
}
