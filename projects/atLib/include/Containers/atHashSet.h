#ifndef atHashSet_h__
#define atHashSet_h__

#include "atVector.h"

template <typename T> class atHashSet
{
  const double _grow_rate = 1.61803399; // PHI
  const int64_t m_itemCount = 16;

public:
  typedef atVector<T> Bucket;

  class Iterator
  {
  public:
    Iterator(atVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx);
    bool operator==(const Iterator &o) const;
    bool operator!=(const Iterator &o) const;
    Iterator& operator++();

    T* operator->();
    T& operator*();

  protected:
    atVector<Bucket> *m_pBuckets;
    int64_t m_bucketIdx;
    int64_t m_itemIdx;
  };

  class ConstIterator : public Iterator
  {
  public:
    ConstIterator(const atVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx);
    bool operator==(const ConstIterator &o) const;
    bool operator!=(const ConstIterator &o) const;
    ConstIterator& operator++();

    const T* operator->();
    const T& operator*();
  };

  atHashSet(const int64_t &bucketCount = 1);
  atHashSet(const std::initializer_list<T> &values);
  atHashSet(const atVector<T> &values);
  atHashSet(std::initializer_list<T> &&values);
  atHashSet(atVector<T> &&values);

  atHashSet(const atHashSet<T> &o);
  atHashSet(atHashSet<T> &&o);

  void Clear();
  void Add(const T &value);
  void Add(T &&value);
  bool TryAdd(const T &value);
  bool TryAdd(T &&value);
  bool Remove(const T &value);
  bool Contains(const T &value) const;

  void Rehash(const int64_t &bucketCount);

  int64_t Size() const;

  atVector<T> Values() const;
  atHashSet<T> InclusiveSubset(const atHashSet<T> &set);
  atHashSet<T> ExclusiveSubset(const atHashSet<T> &set);

  atHashSet<T>& operator=(const atHashSet<T> &o);
  atHashSet<T>& operator=(atHashSet<T> &&o);

  bool operator==(const atHashSet<T> &rhs) const;
  bool operator!=(const atHashSet<T> &rhs) const;

  template<typename T> friend int64_t atStreamRead(atReadStream *pStream, atHashSet<T> *pTarget, const int64_t count);
  template<typename T> friend int64_t atStreamWrite(atWriteStream *pStream, atHashSet<T> *pSource, const int64_t count);

  Iterator begin();
  Iterator end();

  ConstIterator begin() const;
  ConstIterator end() const;

protected:
  void TryRehash();

  atVector<Bucket> m_buckets;
  int64_t m_size;

  bool BucketContainsValue(const Bucket &bucket, const T &value) const;

  int64_t BucketIndex(const T &value) const;
  Bucket& FindBucket(const T &value);
  const Bucket& FindBucket(const T &value) const;
};

#include "atHashSet.inl"

#endif // atHashSet_h__
