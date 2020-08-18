#ifndef atGLBuffer_h__
#define atGLBuffer_h__

#include "atGPUBuffer.h"

class atGLBuffer : public atGPUBuffer
{
public:
  atGLBuffer(const atBufferType &type);

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  int64_t Target();

  bool Delete() override;
  bool Upload() override;
  bool Update() override;
};

#endif // atGLBuffer_h__

