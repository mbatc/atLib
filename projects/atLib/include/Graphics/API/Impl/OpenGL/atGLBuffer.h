#ifndef atGLBuffer_h__
#define atGLBuffer_h__

#include "atGFXBufferInterface.h"

class atGLBuffer : public atGFXBufferInterface
{
public:
  template<typename T> atGLBuffer(const T &data, const atBufferType &type = atBT_VertexData);

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  int64_t Target();

  bool Delete() override;
  bool Upload() override;
  bool Update() override;
};

template<typename T> inline atGLBuffer::atGLBuffer(const T &data, const atBufferType &type) : atGFXBufferInterface(data, type) {}

#endif // atGLBuffer_h__

