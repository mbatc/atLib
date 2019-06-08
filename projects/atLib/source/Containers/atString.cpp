
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

#include "atString.h"
#include <string.h>
#include <codecvt>

char _ToLower(const char c) { return c >= 65 && c <= 90 ? c + 32 : c; }
char _ToUpper(const char c) { return c >= 97 && c <= 122 ? c - 32 : c; }

atString::atString(atVector<char> &&move)
{
  m_data.swap(move);
  validate();
}

atString::atString(char *pStart, char *pEnd) : atString((const char*)pStart, (const char*)pEnd) {}

atString::atString(const char *pStart, const char *pEnd)
{
  m_data.assign(pStart, pEnd);
  validate();
}

atString atString::_to_lower(const char *str, const int64_t len)
{
  atVector<char> data;
  data.reserve(len);
  for (const char c : atIterate(str, len))
    data.push_back(_ToLower(c));
  return atString(data);
}

atString atString::_to_upper(const char *str, const int64_t len)
{
  atVector<char> data;
  data.reserve(len);
  for (const char c : atIterate(str, len))
    data.push_back(_ToUpper(c));
  return atString(data);
}

atString atString::_replace(const char *str, const int64_t len, const char _char, const char with, const int64_t start, int64_t count)
{
  atUnused(len);
  atString ret = str;
  int64_t pos = start;
  while ((pos = ret.find(_char, pos)) >= 0 && count != 0)
  {
    ret[pos++] = with;
    --count;
  }
  return ret;
}

atString atString::_replace(const char *str, const int64_t len, const char *find, const char *with, const int64_t start, int64_t count)
{
  if (_find(str, len, find, start) < 0)
    return str;

  atString ret;
  ret.vector().reserve(len);;
  ret.vector().insert(0, str, str + start);

  const int64_t matchLen = strlen(find);
  int64_t pos = start;
  int64_t found = 0;
  while ((found = _find(str, len, find, pos)) >= 0 && count != 0)
  {
    ret.vector().insert(ret.vector().size() - 1, str + pos, str + found);
    ret.append(with);
    pos = found + matchLen;
    --count;
  }
  ret.append(str + pos);
  return ret;
}

void atString::append(const char *str)
{
  if (!str)
    return;
  m_data.insert(length(), str, str + strlen(str));
}

atString atString::substr(const int64_t start, const int64_t end) const
{
  atVector<char> subData;
  subData.assign(m_data.begin() + atMax(0, atMin(start, length())), end < 0 ? m_data.end() : (m_data.begin() + end));
  subData.push_back(0);
  return std::move(subData);
}

int64_t atString::_find(const char *str, const int64_t len, const char _char, int64_t start, int64_t end)
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

int64_t atString::_find(const char *str, const int64_t len, const char *find, int64_t start, int64_t end)
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

int64_t atString::_find_end(const char *str, const int64_t len, const char *find, int64_t start, int64_t end)
{
  int64_t found = _find(str, len, find, start, end);
  return found >= 0 ? found + strlen(find) : found;
}

int64_t atString::_find_reverse(const char *str, const int64_t len, const char _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len - 1);
  for (int64_t i = end; i >= start; --i)
    if (str[i] == _char)
      return i;
  return AT_INVALID_INDEX;
}

int64_t atString::_find_reverse(const char *str, const int64_t len, const char *find, int64_t start, int64_t end)
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

int64_t atString::_find_first_not(const char *str, const int64_t len, const char _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  for (int64_t i = start; i < end; ++i)
    if (str[i] != _char)
      return i;
  return end == len ? end : AT_INVALID_INDEX;
}

int64_t atString::_find_first_not(const char *str, const int64_t len, const char *find, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(find);
  bool found = false;
  for (int64_t i = start; i < end; ++i, found = false)
  {
    for (int64_t j = 0; j < nChecks && !found; ++j)
      found |= str[i] == find[j];
    if (!found)
      return i;
  }
  return end == len ? end : AT_INVALID_INDEX;
}

int64_t atString::_find_last_not(const char *str, const int64_t len, const char _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len - 1);

  for (int64_t i = end; i >= start; --i)
    if (str[i] != _char)
      return i;
  return AT_INVALID_INDEX;
}

int64_t atString::_find_last_not(const char *str, const int64_t len, const char *find, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(find);
  bool found = false;
  for (int64_t i = end - 1; i >= start; --i, found = false)
  {
    for (int64_t j = 0; j < nChecks && !found; ++j)
      found |= str[i] == find[j];
    if (!found)
      return i;
  }
  return AT_INVALID_INDEX;
}

int64_t atString::_find_first_of(const char *str, const int64_t len, const char _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  for (int64_t i = start; i < end; ++i)
    if (str[i] == _char)
      return i;
  return AT_INVALID_INDEX;
}

int64_t atString::_find_first_of(const char *str, const int64_t len, const char *set, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(set);
  for (int64_t i = start; i < end; ++i)
    for (int64_t j = 0; j < nChecks; ++j)
      if (str[i] == set[j])
        return i;
  return AT_INVALID_INDEX;
}

