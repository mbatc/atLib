#include "atGPUBuffer.h"

atGPUBuffer::atGPUBuffer(const atBufferType &bufferType)
  : m_bufferSize(0)
  , m_mapping(bufferType)
{}

void atGPUBuffer::Set(const void *pData, const int64_t &bufLen, const atTypeDesc &desc)
{
  m_bufferInfo = desc;
  Resize(bufLen);
  void *pDest = Map(atGB_MF_Write);
  if (pDest)
  {
    memcpy(pDest, pData, bufLen);
    Unmap();
  }

  m_bufferInfo = desc;
}

atVector<uint8_t> atGPUBuffer::Download()
{
  atVector<uint8_t> data;
  if (data.size() > 0)
  {
    void *pMapped = Map(atGB_MF_Read);
    if (pMapped)
    {
      data.resize(Size());
      memcpy(data.data(), pMapped, Size());
      Unmap();
    }
  }
  return data;
}

const atTypeDesc& atGPUBuffer::Desc() { return m_bufferInfo; }
const atBufferType& atGPUBuffer::Type() { return m_mapping; }
const int64_t& atGPUBuffer::Size() { return m_bufferSize; }
int64_t atGPUBuffer::Count() { return Size() / (m_bufferInfo.size * m_bufferInfo.width); }
