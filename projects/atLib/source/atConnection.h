#ifndef atConnection_h__
#define atConnection_h__

#include "atSocket.h"

class atConnection
{
public:
  atConnection(const atString &addr, const atString &port, const bool host = false);
  atConnection(const atConnection &copy);
  atConnection(atConnection &&move);
  atConnection(const atSocket &socket);
  atConnection(atSocket &&socket);

  bool Recieve(atVector<uint8_t> *pData);
  bool Send(const atVector<uint8_t> &data);
  bool Listen();
  atConnection Accept();

protected:
  atSocket m_socket;
};

#endif // atConnection_h__
