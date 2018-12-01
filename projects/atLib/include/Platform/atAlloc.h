
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

#ifndef _atAlloc_h__
#define _atAlloc_h__

#include "atAssert.h"

void* _atRawAlloc(const int64_t size);
void* _atRawRealloc(void *pBlock, const int64_t size);

void* _atAllocTrace(const int64_t size, const int64_t line, const char *file, const char *function);
void* _atReallocTrace(void *pBlock, const int64_t size, const int64_t line, const char *file, const char *function);

void* _atAllocRelTrace(const int64_t size, const int64_t line, const char *file, const char *function);
void* _atReallocRelTrace(void *pBlock, const int64_t size, const int64_t line, const char *file, const char *function);

void atFree(void *pBlock);

#define atAlloc(size) _atAllocTrace(size, __LINE__, __FILE__, __FUNCSIG__)
#define atRealloc(block, size) _atReallocTrace(block, size, __LINE__, __FILE__, __FUNCSIG__)

#define atAllocRelTrace(size) _atAllocRelTrace(size, __LINE__, __FILE__, __FUNCSIG__ )
#define atReallocRelTrace(block, size) _atReallocRelTrace(block, size, __LINE__, __FILE__, __FUNCSIG__)

#endif