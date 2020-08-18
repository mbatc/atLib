#ifndef atDXBuffer_h__
#define atDXBuffer_h__

#include "atGPUBuffer.h"

class atDXBuffer : public atGPUBuffer
{
public:
  atDXBuffer(const atBufferType &type);

  bool Update() override;
  bool Upload() override;
  bool Delete() override;

  atGraphicsAPI API() override { return atGfxApi_DirectX; }
};

#endif // atDXBuffer_h__

