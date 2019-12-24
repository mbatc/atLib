#ifndef atDXBuffer_h__
#define atDXBuffer_h__

#include "atGFXBufferInterface.h"

class atDXBuffer : public atGFXBufferInterface
{
public:
  atDXBuffer(const atBufferType &type);

  template<typename T> atDXBuffer(const T &data, const atBufferType &type = atBT_VertexData);

  bool Update() override;
  bool Upload() override;
  bool Delete() override;

  atGraphicsAPI API() override { return atGfxApi_DirectX; }
};

template<typename T> inline atDXBuffer::atDXBuffer(const T &data, const atBufferType &type) : atGFXBufferInterface(data, type) {}

#endif // atDXBuffer_h__

