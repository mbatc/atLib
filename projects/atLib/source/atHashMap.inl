
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

template<class Key, class Value> atHashMap<Key, Value>::atHashMap(const int64_t bucketCount) { m_buckets.resize(max(bucketCount, 1)); m_size = 0; }
template<class Key, class Value> atHashMap<Key, Value>::atHashMap(const atHashMap<Key, Value> &copy) { m_buckets = copy.m_buckets; m_size = copy.m_size; }
template<class Key, class Value> atHashMap<Key, Value>::atHashMap(atHashMap<Key, Value> &&move) { m_buckets = std::move(move.m_buckets); m_size = move.m_size; move.m_size = 0; }
template<class Key, class Value> void atHashMap<Key, Value>::Clear() { m_buckets.resize(1); m_buckets[0].clear(); m_size = 0; }
template<class Key, class Value> int64_t atHashMap<Key, Value>::Size() { return m_size; }

template<class Key, class Value> bool atHashMap<Key, Value>::TryAdd(const KVP &kvp)
{
  Bucket &bucket = GetBucket(kvp.m_key);
  for (KVP &item : bucket)
    if (item.m_key == kvp.m_key)
      return false;
  bucket.push_back(kvp);
  ++m_size;
  return true;
}

template<class Key, class Value> bool atHashMap<Key, Value>::Contains(const Key &key) const
{
  for (const KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return true;
  return false;
}

template<class Key, class Value> void atHashMap<Key, Value>::Remove(const Key &key)
{
  Bucket &bucket = GetBucket(key);
  for (KVP &kvp : bucket)
    if (kvp.m_key == key)
    {
      std::swap(kvp, *(bucket.end() - 1));
      bucket.pop_back();
      --m_size;
      return;
    }
}

template<class Key, class Value> Value& atHashMap<Key, Value>::Get(const Key &key)
{
  for (KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return kvp.m_val;
  atAssert(false, "[Key] does not exists");
  return (*m_buckets[0].data()).m_val;
}

template<class Key, class Value> Value* atHashMap<Key, Value>::TryGet(const Key &key)
{
  for (KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return &kvp.m_val;
  return nullptr;
}

template<class Key, class Value> const Value& atHashMap<Key, Value>::Get(const Key &key) const
{
  for (const KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return kvp.m_val;
  atAssert(false, "[Key] does not exists");
}

template<class Key, class Value> const Value* atHashMap<Key, Value>::TryGet(const Key &key) const
{
  for (const KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return &kvp.m_val;
  return nullptr;
}

template<class Key, class Value> atVector<Key> atHashMap<Key, Value>::GetKeys() const
{
  atVector<Key> ret;
  for (const Bucket &bucket : m_buckets)
    for (KVP &kvp : bucket)
      ret.push_back(kvp.m_key);
  return ret;
}

template<class Key, class Value> atVector<Value> atHashMap<Key, Value>::GetValues() const
{
  atVector<Value> ret;
  for (const Bucket &bucket : m_buckets)
    for (KVP &kvp : bucket)
      ret.push_back(kvp.m_val);
  return ret;
}

template<class Key, class Value> const atHashMap<Key, Value>& atHashMap<Key, Value>::operator=(const atHashMap<Key, Value>& rhs)
{
  m_buckets = rhs.m_buckets;
  m_size = rhs.m_size;
  return *this;
}

template<class Key, class Value> const typename atHashMap<Key, Value>::Bucket& atHashMap<Key, Value>::GetBucket(const Key & key) const { return m_buckets[m_buckets.size() > 1 ? FindBucket(key) : 0]; }
template<class Key, class Value> typename atHashMap<Key, Value>::ConstIterator atHashMap<Key, Value>::begin() const { return ConstIterator(this, 0, m_buckets[0].data()); }
template<class Key, class Value> typename atHashMap<Key, Value>::ConstIterator atHashMap<Key, Value>::end() const { return ConstIterator(this, m_buckets.size() - 1, m_buckets[m_buckets.size() - 1].end()); }
template<class Key, class Value> typename atHashMap<Key, Value>::Iterator atHashMap<Key, Value>::begin() { return Iterator(this, 0, m_buckets[0].data()); }
template<class Key, class Value> typename atHashMap<Key, Value>::Iterator atHashMap<Key, Value>::end() { return Iterator(this, m_buckets.size() - 1, m_buckets[m_buckets.size() - 1].end()); }
template<class Key, class Value> typename atHashMap<Key, Value>::Bucket& atHashMap<Key, Value>::GetBucket(const Key &key) { return m_buckets[m_buckets.size() > 1 ? FindBucket(key) : 0]; }
template<class Key, class Value> void atHashMap<Key, Value>::Add(const Key &key, const Value &val) { atAssert(TryAdd(key, val), "Duplicate Key!"); }
template<class Key, class Value> bool atHashMap<Key, Value>::TryAdd(const Key &key, const Value &val) { return TryAdd(KVP(key, val)); }
template<class Key, class Value> void atHashMap<Key, Value>::Add(const KVP &kvp) { atAssert(TryAdd(kvp), "Duplicate Key!"); }
template<class Key, class Value> const Value& atHashMap<Key, Value>::operator[](const Key &key) const { return Get(key); }
template<class Key, class Value> bool atHashMap<Key, Value>::Rehash(const int64_t bucketCount) { return false; }
template<class Key, class Value> int64_t atHashMap<Key, Value>::FindBucket(const Key & key) const { return 0; }
template<class Key, class Value> Value& atHashMap<Key, Value>::operator[](const Key &key) { return Get(key); }

// -------------------------------------------------------
//                |** Hash Map Iterator **|

template<class Key, class Value> atHashMap<Key, Value>::Iterator::Iterator(atHashMap<Key, Value> *pMap, const int64_t bucketIndex, atKeyValue<Key, Value> *pBucketIterator)
{
  m_pMap = pMap;
  m_pKVP = pBucketIterator;
  m_bucket = bucketIndex;
}

template<class Key, class Value> atHashMap<Key, Value>::Iterator::Iterator(Iterator &&move)
  : iterator(move.m_pMap, move.m_bucket, move.m_pKVP)
{
  move.m_pMap = nullptr;
  move.m_pKVP = nullptr;
  move.m_bucket = 0;
}

template<class Key, class Value> const typename atHashMap<Key, Value>::Iterator& atHashMap<Key, Value>::Iterator::operator++()
{
  if (++m_pKVP == m_pMap->m_buckets[m_bucket].end() && m_bucket < m_pMap->m_buckets.size() - 1)
  {
    m_bucket++;
    m_pKVP = m_pMap->m_buckets[m_bucket].begin();
  }
  return *this;
}

template<class Key, class Value> atHashMap<Key, Value>::ConstIterator::ConstIterator(const atHashMap<Key, Value> *pMap, const int64_t bucketIndex, const atKeyValue<Key, Value> *pBucketIterator)
{
  m_pMap = pMap;
  m_pKVP = pBucketIterator;
  m_bucket = bucketIndex;
}

template<class Key, class Value> atHashMap<Key, Value>::ConstIterator::ConstIterator(ConstIterator &&move)
  : const_iterator(move.m_pMap, move.m_bucket, move.m_pKVP)
{
  move.m_pMap = nullptr;
  move.m_pKVP = nullptr;
  move.m_bucket = 0;
}

template<class Key, class Value> const typename atHashMap<Key, Value>::ConstIterator& atHashMap<Key, Value>::ConstIterator::operator++()
{
  if (++m_pKVP == m_pMap->m_buckets[m_bucket].end() && m_bucket < m_pMap->m_buckets.size() - 1)
  {
    m_bucket++;
    m_pKVP = m_pMap->m_buckets[m_bucket].begin();
  }
  return *this;
}

template<class Key, class Value> atHashMap<Key, Value>::Iterator::Iterator(const Iterator &copy) : iterator(copy.m_pMap, copy.m_bucket, copy.m_pKVP) {}
template<class Key, class Value> bool atHashMap<Key, Value>::Iterator::operator==(const Iterator &rhs) const { return m_pKVP == rhs.m_pKVP; }
template<class Key, class Value> bool atHashMap<Key, Value>::Iterator::operator!=(const Iterator &rhs) const { return m_pKVP != rhs.m_pKVP; }
template<class Key, class Value> atKeyValue<Key, Value>* atHashMap<Key, Value>::Iterator::operator->() { return m_pKVP; }
template<class Key, class Value> atKeyValue<Key, Value>& atHashMap<Key, Value>::Iterator::operator*() { return *m_pKVP; }
template<class Key, class Value> atHashMap<Key, Value>::ConstIterator::ConstIterator(const ConstIterator &copy) : const_iterator(copy.m_pMap, copy.m_bucket, copy.m_pKVP) {}
template<class Key, class Value> bool atHashMap<Key, Value>::ConstIterator::operator==(const ConstIterator &rhs) const { return m_pKVP == rhs.m_pKVP; }
template<class Key, class Value> bool atHashMap<Key, Value>::ConstIterator::operator!=(const ConstIterator &rhs) const { return m_pKVP != rhs.m_pKVP; }
template<class Key, class Value> const atKeyValue<Key, Value>* atHashMap<Key, Value>::ConstIterator::operator->() const { return m_pKVP; }
template<class Key, class Value> const atKeyValue<Key, Value>& atHashMap<Key, Value>::ConstIterator::operator*() const { return *m_pKVP; }
