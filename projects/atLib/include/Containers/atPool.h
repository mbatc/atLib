#ifndef atPool_h__
#define atPool_h__

// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

  public:
    Iterator(atPool *pPool, int64_t start);
    Iterator(const Iterator &it);

    T &operator*();
    T *operator->();

    bool operator==(const Iterator &rhs) const;
    bool operator!=(const Iterator &rhs) const;

    Iterator& operator++();

  protected:
    int64_t m_idx = -1;
    atPool *m_pPool = nullptr;;
  };

  class ConstIterator
  {
    friend atPool;

  public:
    ConstIterator(atPool *pPool, int64_t start);
    ConstIterator(const ConstIterator &it);

    const T &operator*();
    const T *operator->();

    bool operator==(const ConstIterator &rhs) const;
    bool operator!=(const ConstIterator &rhs) const;

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

  // Check if an index is valid
  bool Contains(const int64_t &index) const;

  // Erase the item at the specified index
  bool erase(const int64_t &index);

  // Construct an item in place in the next available slot
  template<typename... Args> int64_t emplace(Args&&... args);

  // Accessor operators
  T& operator[](const int64_t &index);
  const T& operator[](const int64_t &index) const;

  Iterator begin();
  Iterator end();

  const ConstIterator begin() const;
  const ConstIterator end() const;

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

#include "atPool.inl"

#endif // atPool_h__
