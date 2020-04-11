
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

#ifndef _atAssert_h__
#define _atAssert_h__

#include "atTypes.h"

// Don't include system headers in atPlatform
#define atPLATFORM_NO_SYS_INCLUDE
#include "atPlatform.h"
#undef atPLATFORM_NO_SYS_INCLUDE

void _atAssert(const bool cond, const char *expression, const char *message, const int64_t line, const char *file, const char *function);

#define atRelAssert(cond, message) _atAssert((bool)(cond), #cond, message, __LINE__, __FILE__, atFUNCSIG)

#ifdef _DEBUG
#define atAssert(cond, message) atRelAssert(cond, message)
#else
#define atAssert(cond, message) (void())
#endif

#define atFail(message) atAssert(false, message)
#define atRelFail(message) atRelAssert(false, message)

#endif