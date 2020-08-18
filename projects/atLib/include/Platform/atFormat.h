
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#ifndef atFormat_h__
#define atFormat_h__

#include "atTypes.h"

class atFormat
{
public:
  static int64_t GLMaxWidth() { return 4; }
  static int64_t GL(const atType &type, const int64_t &width);
  static int64_t GLInternal(const atType &type, const int64_t &width);
  static int64_t GLType(const atType &type);
  static int64_t GLDepth(const atType &type);
  static int64_t GLDepthInternal(const atType &type);

  static int64_t DXGIMaxWidth() { return 4; }
  static int64_t DXGI(const atType &type, const int64_t &width);
  static int64_t DXGIDepth(const atType &type);
};

#endif // atFormat_h__
