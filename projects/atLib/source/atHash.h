#ifndef atHash_h__
#define atHash_h__

#include "atAssert.h"

template <typename T> int64_t atHash(const T &val) { atAssert(false, "atHash is not defined for T"); return 0; }
template <> int64_t atHash<int64_t>(const int64_t &val);
template <> int64_t atHash<int32_t>(const int32_t &val);
template <> int64_t atHash<int16_t>(const int16_t &val);
template <> int64_t atHash<int8_t>(const int8_t &val);

template <> int64_t atHash<double>(const double &val);
template <> int64_t atHash<float>(const float &val);

#endif // atHash_h__
