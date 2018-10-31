#include "atNetworkWriter.h"
#include <time.h>

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

int64_t atNetworkWriter::Write(void *pData, const int64_t len)
{
  if (!CanWrite())
    return 0;
  return m_pConn->Write((uint8_t*)pData, len);
}

bool atNetworkWriter::CanWrite() { return m_pConn && m_pConn->CanWrite(); }
