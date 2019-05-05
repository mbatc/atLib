
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

#include "atLimits.h"
#include <float.h>

template<> double atLimitsMax<double>() { return DBL_MAX; }
template<> float atLimitsMax<float>() { return FLT_MAX; }
template<> int8_t atLimitsMax<int8_t>() { return UINT8_MAX; }
template<> int16_t atLimitsMax<int16_t>() {return INT16_MAX;}
template<> int32_t atLimitsMax<int32_t>() {return INT32_MAX;}
template<> int64_t atLimitsMax<int64_t>() {return INT64_MAX;}
template<> uint8_t atLimitsMax<uint8_t>() { return UINT8_MAX; }
template<> uint16_t atLimitsMax<uint16_t>() { return UINT16_MAX; }
template<> uint32_t atLimitsMax<uint32_t>() { return UINT32_MAX; }
template<> uint64_t atLimitsMax<uint64_t>() { return UINT64_MAX; }

template<> double atLimitsMin<double>() { return DBL_MIN; }
template<> float atLimitsMin<float>() { return FLT_MIN; }
template<> int8_t atLimitsMin<int8_t>() { return INT8_MIN; }
template<> int16_t atLimitsMin<int16_t>() { return INT16_MIN; }
template<> int32_t atLimitsMin<int32_t>() { return INT32_MIN; }
template<> int64_t atLimitsMin<int64_t>() { return INT64_MIN; }
template<> uint8_t atLimitsMin<uint8_t>() { return 0; }
template<> uint16_t atLimitsMin<uint16_t>() { return 0; }
template<> uint32_t atLimitsMin<uint32_t>() { return 0; }
template<> uint64_t atLimitsMin<uint64_t>() { return 0; }

template<> double atLimitsSmallest<double>() { return DBL_EPSILON; }
template<> float atLimitsSmallest<float>() { return FLT_EPSILON; }
