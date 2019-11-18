#ifndef atGFXBufferInterface_h__
#define atGFXBufferInterface_h__

#include "atMemoryWriter.h"
#include "atGFXResource.h"

enum atBufferType
{
  atBT_VertexData,
  atBT_IndexData,
  atBT_ShaderData,
  atBT_Count,
};

class atGFXBufferInterface : public atGFXResource
{
public:
  template<typename T> atGFXBufferInterface(const T &data, const atBufferType &bufferType = atBT_VertexData);
  template<typename T> void Set(const T &data);
  template<typename T> void Set(const atVector<T> &data);
  void Set(const void *pData, const int64_t &bufLen, const atTypeDesc &desc);

  virtual bool Sync() { return false; }   // Sync the CPU buffer with the GPU version
  virtual bool Update() { return false; }

  const atTypeDesc& Desc() { return m_bufferInfo; }
  const atBufferType& Type() { return m_mapping; }
  const atVector<uint8_t>& Data() { return m_data; } // Return the CPU side buffer may not be up to date
  const int64_t& Size() { return m_data.size(); }    // Return the size of the buffer
  int64_t Count() { return Size() / (m_bufferInfo.size * m_bufferInfo.width); }

protected:
  atTypeDesc m_bufferInfo;
  atBufferType m_mapping;
  atVector<uint8_t> m_data;
};

template<typename T> atGFXBufferInterface::atGFXBufferInterface(const T &data, const atBufferType &bufferType)
{
  m_mapping = bufferType;
  Set(data);
}

template<typename T> void atGFXBufferInterface::Set(const T &data)
{
  Set((const void*)&data, sizeof(T), atGetTypeDesc(data));
  if (m_pResource)
    Update();
}

template<typename T> void atGFXBufferInterface::Set(const atVector<T> &data)
{
  Set((const void*)data.data(), sizeof(T) * data.size(), atGetTypeDesc(data));
  if (m_pResource)
    Update();
}

#endif // atGFXBufferInterface_h__
