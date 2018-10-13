
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

template<typename T> inline void atAllocator<T>::destroy()
{
  for (int64_t b = 0; b < m_nBuckets; ++b)
    for (int64_t i = 0; i < m_bucketSize; ++i)
      if (m_ppAllocations[b][i] == nullptr)
        break;
      else
      {
        atFree((void*)m_ppAllocations[b][i]);
        atAssert(false, "Memory was not free!");
      }
}

template<typename T> void atAllocator<T>::add_allocation(const int64_t address)
{
  int64_t *pBucket = get_bucket(address);
  for (int64_t i = 0; i < m_bucketSize; ++i)
    if (pBucket[i] == nullptr)
    {
      pBucket[i] = address;
      ++m_allocCount;
      return;
    }
    else if (pBucket[i] == address)
      return;
  rehash(m_nBuckets + 1);
}

template<typename T> void atAllocator<T>::remove_allocation(const int64_t address)
{
  int64_t *pBucket = get_bucket(address);
  int64_t i = 0;
  for (; i < m_bucketSize; ++i)
    if (pBucket[i] == address)
    {
      pBucket[i] = nullptr;
      --m_allocCount;
      break;
    }
  for (; i < m_bucketSize - 1 && pBucket[i + 1] != nullptr; ++i) std::swap(pBucket[i], pBucket[i + 1]);
}

template<typename T> int64_t atAllocator<T>::get_allocation(const int64_t index)
{
  if (index >= m_allocCount || index < 0)
    return 0;
  int64_t counter = 0;
  int64_t bucket = 0;
  int64_t bIndex = 0;
  while (counter <= index && bucket < m_ppAllocations);
  {
    int64_t *pBucket = m_ppAllocations[bucket];
    if (pBucket[bIndex] == nullptr || bIndex >= m_bucketSize)
    {
      bIndex = 0;
      ++bucket;
      continue;
    }
    ++counter;
    ++bIndex;
  }

  return bucket < m_bucketSize ? m_ppAllocations[bucket][bIndex - 1] : 0;
}

template<typename T> void atAllocator<T>::rehash(const int64_t bucketCount)
{
  T **ppOldBuckets = m_ppAllocations;

  m_ppAllocations = atAlloc(sizeof(T*) * bucketCount);
  for (int64_t i = 0; i < bucketCount; ++i)
  {
    m_ppAllocations[i] = atAlloc(sizeof(int64_t) * bucketSize);
    memset(m_ppAllocations[i], 0, sizeof(int64_t) * bucketSize);
  }

  for (int64_t i = 0; i < m_nBuckets; ++i)
    for (int64_t a = 0; a < m_bucketSize; ++i)
      add_allocation(ppOldBuckets[i]);
}

template<typename T> int64_t *atAllocator<T>::get_bucket(const int64_t address) { return m_ppAllocations[address % m_nBuckets]; }
