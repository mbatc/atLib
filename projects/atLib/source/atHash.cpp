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
