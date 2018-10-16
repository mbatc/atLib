
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

template <typename T> atStringBasic<T>::atStringBasic(atVector<T> &&move)
{
  m_data.swap(move);
  validate();
}

template<typename T> atStringBasic<T> atStringBasic<T>::_replace(const T *str, const int64_t len, const T _char, const T with, const int64_t start, int64_t count)
{
  atString ret = str;
  int64_t pos = start;
  while ((pos = ret.find(_char, pos)) >= 0 && count != 0)
  {
    ret[pos++] = with;
    --count;
  }
  return ret;
}

template<typename T> atStringBasic<T> atStringBasic<T>::_replace(const T *str, const int64_t len, const T *find, const T *with, const int64_t start, int64_t count)
{
  if (_find(str, len, find, start) < 0)
    return str;

  atStringBasic<T> ret;
  ret.vector().reserve(ret.length());
  ret.vector().push_back('\0');
  ret.vector().insert(0, str, str + start);

  const int64_t matchLen = strlen(find);
  int64_t pos = start;
  int64_t found = 0;
  while ((found = _find(str, len, find, pos)) >= 0 && count != 0)
  {
    ret.vector().insert(pos, str + pos, str + found);
    ret.append(with);
    pos = found + matchLen;
    --count;
  }
  ret.append(str + pos);
  return ret;
}

template <typename T> void atStringBasic<T>::append(const T* str)
{
  if (!str)
    return;
  m_data.insert(length(), str, str + strlen(str));
}

template <typename T> atStringBasic<T> atStringBasic<T>::substr(const int64_t start, const int64_t end) const
{
  atVector<T> subData;
  subData.assign(m_data.begin() + atMax(0, atMin(start, length())), end < 0 ? m_data.end() : (m_data.begin() + end));
  subData.push_back(0);
  return std::move(subData);
}

template <typename T> int64_t atStringBasic<T>::_find(const T *str, const int64_t len, const T _char, int64_t start, int64_t end)
{
  if (len == 0)
    return -1;
  start = atMin(len, atMax(start, 0));
  end = atMin(end, len);
  if (start >= end)
    return -1;
  int64_t found = atMax(strchr(str + start, _char) - str, -1);
  return found > end ? -1 : found;
}

template <typename T> int64_t atStringBasic<T>::_find(const T *str, const int64_t len, const T *find, int64_t start, int64_t end)
{
  if (len == 0)
    return -1;
  start = atMin(len, atMax(start, 0));
  end = atMin(end, len);
  if (start >= end)
    return -1;
  int64_t found = atMax(strstr(str + start, find) - str, -1);
  return found > end ? -1 : found;
}

template<typename T> int64_t atStringBasic<T>::_find_end(const T *str, const int64_t len, const T *find, int64_t start, int64_t end)
{
  int64_t found = _find(str, len, find, start, end);
  return found >= 0 ? found + strlen(find) : found;
}

