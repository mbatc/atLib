
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

#ifndef _atString_h__
#define _atString_h__

#include "atVector.h"
#include "atMath.h"

enum atStringCompareOptions
{
  atSCO_None,
  atSCO_MatchCase,
};

template <typename T> class atStringBasic
{
public:
  typedef T* iterator;
  typedef const T* const_iterator;

  template <class T1> atStringBasic(const T1 *pStart, const T1 *pEnd);
  template <class T1> atStringBasic(const atStringBasic<T1> &str);

  atStringBasic();
  atStringBasic(const T *str);
  atStringBasic(const atStringBasic &copy);
  atStringBasic(atStringBasic &&move);
  atStringBasic(const atVector<T> &str);
  atStringBasic(atVector<T> &&move);
  
  static atStringBasic<T> _to_lower(const T *str, const int64_t len);
  static atStringBasic<T> _to_upper(const T *str, const int64_t len);

  atStringBasic<T> to_lower();
  atStringBasic<T> to_upper();

  //******************
  // Compare functions
  bool compare(const T *str, const atStringCompareOptions options = atSCO_MatchCase) const;
  static bool compare(const T *lhs, const T *rhs, const atStringCompareOptions options = atSCO_MatchCase);

  //******************
  // Replace functions

  static atStringBasic<T> _replace(const T *str, const int64_t len, const T _char, const T with, const int64_t start = 0, int64_t count = -1);
  static atStringBasic<T> _replace(const T *str, const int64_t len, const T* find, const T* with, const int64_t start = 0, int64_t count = -1);
  atStringBasic<T> replace(const T _char, const T with, const int64_t start = 0, int64_t count = -1) const;
  atStringBasic<T> replace(const T* str, const T* with, const int64_t start = 0, int64_t count = -1) const;
  void append(const T* str);
  void append(const T _char);

  atStringBasic<T> substr(const int64_t start, const int64_t end) const;

  //***************
  // Static Find functions
  // Returns the index of the char/substring
  // Returns -1 if not found 

  static int64_t _find(const T *str, const int64_t len, const T _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find(const T *str, const int64_t len, const T *find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_end(const T *str, const int64_t len, const T *find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_reverse(const T *str, const int64_t len, const T _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_reverse(const T *str, const int64_t len, const T* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_not(const T *str, const int64_t len, const T _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_not(const T *str, const int64_t len, const T* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_not(const T *str, const int64_t len, const T _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_not(const T *str, const int64_t len, const T* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_of(const T *str, const int64_t len, const T _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_of(const T *str, const int64_t len, const T *set, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_of(const T *str, const int64_t len, const T _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_of(const T *str, const int64_t len, const T* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first(const T *str, const int64_t len, const T _char);
  static int64_t _find_first(const T *str, const int64_t len, const T* find);
  static int64_t _find_last(const T *str, const int64_t len, const T _char);
  static int64_t _find_last(const T *str, const int64_t len, const T* find);

  //***************
  // Find functions
  // Returns the index of the char/substring
  // Returns -1 if not found 

  int64_t find(const T _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find(const T *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_end(const  T *str, int64_t start = 0, int64_t end = INT64_MAX);
  int64_t find_reverse(const T _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_reverse(const T *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_not(const T _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_not(const T *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_not(const T _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_not(const T *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_of(const T _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_of(const T *set, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_of(const T _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_of(const T *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first(const T _char) const;
  int64_t find_first(const T *str) const;
  int64_t find_last(const T _char) const;
  int64_t find_last(const T *str) const;

  static atVector<atStringBasic<T>> _split(const T *src, const int64_t len, const T &_char);
  static atVector<atStringBasic<T>> _split(const T *src, const int64_t len, const T *split, const bool isSet = false);
  atVector<atStringBasic<T>> split(const T &_char);
  atVector<atStringBasic<T>> split(const T *split, bool isSet = false);

  const T* c_str() const;
  int64_t capacity()  const;
  int64_t length() const;

  atVector<T> &vector();
  const atVector<T> &vector() const;

  void set_string(atVector<T> data);
  void set_string(const T* str, const int64_t len);

  T& operator[](int64_t index);
  const T& operator[](int64_t index) const;

  // implicit conversion to c-string
  operator const T* () const;

  bool operator==(const T *rhs) const;
  bool operator!=(const T *rhs) const;

  atStringBasic<T> operator=(const atStringBasic<T> &str);
  atStringBasic<T> operator=(const T *rhs);
  atStringBasic<T> operator=(const T rhs);
  atStringBasic<T> operator+=(const atStringBasic<T> &rhs);
  atStringBasic<T> operator+=(const T *rhs);
  atStringBasic<T> operator+=(const T rhs);
  atStringBasic<T> operator+=(const atVector<T> &rhs);
  atStringBasic<T> operator+(const T *rhs) const;
  atStringBasic<T> operator+(const T rhs) const;
  atStringBasic<T> operator+(const atVector<T> &rhs) const;
  atStringBasic<T> operator+(const atStringBasic<T> &str) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  static const T* Numerals();
  static const T* Whitespace();
  static const T* Hex();
  static const T* Binary();
  static const T* AlphabetUpper();
  static const T* AlphabetLower();
  static const T* AlphabetAll();

protected:
  void validate();

  atVector<T> m_data;
};

template <typename T> atStringBasic<T> operator+(const atStringBasic<T> &lhs, const atStringBasic<T> &rhs) { return lhs.operator+(rhs); }
template <typename T> atStringBasic<T> operator+(const T &_char, const atStringBasic<T> &rhs) { return atString(lhs).operator+(rhs); }
template <typename T> atStringBasic<T> operator+(const atStringBasic<T> &rhs, const T &_char) { return lhs.operator+(rhs); }
template <typename T> atStringBasic<T> operator+(const T *lhs, const atStringBasic<T> &rhs) { return atString(lhs).operator+(rhs); }
template <typename T> atStringBasic<T> operator+(const atStringBasic<T> &lhs, const T *rhs) { return lhs.operator+(rhs); }

template <> atTypeDesc atGetTypeDesc(const atStringBasic<char> &str);
template <> atTypeDesc atGetTypeDesc(const atStringBasic<wchar_t> &str);

typedef atStringBasic<wchar_t> atWideString;
typedef atStringBasic<char> atString;

atWideString ToWideString(const atString &str);
atString ToString(const atWideString &str);

template <typename T> _atStreamRead(atStringBasic<T>)
{ 
  atVector<T> bytes;
  int64_t ret = atStreamRead(pStream, &bytes);
  pData->set_string(bytes);
  return ret;
}

template <typename T> _atStreamWrite(atStringBasic<T>) { return atStreamWrite(pStream, data.vector()); }

#include "atString.inl"
#endif
