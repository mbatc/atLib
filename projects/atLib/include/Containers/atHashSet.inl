#include "atMath.h"
#include "atHash.h"

template<typename T>
inline atHashSet<T>::atHashSet(const int64_t &bucketCount)
{
  m_buckets.resize(atMax(1LL, bucketCount));
  m_size = 0;
}

template<typename T>
inline atHashSet<T>::atHashSet(const std::initializer_list<T> &values)
  : atHashSet(values.size() / m_itemCount + 1)
{
  for (const T &value : values)
    TryAdd(value);
}

template<typename T>
inline atHashSet<T>::atHashSet(const atVector<T> &values)
  : atHashSet(values.size() / m_itemCount + 1)
{
  for (const T &value : values)
    TryAdd(value);
}

template<typename T>
inline atHashSet<T>::atHashSet(std::initializer_list<T> &&values)
  : atHashSet(values.size() / m_itemCount + 1)
{
  for (T &value : values)
    TryAdd(std::move(value));
  values.clear();
}

template<typename T>
inline atHashSet<T>::atHashSet(atVector<T> &&values)
  : atHashSet(values.size() / m_itemCount + 1)
{
  for (T &value : values)
    TryAdd(std::move(value));
  values.clear();
}

template<typename T>
inline atHashSet<T>::atHashSet(const atHashSet<T> &o)
{
  m_buckets = o.m_buckets;
}

template<typename T>
inline atHashSet<T>::atHashSet(atHashSet<T> &&o)
{
  m_buckets = std::move(o.m_buckets);
}

template<typename T>
inline void atHashSet<T>::Clear()
{
  m_buckets.resize(1);
  m_buckets[0].clear();
  m_size = 0;
}

template<typename T>
inline void atHashSet<T>::Add(const T &value)
{
  atRelAssert(TryAdd(value), "Duplicate Key!");
}

template<typename T>
inline void atHashSet<T>::Add(T &&value)
{
  atRelAssert(TryAdd(std::forward<T>(value)), "Duplicate Key!");
}

template<typename T>
inline bool atHashSet<T>::TryAdd(const T &value)
{
  Bucket &bucket = FindBucket(value);
  if (BucketContainsValue(bucket, value))
    return false;
  bucket.emplace_back(value);
  ++m_size;
  TryRehash();
  return true;
}

template<typename T>
inline bool atHashSet<T>::TryAdd(T &&value)
{
  Bucket &bucket = FindBucket(value);
  if (BucketContainsValue(bucket, value))
    return false;
  bucket.emplace_back(std::move(value));
  ++m_size;
  TryRehash();
  return true;
}

template<typename T>
inline bool atHashSet<T>::Remove(const T &value)
{
  Bucket &bucket = FindBucket(value);
  for (int64_t i = 0; i < bucket.size(); ++i)
    if (bucket[i] == value)
    {
      bucket.erase(i);
      --m_size;
      return true;
    }

  return false;
}

template<typename T>
inline bool atHashSet<T>::Contains(const T &value) const
{
  return BucketContainsValue(FindBucket(value), value);
}

template<typename T>
inline void atHashSet<T>::Rehash(const int64_t &bucketCount)
{
  atHashSet<T> newMap(atMax(1LL, bucketCount));
  for (T &value : *this)
    newMap.Add(std::move(value));
  *this = std::move(newMap);
}

template<typename T> inline int64_t atHashSet<T>::Size() const { return m_size; }

template<typename T>
inline atVector<T> atHashSet<T>::Values() const
{
  atVector<T> values;
  values.reserve(Size());
  for (const T &value : *this)
    values.push_back(value);
  return values;
}

template<typename T>
inline atHashSet<T> atHashSet<T>::InclusiveSubset(const atHashSet<T> &set)
{
  atHashSet<T> subset;
  for (const T &value : set)
    if (Contains(value))
      subset.Add(value);
  return subset;
}

template<typename T>
inline atHashSet<T> atHashSet<T>::ExclusiveSubset(const atHashSet<T> &set)
{
  atHashSet<T> subset;
  for (const T &value : set)
    if (!Contains(value))
      subset.Add(value);
  return subset;
}

template<typename T>
inline atHashSet<T>& atHashSet<T>::operator=(const atHashSet<T> &o)
{
  m_buckets = o.m_buckets;
  m_size = o.m_size;
  return *this;
}

template<typename T>
inline atHashSet<T>& atHashSet<T>::operator=(atHashSet<T> &&o)
{
  m_buckets = std::move(o.m_buckets);
  m_size = o.m_size;
  o.m_size = 0;
  return *this;
}

template<typename T>
inline bool atHashSet<T>::operator==(const atHashSet<T> &rhs) const
{
  if (rhs.Size() != Size())
    return false;
  for (const T &value : rhs)
    if (!Contains(value))
      return false;
  return true;
}

