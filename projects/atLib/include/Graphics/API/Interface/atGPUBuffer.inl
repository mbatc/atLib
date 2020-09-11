
template<typename T>
void atGPUBuffer::Set(const atVector<T> &data)
{
  Set((const void *)data.data(), sizeof(T) * data.size(), atGetTypeDesc(data));
}

template<typename T> atGPUMappedBuffer<T> atGPUBuffer::Map(const atGPUBuffer_MapFlags &flags) { return atGPUMappedBuffer<T>(this, flags); }

template<typename T>
atGPUMappedBuffer<T>::atGPUMappedBuffer(atGPUBuffer *pBuffer, const atGPUBuffer_MapFlags &flags)
{
  m_pMappedBuffer = (T*)pBuffer->Map(flags);
  m_elementCount = pBuffer->Size() / sizeof(T);
}

template<typename T>
inline atGPUMappedBuffer<T>::~atGPUMappedBuffer()
{
  if (m_pMappedBuffer && m_pBuffer)
    m_pBuffer->Unmap();
  m_pMappedBuffer = nullptr;
  m_elementCount = 0;
  m_pMappedBuffer = nullptr;
}

template<typename T> atVector<T> Download()
{
  atVector<T> values;
  values.resize(Size() / sizeof(T));
  if (values.size() > 0)
  {
    void *pMapped = Map(atGB_MF_Read);
    memcpy(values.data(), pMapped, values.size() * sizeof(T));
    Unmap();
  }
  return values;
}

template<typename T> T &atGPUMappedBuffer<T>::operator[](const int64_t &index) { return at(index); }
template<typename T> const T &atGPUMappedBuffer<T>::operator[](const int64_t &index) const { return at(index); }
template<typename T> T &atGPUMappedBuffer<T>::at(const int64_t &index) { return m_pMappedBuffer[Index]; }
template<typename T> const T &atGPUMappedBuffer<T>::at(const int64_t &index) const { return m_pMappedBuffer[index]; }

template<typename T> int64_t atGPUMappedBuffer<T>::Size() const { return m_elementCount; }
template<typename T> int64_t atGPUMappedBuffer<T>::SizeBytes() const { return m_pBuffer->Size(); }
template<typename T> T *atGPUMappedBuffer<T>::Buffer() const { return m_pMappedBuffer; }

template<typename T> T *atGPUMappedBuffer<T>::begin() { return m_pMappedBuffer; }
template<typename T> T *atGPUMappedBuffer<T>::end() { return m_pMappedBuffer + m_elementCount; }

template<typename T> const T *atGPUMappedBuffer<T>::begin() const { return m_pMappedBuffer; }
template<typename T> const T *atGPUMappedBuffer<T>::end() const { return m_pMappedBuffer + m_elementCount; }
