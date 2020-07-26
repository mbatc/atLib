#ifndef atDXRenderTarget_h__
#define atDXRenderTarget_h__

#ifdef atPLATFORM_WIN32

#include "atRenderTarget.h"

class atDXRenderTarget : public atRenderTarget
{
public:
  bool Upload() override;
  bool Delete() override;
  bool Bind() override;
  
  // Clear all attachments
  // Color textures to 'color'
  // Depth textures to 'depth'
  virtual bool Clear(const atVec4F &color, const float &depth = 1.0f) override;

  atGraphicsAPI API() override { return atGfxApi_None; }
};

#endif

#endif // atDXRenderTarget_h__
