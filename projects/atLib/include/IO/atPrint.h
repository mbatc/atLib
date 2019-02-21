
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

#ifndef atPrint_h__
#define atPrint_h__

#include "atString.h"

class atPrint
{
public:
  atPrint() = delete;
  
  template <typename T> static atString Print(const T &object);
  template <typename T> static atString Float(const T &val);
  template <typename T> static atString Int(const T &val);

  template <typename T> static atString Vector(const atVector2<T> &vec, const char *separator = " ");
  template <typename T> static atString Vector(const atVector3<T> &vec, const char *separator = " ");
  template <typename T> static atString Vector(const atVector4<T> &vec, const char *separator = " ");

  static atString Float(const double &val);
  static atString Int(const int64_t &val);
};

#include "atPrint.inl"
#endif // atPrint_h__
