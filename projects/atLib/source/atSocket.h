
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

#ifndef atSocket_h__
#define atSocket_h__

#include "atString.h"
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define atWSAMajorVer 2
#define atWSAMinorVer 2

typedef SOCKET atSocketHandle;

class atSocket
{
  atSocket();
public:
  atSocket(atSocket &&move);
  ~atSocket();

  static atSocket Host(const atString &port);
  static atSocket Connect(const atString &addr, const atString &port);

  bool IsHost() const;
  bool IsValid() const;

  bool CanAccept() const;
  bool CanRead() const;
  bool CanWrite() const;
  
  atSocket Accept() const;
  int64_t Read(uint8_t *pData, const int64_t maxLen) const;
  int64_t Write(const uint8_t *pData, const int64_t len) const;

  const atSocketHandle &Handle() const;
  const atString &Port() const;
  const atString &Address() const;

protected:
  atSocketHandle m_handle;
  atString m_addr;
  atString m_port;
  bool m_isHost;


  static int64_t s_nSockets;
};

#endif // atSocket_h__
