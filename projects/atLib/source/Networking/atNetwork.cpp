#include "atNetwork.h"

static const int64_t recvBlockSize = 512;

enum _atConnectionJobType
{
  _atCJT_Send,
  _atCJT_Host,
  _atCJT_Connect,
  _atCJT_Recieve
};

struct _atRecieveData {};

struct _atSendData { atVector<uint8_t> buffer; };

struct _atConnectData
{
  atString url;
  uint16_t port;
};

struct _atHostData
{
  uint16_t port;
};

struct ConnectionJob
{
  atNetwork::Connection *pConnection;
  _atConnectionJobType jobType;
  void *pJobData;
};

void _DoHostJob(atNetwork::Connection *pConnection, _atHostData *pData);
void _DoConnectJob(atNetwork::Connection *pConnection, _atConnectData *pData);
void _DoSendJob(atNetwork::Connection *pConnection, _atSendData *pData);
void _DoRecieveJob(atNetwork::Connection *pConnection, _atRecieveData *pData);

atNetwork::atNetwork(const bool asyncJobs)
  : m_running(true)
{
  if (asyncJobs)
    m_pJobThread = atNew<std::thread>(&atNetwork::ProcessJobs, this);
}

atNetwork::~atNetwork()
{
  CloseAllConnections();
  m_running = false;
  if (m_pJobThread)
  {
    m_pJobThread->join();
    atDelete(m_pJobThread);
    m_pJobThread = nullptr;
  }
}

atConnectionHandle atNetwork::Host(const uint16_t &port)
{
  // Create send data
  _atHostData *pData = atNew<_atHostData>();
  pData->port = port;

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = atNew<Connection>();
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Host;

  int64_t handle = GetNextHandle();
  m_connections.Add(handle, pJob->pConnection);
  QueueJob(pJob);
  return handle;
}

atConnectionHandle atNetwork::Connect(const atString &url, const uint16_t &port)
{
  // Create send data
  _atConnectData *pData = atNew<_atConnectData>();
  pData->port = port;
  pData->url = url;

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = atNew<Connection>();
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Connect;

  int64_t handle = GetNextHandle();
  m_connections.Add(handle, pJob->pConnection);
  QueueJob(pJob);
  return handle;
}

void atNetwork::CloseAllConnections()
{
  for (atConnectionHandle &handle : Connections())
    Disconnect(handle);
}

bool atNetwork::IsConnected(const atConnectionHandle &handle)
{
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->socket.IsValid() : false;
}

bool atNetwork::Disconnect(const atConnectionHandle &handle)
{
  if (m_connections.Contains(handle))
  {
    m_connections.Remove(handle);
    return true;
  }
  return false;
}

int64_t atNetwork::BytesToRecieve(const atConnectionHandle &handle)
{
  atScopeLock lock(m_connectionLock);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->recv.size() : 0;
}

atVector<uint8_t> atNetwork::Recieve(const atConnectionHandle &handle)
{
  atScopeLock lock(m_connectionLock);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->recv : atVector<uint8_t>();
}

atVector<atConnectionHandle> atNetwork::Connections()
{
  atScopeLock lock(m_connectionLock);
  return m_connections.GetKeys();
}

bool atNetwork::Send(const atConnectionHandle &handle, const atVector<uint8_t> &data)
{
  atScopeLock lock(m_connectionLock);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return false;

  // Create send data
  _atSendData *pData = atNew<_atSendData>();
  pData->buffer = data;

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Send;
  QueueJob(pJob);
  return true;
}

void atNetwork::ProcessJobs()
{
  while (m_running)
  {
    m_connectionLock.lock();
    for (auto &kvp : m_connections)
      if (kvp.m_val->socket.CanRead())
      {
        // Create Recieve data
        _atRecieveData *pData = atNew<_atRecieveData>();
        // Create Recieve Job
        ConnectionJob *pJob = atNew<ConnectionJob>();
        pJob->pConnection = kvp.m_val;
        pJob->pJobData = (void*)pData;
        pJob->jobType = _atCJT_Recieve;
        QueueJob(pJob);
      }
    m_connectionLock.unlock();

    if (m_jobQueue.size() == 0)
    {
      atSleep(10);
      continue;
    }

    m_jobLock.lock();
    ConnectionJob *pJob = m_jobQueue.front();
    DoJob(pJob);
    m_jobQueue.erase(0);
    m_jobLock.unlock();

    DeleteJob(pJob);
  }
}

void atNetwork::DoJob(ConnectionJob *pJob)
{
  switch (pJob->jobType)
  {
  case _atCJT_Host: _DoHostJob(pJob->pConnection, (_atHostData*)pJob->pJobData); break;
  case _atCJT_Connect: _DoConnectJob(pJob->pConnection, (_atConnectData*)pJob->pJobData); break;
  case _atCJT_Recieve: _DoRecieveJob(pJob->pConnection, (_atRecieveData*)pJob->pJobData); break;
  case _atCJT_Send: _DoSendJob(pJob->pConnection, (_atSendData*)pJob->pJobData); break;
  }
}

void atNetwork::QueueJob(ConnectionJob *pJob)
{
  atScopeLock lock(m_jobLock);
  m_jobQueue.push_back(pJob);
}

void atNetwork::DeleteJob(ConnectionJob *pJob)
{
  switch (pJob->jobType)
  {
  case _atCJT_Host: atDelete((_atHostData*)pJob->pJobData); break;
  case _atCJT_Connect: atDelete((_atConnectData*)pJob->pJobData); break;
  case _atCJT_Recieve: atDelete((_atRecieveData*)pJob->pJobData); break;
  case _atCJT_Send: atDelete((_atSendData*)pJob->pJobData); break;
  }
  pJob->pJobData = nullptr;
}

int64_t atNetwork::GetNextHandle()
{
  static int64_t nextHandle = 0;
  return nextHandle++;
}

void _DoHostJob(atNetwork::Connection *pConnection, _atHostData *pData)
{
  pConnection->socket = atSocket::Host(atString(pData->port));
}

void _DoConnectJob(atNetwork::Connection *pConnection, _atConnectData *pData)
{
  pConnection->socket = atSocket::Connect(pData->url, atString(pData->port));
}

void _DoSendJob(atNetwork::Connection *pConnection, _atSendData *pData)
{
  pConnection->socket.Write(pData->buffer.data(), pData->buffer.size());
}

void _DoRecieveJob(atNetwork::Connection *pConnection, _atRecieveData *pData)
{
  atVector<uint8_t> &recv = pConnection->recv;
  int64_t bytesRead = 0;
  uint8_t buffer[recvBlockSize] = { 0 };
  while ((bytesRead = pConnection->socket.Read(buffer, recvBlockSize)) > 0)
  {
    int64_t offset = recv.size();
    recv.resize(recv.size() + bytesRead);
    memcpy(recv.data() + offset, buffer, bytesRead);
  }
}
