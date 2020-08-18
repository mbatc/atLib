
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

#include "atHash.h"
#include "atIterator.h"

thread_local atMemoryWriter atHash::writer;

uint64_t atMurmur(const void *key, int64_t len, uint64_t seed = 2147483647)
{
  const uint64_t m = 0xc6a4a7935bd1e995LLU;
  const int r = 47;
  uint64_t hash = seed ^ (len * m);
  const uint64_t *data = (const uint64_t *)key;
  const uint64_t *end = (len >> 3) + data;
  while (data != end)
  {
    uint64_t k = *data++;
    k *= m;
    k ^= k >> r;
    k *= m;
    hash ^= k;
    hash *= m;
  }

  const unsigned char *data2 = (const unsigned char *)data;
  switch (len & 7)
  {
  case 7: hash ^= (uint64_t)(data2[6]) << 48;
  case 6: hash ^= (uint64_t)(data2[5]) << 40;
  case 5: hash ^= (uint64_t)(data2[4]) << 32;
  case 4: hash ^= (uint64_t)(data2[3]) << 24;
  case 3: hash ^= (uint64_t)(data2[2]) << 16;
  case 2: hash ^= (uint64_t)(data2[1]) << 8;
  case 1: hash ^= (uint64_t)(data2[0]);
    hash *= m;
  };
  hash ^= hash >> r;
  hash *= m;
  hash ^= hash >> r;
  return hash;
}

uint64_t atOneAtTime(const void *pData, int64_t len, uint64_t seed = 2147483647)
{
  const uint64_t m = 0xc6a4a7935bd1e995LLU;
  const int r = 47;
  uint64_t hash = seed ^ (len * m);
  for (const uint8_t byte : atIterate((uint8_t*)pData, len))
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

int64_t atHash::Hash(const atMemoryWriter &mem) { return (int64_t)(atMurmur(mem.m_data.data(), mem.m_data.size())); }
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
