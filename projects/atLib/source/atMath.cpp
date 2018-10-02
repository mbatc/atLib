
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

template <> atTypeDesc atGetTypeDesc<atVec2F>() { return{ atGetType<float>(), 2 }; }
template <> atTypeDesc atGetTypeDesc<atVec3F>() { return{ atGetType<float>(), 3 }; }
template <> atTypeDesc atGetTypeDesc<atVec4F>() { return{ atGetType<float>(), 4 }; }

template <> atTypeDesc atGetTypeDesc<atVec2F64>() { return{ atGetType<double>(), 2 }; }
template <> atTypeDesc atGetTypeDesc<atVec3F64>() { return{ atGetType<double>(), 3 }; }
template <> atTypeDesc atGetTypeDesc<atVec4F64>() { return{ atGetType<double>(), 4 }; }

template <> atTypeDesc atGetTypeDesc<atVec2I>() { return{ atGetType<int32_t>(), 2 }; }
template <> atTypeDesc atGetTypeDesc<atVec3I>() { return{ atGetType<int32_t>(), 3 }; }
template <> atTypeDesc atGetTypeDesc<atVec4I>() { return{ atGetType<int32_t>(), 4 }; }

template <> atTypeDesc atGetTypeDesc<atVec2I64>() { return{ atGetType<int64_t>(), 2 }; }
template <> atTypeDesc atGetTypeDesc<atVec3I64>() { return{ atGetType<int64_t>(), 3 }; }
template <> atTypeDesc atGetTypeDesc<atVec4I64>() { return{ atGetType<int64_t>(), 4 }; }

template <> atTypeDesc atGetTypeDesc<atVector2<int8_t>>() { return{ atGetType<int8_t>(), 2 }; }
template <> atTypeDesc atGetTypeDesc<atVector3<int8_t>>() { return{ atGetType<int8_t>(), 3 }; }
template <> atTypeDesc atGetTypeDesc<atVector4<int8_t>>() { return{ atGetType<int8_t>(), 4 }; }

template <> atTypeDesc atGetTypeDesc<atVector2<int16_t>>() { return{ atGetType<int16_t>(), 2 }; }
template <> atTypeDesc atGetTypeDesc<atVector3<int16_t>>() { return{ atGetType<int16_t>(), 3 }; }
template <> atTypeDesc atGetTypeDesc<atVector4<int16_t>>() { return{ atGetType<int16_t>(), 4 }; }

template <> atTypeDesc atGetTypeDesc<atMatrix<double, 2, 2>>() { return{ atGetType<double>(), 4}; }
template <> atTypeDesc atGetTypeDesc<atMatrix<double, 3, 3>>() { return{ atGetType<double>(), 9}; }
template <> atTypeDesc atGetTypeDesc<atMatrix<double, 4, 4>>() { return{ atGetType<double>(), 16 }; }

template <> atTypeDesc atGetTypeDesc<atMatrix<float, 2, 2>>() { return{ atGetType<float>(), 4 }; }
template <> atTypeDesc atGetTypeDesc<atMatrix<float, 3, 3>>() { return{ atGetType<float>(), 9 }; }
template <> atTypeDesc atGetTypeDesc<atMatrix<float, 4, 4>>() { return{ atGetType<float>(), 16 }; }