template <typename T> int64_t atStringBasic<T>::_find_reverse(const T *str, const int64_t len, const T _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len - 1);
  for (int64_t i = end; i >= start; --i)
    if (str[i] == _char)
      return i;
  return AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_reverse(const T *str, const int64_t len, const T* find, int64_t start, int64_t end)
{
  int64_t ssLen = 0;
  if (!find || (ssLen = strlen(find)) == 0)
    return AT_INVALID_INDEX;

  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  for (int64_t i = end - ssLen; i >= start; --i)
    if (str[i] == find[0] && str[i + ssLen - 1] == find[ssLen - 1])
    {
      bool isMatch = true;
      for (int64_t j = 1; isMatch && j < ssLen - 1; ++j)
        if (str[i + j] != find[j])
          isMatch = false;
      if (isMatch)
        return i;
    }
  return AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_first_not(const T *str, const int64_t len, const T _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  for (int64_t i = start; i < end; ++i)
    if (str[i] != _char)
      return i;
  return end == len ? end : AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_first_not(const T *str, const int64_t len, const T* find, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(find);
  bool found = false;
  for (int64_t i = start; i < end; ++i, found = false)
    if (memchr(set, str[i], nChecks) == nullptr)
      return i;
  return end == len ? end : AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_last_not(const T *str, const int64_t len, const T _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len - 1);

  for (int64_t i = end; i >= start; --i)
    if (str[i] != _char)
      return i;
  return AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_last_not(const T *str, const int64_t len, const T* find, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(find);
  bool found = false;
  for (int64_t i = end - 1; i >= start; --i, found = false)
    if (memchr(set, str[i], nChecks) == nullptr)
      return i;
  return AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_first_of(const T *str, const int64_t len, const T _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);
  T *pFound = memchr(str + start, _char, end);
  return pFound ? pFound - str : AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_first_of(const T *str, const int64_t len, const T *set, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);
  int64_t nChecks = strlen(set);
  for (int64_t i = start; i < end; ++i)
    if (memchr(set, str[i], nChecks) != nullptr)
      return i;
  return AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_last_of(const T *str, const int64_t len, const T _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);
  T* pFound = strrchr(str, _char);
  return pFound ? pFound - str : AT_INVALID_INDEX;
}

template <typename T> int64_t atStringBasic<T>::_find_last_of(const T *str, const int64_t len, const T* find, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(find);
  for (int64_t i = end - 1; i >= start; --i)
    if (memchr(set, str[i], nChecks) != nullptr)
      return i;
  return AT_INVALID_INDEX;
}

template <typename T> void atStringBasic<T>::set_string(const T* str, const int64_t len)
{
  m_data.clear();
  m_data.insert(0, str, str + len);
  validate();
}

template <typename T> void atStringBasic<T>::validate()
{
  if (m_data.size() == 0 || m_data.back() != '\0')
    m_data.push_back(0); // make sure there is a null terminating character
}

template<typename T> atVector<atStringBasic<T>> atStringBasic<T>::_split(const T *src, const int64_t len, const T &_char)
{
  atVector<atString> ret;
  atString atStr;
  int64_t start = 0;
  int64_t end = _find(src, len, _char);
  while (end != -1)
  {
    atStr.set_string(src + start, end - start);
    start = end + 1;
    end = _find(src, len, _char, start);

    if(atStr.length() > 0)
      ret.push_back(atStr);
  }
  ret.emplace_back(src + start, src + len);
  return ret;
}

template<typename T> atVector<atStringBasic<T>> atStringBasic<T>::_split(const T *src, const int64_t len, const T *split,const bool isSet)
{
  atVector<atString> ret;
  atString atStr;
  int64_t start = 0;
  int64_t (*find_func)(const T *, const int64_t, const T*, int64_t, int64_t);
  if (isSet)
    find_func = _find_first_of;
  else
    find_func = _find;
  int64_t end = find_func(src, len, split, 0, INT64_MAX);
  int64_t setLen = isSet ? 1 : strlen(split);
  while (end != -1)
  {
    atStr.set_string(src + start, end - start);
    start = end + setLen;
    end = find_func(src, len, split, start, INT64_MAX);
    if (atStr.length() > 0)
      ret.push_back(atStr);
  }
  if(start < len)
    ret.emplace_back(src + start, src + len);
  return ret;
}

template <typename T> template <class T1> atStringBasic<T>::atStringBasic(const T1 *pStart, const T1 *pEnd) { m_data.assign(pStart, pEnd); validate(); }
template <typename T> template <class T1> atStringBasic<T>::atStringBasic(const atStringBasic<T1> &str) { m_data.assign(str.begin(), str.end()); validate(); }
template <typename T> atStringBasic<T>::atStringBasic() { set_string("", 0); }
template <typename T> atStringBasic<T>::atStringBasic(const T *str) { set_string(str, strlen(str)); }
template <typename T> atStringBasic<T>::atStringBasic(const atStringBasic &copy) { set_string(copy.m_data); }
template <typename T> atStringBasic<T>::atStringBasic(atStringBasic &&move) { m_data.swap(move.m_data); }
template <typename T> atStringBasic<T>::atStringBasic(const atVector<T> &str) { set_string(str.data(), str.size()); }
template <typename T> bool atStringBasic<T>::compare(const T *str) const { return !strcmp(str, c_str()); }
template <typename T> void atStringBasic<T>::append(const T _char) { m_data.insert(length(), _char); }
template <typename T> atStringBasic<T> atStringBasic<T>::replace(const T _char, const T with, const int64_t start, int64_t count) const { return _replace(c_str(), length(), _char, with, start, count); }
template <typename T> atStringBasic<T> atStringBasic<T>::replace(const T* str, const T* with, const int64_t start, int64_t count) const { return _replace(c_str(), length(), str, with, start, count); }
template <typename T> int64_t atStringBasic<T>::_find_first(const T *str, const int64_t len, const T _char) { return _find(str, len, _char, 0); }
template <typename T> int64_t atStringBasic<T>::_find_first(const T *str, const int64_t len, const T* find) { return _find(str, len, find, 0); }
template <typename T> int64_t atStringBasic<T>::_find_last(const T *str, const int64_t len, const T _char) { return _find_reverse(str, len, _char, len - 1); }
template <typename T> int64_t atStringBasic<T>::_find_last(const T *str, const int64_t len, const T* find) { return _find_reverse(str, len, find, len - 1); }
template <typename T> int64_t atStringBasic<T>::find(const T _char, int64_t start, int64_t end) const { return _find(m_data.data(), length(), _char, start, end); }
template <typename T> int64_t atStringBasic<T>::find(const T* str, int64_t start, int64_t end) const { return _find(m_data.data(), length(), str, start, end); }
template <typename T> int64_t atStringBasic<T>::find_end(const T *str, int64_t start, int64_t end) { return _find_end(m_data.data(), length(), str, start, end); }
template <typename T> int64_t atStringBasic<T>::find_reverse(const T _char, int64_t start, int64_t end) const { return _find_reverse(m_data.data(), length(), _char, start, end); }
template <typename T> int64_t atStringBasic<T>::find_reverse(const T* str, int64_t start, int64_t end) const { return _find_reverse(m_data.data(), length(), str, start, end); }
template <typename T> int64_t atStringBasic<T>::find_first_not(const T _char, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), _char, start, end); }
template <typename T> int64_t atStringBasic<T>::find_first_not(const T* str, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), str, start, end); }
template <typename T> int64_t atStringBasic<T>::find_last_not(const T _char, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), _char, start, end); }
template <typename T> int64_t atStringBasic<T>::find_last_not(const T* str, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), str, start, end); }
template <typename T> int64_t atStringBasic<T>::find_first_of(const T _char, int64_t start, int64_t end) const { return _find_first_of(m_data.data(), length(), _char, start, end); }
template <typename T> int64_t atStringBasic<T>::find_first_of(const T *set, int64_t start, int64_t end) const { return _find_first_of(m_data.data(), length(), set, start, end); }
template <typename T> int64_t atStringBasic<T>::find_last_of(const T _char, int64_t start, int64_t end) const { return _find_last_of(m_data.data(), length(), _char, start, end); }
template <typename T> int64_t atStringBasic<T>::find_last_of(const T* str, int64_t start, int64_t end) const { return _find_last_of(m_data.data(), length(), str, start, end); }
template <typename T> int64_t atStringBasic<T>::find_first(const T _char) const { return find(_char, 0); }
template <typename T> int64_t atStringBasic<T>::find_first(const T* str) const { return find(str, 0); }
template <typename T> int64_t atStringBasic<T>::find_last(const T _char) const { return find_reverse(_char); }
template <typename T> int64_t atStringBasic<T>::find_last(const T* str) const { return find_reverse(str); }
template <typename T> atVector<atStringBasic<T>> atStringBasic<T>::split(const T & _char) { return _split(m_data.data(), length(), _char); }
template <typename T> atVector<atStringBasic<T>> atStringBasic<T>::split(const T * split, bool isSet) { return _split(m_data.data(), length(), split, isSet); }
template <typename T> const T* atStringBasic<T>::c_str() const { return m_data.data(); }
template <typename T> int64_t atStringBasic<T>::capacity()  const { return m_data.capacity(); }
template <typename T> int64_t atStringBasic<T>::length() const { return m_data.size() - 1; }
template <typename T> atVector<T> &atStringBasic<T>::vector() { return m_data; }
template <typename T> const atVector<T> &atStringBasic<T>::vector() const { return m_data; }
template <typename T> void atStringBasic<T>::set_string(atVector<T> data) { set_string(data.data(), data.size()); }
template <typename T> T& atStringBasic<T>::operator[](int64_t index) { return m_data.at(index); }
template <typename T> const T& atStringBasic<T>::operator[](int64_t index) const { return m_data.at(index); }
template <typename T> atStringBasic<T>::operator const T* () const { return c_str(); }
template <typename T> bool atStringBasic<T>::operator==(const T *rhs) const { return compare(rhs); }
template <typename T> bool atStringBasic<T>::operator!=(const T *rhs) const { return !compare(rhs); }
template <typename T> atStringBasic<T> atStringBasic<T>::operator=(const atStringBasic<T> &str) { return *this = str.c_str(); }
template <typename T> atStringBasic<T> atStringBasic<T>::operator=(const T *rhs) { set_string(rhs, strlen(rhs)); return *this; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator=(const T rhs) { set_string({ rhs, '\0' }, 1); return *this; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+=(const atStringBasic<T> &rhs) { append(rhs);  return *this; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+=(const T *rhs) { append(rhs); return *this; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+=(const T rhs) { append(rhs); return *this; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+=(const atVector<T> &rhs) { ret.append(atString(rhs).c_str()); return *this; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+(const T *rhs) const { atStringBasic<T> ret(*this); return ret += rhs; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+(const T rhs) const { atStringBasic<T> ret(*this); return ret += rhs; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+(const atVector<T> &rhs) const { atStringBasic<T> ret(*this); return ret += rhs; }
template <typename T> atStringBasic<T> atStringBasic<T>::operator+(const atStringBasic<T> &str) const { atStringBasic<T> ret(*this); return ret += str; }
template <typename T> typename atStringBasic<T>::iterator atStringBasic<T>::begin() { return m_data.begin(); }
template <typename T> typename atStringBasic<T>::iterator atStringBasic<T>::end() { return m_data.end() - 1; }
template <typename T> typename atStringBasic<T>::const_iterator atStringBasic<T>::begin() const { return m_data.begin(); }
template <typename T> typename atStringBasic<T>::const_iterator atStringBasic<T>::end() const { return m_data.end() - 1; }