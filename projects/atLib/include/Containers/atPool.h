#ifndef atPool_h__
#define atPool_h__


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

#include "atVector.h"

template<typename T> class atPool
{
public:
  class Iterator
  {
    friend atPool;

    Iterator(atPool *pPool, int64_t start);
    Iterator(const Iterator &it);

    bool operator==(const Iterator &rhs) const;
    Iterator& operator++();
  };

  class ConstIterator
  {
    friend atPool;

    ConstIterator(atPool *pPool, int64_t start);
    ConstIterator(const ConstIterator &it);

    bool operator==(const ConstIterator &rhs) const;
    ConstIterator& operator++();
  };

  // Construct a pool with the specified initial capacity
  atPool(const int64_t &capacity = 1);

  // Get the number of the items in the pool
  const int64_t& size() const;

  // Get the capacity of the pool
  const int64_t& capacity() const;

  // Get the item at the specified index
  T& at(const int64_t &index);
  const T& at(const int64_t &index) const;

  // Add an item in the next available slot
  // Returns the index of the slot it was added to
  int64_t Add(const T &value);
  int64_t Add(T &&value);

  // Erase the item at the specified index
  bool erase(const int64_t &index);

  // Construct an item in place in the next available slot
  template<typename... Args> int64_t emplace(Args&&... args);

  // Accessor operators
  T& operator[](const int64_t &index);
  const T& operator[](const int64_t &index) const;

protected:
  bool TryGrow(const int64_t &requiredCapacity);
  bool Grow(const int64_t &capacity);
  int64_t GetNextAvailSlot();
  
  T *m_pData;
  int64_t m_capacity;
  int64_t m_size;

  atVector<bool> m_usedFlags;
  atVector<int64_t> m_freeSlots;
};

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
inline bool atPool<T>::erase(const int64_t &index)
{
  if (!m_usedFlags[index])
    return false;

  atDestruct(m_pData + index);
  m_freeSlots.push_back(index);
  m_usedFlags[index] = false;
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
inline bool atPool<T>::TryGrow(const int64_t &requiredCapacity)
{
  if (m_capacity > requiredCapacity)
    return false;

  return Grow(atMax(requiredCapacity, m_capacity * 2));
}

template<typename T>
inline bool atPool<T>::Grow(const int64_t &capacity)
{
  T *pNewMem = atAlloc(sizeof(T) * capacity);
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
  atConstruct(m_pData + slot, args);
  m_usedFlags[slot] = true;
  return slot;
}

#endif // atPool_h__
