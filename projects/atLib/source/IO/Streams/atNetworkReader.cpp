
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#include "atNetworkReader.h"
#include <time.h>

atNetworkReader::atNetworkReader() : m_pHost(nullptr) , m_pConn(nullptr) {}
atNetworkReader::atNetworkReader(const atString & port, const int64_t timeout) : atNetworkReader() { Open(port, timeout); }
atNetworkReader::~atNetworkReader() { Close(); }

bool atNetworkReader::Open(const atString &port, const int64_t timeout)
{
  m_pHost = atNew<atSocket>(atSocket::Host(port));
  int64_t start = clock();
  while (!CanRead() && (clock() - start) < timeout);
  return CanRead();
}

void atNetworkReader::Close()
{
  if (m_pHost)
    atDelete(m_pHost);
  if (m_pConn)
    atDelete(m_pConn);
  m_pConn = nullptr;
  m_pHost = nullptr;
}

bool atNetworkReader::CanRead() 
{
  if (!m_pConn)
    if (m_pHost && m_pHost->CanAccept())
      m_pConn = atNew<atSocket>(m_pHost->Accept());
  return m_pConn && m_pConn->CanRead(); 
}

int64_t atNetworkReader::Read(void *pBuffer, const int64_t size)
{
  if (!CanRead())
    return 0;
  return m_pConn->Read((uint8_t*)pBuffer, size);
}
