template <typename T> atIterator<T>::atIterator(T *pStart, const int64_t len)
  : m_pBegin(pStart)
{
  m_pEnd = pStart + atMax(len, 0);
}

template <typename T> atIteratorConst<T>::atIteratorConst(const T *pStart, const int64_t len)
  : m_pBegin(pStart)
{
  m_pEnd = pStart + atMax(len, 0);
}

template <typename T> T* atIterator<T>::begin() { return m_pBegin; }
template <typename T> T* atIterator<T>::end() { return m_pEnd; }
template <typename T> const T* atIteratorConst<T>::begin() const { return m_pBegin; }
template <typename T> const T* atIteratorConst<T>::end() const { return m_pEnd; }

template <typename T> atIterator<T> atIterate(T *pStart, int64_t len) { return atIterator<T>(pStart, len); }
template <typename T> atIteratorConst<T> atIterate(const T *pStart, int64_t len) { return atIteratorConst<T>(pStart, len); }
