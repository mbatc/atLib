#ifndef atGFXRenderTarget_h__
#define atGFXRenderTarget_h__

#include "atGFXResource.h"
#include "atGFXTexInterface.h"

class atWindow;

class atGFXRenderTarget : public atGFXResource
{
public:
  void AttachColour(atGFXTexInterface *pTexture, const int64_t &slot = 0, const int64_t &layer = 0);
  void AttachDepth(atGFXTexInterface *pTexture);
  
  void DetachColour(const int64_t &slot = 0);
  void DetachDepth();

  // Clear all attachments
  // Color textures to 'color'
  // Depth textures to 'depth'
  virtual bool Clear(const atVec4F &color, const float &depth = 1.0f);

  atGraphicsAPI API() override { return atGfxApi_None; }

protected:
  struct Attachment
  {
    int64_t slot = 0;
    int64_t mipLayer = 0;
    atGFXTexInterface *pTex = nullptr;
  };

  atVector<Attachment> m_color;
  Attachment m_depth;
};

#endif // atGFXRenderTarget_h__
