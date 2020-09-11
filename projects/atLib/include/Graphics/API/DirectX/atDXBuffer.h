#ifndef atDXBuffer_h__
#define atDXBuffer_h__

#include "atGPUBuffer.h"
#include <atomic>

class atDXBuffer : public atGPUBuffer
{
public:
  atDXBuffer(const atBufferType &type, const int64_t &size = 0);

  bool Upload() override;
  bool Delete() override;

  bool Resize(const int64_t &size);
  void* Map(const atGPUBuffer_MapFlags &flags);
  bool Unmap();

  atGraphicsAPI API() override { return atGfxApi_DirectX; }

protected:
  int64_t m_gpuBufferSize = 0;
  std::atomic<int64_t> m_mappedCount = 0;
  void *m_pMappedPtr = nullptr;
};

#endif // atDXBuffer_h__

