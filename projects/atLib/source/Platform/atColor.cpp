
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

#include "atColor.h"

atCol atColor::RGBAtoARGB(const atCol col) { return Pack(atCOL_GETA(col), atCOL_GETR(col), atCOL_GETG(col), atCOL_GETB(col)); }
atCol atColor::ARGBtoRGBA(const atCol col) { return Pack(atCOL_GETG(col), atCOL_GETB(col), atCOL_GETA(col), atCOL_GETR(col)); }

template <> atCol atColor::PackARGB<float>(const float r, const float g, const float b, const float a) { return Pack<int64_t>((int64_t)a * 255, (int64_t)r * 255, (int64_t)g * 255, (int64_t)b * 255); }
template <> atCol atColor::PackARGB<double>(const double r, const double g, const double b, const double a) { return Pack<int64_t>((int64_t)a * 255, (int64_t)r * 255, (int64_t)g * 255, (int64_t)b * 255); }

template <> atCol atColor::Pack<float>(const float r, const float g, const float b, const float a) { return Pack<int64_t>((int64_t)r * 255, (int64_t)g * 255, (int64_t)b * 255, (int64_t)a * 255); }
template <> atCol atColor::Pack<double>(const double r, const double g, const double b, const double a) { return Pack<int64_t>((int64_t)r * 255, (int64_t)g * 255, (int64_t)b * 255, (int64_t)a * 255); }

template <> atVector4<float> atColor::UnPackARGB(const atCol &col) { return atVector4<float>(UnPackARGB<int64_t>(col)) / 255.f; }
template <> atVector4<double> atColor::UnPackARGB(const atCol &col) { return atVector4<double>(UnPackARGB<int64_t>(col)) / 255.0; }

template <> atVector4<float> atColor::UnPack(const atCol &col) { return atVector4<float>(UnPack<int64_t>(col)) / 255.0f; }
template <> atVector4<double> atColor::UnPack(const atCol &col) { return atVector4<double>(UnPack<int64_t>(col)) / 255.0; }
