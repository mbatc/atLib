
template<typename T> bool atResourceHandle::Is() const { return HasResource() && m_pInstance->resource.Is<T>(); }
template<typename T> atResourceReadHandle<T> atResourceHandle::GetReadable() const { return atResourceReadHandle<T>(*this); }
template<typename T> atResourceWriteHandle<T> atResourceHandle::GetWriteable() const { return atResourceWriteHandle<T>(*this); }
