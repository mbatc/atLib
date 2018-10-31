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
