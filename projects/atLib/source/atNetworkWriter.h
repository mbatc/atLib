
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

#ifndef atNetworkWriter_h__
#define atNetworkWriter_h__

#include "atWriteStream.h"
#include "atSocket.h"
#include "atString.h"

class atNetworkWriter : public atWriteStream
{
public:
  ~atNetworkWriter();
  atNetworkWriter();

  // See Open() for more information

  atNetworkWriter(const atString &addr, const atString &port, const int64_t timeout = -1);

  // if timeout == -1, 
  //  - Open() will not wait until the target is writeable
  //  - Open() will always return true
  //  - CanWrite() can be used to check if the target is writeable, allowing Open() to be a non-blocking action
  //
  // if timeout > 0, 
  //  - Open() will return when the target is writeable or after [timeout] milliseconds
  //  - Open() will return true if the target is writeable
  //  - Open() will return false if the timeout was reached

  bool Open(const atString &addr, const atString &port, const int64_t timeout = -1);

  // Close the connection
  void Close();

  // if TRUE the target can be written to
  // if FALSE all calls to Write() will fail
  
  bool CanWrite();

  int64_t Write(void *pData, const int64_t len);
  template <typename T> int64_t Write(const T *pData, const int64_t count = 1);
  template <typename T> int64_t Write(const T &data);

protected:
  atSocket *m_pConn;
};

template<typename T> int64_t atNetworkWriter::Write(const T *pData, const int64_t count) { return this->Write(pData, count); }
template<typename T> int64_t atNetworkWriter::Write(const T &data) { return this->Write(data); }

#endif // atNetworkWriter_h__
