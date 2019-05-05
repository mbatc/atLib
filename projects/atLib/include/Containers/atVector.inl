
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

template<typename T> template<typename T2> atVector<T>::atVector(const atVector<T2> &vec)
{
  reserve(vec.size());
  for (const T2 &i : vec)
    push_back(T2(i));
}

template<typename T> atVector<T>::atVector(atVector<T> &&move)
{
  swap(move);
  move.make_empty();
}

template<typename T> T& atVector<T>::at(const int64_t index)
{
  atAssert(index >= 0 && index < m_size, "Index out of Range");
  return m_pData[index];
}

template<typename T> void atVector<T>::set_data(T* pBuffer, const int64_t size, const int64_t capacity)
{
  make_empty();
  m_pData = pBuffer;
  m_size = size;
  m_capacity = capacity;
}

template<typename T> T* atVector<T>::take_data()
{
  T* ret = m_pData;
  m_pData = 0;
  m_size = 0;
  m_capacity = 0;
  return ret;
}

template<typename T> void atVector<T>::insert(const int64_t index, vector_const_iterator start, vector_const_iterator end)
{
  if (start >= end)
    return;

  int64_t startIndex = m_size;
  grow_reserve(m_size + (end - start));
  for (int64_t i = 0; start + i < end; i++)
    emplace_back(*(start + i));
  move_to_index(startIndex, index, end - start);
}


template<typename T> template<typename... Args> void atVector<T>::emplace_back_array(const int64_t count, Args... args)
{
  grow_reserve(m_size + count);
  atConstructArray<T, Args...>(m_pData + m_size, count, std::forward<Args>(args)...);
  m_size += count;
}

template<typename T> template<typename... Args> void atVector<T>::emplace_array(const int64_t index, const int64_t count, Args... args)
{
  emplace_back_array(count, std::forward<Args>(args)...);
  move_to_index(size() - count, index, count);
}

template<typename T> template<typename... Args> void atVector<T>::emplace(const int64_t index, Args... args)
{
  emplace_back(index, std::forward<Args>(args)...);
  move_item(size() - 1, index);
}

template<typename T> void atVector<T>::resize(const int64_t size)
{
  if (try_resize(size))
    return;
  atAssert(try_grow(size), "Could not resize, try_grow failed!");
  atAssert(try_resize(size), "Could not resize atVector");
}

template<typename T> void atVector<T>::reserve(const int64_t capacity)
{
  if (capacity == m_capacity)
    return; // no need to realloc
  if (try_realloc(capacity))
    return; // [capacity] is > [m_size] so realloc succeeded

            // [capacity] is < [m_size] so resize the vector before realloc
  atAssert(try_resize(capacity), "Could not resize, reserve failed!");
  try_realloc(capacity);
}

template<typename T> void atVector<T>::make_empty()
{
  clear();
  shrink_to_fit();
}

template<typename T> void atVector<T>::erase(const int64_t index)
{
  move_to_back(index, 1);
  pop_back();
}

template<typename T> void atVector<T>::erase(const int64_t start, const int64_t end)
{
  move_to_back(start, end - start);
  resize(size() - (end - start));
}


template<typename T> void atVector<T>::assign(const T &item, const int64_t count)
{
  reserve(count);
  resize(0);
  for (int64_t i = 0; i < count; ++i)
    emplace_back(item);
}

template<typename T> void atVector<T>::assign(vector_const_iterator start, vector_const_iterator end)
{
  const int64_t count = end - start;
  resize(0);
  reserve(count);
  if (std::is_integral<T>::value)
  {
    memcpy(m_pData, start, sizeof(T) * count);
    m_size = count;
  }
  else
    for (int64_t i = 0; i < count; ++i)
      emplace_back(start[i]);
}

template<typename T> template<typename T1> void atVector<T>::assign(T1* start, T1* end)
{
  const int64_t count = end - start;
  resize(0);
  reserve(count);;
  for (int64_t i = 0; i < count; ++i)
    push_back((T)start[i]);
}