template<typename T>
inline bool atHashSet<T>::operator!=(const atHashSet<T> &rhs) const
{
  return !(*this == rhs);
}

template<typename T>
inline typename atHashSet<T>::Iterator atHashSet<T>::begin()
{
  return Iterator(&m_buckets, 0, 0);
}

template<typename T>
inline typename atHashSet<T>::Iterator atHashSet<T>::end()
{
  return Iterator(&m_buckets, m_buckets.size(), 0);
}

template<typename T>
inline typename atHashSet<T>::ConstIterator atHashSet<T>::begin() const
{
  return ConstIterator(&m_buckets, 0, 0);
}

template<typename T>
inline typename atHashSet<T>::ConstIterator atHashSet<T>::end() const
{
  return ConstIterator(&m_buckets, m_buckets.size(), 0);
}

template<typename T>
inline void atHashSet<T>::TryRehash()
{
  if ((Size() + 1) / m_buckets.size() > m_itemCount)
    Rehash(atMax(m_buckets.size() + 1, m_buckets.size() * _grow_rate));
}

template<typename T>
inline bool atHashSet<T>::BucketContainsValue(const Bucket &bucket, const T &value) const
{
  for (const T &item : bucket)
    if (item == value)
      return true;
  return false;
}

template<typename T>
inline int64_t atHashSet<T>::BucketIndex(const T &value) const
{
  return atHash::Hash(value) % m_buckets.size();
}

template<typename T>
inline typename atHashSet<T>::Bucket& atHashSet<T>::FindBucket(const T &value)
{
  return m_buckets[BucketIndex(value)];
}

template<typename T>
inline const typename atHashSet<T>::Bucket& atHashSet<T>::FindBucket(const T &value) const
{
  return m_buckets[BucketIndex(value)];
}

template<typename T>
inline int64_t atStreamRead(atReadStream *pStream, atHashSet<T> *pTarget, const int64_t count)
{
  int64_t len = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    len += atStreamRead(pStream, &pTarget[i].m_size, 1);
    len += atStreamRead(pStream, &pTarget[i].m_buckets, 1);
  }
  return len;
}


template<typename T>
inline int64_t atStreamWrite(atWriteStream *pStream, atHashSet<T> *pSource, const int64_t count)
{
  int64_t len = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    len += atStreamWrite(pStream, &pTarget[i].m_size, 1);
    len += atStreamWrite(pStream, &pTarget[i].m_buckets, 1);
  }
  return len;
}

template<typename T> inline atHashSet<T>::Iterator::Iterator(atVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx)
  : m_pBuckets(pBuckets)
  , m_bucketIdx(bucketIdx)
  , m_itemIdx(itemIdx)
{}

template<typename T> inline bool atHashSet<T>::Iterator::operator==(const typename Iterator &o) const
{
  return o.m_pBuckets == m_pBuckets && o.m_bucketIdx == m_bucketIdx && o.m_itemIdx == o.m_itemIdx;
}

template<typename T> inline bool atHashSet<T>::Iterator::operator!=(const typename Iterator &o) const { return !(*this == o); }

template<typename T> inline typename atHashSet<T>::Iterator& atHashSet<T>::Iterator::operator++()
{
  m_itemIdx++;
  if (m_itemIdx >= m_pBuckets->at(m_bucketIdx).size())
  {
    m_itemIdx = 0;
    m_bucketIdx++;
  }
  return *this;
}

template<typename T> inline T* atHashSet<T>::Iterator::operator->()
{
  return &(*this);
}

template<typename T> inline T& atHashSet<T>::Iterator::operator*()
{
  return m_pBuckets->at(m_bucketIdx)[m_itemIdx];
}

template<typename T> inline atHashSet<T>::ConstIterator::ConstIterator(const atVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx) : Iterator(pBuckets, bucketIdx, itemIdx) {}
template<typename T> inline bool atHashSet<T>::ConstIterator::operator==(const typename ConstIterator &o) const { return (ConstIterator&)Iterator::operator==((Iterator&)o); }
template<typename T> inline bool atHashSet<T>::ConstIterator::operator!=(const typename ConstIterator &o) const { return (ConstIterator&)Iterator::operator++((Iterator&)o); }
template<typename T> inline typename atHashSet<T>::ConstIterator& atHashSet<T>::ConstIterator::operator++() { return (ConstIterator&)Iterator::operator++(); }
template<typename T> const T* atHashSet<T>::ConstIterator::operator->() { return &(*((Iterator*)this)); }
template<typename T> const T& atHashSet<T>::ConstIterator::operator*() { return *((Iterator*)this); }
