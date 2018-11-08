#ifndef atLimits_h__
#define atLimits_h__

#include "atTypes.h"

template <typename T> T atLimitsMax() { return{ 0 }; }

template <> double atLimitsMax<double>();
template <> float atLimitsMax<float>();
template <> int8_t atLimitsMax<int8_t>();
template <> int16_t atLimitsMax<int16_t>();
template <> int32_t atLimitsMax<int32_t>();
template <> int64_t atLimitsMax<int64_t>();
template <> uint8_t atLimitsMax<uint8_t>();
template <> uint16_t atLimitsMax<uint16_t>();
template <> uint32_t atLimitsMax<uint32_t>();
template <> uint64_t atLimitsMax<uint64_t>();

template <typename T> T atLimitsMin() { return{ 0 }; }

template <> double atLimitsMin<double>();
template <> float atLimitsMin<float>();
template <> int8_t atLimitsMin<int8_t>();
template <> int16_t atLimitsMin<int16_t>();
template <> int32_t atLimitsMin<int32_t>();
template <> int64_t atLimitsMin<int64_t>();
template <> uint8_t atLimitsMin<uint8_t>();
template <> uint16_t atLimitsMin<uint16_t>();
template <> uint32_t atLimitsMin<uint32_t>();
template <> uint64_t atLimitsMin<uint64_t>();

#endif // atLimits_h__
