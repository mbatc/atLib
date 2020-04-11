
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

#include "atAssert.h"

#ifdef atPLATFORM_WIN32
#include <crtdbg.h>
#endif

void _atAssert(const bool cond, const char *expression, const char *message, const int64_t line, const char *file, const char *function)
{
  if (!cond)
  {
#ifdef atPLATFORM_WIN32
#ifdef _DEBUG
    expression;
    if (_CrtDbgReport(_CRT_ASSERT, file, (int)line, function, message))
#endif
      _CrtDbgBreak();
#endif

#ifdef atPLATFORM_LINUX
    printf("Assertion Failed: %s\nMessage: %s\n\nFile: %s, ln %lld, func '%s'", expression, message, file, line, function);
    __builtin_trap();
#endif
  }
}
