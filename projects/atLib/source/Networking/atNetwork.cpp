#include "atNetwork.h"

static const int64_t recvBlockSize = 512;

enum _atConnectionJobType
{
  _atCJT_Send,
  _atCJT_Host,
  _atCJT_Connect,
  _atCJT_Recieve,
  _atCJT_Disconnect
};

struct _atReceiveData {};
struct _atDisconnectData {};
struct _atSendData { atVector<uint8_t> buffer; };
struct _atConnectData { atString url; uint16_t port; };
struct _atHostData { uint16_t port; };

struct ConnectionJob
{
  _atConnectionJobType jobType;
  
  atNetwork::Connection *pConnection = nullptr;
  void *pJobData = nullptr;

  bool done = false;
  bool failed = false;
  int64_t refs = 0;
};

bool _DoHostJob(atNetwork::Connection *pConnection, _atHostData *pData);
bool _DoConnectJob(atNetwork::Connection *pConnection, _atConnectData *pData);
bool _DoSendJob(atNetwork::Connection *pConnection, _atSendData *pData);
void _DoRecieveJob(atNetwork::Connection *pConnection, _atReceiveData *pData);
void _DoDisconnectJob(atNetwork::Connection *pConnection, _atDisconnectData *pData);

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

atNetwork::JobStatus atNetwork::Host(const uint16_t &port)
{
  // Create host data
  _atHostData *pData = atNew<_atHostData>();
  pData->port = port;

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = atNew<Connection>();
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Host;

  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  int64_t handle = GetNextHandle();
  m_connections.Add(handle, pJob->pConnection);

  return QueueJob(pJob, handle);
}

atNetwork::JobStatus atNetwork::Connect(const atString &url, const uint16_t &port)
{
  // Create connect data
  _atConnectData *pData = atNew<_atConnectData>();
  pData->port = port;
  pData->url = url;

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = atNew<Connection>();
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Connect;

  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  int64_t handle = GetNextHandle();
  m_connections.Add(handle, pJob->pConnection);
  return QueueJob(pJob, handle);
}

atNetwork::JobStatus atNetwork::Receive(const atConnectionHandle &handle)
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return JobStatus(nullptr, -1);

  // Create receive data
  _atReceiveData *pData = atNew<_atReceiveData>();

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Recieve;
  return QueueJob(pJob, handle);
}

atNetwork::JobStatus atNetwork::Send(const atConnectionHandle &handle, const atVector<uint8_t> &data)
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return JobStatus(nullptr, -1);

  // Create send data
  _atSendData *pData = atNew<_atSendData>();
  pData->buffer = data;

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Send;
  return QueueJob(pJob, handle);
}

atNetwork::JobStatus atNetwork::Disconnect(const atConnectionHandle &handle)
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return JobStatus(nullptr, -1);

  // Create disconnect data
  _atDisconnectData *pData = atNew<_atDisconnectData>();

  // Create Send Job
  ConnectionJob *pJob = atNew<ConnectionJob>();
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Disconnect;

  // Remove from connection list
  m_connections.Remove(handle);
  
  return QueueJob(pJob, handle);
}

void atNetwork::CloseAllConnections()
{
  for (atConnectionHandle &handle : Connections())
    Disconnect(handle);
}

bool atNetwork::IsConnected(const atConnectionHandle &handle)
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->socket.IsValid() : false;
}

bool atNetwork::CanRecieve(const atConnectionHandle &handle, int64_t *pNumBytes)
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->socket.CanRead(pNumBytes) : false;
}

atVector<atConnectionHandle> atNetwork::Connections()
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  return m_connections.GetKeys();
}

atVector<uint8_t> atNetwork::GetRecieve(const atConnectionHandle &handle)
{
  atConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? std::move((*ppCon)->recv) : atVector<uint8_t>();
}

void atNetwork::ProcessJobs()
{
  while (m_running)
  {
    if (m_jobQueue.size() == 0)
    {
      atSleep(10);
      continue;
    }

    m_jobLock.lock();
    JobStatus stat = m_jobQueue.front();
    DoJob(stat.m_pJob);
    m_jobQueue.erase(0);
    m_jobLock.unlock();
  }
}

