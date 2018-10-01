
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

#ifndef _atVector_h__
#define _atVector_h__

#include "atMemory.h"
#include "atReadStream.h"
#include "atWriteStream.h"
#include <vector>

template <class T> class atVector
{
  const double _grow_rate = 1.61803399; // PHI
public:
  typedef T* vector_iterator;
  typedef const T* vector_const_iterator;

  ~atVector() { make_empty(); }

  // Creates an empty atVector
  atVector() {}

  // Creates an atVector with an initial capacity of [reserve]
  atVector(const int64_t _reserve) { reserve(_reserve); }
  atVector(const std::initializer_list<T> &list) { assign(list.begin(), list.end()); }
  atVector(T* pData, int64_t len) { assign(pData, pData + len); }

  // Creates an atVector with size [size] and copies [initial] into each element
  atVector(const int64_t size, const T &initial) { assign(initial, size); }
  atVector(const atVector<T> &copy) { assign(copy); }
  atVector(const std::vector<T> &copy) { assign(copy); }

  atVector(atVector<T> &&move)
  {
    swap(move);
    move.make_empty();
  }

  //***************************
  // Non-Const Member functions
  T& back() { return at(m_size - 1); }
  T& front() { return at(0); }
  T& at(const int64_t index)
  {
    assert_index(index);
    return m_pData[index];
  }

  // Retrieve a pointer to the internal memory block
  T* data() { return m_pData; }

  void set_data(T* pBuffer, int64_t size) 
  {
    make_empty(); 
    m_pData = pBuffer;
    m_size = size;
    m_capacity = size;
  }

  T* take_data() 
  {
    T* ret = m_pData;
    m_pData = 0;
    m_size = 0;
    m_capacity = 0;
    return ret;
  }

  void push_back(const T &item) { emplace_back(item); }
  void pop_back() { resize(m_size - 1); }
  
  void insert(const int64_t index, const T &item) { emplace(index, item); }
  void insert(const int64_t index, const atVector<T> &items) { insert(index, items.begin(), items.end()); }
  void insert(const int64_t index, const std::vector<T> &items) { insert(index, items.begin(), items.end()); }

  void insert(const int64_t index, vector_const_iterator start, vector_const_iterator end)
  {
    if (start >= end)
      return;

    grow_reserve(m_size + (end - start));
    const int64_t startIndex = m_size;
    for (int64_t i = 0; start + i < end; i++)
      emplace_back(*(start + i));
    move_to_index(startIndex, index, end - start);
  }

  template <class... Args> void emplace_back(Args... args) { emplace_back_array(1, std::forward<Args>(args)...); }

  template <class... Args> void emplace_back_array(const int64_t count, Args... args)
  {
    grow_reserve(m_size + count);
    atConstructArray<T, Args...>(m_pData + m_size, count, std::forward<Args>(args)...);
    m_size += count;
  }

  template <class... Args> void emplace_array(const int64_t index, const int64_t count, Args... args)
  {
    emplace_back_array(count, std::forward<Args>(args)...);
    move_to_index(size() - count, index, count);
  }

  template <class... Args> void emplace(const int64_t index, Args... args)
  {
    emplace_back(index, std::forward<Args>(args)...);
    move_item(size() - 1, index);
  }

  // Memory Management
  void resize(const int64_t size)
  {
    if (try_resize(size))
      return;
    atAssert(try_grow(size), "Could not resize, try_grow failed!");
    atAssert(try_resize(size), "Could not resize atVector");
  }

  void reserve(const int64_t capacity)
  {
    if (capacity == m_capacity)
      return; // no need to realloc
    if (try_realloc(capacity))
      return; // [capacity] is > [m_size] so realloc succeeded

    // [capacity] is < [m_size] so resize the vector before realloc
    atAssert(try_resize(capacity), "Could not resize, reserve failed!");
    try_realloc(capacity);
  }

  void shrink_to_fit() { reserve(m_size); }
  void clear() { resize(0); }
  void make_empty()
  {
    clear();
    shrink_to_fit();
  }

  void erase(const int64_t index)
  {
    move_to_back(index, 1);
    pop_back();
  }

  void erase(const int64_t index, const int64_t end)
  {
    move_to_back(index, end - index);
    resize(size() - (end - index));
  }

  void erase(vector_iterator start, vector_iterator end) { erase(start - m_pData, end - m_pData); }

  void assign(const T &item, const int64_t count)
  {
    reserve(count);
    resize(0);
    for(int64_t i = 0; i < count; ++i)
      emplace_back(item);
  }

  void assign(vector_const_iterator start, vector_const_iterator end)
  {
    const int64_t count = end - start;
    resize(0);
    reserve(count);
    for (int64_t i = 0; i < count; ++i)
      emplace_back(start[i]);
  }

  // assign and attempt convert
  template <class T1> void assign(T1* start, T1* end)
  {
    const int64_t count = end - start;
    resize(0);
    reserve(count);
    for (int64_t i = 0; i < count; ++i)
      push_back((T)start[i]);
  }

  void assign(const std::vector<T> &copy) { assign(copy.begin(), copy.end()); }
  void assign(const atVector<T> &copy) { assign(copy.begin(), copy.end()); }
  void assign(std::initializer_list<T> list) { assign(list.begin(), list.end()) }

  // Access the element at [index]
  // Debug assert on invalid index
  T& operator[](const int64_t index) { return at(index); }
  T& operator[](const int32_t index) { return at((int64_t)index); }
  T& operator[](const int16_t index) { return at((int64_t)index); }
  T& operator[](const int8_t index) { return at((int64_t)index); }

  //***************************
  // Const Member functions
  bool empty() const { return m_size == 0; }
  int64_t size() const { return m_size; }
  int64_t capacity() const { return m_capacity; }

  const T& back() const { return at(m_size - 1); }
  const T& front() const { return at(0); }
  const T& at(const int64_t index) const 
  {
    assert_index(index);
    return m_pData[index];
  }

  // Retrieve a const-pointer to the internal memory block
  const T* data() const { return m_pData; }

  // Const access to the element at [index]
  // Debug assert on invalid index

  atVector<T> operator=(const atVector<T> &rhs) 
  {
    assign(rhs); 
    return *this; 
  }

  atVector<T> operator+=(const atVector<T> &rhs)
  {
    insert(size(), rhs);
    return *this;
  }

  atVector<T> operator+(const atVector<T> &rhs)
  {
    atVector<T> ret(size() + rhs.size());
    ret += *this;
    ret += rhs;
    return ret;
  }

  const T& operator[](const int64_t index) const { return at(index); }
  const T& operator[](const int32_t index) const { return at((int64_t)index); }
  const T& operator[](const int16_t index) const { return at((int64_t)index); }
  const T& operator[](const int8_t index) const { return at((int64_t)index); }

  void swap(atVector<T> &with) {
    std::swap(m_pData, with.m_pData);
    std::swap(m_size, with.m_size);
    std::swap(m_capacity, with.m_capacity);
  }
  
  void move_item(const int64_t from, const int64_t to) 
  {
    const int64_t dir = from < to ? 1 : -1;
    for (int64_t i = from; i != to; i += dir)
      std::swap(at(i), at(i + dir));
  }

  void move_to_index(const int64_t index, const int64_t to, const int64_t count = 1) {
    if (index == to)
      return;
    for (int64_t i = 0; i < count; ++i)
      move_item(index + i, to + i);
  }

  void move_to_back(const int64_t index, const int64_t count = 1) { move_to_index(index, m_size - count, count); }
  void move_to_front(const int64_t index, const int64_t count = 1) { move_to_index(index, 0, count); }
  //***************************
  // Iterator Member functions
  vector_iterator begin() { return m_pData; }
  vector_iterator end() { return m_pData + m_size; }
  vector_const_iterator begin() const { return m_pData; }
  vector_const_iterator end() const { return m_pData + m_size; }
protected:

  // Vector will only resize if [capacity] > [m_capacity]
  void grow_reserve(const int64_t capacity)
  {
    if (capacity <= m_capacity)
      return; 
    try_grow(capacity);
  }

  bool try_grow(const int64_t minSize) {
    int64_t allocSize = (int64_t)((double)m_capacity * _grow_rate + 0.5);
    if (allocSize < minSize)
      allocSize = minSize;
    return try_realloc(allocSize);
  }

  // Return false if [m_capacity] < [size]
  // Sets [m_size] to [size] on success
  bool try_resize(const int64_t size) {
    if (m_capacity < size)
      return false;
    if(m_size < size)
      emplace_back_array(size - m_size); // emplace will call the default constructor of the new elements
    else if (m_size > size)
      atDestructArray<T>(m_pData + size, m_size - size);
    m_size = size;
    return true;
  }

  // Returns false if [size] < [m_size]
  // Sets [m_pData] to new memory block on success
  bool try_realloc(const int64_t size) {
    if (size < m_size)
      return false;
    realloc(size);
    return true;
  }

  // Reallocate internal memory block
  // [size] must be >= [m_size]
  // Sets [m_capacity] to [size] on success
  void realloc(const int64_t size) {
    atAssert(size >= m_size, "Realloc size not large enough to contain all items");
    T *pNew = nullptr;
    if (size > 0)
    {
      pNew = (T*)atAlloc(size * sizeof(T));
      for (int64_t i = 0; i < m_size; ++i)
      {
        atConstruct(pNew + i, std::move(m_pData[i]));
        atDestruct(m_pData + i);
      }
    }
    atFree(m_pData);
    m_pData = pNew;
    m_capacity = size;
  }

  // Debug assert if index is invalid
  void assert_index(const int64_t index) const {
#ifdef _DEBUG
    atAssert(index >= 0 && index < m_size, "Index out of Range");
#endif
  }

  //*****************
  // Member variables
  int64_t m_size = 0;
  int64_t m_capacity = 0;
  T *m_pData = nullptr;
};

template <typename T> _atStreamWrite(atVector<T>)
{
  int64_t ret = atStreamWrite(pStream, data.size());
  for (const T &val : data)
    ret += atStreamWrite(pStream, val);
  return ret;
}

template <typename T> _atStreamRead(atVector<T>)
{
  int64_t size = 0;
  int64_t ret = atStreamRead(pStream, &size);
  T* pBuffer = (T*)atAlloc(sizeof(T) * size);
  for (int64_t i = 0; i < size; ++i)
    ret += atStreamRead(pStream, pBuffer + i);
  pData->set_data(pBuffer, size);

  return ret;
}

#endif