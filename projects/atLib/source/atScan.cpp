
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

#include "atScan.h"

static const char *s_whitespace = " \n\r\t";

static int64_t _scan_integer(const char *str, int64_t *pLen, const char *candidates)
{
  const int64_t len = strlen(str);
  const int64_t nextChar = atString::_find_first_not(str, len, atString::Whitespace());
  const int64_t nCandidates = strlen(candidates);
  const int64_t lastNum = atString::_find_first_not(str, len, candidates);
  const int64_t intlen = lastNum - nextChar;
  const int64_t baseChar = candidates[0];
  int64_t mag = (int64_t)pow(nCandidates, intlen - 1);
  int64_t res = 0;
  for (int64_t c = nextChar; c < lastNum; ++c, mag /= nCandidates)
    res += (str[c] - baseChar) * mag;

  if (pLen && intlen > 0)
    *pLen = intlen;

  return res;
}

int64_t atScan::Int(const char *str, int64_t *pLen)
{
  return _scan_integer(str, pLen, atString::Numerals());

  // const int64_t len = strlen(str);
  // const int64_t nextChar = atString::_find_first_not(str, len, atString::Whitespace());
  // const int64_t lastNum = atString::_find_first_not(str, len, atString::Numerals());
  // const int64_t intlen = lastNum - nextChar;
  // 
  // int64_t mag = (int64_t)pow(10, intlen - 1);
  // int64_t res = 0;
  // for (int64_t c = nextChar; c < lastNum; ++c, mag /= 10)
  //   res += (str[c] - 48) * mag;
  // 
  // if (pLen && intlen >0)
  //   *pLen = intlen;
  // 
  // return res;
}

int64_t atScan::Hex(const char * str, int64_t * pLen) { return _scan_integer(str, pLen, atString::Hex()); }

atString atScan::String(const char *str, int64_t *pLen)
{
  int64_t len = strlen(str);
  int64_t start = atString::_find_first_not(str, len, s_whitespace);
  int64_t end = atString::_find_first_of(str, len, s_whitespace, start);
  if (start < 0)
    return "";
  atString ret(str + start, end < 0 ? str + len : (str + end));
  if (pLen)
    *pLen = end;
  return ret;
}

double atScan::Float(const char *str, int64_t *pLen)
{
  const int64_t len = strlen(str);
  const int64_t nextChar = atString::_find_first_not(str, len, atString::Whitespace());
  const int64_t firstNum = atString::_find_first_of(str, len, "0123456789.", nextChar);
  const int64_t dot = atString::_find_first_of(str, len, ".", firstNum);
  const int64_t lastNum = atString::_find_first_not(str, len, "0123456789.", firstNum);
  const int64_t floatlen = lastNum - firstNum;

  if (firstNum != nextChar)
    return 0.0f;

  int64_t mag = (int64_t)pow(10, floatlen - (dot > firstNum && dot < lastNum ? 2 : 1));
  int64_t exponent = atMax(0, lastNum - dot - 1);

  uint64_t value = 0;
  for (int64_t c = firstNum; c < lastNum; ++c)
    if (str[c] != '.')
    {
      value += (uint64_t)((str[c] - 48) * mag); 
      mag /= 10;
    }

  if (pLen && floatlen > 0)
    *pLen = floatlen;

  return value * pow(10, -exponent);
}