int64_t atString::_find_last_of(const char *str, const int64_t len, const char _char, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  for (int64_t i = end - 1; i >= start; --i)
    if (str[i] == _char)
      return i;
  return AT_INVALID_INDEX;
}

int64_t atString::_find_last_of(const char *str, const int64_t len, const char *find, int64_t start, int64_t end)
{
  start = atMin(atMax(start, 0), len);
  end = atMin(end, len);

  int64_t nChecks = strlen(find);
  for (int64_t i = end - 1; i >= start; --i)
    for (int64_t j = 0; j < nChecks; ++j)
      if (str[i] == find[j])
        return i;
  return AT_INVALID_INDEX;
}

void atString::set_string(const char *str, const int64_t len)
{
  m_data.clear();
  m_data.insert(0, str, str + len);
  validate();
}

void atString::validate()
{
  if (m_data.size() == 0 || m_data.back() != '\0')
    m_data.push_back(0); // make sure there is a null terminating character
}

atVector<atString> atString::_split(const char *src, const int64_t len, const char &_char, const bool dropEmpty)
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

    if (!dropEmpty || atStr.length() > 0)
      ret.push_back(atStr);
  }
  ret.emplace_back(src + start, src + len);
  return ret;
}

atVector<atString> atString::_split(const char *src, const int64_t len, const char *split, const bool isSet, const bool dropEmpty)
{
  atVector<atString> ret;
  atString atStr;
  int64_t start = 0;
  int64_t(*find_func)(const char *, const int64_t, const char*, int64_t, int64_t);
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
    if (!dropEmpty || atStr.length() > 0)
      ret.push_back(atStr);
  }
  if (start < len)
    ret.emplace_back(src + start, src + len);
  return ret;
}

bool atString::compare(const char *lhs, const char *rhs, const atStringCompareOptions options)
{
  if (options == atSCO_MatchCase)
    return !strcmp(lhs, rhs);
  int64_t len = strlen(lhs);
  int64_t len2 = strlen(rhs);
  if (len != len2) return false;

  char cUpper = 0;
  char cLower = 0;

  for (int64_t i = 0; i < len; ++i)
  {
    char c = rhs[i];
    cUpper = _ToUpper(c);
    cLower = _ToLower(c);
    if (lhs[i] != cUpper && lhs[i] != cLower)
      return false;
  }
  return true;
}

template<> atTypeDesc atGetTypeDesc(const atString &str)
{
  atTypeDesc ret = atGetTypeDesc<char>();
  ret.count = str.length() + 1;
  ret.size = atSize(ret.type) * ret.count;
  return ret;
}

atString atString::join(const atVector<atString> &strings, const atString &separator, const bool ignoreEmpty)
{
  atString ret;
  for (const atString &str : strings)
    if (!ignoreEmpty || str.length() > 0)
    {
      ret += str;
      if (&str - &strings.back())
        ret += separator;
    }
  return ret;
}

atString operator+(const char &_char, const atString &rhs) { return atString(_char).operator+(rhs); }
atString operator+(const char *lhs, const atString &rhs) { return atString(lhs).operator+(rhs); }
atString operator+(char _char, const atString &rhs) { return atString(_char).operator+(rhs); }
atString operator+(char *lhs, const atString &rhs) { return atString(lhs).operator+(rhs); }

