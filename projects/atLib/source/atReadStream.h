
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

#ifndef atReadStream_h__
#define atReadStream_h__

#include "atTypes.h"

#define _atStreamRead(type) int64_t atStreamRead(atReadStream *pStream, type *pData)
#define atTrivialStreamRead(type) static int64_t atStreamRead(atReadStream *pStream, type *pData) { return atStreamRead(pStream, (void*)pData, sizeof(type)); }

class atReadStream
{
public:
  // Read data into pBuffer. 
  // Returns the number of bytes read
  virtual int64_t Read(void *pBuffer, const int64_t size) = 0;
  template<typename T> int64_t Read(T * pBuffer, const int64_t count);
};

int64_t atStreamRead(atReadStream *pStream, void *pData, const int64_t count);

template<typename T> int64_t atReadStream::Read(T *pBuffer, const int64_t count) { return atStreamRead(this, pBuffer, count); }

atTrivialStreamRead(int64_t)
atTrivialStreamRead(int32_t)
atTrivialStreamRead(int16_t)
atTrivialStreamRead(int8_t)

atTrivialStreamRead(uint64_t)
atTrivialStreamRead(uint32_t)
atTrivialStreamRead(uint16_t)
atTrivialStreamRead(uint8_t)

atTrivialStreamRead(char)

atTrivialStreamRead(double)
atTrivialStreamRead(float)

#endif // atReadStream_h__
