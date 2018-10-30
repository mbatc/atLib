#include "atConnection.h"

#define RECVMAX 2048

atConnection::atConnection(const atString &addr, const atString &port, const bool host) : m_socket(addr, port, host) {}
atConnection::atConnection(const atConnection &copy) : m_socket (copy.m_socket) {}
atConnection::atConnection(atConnection &&move) : m_socket(move.m_socket) {}
atConnection::atConnection(const atSocket &socket) : m_socket(socket) {}
atConnection::atConnection(atSocket &&socket) : m_socket(socket) {}

bool atConnection::Recieve(atVector<uint8_t> *pData)
{
  uint64_t remaining = 0;
  if (recv(m_socket.Handle(), (char*)&remaining, sizeof(uint64_t), 0) != sizeof(uint64_t) == 0)
    return false;

  pData->resize(remaining);
  while (remaining > 0)
  {
    recv(m_socket.Handle(), (char*)pData->data() + (pData->size() - remaining), atMin(RECVMAX, remaining), 0);
    remaining -= RECVMAX;
  }
  return true;
}

bool atConnection::Listen()
{
  if (!m_socket.IsHost())
    return false;
  if (listen(m_socket.Handle(), SOMAXCONN) == SOCKET_ERROR)
    return false;
  return true;
}

bool atConnection::Send(const atVector<uint8_t> &data) 
{
  uint64_t msgSize = data.size();
  return send(m_socket.Handle(), (char*)&msgSize, sizeof(int64_t), 0) != SOCKET_ERROR ? 
    send(m_socket.Handle(), (char*)data.data(), (int)data.size(), 0) != SOCKET_ERROR : false;
}

atConnection atConnection::Accept() { return accept(m_socket.Handle(), NULL, NULL); }
