
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
