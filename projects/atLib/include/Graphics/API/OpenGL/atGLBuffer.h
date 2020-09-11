#ifndef atGLBuffer_h__
#define atGLBuffer_h__

#include "atGPUBuffer.h"
#include <atomic>

class atGLBuffer : public atGPUBuffer
{
public:
  atGLBuffer(const atBufferType &type, const int64_t &size);

  atGraphicsAPI API() override { return atGfxApi_OpenGL; }

  int64_t Target();
  static int64_t Target(const atBufferType &target);

  bool Delete() override;
  bool Upload() override;

  bool Resize(const int64_t &size) override;
  void* Map(const atGPUBuffer_MapFlags &flags) override;
  bool Unmap() override;

  void Bind(const atBufferType &target);

protected:
  std::atomic<int64_t> m_mappedCount = 0;
  int64_t m_gpuBufferSize = 0;
  void *m_pMappedPtr = nullptr;
};

#endif // atGLBuffer_h__