template<typename T> template<typename T2> inline atVector<T>::operator std::vector<T2>()
{
  std::vector<T2> ret;
  ret.reserve(size());
  for (T &i : *this)
    ret.push_back(T2(i));
  return ret;
}

template<typename T> inline atVector<T>::operator std::vector<T>()
{
  std::vector<T> ret;
  ret.reserve(size());
  for (T &i : *this)
    ret.push_back(i);
  return ret;
}

template<typename T> const T& atVector<T>::at(const int64_t index) const
{
  atAssert(index >= 0 && index < m_size, "Index out of Range");
  return m_pData[index];
}

template<typename T> const atVector<T> &atVector<T>::operator=(atVector<T> &&rhs)
{
  swap(rhs);
  rhs.make_empty();
  return *this;
}

template<typename T> const atVector<T> &atVector<T>::operator=(const atVector<T> &rhs)
{
  assign(rhs);
  return *this;
}

template<typename T> const atVector<T> &atVector<T>::operator+=(const atVector<T> &rhs)
{
  insert(size(), rhs);
  return *this;
}

template<typename T> const atVector<T> &atVector<T>::operator+(const atVector<T> &rhs)
{
  atVector<T> ret(size() + rhs.size());
  ret += *this;
  ret += rhs;
  return ret;
}

template<typename T> void atVector<T>::swap(atVector<T> &with) 
{
  std::swap(m_pData, with.m_pData);
  std::swap(m_size, with.m_size);
  std::swap(m_capacity, with.m_capacity);
}

template<typename T> void atVector<T>::move_item(const int64_t from, const int64_t to)
{
  const int64_t dir = from < to ? 1 : -1;
  for (int64_t i = from; i != to; i += dir)
    std::swap(at(i), at(i + dir));
}

template<typename T> void atVector<T>::move_to_index(const int64_t index, const int64_t to, const int64_t count = 1) 
{
  if (index == to)
    return;
  for (int64_t i = 0; i < count; ++i)
    move_item(index + i, to + i);
}

template<typename T> void atVector<T>::grow_reserve(const int64_t capacity)
{
  if (capacity <= m_capacity)
    return;
  try_grow(capacity);
}

template<typename T> bool atVector<T>::try_grow(const int64_t minSize) 
{
  int64_t allocSize = (int64_t)((double)m_capacity * _grow_rate + 0.5);
  if (allocSize < minSize)
    allocSize = minSize;
  return try_realloc(allocSize);
}

template<typename T> bool atVector<T>::try_resize(const int64_t size) 
{
  if (m_capacity < size)
    return false;
  if (m_size < size)
    emplace_back_array(size - m_size); // emplace will call the default constructor of the new elements
  else if (m_size > size)
    atDestructArray<T>(m_pData + size, m_size - size);
  m_size = size;
  return true;
}

template<typename T> bool atVector<T>::try_realloc(const int64_t size) 
{
  if (size < m_size)
    return false;
  realloc(size);
  return true;
}

