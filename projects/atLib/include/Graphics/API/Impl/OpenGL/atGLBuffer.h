#ifndef atGLBuffer_h__
#define atGLBuffer_h__

#include "atGFXBufferInterface.h"

class atGLBuffer : public atGFXBufferInterface
{
public:
  atGraphicsAPI API() override { return atGfxApi_OpenGL; }
};

#endif // atGLBuffer_h__
