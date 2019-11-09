#ifndef atGLTexture_h__
#define atGLTexture_h__

#include "atGFXTexInterface.h"

class atGLTexture : public atGFXTexInterface
{
public:
  atGraphicsAPI API() override { return atGfxApi_OpenGL; }
};

#endif // atGLTexture_h__