bool atString::compare(const char *str, const atStringCompareOptions options) const { return compare(c_str(), str, options); }
atString atString::to_lower() const { return _to_lower(c_str(), length()); }
atString atString::to_upper() const { return _to_upper(c_str(), length()); }
atString::atString() { set_string("", 0); }
atString::atString(char c) { m_data.assign(c, 1); validate(); }
atString::atString(char *str) { set_string(str, strlen(str)); }
atString::atString(const char *str) : atString((char*)str) {}
atString::atString(const atString &copy) { set_string(copy.m_data); }
atString::atString(atString &&move) { m_data.swap(move.m_data); }
atString::atString(const atVector<char> &str) { set_string(str.data(), str.size()); }
void atString::append(const char _char) { m_data.insert(length(), &_char, &_char + 1); }
atString atString::replace(const char _char, const char with, const int64_t start, int64_t count) const { return _replace(c_str(), length(), _char, with, start, count); }
atString atString::replace(const char *str, const char *with, const int64_t start, int64_t count) const { return _replace(c_str(), length(), str, with, start, count); }
int64_t atString::_find_first(const char *str, const int64_t len, const char _char) { return _find(str, len, _char, 0); }
int64_t atString::_find_first(const char *str, const int64_t len, const char *find) { return _find(str, len, find, 0); }
int64_t atString::_find_last(const char *str, const int64_t len, const char _char) { return _find_reverse(str, len, _char, len - 1); }
int64_t atString::_find_last(const char *str, const int64_t len, const char *find) { return _find_reverse(str, len, find, len - 1); }
int64_t atString::find(const char _char, int64_t start, int64_t end) const { return _find(m_data.data(), length(), _char, start, end); }
int64_t atString::find(const char *str, int64_t start, int64_t end) const { return _find(m_data.data(), length(), str, start, end); }
int64_t atString::find_end(const char *str, int64_t start, int64_t end) { return _find_end(m_data.data(), length(), str, start, end); }
int64_t atString::find_reverse(const char _char, int64_t start, int64_t end) const { return _find_reverse(m_data.data(), length(), _char, start, end); }
int64_t atString::find_reverse(const char *str, int64_t start, int64_t end) const { return _find_reverse(m_data.data(), length(), str, start, end); }
int64_t atString::find_first_not(const char _char, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), _char, start, end); }
int64_t atString::find_first_not(const char *str, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), str, start, end); }
int64_t atString::find_last_not(const char _char, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), _char, start, end); }
int64_t atString::find_last_not(const char *str, int64_t start, int64_t end) const { return _find_first_not(m_data.data(), length(), str, start, end); }
int64_t atString::find_first_of(const char _char, int64_t start, int64_t end) const { return _find_first_of(m_data.data(), length(), _char, start, end); }
int64_t atString::find_first_of(const char *set, int64_t start, int64_t end) const { return _find_first_of(m_data.data(), length(), set, start, end); }
int64_t atString::find_last_of(const char _char, int64_t start, int64_t end) const { return _find_last_of(m_data.data(), length(), _char, start, end); }
int64_t atString::find_last_of(const char *str, int64_t start, int64_t end) const { return _find_last_of(m_data.data(), length(), str, start, end); }
int64_t atString::find_first(const char _char) const { return find(_char, 0); }
int64_t atString::find_first(const char *str) const { return find(str, 0); }
int64_t atString::find_last(const char _char) const { return find_reverse(_char); }
int64_t atString::find_last(const char *str) const { return find_reverse(str); }
atVector<atString> atString::split(const char & _char, const bool dropEmpty) const { return _split(m_data.data(), length(), _char, dropEmpty); }
atVector<atString> atString::split(const char * split, bool isSet, const bool dropEmpty) const { return _split(m_data.data(), length(), split, isSet, dropEmpty); }
const char *atString::c_str() const { return m_data.data(); }
int64_t atString::capacity()  const { return m_data.capacity(); }
int64_t atString::length() const { return m_data.size() - 1; }
atVector<char> &atString::vector() { return m_data; }
const atVector<char> &atString::vector() const { return m_data; }
void atString::set_string(atVector<char> data) { set_string(data.data(), data.size()); }
char& atString::operator[](int64_t index) { return m_data.at(index); }
const char& atString::operator[](int64_t index) const { return m_data.at(index); }
atString::operator const char* () const { return c_str(); }
bool atString::operator==(const char *rhs) const { return compare(rhs); }
bool atString::operator!=(const char *rhs) const { return !compare(rhs); }
atString atString::operator=(const atString &str) { return *this = str.c_str(); }
atString atString::operator=(const char *rhs) { set_string(rhs, strlen(rhs)); return *this; }
atString atString::operator=(const char rhs) { set_string({ rhs, '\0' }); return *this; }
atString atString::operator+=(const atString &rhs) { append(rhs);  return *this; }
atString atString::operator+=(const char *rhs) { append(rhs); return *this; }
atString atString::operator+=(const char rhs) { append(rhs); return *this; }
atString atString::operator+=(const atVector<char> &rhs) { append(atString(rhs).c_str()); return *this; }
atString atString::operator+(const char *rhs) const { atString ret(*this); return ret += rhs; }
atString atString::operator+(const char rhs) const { atString ret(*this); return ret += rhs; }
atString atString::operator+(const atVector<char> &rhs) const { atString ret(*this); return ret += rhs; }
atString atString::operator+(const atString &str) const { atString ret(*this); return ret += str; }
typename atString::iterator atString::begin() { return m_data.begin(); }
typename atString::iterator atString::end() { return m_data.end() - 1; }
typename atString::const_iterator atString::begin() const { return m_data.begin(); }
typename atString::const_iterator atString::end() const { return m_data.end() - 1; }
const char* atString::Numerals() { return "0123456789"; }
const char* atString::Whitespace() { return " \n\t\r"; }
const char* atString::Hex() { return "0x0123456789ABCDEF"; }
const char* atString::Binary() { return "01"; }
const char* atString::AlphabetUpper() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
const char* atString::AlphabetLower() { return "abcdefghijklmnopqrstuvwxyz"; }
const char* atString::AlphabetAll() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; }

int64_t atStreamRead(atReadStream *pStream, atString *pData, const int64_t count)
{
  atVector<char> bytes;
  int64_t ret = 0;
  for (atString &str : atIterate(pData, count))
  {
    ret += atStreamRead(pStream, &bytes, 1);
    str.set_string(bytes);
  }
  return ret;
}

int64_t atStreamWrite(atWriteStream *pStream, const atString *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const atString &str : atIterate(pData, count))
    ret += atStreamWrite(pStream, &str.vector(), 1);
  return ret;
}