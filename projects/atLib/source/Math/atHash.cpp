
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

#include "atHash.h"

thread_local atMemoryWriter atHash::writer;

int64_t atHash::Hash(const atMemoryWriter &mem)
{
  // One-at-a-Time hash
  int64_t hash = 0;

  for (const uint8_t byte : mem.m_data)
  {
    hash += byte;
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash += (hash >> 11);
  hash += (hash << 15);
  return hash;
}

int64_t atHash::Hash(const int64_t val) { return val; }
int64_t atHash::Hash(const int32_t val) { return (int64_t)val; }
int64_t atHash::Hash(const int16_t val) { return (int64_t)val; }
int64_t atHash::Hash(const int8_t val) { return (int64_t)val; }
int64_t atHash::Hash(const uint64_t val) { return val; }
int64_t atHash::Hash(const uint32_t val) { return (int64_t)val; }
int64_t atHash::Hash(const uint16_t val) { return (int64_t)val; }
int64_t atHash::Hash(const uint8_t val) { return (int64_t)val; }
int64_t atHash::Hash(const double val) { return *(int64_t*)&val; }
int64_t atHash::Hash(const float val) { return *(int64_t*)&val; }
