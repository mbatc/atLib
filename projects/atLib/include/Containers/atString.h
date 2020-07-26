
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

#ifndef _atString_h__
#define _atString_h__

#include "atIterator.h"
#include "atVector.h"
#include "atMath.h"
#include <string>

enum atStringCompareOptions
{
  atSCO_None,
  atSCO_MatchCase,
};

class atString
{
public:
  typedef char* iterator;
  typedef const char* const_iterator;

  atString();
  atString(char c);
  atString(char *str);
  atString(const char *str);
  atString(const atString &copy);
  atString(atString &&move);
  atString(const atVector<char> &str);
  atString(atVector<char> &&move);
  atString(char *pStart, char *pEnd);
  atString(const char *pStart, const char *pEnd);

  template<typename T> explicit atString(const T &o);
  
  atString(const atVector<uint8_t> &str);
  explicit operator atVector<uint8_t>() const;
  explicit operator std::string() const;

  // implicit conversion to c-string
  operator const char* () const;

  // explicit conversion to types that define atFromString()
  template<typename T> explicit operator T() const;

  static atString _to_lower(const char *str);
  static atString _to_upper(const char *str);

  atString to_lower() const;
  atString to_upper() const;

  //******************
  // Compare functions
  bool compare(const char *str, const atStringCompareOptions options = atSCO_MatchCase) const;
  static bool compare(const char *lhs, const char *rhs, const atStringCompareOptions options = atSCO_MatchCase, const int64_t &compareLen = -1);

  //******************
  // Replace functions

  static atString _replace(const char *str, const char _char, const char with, const int64_t start = 0, int64_t count = -1);
  static atString _replace(const char *str, const char* find, const char* with, const int64_t start = 0, int64_t count = -1);
  atString replace(const char _char, const char with, const int64_t start = 0, int64_t count = -1) const;
  atString replace(const char* str, const char* with, const int64_t start = 0, int64_t count = -1) const;
  void append(const char* str);
  void append(const char _char);

  atString substr(const int64_t start, const int64_t end) const;

  //***************
  // Static Find functions
  // Returns the index of the char/substring
  // Returns -1 if not found 

  static int64_t _find(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find(const char *str, const char *find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_end(const char *str, const char *find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_reverse(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_reverse(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_not(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_not(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_not(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_not(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_of(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_of(const char *str, const char *set, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_of(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_of(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first(const char *str, const char _char);
  static int64_t _find_first(const char *str, const char* find);
  static int64_t _find_last(const char *str, const char _char);
  static int64_t _find_last(const char *str, const char* find);
  static bool _starts_with(const char *str, const char *find);
  static bool _starts_with(const char *str, const char _char);

  //***************
  // Find functions
  // Returns the index of the char/substring
  // Returns -1 if not found 

  int64_t find(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_end(const  char *str, int64_t start = 0, int64_t end = INT64_MAX);
  int64_t find_reverse(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_reverse(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_not(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_not(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_not(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_not(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_of(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_of(const char *set, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_of(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_of(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first(const char _char) const;
  int64_t find_first(const char *str) const;
  int64_t find_last(const char _char) const;
  int64_t find_last(const char *str) const;
  bool starts_with(const char *str) const;

  static atVector<atString> _split(const char *src, const char &_char, const bool dropEmpty = true);
  static atVector<atString> _split(const char *src, const char *split, const bool isSet = false, const bool dropEmpty = true);
  atVector<atString> split(const char &_char, const bool dropEmpty = true) const;
  atVector<atString> split(const char *split, bool isSet = false, const bool dropEmpty = true) const;
  
  static atString join(const atVector<atString> &strings, const atString &separator, const bool ignoreEmpty = true);

  const char* c_str() const;
  int64_t capacity()  const;
  int64_t length() const;

  atVector<char> &vector();
  const atVector<char> &vector() const;

  void set_string(const atVector<char> &data);
  void set_string(const char* str, const int64_t len);

  char& operator[](int64_t index);
  const char& operator[](int64_t index) const;

  bool operator==(const char *rhs) const;
  bool operator!=(const char *rhs) const;

  const atString& operator=(const atString &str);
  const atString& operator=(atString &&str);
  const atString& operator=(const char *rhs);
  const atString& operator=(const char rhs);
  atString operator+=(const atString &rhs);
  atString operator+=(const char *rhs);
  atString operator+=(const char rhs);
  atString operator+=(const atVector<char> &rhs);
  atString operator+(const char *rhs) const;
  atString operator+(const char rhs) const;
  atString operator+(const atVector<char> &rhs) const;
  atString operator+(const atString &str) const;

  template<typename T> inline atString operator+(const T &rhs) const;
  template<typename T> inline atString operator+=(const T &rhs);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  static const char* Numerals();
  static const char* Whitespace();
  static const char* Hex();
  static const char* Binary();
  static const char* AlphabetUpper();
  static const char* AlphabetLower();
  static const char* AlphabetAll();

protected:
  void validate();

  atVector<char> m_data;
};

atString operator+(const char _char, const atString &rhs);
atString operator+(const char *lhs, const atString &rhs);
atString operator+(char _char, const atString &rhs);
atString operator+(char *lhs, const atString &rhs);

template<> inline atTypeDesc atGetTypeDesc(const atString &str);
template<typename T> inline T atFromString(const atString &str)
{
  static_assert(!std::is_same<T, T>::value, "atFromString is not defined for this type.");
  return T();
}

template<typename T> atString atToString(const T &o)
{
  static_assert(!std::is_same<T, T>::value, "atToString is not defined for type T");
  return atString();
}

#include "atFromString.h"
#include "atToString.h"

int64_t atStreamRead(atReadStream *pStream, atString *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atString *pData, const int64_t count);

#include "atString.inl"
#endif
