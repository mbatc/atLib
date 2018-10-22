
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#ifndef atHashMap_h__
#define atHashMap_h__

#include "atKeyValue.h"
#include "atVector.h"

template <class Key, class Value> class atHashMap
{
public:
  const int64_t m_itemCount = 16;

  typedef atKeyValue<Key, Value> KVP;
  typedef atVector<KVP> Bucket;

  class Iterator
  {
  public:
    Iterator(atHashMap<Key, Value> *pMap, const int64_t bucketIndex, atKeyValue<Key, Value> *pBucketIterator);
    Iterator(const Iterator &copy);
    Iterator(Iterator &&move);

    bool operator==(const Iterator &rhs) const;
    bool operator!=(const Iterator &rhs) const;

    atKeyValue<Key, Value>* operator->();
    atKeyValue<Key, Value>& operator*();
    const Iterator& operator++();

  protected:
    atKeyValue<Key, Value> *m_pKVP;
    atHashMap<Key, Value> *m_pMap;
    int64_t m_bucket;
  };

  class ConstIterator
  {
  public:
    ConstIterator(const atHashMap<Key, Value> *pMap, const int64_t bucketIndex, const atKeyValue<Key, Value> *pBucketIterator);
    ConstIterator(const ConstIterator &copy);
    ConstIterator(ConstIterator &&move);

    bool operator==(const ConstIterator &rhs) const;
    bool operator!=(const ConstIterator &rhs) const;

    const atKeyValue<Key, Value>* operator->() const;
    const atKeyValue<Key, Value>& operator*() const;
    const ConstIterator& operator++();

  protected:
    const atKeyValue<Key, Value> *m_pKVP;
    const atHashMap<Key, Value> *m_pMap;
    int64_t m_bucket;
  };

  atHashMap(const int64_t bucketCount = 1);
  atHashMap(const atHashMap<Key, Value> &copy);
  atHashMap(atHashMap<Key, Value> &&move);

  void Clear();
  int64_t Size() const;

  void Add(const Key &key, const Value &val = Value());
  void Add(const KVP &kvp);
  bool TryAdd(const Key &key, const Value &val = Value());
  bool TryAdd(const KVP &kvp);

  bool Contains(const Key &key) const;

  void Remove(const Key &key);

  Value& Get(const Key &key);
  Value* TryGet(const Key &key);
  Value& operator[](const Key &key);

  const Value& Get(const Key &key) const;
  const Value* TryGet(const Key &key) const;
  const Value& operator[](const Key &key) const;

  atVector<Key> GetKeys() const;
  atVector<Value> GetValues() const;
  
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;

  const atHashMap<Key, Value>& operator=(const atHashMap<Key, Value> &rhs);

protected:
  Bucket &GetBucket(const Key &key);
  const Bucket &GetBucket(const Key &key) const;

  bool Rehash(const int64_t bucketCount);
  int64_t FindBucket(const Key &key) const;

  atVector<Bucket> m_buckets;
  int64_t m_size;
};

#include "atHashMap.inl"
#endif // atHashMap_h__
