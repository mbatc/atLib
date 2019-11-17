#include "atGFXBufferInterface.h"

void atGFXBufferInterface::Set(const void *pData, const int64_t &bufLen, const atTypeDesc &desc)
{
  m_data.resize(bufLen);
  memcpy(m_data.data(), pData, bufLen);
  m_bufferInfo = desc;
}