void atNetwork::DoJob(ConnectionJob *pJob)
{
  pJob->failed = false;
  switch (pJob->jobType)
  {
  case _atCJT_Host: pJob->failed = !_DoHostJob(pJob->pConnection, (_atHostData*)pJob->pJobData); break;
  case _atCJT_Connect: pJob->failed = !_DoConnectJob(pJob->pConnection, (_atConnectData*)pJob->pJobData); break;
  case _atCJT_Recieve: _DoRecieveJob(pJob->pConnection, (_atReceiveData*)pJob->pJobData); break;
  case _atCJT_Send: pJob->failed = !_DoSendJob(pJob->pConnection, (_atSendData*)pJob->pJobData); break;
  case _atCJT_Disconnect: _DoDisconnectJob(pJob->pConnection, (_atDisconnectData*)pJob->pJobData); break;
  }
  pJob->done = true;
}

atNetwork::JobStatus atNetwork::QueueJob(ConnectionJob *pJob, const atConnectionHandle &handle)
{
  if (m_pJobThread)
  { // If there is a worker thread then queue the job
    atScopeLock lock(m_jobLock);
    m_jobQueue.push_back(JobStatus(pJob, handle));
    return m_jobQueue.back();
  }

  // If there is no worker thread, do the job now and return the JobStatus
  JobStatus localStat = m_jobQueue.back();
  DoJob(localStat.m_pJob);
  return localStat;
}

void atNetwork::DeleteJob(ConnectionJob *pJob)
{
  switch (pJob->jobType)
  {
  case _atCJT_Host: atDelete((_atHostData*)pJob->pJobData); break;
  case _atCJT_Connect: atDelete((_atConnectData*)pJob->pJobData); break;
  case _atCJT_Recieve: atDelete((_atReceiveData*)pJob->pJobData); break;
  case _atCJT_Send: atDelete((_atSendData*)pJob->pJobData); break;
  case _atCJT_Disconnect: atDelete((_atDisconnectData*)pJob->pJobData); break;
  }
  pJob->pJobData = nullptr;
}

int64_t atNetwork::GetNextHandle()
{
  static int64_t nextHandle = 0;
  return nextHandle++;
}

bool _DoHostJob(atNetwork::Connection *pConnection, _atHostData *pData)
{
  pConnection->socket = atSocket::Host(atString(pData->port));
  return pConnection->socket.IsValid();
}

bool _DoConnectJob(atNetwork::Connection *pConnection, _atConnectData *pData)
{
  pConnection->socket = atSocket::Connect(pData->url, atString(pData->port));
  return pConnection->socket.IsValid();
}

bool _DoSendJob(atNetwork::Connection *pConnection, _atSendData *pData)
{
  int64_t sent = pConnection->socket.Write(pData->buffer.data(), pData->buffer.size());
  return pData->buffer.size() == sent;
}

void _DoRecieveJob(atNetwork::Connection *pConnection, _atReceiveData *pData)
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

void _DoDisconnectJob(atNetwork::Connection *pConnection, _atDisconnectData *pData)
{
  pConnection->socket = atSocket();
  pConnection->recv.clear();
  atDelete(pConnection);
}

atNetwork::JobStatus::JobStatus(ConnectionJob *pJob, const atConnectionHandle &handle)
  : m_pJob(pJob)
  , m_handle(handle)
{ m_pJob->refs++; }

atNetwork::JobStatus::JobStatus(const JobStatus &copy) : JobStatus(copy.m_pJob, copy.m_handle) {}

atNetwork::JobStatus::JobStatus(JobStatus &&move)
{
  m_pJob = move.m_pJob;
  m_handle = move.m_handle;
  move.m_handle = -1;
  move.m_pJob = 0;
}

atNetwork::JobStatus::~JobStatus() { Clear(); }

bool atNetwork::JobStatus::Done() const { return m_pJob && m_pJob->done; }
bool atNetwork::JobStatus::Failed() const { return m_pJob && m_pJob->failed; }
atConnectionHandle atNetwork::JobStatus::Handle() const { return m_handle; }

const atNetwork::JobStatus& atNetwork::JobStatus::operator=(const JobStatus &copy)
{
  Clear();
  m_pJob = copy.m_pJob;
  m_handle = copy.m_handle;
  m_pJob->refs++;
  return *this;
}

const atNetwork::JobStatus& atNetwork::JobStatus::operator=(JobStatus &&move)
{
  Clear();
  m_pJob = move.m_pJob;
  m_handle = move.m_handle;
  move.m_pJob = nullptr;
  move.m_handle = -1;
  return *this;
}

void atNetwork::JobStatus::Clear()
{
  if (m_pJob && --m_pJob->refs == 0)
    DeleteJob(m_pJob);
  m_pJob = nullptr;
  m_handle = -1;
}
