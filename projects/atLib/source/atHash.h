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

  template <typename T> static int64_t Hash(const T &o);

protected:
  static thread_local atMemoryWriter writer;
};

#include "atHash.inl"
#endif // atHash_h__s
