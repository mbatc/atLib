
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

#ifndef atNetworkReader_h__
#define atNetworkReader_h__

#include "atReadStream.h"
#include "atSocket.h"
#include "atString.h"

class atNetworkReader : public atReadStream
{
public:
  atNetworkReader();

  // See Open() for more information

  atNetworkReader(const atString &port, const int64_t timeout = -1);
  ~atNetworkReader();

  // Open() will return true if the target is readable
  //
  // if timeout == -1, 
  //  - Open() will not wait until the target is readable
  //  - CanWrite() can be used to check if the target is readable, allowing Open() to be a non-blocking action
  //
  // if timeout > 0, 
  //  - Open() will return when the target is readable or after [timeout] milliseconds
  //  - Open() will return false if the timeout was reached

  bool Open(const atString &port, const int64_t timeout = -1);

  // Close the connection
  void Close();

  // if TRUE the target can be read from
  // if FALSE all calls to Read() will fail

  bool CanRead();

  int64_t Read(void *pBuffer, const int64_t size);
  template <typename T> Read(T *pData, const int64_t count = 1);

protected:
  atSocket *m_pHost;
  atSocket *m_pConn;
};

template<typename T> atNetworkReader::Read(T * pData, const int64_t count) { return this->Read(pData, count); }

#endif // atNetworkReader_h__
