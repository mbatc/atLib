
template<typename T>
inline atPtr<T>::atPtr(T *pPtr, std::function<void(T *)> onDelete,
  std::function<void(T *, int64_t)> onRelease,
  std::function<void(T *, int64_t)> onAcquire)
{
  Create(pPtr, true, onDelete, onRelease, onAcquire);
}

template<typename T> inline atPtr<T>::atPtr(std::nullptr_t) : atPtr((T*)nullptr) {}

template<typename T>
inline atPtr<T>::atPtr(T &&o)
{
  Create(atNew(T)(std::move(o)), false, [](T *pPtr) { atDelete(pPtr); }, nullptr, nullptr);
}

template<typename T>
inline atPtr<T>::atPtr(const T &o)
  : atPtr(std::move(T(o)))
{}

template<typename T> inline atPtr<T>::atPtr(atPtr<T> &&o) { *this = std::move(o); }
template<typename T> inline atPtr<T>::atPtr(const atPtr<T> &o) { *this = o; }

template<typename T>
template<typename T2>
inline atPtr<T>::atPtr(T2 &&o)
{
  Create((T*)atNew(T2)(std::move(o)), false, [](T *pPtr) { atDelete((T2 *)pPtr); }, nullptr, nullptr);
}

template<typename T>
template<typename T2>
inline atPtr<T>::atPtr(const T2 &o)
  : atPtr(T2(o))
{}

template<typename T> inline T *atPtr<T>::Get() { return m_pInstance ? m_pInstance->pData : nullptr; }
template<typename T> inline T *atPtr<T>::operator->() { return Get(); }

template<typename T>
inline T &atPtr<T>::operator*()
{
  atAssert(!IsNull(), "Dereferencing a null ptr");
  return *Get();
}

template<typename T> inline const T *atPtr<T>::Get() const { return m_pInstance ? m_pInstance->pData : nullptr; }
template<typename T> inline const T *atPtr<T>::operator->() const { return Get(); }

template<typename T>
inline const T &atPtr<T>::operator*() const
{
  atAssert(!IsNull(), "Dereferencing a null ptr");
  return *Get();
}

template<typename T>
inline atPtr<T>& atPtr<T>::operator=(T *pPtr)
{
  if (pPtr == Get())
    return *this;
  Create(pPtr, true, nullptr, nullptr, nullptr);
  return *this;
}

template<typename T>
inline atPtr<T> &atPtr<T>::operator=(atPtr<T> &&rhs)
{
  Release();
  std::swap(m_pInstance, rhs.m_pInstance);
  return *this;
}

template<typename T>
inline atPtr<T> &atPtr<T>::operator=(const atPtr<T> &rhs)
{
  Acquire(rhs.m_pInstance);
  return *this;
}

template<typename T> inline int64_t atPtr<T>::GetReferenceCount() const { return m_pInstance ? m_pInstance->refCount.load() : 1; }

template<typename T>
inline bool atPtr<T>::OnRelease(std::function<void(T *, int64_t)> onRelease)
{
  if (!m_pInstance)
    return false;
  m_pInstance->onRelease = onRelease;
  return true;
}

template<typename T>
inline bool atPtr<T>::OnAqcuire(std::function<void(T *, int64_t)> onAcquire)
{
  if (!m_pInstance)
    return false;
  m_pInstance->onAcquire = onAcquire;
  return true;
}

template<typename T>
inline bool atPtr<T>::OnDelete(std::function<void(T *)> onDelete)
{
  if (!m_pInstance || !m_pInstance->isForeign)
    return false;
  m_pInstance->onDelete = onDelete;
  return true;
}

template<typename T> inline bool atPtr<T>::IsForeign() const { return m_pInstance && m_pInstance->isForeign; }
template<typename T> inline bool atPtr<T>::IsNull() const { return Get() == nullptr; }

template<typename T> inline T *atPtr<T>::TakePtr()
{
  T *pPtr = m_pInstance->pData;
  m_pInstance->pData = nullptr;
  return pPtr;
}

template<typename T>
inline void atPtr<T>::Release()
{
  if (!m_pInstance)
    return;
  Instance *pInstance = m_pInstance;
  m_pInstance = nullptr;
  int64_t refCount = --pInstance->refCount;

  // Try call the released callback
  if (pInstance->onRelease)
    pInstance->onRelease(pInstance->pData, refCount);

  // If there are no more references delete the ptr and
  // the instance struct
  if (refCount == 0)
  {
    if (pInstance->pData && pInstance->onDelete)
      pInstance->onDelete(pInstance->pData);
    atDelete(pInstance);
  }
}

template<typename T>
inline void atPtr<T>::Acquire(typename atPtr<T>::Instance *pInstance)
{
  if (m_pInstance == pInstance)
    return; // Same ptr

  Release(); // Release the current ptr
  if (!pInstance)
    return; // Setting to null

  int64_t refCount = ++pInstance->refCount;
  m_pInstance = pInstance;

  // Try call acquire callback
  if (m_pInstance->onAcquire)
    m_pInstance->onAcquire(m_pInstance->pData, refCount);
}

template<typename T>
inline void atPtr<T>::Create(T *pData, bool isForeign, std::function<void(T *)> onDelete, std::function<void(T *, int64_t)> onRelease, std::function<void(T *, int64_t)> onAcquire)
{
  Release();
  m_pInstance = atNew(Instance);
  m_pInstance->pData = pData;
  m_pInstance->isForeign = true;
  m_pInstance->refCount = 1;
  m_pInstance->onDelete = onDelete;
  m_pInstance->onRelease = onRelease;
  m_pInstance->onAcquire = onAcquire;
}

template<typename T> inline atPtr<T>::operator bool() const { return !IsNull(); }
template<typename T> inline bool atPtr<T>::operator==(const T *pRhs) const { return Get() == pRhs; }
template<typename T> inline bool atPtr<T>::operator==(const std::nullptr_t &) const { return IsNull(); }
template<typename T> inline bool atPtr<T>::operator==(const atPtr<T> &rhs) const { return Get() == rhs.Get(); }
template<typename T> inline bool atPtr<T>::operator!=(const T *pRhs) const { return !(*this == pRhs); }
template<typename T> inline bool atPtr<T>::operator!=(const std::nullptr_t &) const { return !IsNull(); }
template<typename T> inline bool atPtr<T>::operator!=(const atPtr<T> &rhs) const { return !(*this == rhs); }
template<typename T> inline bool atPtr<T>::operator<(const T *pRhs) const { return Get() < pRhs; }
template<typename T> inline bool atPtr<T>::operator<(const atPtr<T> &rhs) const { return *this < rhs.Get(); }
template<typename T> inline bool atPtr<T>::operator>(const T *pRhs) const { return Get > pRhs; }
template<typename T> inline bool atPtr<T>::operator>(const atPtr<T> &rhs) const { return *this > rhs.Get(); }
template<typename T> inline bool atPtr<T>::operator<=(const T *pRhs) const { return Get() <= pRhs; }
template<typename T> inline bool atPtr<T>::operator<=(const atPtr<T> &rhs) const { return *this <= rhs.Get(); }
template<typename T> inline bool atPtr<T>::operator>=(const T *pRhs) const { return Get >= pRhs; }
template<typename T> inline bool atPtr<T>::operator>=(const atPtr<T> &rhs) const { return *this >= rhs.Get(); }
template<typename U> inline int64_t atHash(const atPtr<U> &o) { return atHash(o.Get()) ;}
