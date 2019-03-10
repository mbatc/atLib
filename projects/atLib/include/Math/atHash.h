
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

#ifndef atHash_h__
#define atHash_h__

#include "atMemoryWriter.h"


class atHash
{
public:
  static int64_t Hash(const atMemoryWriter &mem);
  static int64_t Hash(const int64_t val);
  static int64_t Hash(const int32_t val);
  static int64_t Hash(const int16_t val);
  static int64_t Hash(const int8_t val);
  static int64_t Hash(const uint64_t val);
  static int64_t Hash(const uint32_t val);
  static int64_t Hash(const uint16_t val);
  static int64_t Hash(const uint8_t val);
  static int64_t Hash(const double val);
  static int64_t Hash(const float val);

  template<typename T> static int64_t Hash(const T &o);
  template<typename T> static int64_t Hash(const T *o);

protected:
  static thread_local atMemoryWriter writer;
};

#include "atHash.inl"
#endif // atHash_h__s
