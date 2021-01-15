
template<typename T>
inline atPool<T>::atPool(const int64_t &capacity)
  : m_capacity(0)
  , m_size(0)
  , m_pData(0)
{
  TryGrow(capacity);
}

template<typename T>
inline const int64_t& atPool<T>::size() const
{
  return m_size;
}

template<typename T>
inline const int64_t& atPool<T>::capacity() const
{
  return m_capacity;
}

template<typename T>
inline T& atPool<T>::at(const int64_t &index)
{
  return m_pData[index];
}

template<typename T>
inline const T& atPool<T>::at(const int64_t &index) const
{
  return m_pData[index];
}

template<typename T>
inline int64_t atPool<T>::Add(const T &value)
{
  return emplace(value);
}

template<typename T>
inline int64_t atPool<T>::Add(T &&value)
{
  return emplace(std::move(value));
}

template<typename T>
inline bool atPool<T>::Contains(const int64_t &index) const
{
  return index < 0 || index >= m_capacity ? false : m_usedFlags[index];
}

template<typename T>
inline bool atPool<T>::erase(const int64_t &index)
{
  if (!m_usedFlags[index])
    return false;

  atDestruct(m_pData + index);
  m_freeSlots.push_back(index);
  m_usedFlags[index] = false;
  --m_size;
  return true;
}

template<typename T>
inline T& atPool<T>::operator[](const int64_t &index)
{
  return at(index);
}

template<typename T>
inline const T& atPool<T>::operator[](const int64_t &index) const
{
  return at(index);
}

template<typename T>
inline typename atPool<T>::Iterator atPool<T>::begin()
{
  return Iterator(this, 0);
}

template<typename T>
inline typename atPool<T>::Iterator atPool<T>::end()
{
  return Iterator(this, m_size);
}

template<typename T>
inline const typename atPool<T>::ConstIterator atPool<T>::begin() const
{
  return ConstIterator(this, 0);
}

template<typename T>
inline const typename atPool<T>::ConstIterator atPool<T>::end() const
{
  return ConstIterator(this, m_size);
}

template<typename T>
inline bool atPool<T>::TryGrow(const int64_t &requiredCapacity)
{
  return m_capacity > requiredCapacity || Grow(atMax(requiredCapacity, m_capacity * 2));
}

template<typename T>
inline bool atPool<T>::Grow(const int64_t &capacity)
{
  T *pNewMem = (T*)atAlloc(sizeof(T) * capacity);
  if (!pNewMem)
    return false;

  // Move existing items to new memory block
  for (int64_t i = 0; i < m_capacity && i < capacity; ++i)
    if (m_usedFlags[i])
    {
      atConstruct(pNewMem + i, std::move(m_pData[i]));
      atDestruct(m_pData + i);
    }

  atFree(m_pData);

  m_capacity = capacity;
  m_usedFlags.resize(capacity, false);
  m_pData = pNewMem;
  return true;
}

template<typename T>
inline int64_t atPool<T>::GetNextAvailSlot()
{
  if (m_freeSlots.size() > 0)
  {
    int64_t idx = m_freeSlots.back();
    m_freeSlots.pop_back();
    return idx;
  }

  return m_size;
}

template<typename T>
template<typename ...Args>
inline int64_t atPool<T>::emplace(Args&&... args)
{
  int64_t slot = GetNextAvailSlot();
  if (!TryGrow(m_size + 1))
    return -1;

  ++m_size;
  atConstruct(m_pData + slot, std::forward<Args>(args)...);
  m_usedFlags[slot] = true;
  return slot;
}

template<typename T>
inline atPool<T>::Iterator::Iterator(atPool *pPool, int64_t start)
  : m_pPool(pPool)
  , m_idx(start)
{}

template<typename T>
inline atPool<T>::Iterator::Iterator(const Iterator &it)
  : m_pPool(it.m_pPool)
  , m_idx(it.m_idx)
{}

template<typename T> inline T &atPool<T>::Iterator::operator*() { return m_pPool->at(m_idx); }
template<typename T> inline T *atPool<T>::Iterator::operator->() { return &m_pPool->at(m_idx); }
template<typename T> bool atPool<T>::Iterator::operator==(const Iterator &rhs) const { return m_pPool == rhs.m_pPool && m_idx == rhs.m_idx; }
template<typename T> bool atPool<T>::Iterator::operator!=(const Iterator &rhs) const { return !(*this == rhs); }
template<typename T> typename atPool<T>::Iterator &atPool<T>::Iterator::operator++() { while (++m_idx < m_pPool->m_size && !m_pPool->m_usedFlags[m_idx]); return *this; }

template<typename T>
inline atPool<T>::ConstIterator::ConstIterator(atPool *pPool, int64_t start)
  : m_pPool(pPool)
  , m_idx(start)
{}

template<typename T>
inline atPool<T>::ConstIterator::ConstIterator(const ConstIterator &it)
  : m_pPool(it.m_pPool)
  , m_idx(it.m_idx)
{}

template<typename T> inline const T &atPool<T>::ConstIterator::operator*() { return m_pPool->at(m_idx); }
template<typename T> inline const T *atPool<T>::ConstIterator::operator->() { return &m_pPool->at(m_idx); }
template<typename T> bool atPool<T>::ConstIterator::operator==(const ConstIterator &rhs) const { return m_pPool == rhs.m_pPool && m_idx == rhs.m_idx; }
template<typename T> bool atPool<T>::ConstIterator::operator!=(const ConstIterator &rhs) const { return !(*this == rhs); }
template<typename T> typename atPool<T>::ConstIterator &atPool<T>::ConstIterator::operator++() { while (++m_idx < m_pPool->m_size && !m_pPool->m_usedFlags[m_idx]); return *this; }