template<typename T> void atVector<T>::realloc(const int64_t size) 
{
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

template<typename T> void atVector<T>::move_to_back(const int64_t index, const int64_t count = 1) { move_to_index(index, m_size - count, count); }
template<typename T> void atVector<T>::move_to_front(const int64_t index, const int64_t count = 1) { move_to_index(index, 0, count); }
template<typename T> typename atVector<T>::vector_iterator atVector<T>::begin() { return m_pData; }
template<typename T> typename atVector<T>::vector_iterator atVector<T>::end() { return m_pData + m_size; }
template<typename T> typename atVector<T>::vector_const_iterator atVector<T>::begin() const { return m_pData; }
template<typename T> typename atVector<T>::vector_const_iterator atVector<T>::end() const { return m_pData + m_size; }
template<typename T> const bool atVector<T>::operator==(const atVector<T>& rhs) const { return size() == rhs.size() && memcmp(rhs.data(), data(), size()) == 0; }
template<typename T> const bool atVector<T>::operator!=(const atVector<T>& rhs) const { return !(rhs == *this); }
template<typename T> const T& atVector<T>::operator[](const int64_t index) const { return at(index); }
template<typename T> const T& atVector<T>::operator[](const int32_t index) const { return at((int64_t)index); }
template<typename T> const T& atVector<T>::operator[](const int16_t index) const { return at((int64_t)index); }
template<typename T> const T& atVector<T>::operator[](const int8_t index) const { return at((int64_t)index); }
template<typename T> atVector<T>::~atVector() { make_empty(); }
template<typename T> atVector<T>::atVector() {}
template<typename T> atVector<T>::atVector(const int64_t _reserve) { reserve(_reserve); }
template<typename T> atVector<T>::atVector(const std::initializer_list<T> &list) { assign(list.begin(), list.end()); }
template<typename T> atVector<T>::atVector(T* pData, int64_t len) { assign(pData, pData + len); }
template<typename T> atVector<T>::atVector(const int64_t size, const T &initial) { assign(initial, size); }
template<typename T> atVector<T>::atVector(const atVector<T> &copy) { assign(copy); }
template<typename T> atVector<T>::atVector(const std::vector<T> &copy) { assign(copy); }
template<typename T> T* atVector<T>::data() { return m_pData; }
template<typename T> T& atVector<T>::back() { return at(m_size - 1); }
template<typename T> T& atVector<T>::front() { return at(0); }
template<typename T> void atVector<T>::push_back(const atVector<T>& item) { for (const T &i : item) push_back(i); }
template<typename T> void atVector<T>::push_back(const T &item) { emplace_back(item); }
template<typename T> void atVector<T>::pop_back() { resize(m_size - 1); }
template<typename T> void atVector<T>::insert(const int64_t index, const T &item) { emplace(index, item); }
template<typename T> void atVector<T>::insert(const int64_t index, const atVector<T> &items) { insert(index, items.begin(), items.end()); }
template<typename T> void atVector<T>::insert(const int64_t index, const std::vector<T> &items) { insert(index, items.begin(), items.end()); }
template<typename T> template<typename... Args> void atVector<T>::emplace_back(Args... args) { emplace_back_array(1, std::forward<Args>(args)...); }
template<typename T> void atVector<T>::shrink_to_fit() { reserve(m_size); }
template<typename T> void atVector<T>::clear() { resize(0); }
template<typename T> void atVector<T>::erase(vector_iterator start, vector_iterator end) { erase(start - m_pData, end - m_pData); }
template<typename T> T& atVector<T>::operator[](const int64_t index) { return at(index); }
template<typename T> T& atVector<T>::operator[](const int32_t index) { return at((int64_t)index); }
template<typename T> T& atVector<T>::operator[](const int16_t index) { return at((int64_t)index); }
template<typename T> T& atVector<T>::operator[](const int8_t index) { return at((int64_t)index); }
template<typename T> const int64_t& atVector<T>::size() const { return m_size; }
template<typename T> const T& atVector<T>::front() const { return at(0); }
template<typename T> const T* atVector<T>::data() const { return m_pData; }
template<typename T> bool atVector<T>::empty() const { return m_size == 0; }
template<typename T> const int64_t& atVector<T>::capacity() const { return m_capacity; }
template<typename T> const T& atVector<T>::back() const { return at(m_size - 1); }
template<typename T> void atVector<T>::assign(const atVector<T> &copy) { assign(copy.begin(), copy.end()); }
template<typename T> void atVector<T>::assign(const std::vector<T> &copy) { assign(copy.begin(), copy.end()); }
template<typename T> void atVector<T>::assign(std::initializer_list<T> list) { assign(list.begin(), list.end()) }
