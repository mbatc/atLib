
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

#include "atNetworkWriter.h"
#include <time.h>
#include <windows.h>

atNetworkWriter::atNetworkWriter() : m_pConn(nullptr) {}
atNetworkWriter::atNetworkWriter(const atString &addr, const atString &port, const int64_t timeout) : m_pConn(nullptr) { Open(addr, port, timeout); }
atNetworkWriter::~atNetworkWriter() { Close(); }

bool atNetworkWriter::Open(const atString &addr, const atString &port, const int64_t timeout)
{
  m_pConn = atNew<atSocket>(atSocket::Connect(addr, port));
  int64_t start = clock();
  while (!CanWrite() && (clock() - start) < timeout) Sleep((DWORD)atMin(timeout - (clock() - start), 10));
  return CanWrite();
}

void atNetworkWriter::Close()
{
  if(m_pConn)
    atDelete(m_pConn);
  m_pConn = nullptr;
}

int64_t atNetworkWriter::Write(const void *pData, const int64_t len)
{
  if (!CanWrite())
    return 0;
  return m_pConn->Write((uint8_t*)pData, len);
}

bool atNetworkWriter::CanWrite() { return m_pConn && m_pConn->CanWrite(); }
