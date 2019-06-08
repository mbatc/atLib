#ifndef atNetwork_h__
#define atNetwork_h__

#include "atSocket.h"
#include "atHashMap.h"
#include "atThreading.h"

typedef int64_t atConnectionHandle;

struct ConnectionJob;

class atNetwork
{
public:
  struct Connection
  {
    atSocket socket;
    atVector<uint8_t> recv;
  };

  atNetwork(const bool asyncJobs = true);
  ~atNetwork();

  atConnectionHandle Host(const uint16_t &port);
  atConnectionHandle Connect(const atString &url, const uint16_t &port);

  void CloseAllConnections();

  bool IsConnected(const atConnectionHandle &handle);
  bool Disconnect(const atConnectionHandle &handle);
  bool Send(const atConnectionHandle &handle, const atVector<uint8_t> &data);
  
  int64_t BytesToRecieve(const atConnectionHandle &handle);
  atVector<uint8_t> Recieve(const atConnectionHandle &handle);

  atVector<atConnectionHandle> Connections();

protected:
  void ProcessJobs();
  void DoJob(ConnectionJob *pJob);
  void QueueJob(ConnectionJob *pJob);
  void DeleteJob(ConnectionJob *pJob);

  static int64_t GetNextHandle();

  atHashMap<atConnectionHandle, Connection*> m_connections;
  
  bool m_running;
  std::thread *m_pJobThread;
  std::mutex m_connectionLock;
  std::mutex m_jobLock;
  atVector<ConnectionJob*> m_jobQueue;
};

#endif // atNetwork_h__
