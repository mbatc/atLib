
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

#ifndef atWriteStream_h__
#define atWriteStream_h__

#include "atStreamSeekable.h"

#define atTrivialStreamWrite(type) inline int64_t atStreamWrite(atWriteStream *pStream, const type *pData, const int64_t count) { return atStreamWrite(pStream, (void*)pData, (int64_t)sizeof(type) * count); }

class atWriteStream : public atStreamSeekable
{
public:
  // Writes pData to the file
  virtual int64_t Write(void *pData, const int64_t len) = 0;
  template<typename T> int64_t Write(const T *pData, const int64_t count = 1);
  template<typename T> int64_t Write(const T &data);
};

int64_t atStreamWrite(atWriteStream *pStream, void *pData, const int64_t count);

template<typename T> int64_t atWriteStream::Write(const T *pData, const int64_t count) { return atStreamWrite(this, pData, count); }
template<typename T> int64_t atWriteStream::Write(const T &data) { return Write(&data, 1); }

atTrivialStreamWrite(int64_t);
atTrivialStreamWrite(int32_t);
atTrivialStreamWrite(int16_t);
atTrivialStreamWrite(int8_t);

atTrivialStreamWrite(uint64_t);
atTrivialStreamWrite(uint32_t);
atTrivialStreamWrite(uint16_t);
atTrivialStreamWrite(uint8_t);
atTrivialStreamWrite(bool)
atTrivialStreamWrite(wchar_t);
atTrivialStreamWrite(char);
atTrivialStreamWrite(double);
atTrivialStreamWrite(float);

#endif // atWriteStream_h__
