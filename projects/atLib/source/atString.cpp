
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

atWideString atToWideString(const atString &str) { return atWideString(str); }
atString atToString(const atWideString &str) { return atString(str); }

atString atToString(const int64_t val)
{
  char buffer[45];
  sprintf(buffer, "%lld", val);
  return atString(buffer);
}

atString atToString(const int32_t val)
{
  char buffer[45];
  sprintf(buffer, "%d", val);
  return atString(buffer);
}

atString atToString(const double val)
{
  char buffer[45];
  sprintf(buffer, "%f", val);
  return atString(buffer);
}

atString atToString(const float val)
{
  char buffer[45];
  sprintf(buffer, "%f", val);
  return atString(buffer);
}

template <> atTypeDesc atGetTypeDesc(const atStringBasic<char> &str)
{
  atTypeDesc ret = atGetTypeDesc<char>();
  ret.count = str.length() + 1;
  ret.size = atSize(ret.type) * ret.count;
  return ret;
}

template <> atTypeDesc atGetTypeDesc(const atStringBasic<wchar_t> &str)
{
  atTypeDesc ret = atGetTypeDesc<wchar_t>();
  ret.count = str.length() + 1;
  ret.size = atSize(ret.type) * ret.count;
  return ret;
}

template <> const char* atStringBasic<char>::Numerals() { return "0123456789"; }
template <> const char* atStringBasic<char>::Whitespace() { return " \n\t\r"; }
template <> const char* atStringBasic<char>::Hex() { return "0x0123456789ABCDEF"; }
template <> const char* atStringBasic<char>::Binary() { return "01"; }
template <> const char* atStringBasic<char>::AlphabetUpper() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
template <> const char* atStringBasic<char>::AlphabetLower() { return "abcdefghijklmnopqrstuvwxyz"; }
template <> const char* atStringBasic<char>::AlphabetAll() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; }

template <> const wchar_t* atStringBasic<wchar_t>::Numerals() { return L"0123456789"; }
template <> const wchar_t* atStringBasic<wchar_t>::Whitespace() { return L" \n\t\r"; }
template <> const wchar_t* atStringBasic<wchar_t>::Hex() { return L"0x0123456789ABCDEF"; }
template <> const wchar_t* atStringBasic<wchar_t>::Binary() { return L"01"; }
template <> const wchar_t* atStringBasic<wchar_t>::AlphabetUpper() { return L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
template <> const wchar_t* atStringBasic<wchar_t>::AlphabetLower() { return L"abcdefghijklmnopqrstuvwxyz"; }
template <> const wchar_t* atStringBasic<wchar_t>::AlphabetAll() { return L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; }
