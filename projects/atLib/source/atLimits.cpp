#include "atLimits.h"
#include <float.h>

template <> double atLimitsMax<double>() { return DBL_MAX; }
template <> float atLimitsMax<float>() { return FLT_MAX; }
template <> int8_t atLimitsMax<int8_t>() { return UINT8_MAX; }
template <> int16_t atLimitsMax<int16_t>() {return INT16_MAX;}
template <> int32_t atLimitsMax<int32_t>() {return INT32_MAX;}
template <> int64_t atLimitsMax<int64_t>() {return INT64_MAX;}
template <> uint8_t atLimitsMax<uint8_t>() { return UINT8_MAX; }
template <> uint16_t atLimitsMax<uint16_t>() { return UINT16_MAX; }
template <> uint32_t atLimitsMax<uint32_t>() { return UINT32_MAX; }
template <> uint64_t atLimitsMax<uint64_t>() { return UINT64_MAX; }

template <> double atLimitsMin<double>() { return DBL_MIN; }
template <> float atLimitsMin<float>() { return FLT_MIN; }
template <> int8_t atLimitsMin<int8_t>() { return INT8_MIN; }
template <> int16_t atLimitsMin<int16_t>() { return INT16_MIN; }
template <> int32_t atLimitsMin<int32_t>() { return INT32_MIN; }
template <> int64_t atLimitsMin<int64_t>() { return INT64_MIN; }
template <> uint8_t atLimitsMin<uint8_t>() { return 0; }
template <> uint16_t atLimitsMin<uint16_t>() { return 0; }
template <> uint32_t atLimitsMin<uint32_t>() { return 0; }
template <> uint64_t atLimitsMin<uint64_t>() { return 0; }