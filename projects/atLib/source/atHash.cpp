#include "atHash.h"

template <> int64_t atHash<int64_t>(const int64_t &val) { return val; }
template <> int64_t atHash<int32_t>(const int32_t &val) { return val; }
template <> int64_t atHash<int16_t>(const int16_t &val) { return val; }
template <> int64_t atHash<int8_t>(const int8_t &val) { return val; }
template <> int64_t atHash<double>(const double &val) { atAssert(false, "atHash is not defined for T"); return 0; }
template <> int64_t atHash<float>(const float &val) { atAssert(false, "atHash is not defined for T"); return 0; }