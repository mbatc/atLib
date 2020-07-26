#ifndef atRenderTarget_h__
#define atRenderTarget_h__

#include "atGFXResource.h"
#include "atTexture.h"

class atWindow;

class atRenderTarget : public atGFXResource
{
public:
  void AttachColour(atTexture *pTexture, const int64_t &slot = 0, const int64_t &layer = 0);
  void AttachDepth(atTexture *pTexture);
  
  void DetachColour(const int64_t &slot = 0);
  void DetachDepth();

  // Clear all attachments
  // Color textures to 'color'
  // Depth textures to 'depth'
  virtual bool Clear(const atVec4F &color, const float &depth = 1.0f) = 0;

  atGraphicsAPI API() override { return atGfxApi_None; }

protected:
  struct Attachment
  {
    int64_t slot = 0;
    int64_t mipLayer = 0;
    atTexture *pTex = nullptr;
  };

  atVector<Attachment> m_color;
  Attachment m_depth;
};

#endif // atRenderTarget_h__
