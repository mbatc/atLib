#ifndef atDXBuffer_h__
#define atDXBuffer_h__

#ifdef atPLATFORM_WIN32

#include "atGFXBufferInterface.h"

class atDXBuffer : public atGFXBufferInterface
{
public:
  template<typename T> atDXBuffer(const T &data, const atBufferType &type = atBT_VertexData);

  bool Update() override;
  bool Upload() override;
  bool Delete() override;

  atGraphicsAPI API() override { return atGfxApi_DirectX; }
};

template<typename T> inline atDXBuffer::atDXBuffer(const T &data, const atBufferType &type) : atGFXBufferInterface(data, type) {}

#endif

#endif // atDXBuffer_h__

