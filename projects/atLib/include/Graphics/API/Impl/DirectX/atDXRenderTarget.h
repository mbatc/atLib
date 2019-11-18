#ifndef atDXRenderTarget_h__
#define atDXRenderTarget_h__

#include "atGFXRenderTarget.h"

class atDXRenderTarget : public atGFXRenderTarget
{
public:
  bool Upload() override;
  bool Delete() override;
  bool Bind() override;
  
  // Clear all attachments
  // Color textures to 'color'
  // Depth textures to 'depth'
  virtual bool Clear(const atVec4F &color, const float &depth = 1.0f);

  atGraphicsAPI API() override { return atGfxApi_None; }

protected:

};

#endif // atDXRenderTarget_h__
