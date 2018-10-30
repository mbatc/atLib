
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
#include <WS2tcpip.h>
#include "atSocket.h"

int64_t atSocket::s_nSockets = 0;

static void _InitialiseSockets() { WSAData wsaData; atAssert(WSAStartup(MAKEWORD(atWSAMajorVer, atWSAMinorVer), &wsaData) == 0, "WSAStartup Failed()"); }
static void _DeInitSockets() { atAssert(WSACleanup() == 0, "WSACleanup Failed()"); }

static SOCKET _CreateSocket(const atString &addr, const atString &port, const bool host)
{
  addrinfo *pInfo = nullptr;
  addrinfo hints = { 0 };

  hints.ai_flags = host ? AI_PASSIVE: 0;
  hints.ai_family = host ? AF_INET : AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  if (getaddrinfo(addr.c_str(), port.c_str(), &hints, &pInfo) != 0)
    return INVALID_SOCKET;

  SOCKET handle = INVALID_SOCKET;
  for(addrinfo *pTest = pInfo; pTest != nullptr; pTest = pTest->ai_next)
  {
    int connectResult = SOCKET_ERROR;
    handle = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
    if (host)
      connectResult = bind(handle, pTest->ai_addr, (int)pTest->ai_addrlen);
    else
      connectResult = connect(handle, pTest->ai_addr, (int)pTest->ai_addrlen);
    if (connectResult == SOCKET_ERROR)
    {
      closesocket(handle);
      handle = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(pInfo);
  return handle;
}

static void _CloseSocket(SOCKET handle)
{
  shutdown(handle, SD_SEND);
  closesocket(handle);
}


atSocket::~atSocket()
{
  if (m_handle != INVALID_SOCKET)
    _CloseSocket(m_handle);
  m_handle = INVALID_SOCKET;
  --s_nSockets;
  if (s_nSockets == 0)
    _DeInitSockets();
}

atSocket::atSocket(const atString &addr, const atString &port, const bool host /*= false*/)
  : m_addr(addr)
  , m_port(port)
  , m_isHost(host)
  , m_handle(INVALID_SOCKET)
{
  if(s_nSockets == 0)
    _InitialiseSockets();
  ++s_nSockets;
  m_handle = _CreateSocket(addr, port, host);
}

atSocket::atSocket(atSocket &&move)
  : m_addr(move.m_addr)
  , m_port(move.m_port)
  , m_isHost(move.m_isHost)
  , m_handle(move.m_handle)
{ move.m_handle = INVALID_SOCKET; }

bool atSocket::IsHost() const { return m_isHost; }
bool atSocket::IsConnected() const { return m_handle != INVALID_SOCKET; }
const atString& atSocket::Port() const { return m_port; }
const atString& atSocket::Address() const { return m_addr; }
const atSocketHandle& atSocket::Handle() const { return m_handle; }
atSocket::atSocket(const atSocket &copy) : atSocket(copy.m_addr, copy.m_port, copy.m_isHost) {}
atSocket::atSocket(atSocketHandle handle) : m_handle(handle) {}
