
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

#include "atMath.h"
#include <math.h>

template<> atTypeDesc atGetTypeDesc<atVector2<float>>() { return{ atGetType<float>(), 2 }; }
template<> atTypeDesc atGetTypeDesc<atVector3<float>>() { return{ atGetType<float>(), 3 }; }
template<> atTypeDesc atGetTypeDesc<atVector4<float>>() { return{ atGetType<float>(), 4 }; }

template<> atTypeDesc atGetTypeDesc<atVector2<double>>() { return{ atGetType<double>(), 2 }; }
template<> atTypeDesc atGetTypeDesc<atVector3<double>>() { return{ atGetType<double>(), 3 }; }
template<> atTypeDesc atGetTypeDesc<atVector4<double>>() { return{ atGetType<double>(), 4 }; }

template<> atTypeDesc atGetTypeDesc<atVector2<int32_t>>() { return{ atGetType<int32_t>(), 2 }; }
template<> atTypeDesc atGetTypeDesc<atVector3<int32_t>>() { return{ atGetType<int32_t>(), 3 }; }
template<> atTypeDesc atGetTypeDesc<atVector4<int32_t>>() { return{ atGetType<int32_t>(), 4 }; }

template<> atTypeDesc atGetTypeDesc<atVector2<int64_t>>() { return{ atGetType<int64_t>(), 2 }; }
template<> atTypeDesc atGetTypeDesc<atVector3<int64_t>>() { return{ atGetType<int64_t>(), 3 }; }
template<> atTypeDesc atGetTypeDesc<atVector4<int64_t>>() { return{ atGetType<int64_t>(), 4 }; }

template<> atTypeDesc atGetTypeDesc<atVector2<int8_t>>() { return{ atGetType<int8_t>(), 2 }; }
template<> atTypeDesc atGetTypeDesc<atVector3<int8_t>>() { return{ atGetType<int8_t>(), 3 }; }
template<> atTypeDesc atGetTypeDesc<atVector4<int8_t>>() { return{ atGetType<int8_t>(), 4 }; }

template<> atTypeDesc atGetTypeDesc<atVector2<int16_t>>() { return{ atGetType<int16_t>(), 2 }; }
template<> atTypeDesc atGetTypeDesc<atVector3<int16_t>>() { return{ atGetType<int16_t>(), 3 }; }
template<> atTypeDesc atGetTypeDesc<atVector4<int16_t>>() { return{ atGetType<int16_t>(), 4 }; }

template<> atTypeDesc atGetTypeDesc<atMatrix4x4<double>>() { return{ atGetType<double>(), 16 }; }
template<> atTypeDesc atGetTypeDesc<atMatrix4x4<float>>() { return{ atGetType<float>(), 16 }; }

template<> inline float atMod(const float &a, const float &b) { return fmodf(a, b); }
template<> inline double atMod(const double &a, const double &b) { return fmodl(a, b); }
