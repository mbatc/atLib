#include "atGPUBuffer.h"

atGPUBuffer::atGPUBuffer(const atBufferType &bufferType) { m_mapping = bufferType; }

void atGPUBuffer::Set(const void *pData, const int64_t &bufLen, const atTypeDesc &desc)
{
  m_data.resize(bufLen);
  memcpy(m_data.data(), pData, bufLen);
  m_bufferInfo = desc;
}
