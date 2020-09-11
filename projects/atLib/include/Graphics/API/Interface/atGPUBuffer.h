#ifndef atGPUBuffer_h__
#define atGPUBuffer_h__

#include "atMemoryWriter.h"
#include "atGFXResource.h"
#include <memory>

enum atGPUBuffer_MapFlags
{
  atGB_MF_Read = 1 << 0,
  atGB_MF_Write = 1 << 1,
  atGB_MF_ReadWrite = atGB_MF_Read | atGB_MF_Write,
};

template<typename T> class atGPUMappedBuffer
{
  friend class atGPUBuffer;

  atGPUMappedBuffer(atGPUBuffer *pBuffer, const atGPUBuffer_MapFlags &flags);
  ~atGPUMappedBuffer();

public:
  // Get the element at the specified index
  T& operator[](const int64_t &index);
  const T& operator[](const int64_t &index) const;

  // Get the element at the specified index
  T& at(const int64_t &index);
  const T& at(const int64_t &index) const;

  // Get the buffers size in elements
  int64_t Size() const;

  // Get the buffers size in bytes
  int64_t SizeBytes() const;

  // Get a pointer to the mapped buffer
  T* Buffer() const;

  // Iterators
  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

protected:
  atGPUBuffer *m_pBuffer;
  T *m_pMappedBuffer;
  int64_t m_elementCount;
};

class atGPUBuffer : public atGFXResource
{
public:
  atGPUBuffer(const atBufferType &bufferType);

  // Set the buffer from a vector of values
  template<typename T> void Set(const atVector<T> &data);

  // Set the buffer data
  void Set(const void *pData, const int64_t &bufLen, const atTypeDesc &desc);

  // Set the size of the buffer
  virtual bool Resize(const int64_t &size) = 0;

  // Download the buffer from the GPU as an array of bytes
  atVector<uint8_t> Download();

  // Download the buffer from the GPU as an array of elements
  // This does not convert data. T should be the same as the
  // buffers primitive type
  template<typename T> atVector<T> Download();

  // Get a pointer to readable/writeable client memory
  virtual void* Map(const atGPUBuffer_MapFlags &flags) = 0;
  virtual bool Unmap() = 0;

  // Get a typed mapped buffer for reading or writing
  template<typename T> atGPUMappedBuffer<T> Map(const atGPUBuffer_MapFlags &flags);

  // Get the primitive type information of the buffer
  const atTypeDesc& Desc();

  // Get the type of the buffer
  const atBufferType& Type();

  // Return the size of the buffer in bytes
  const int64_t& Size();

  // Get the number of elements in the buffer
  int64_t Count();

protected:
  atTypeDesc m_bufferInfo;
  atBufferType m_mapping;
  int64_t m_bufferSize;
};

#include "atGPUBuffer.inl"

#endif // atGFXBufferInterface_h__
