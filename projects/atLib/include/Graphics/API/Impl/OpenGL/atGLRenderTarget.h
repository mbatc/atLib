#ifndef atGLRenderTarget_h__
#define atGLRenderTarget_h__

#include "atGFXRenderTarget.h"

class atGLRenderState : public atGFXRenderTarget
{
public:
  bool Bind() override;
  bool Delete() override;
  bool Upload() override;

  // Clear all attachments
  // Color textures to 'color'
  // Depth textures to 'depth'
  bool Clear(const atVec4F &color, const float &depth = 1.0f) override;

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }
};

#endif // atGLRenderTarget_h__
