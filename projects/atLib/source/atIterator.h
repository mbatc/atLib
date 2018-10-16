#ifndef atIterator_h__
#define atIterator_h__

#include "atTypes.h"

template <typename T> class atIterator
{
public:
  atIterator(T *pStart, const int64_t len);

  T* begin();
  T* end();

  T* m_pBegin;
  T* m_pEnd;
};


template <typename T> class atIteratorConst
{
public:
  atIteratorConst(const T *pStart, const int64_t len);

  const T* begin() const;
  const T* end() const;

  const T* m_pBegin;
  const T* m_pEnd;
};

template <typename T> atIterator<T> atIterate(T *pStart, int64_t len);
template <typename T> atIteratorConst<T> atIterate(const T *pStart, int64_t len);

#include "atIterator.inl"
#endif // atIterator_h__
