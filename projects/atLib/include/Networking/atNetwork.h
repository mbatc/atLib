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

  class JobStatus
  {
    friend atNetwork;

  public:
    JobStatus(ConnectionJob* pJob, const atConnectionHandle &handle);
    JobStatus(const JobStatus &copy);
    JobStatus(JobStatus &&move);
    ~JobStatus();

    bool Done() const;
    bool Failed() const;
    atConnectionHandle Handle() const;

    const JobStatus& operator=(const JobStatus &copy);
    const JobStatus& operator=(JobStatus &&copy);

  protected:
    void Clear();

    atConnectionHandle m_handle;
    ConnectionJob* m_pJob;
  };

  atNetwork(const bool asyncJobs = true);
  ~atNetwork();

  //******************
  // Asynchronous Jobs
  //******************

  // Start a send Job. Use the JobStatus object to query the host job status
  JobStatus Host(const uint16_t &port);

  // Start a send Job. Use the JobStatus object to query the connect job status
  JobStatus Connect(const atString &url, const uint16_t &port);
  
  // Start a send Job. Use the JobStatus object to query the send job status
  JobStatus Send(const atConnectionHandle &handle, const atVector<uint8_t> &data);
  
  // Start a receive Job. Use the JobStatus object to query the receive job status
  // Bytes received from a send job can be retrieved via GetRecieved()
  JobStatus Receive(const atConnectionHandle &handle);

  // Start a disconnect Job. Use the JobStatus object to query the disconnect job status
  JobStatus Disconnect(const atConnectionHandle &handle);

  //******************
  // Synchronous Jobs
  //******************

  // Close all active connections. Invalidates all open connection handles.
  void CloseAllConnections();

  //******************
  // Connection Status
  //******************

  // Check if a atConnectionHandle is actively connected
  bool IsConnected(const atConnectionHandle &handle);

  // Check if a atConnectionHandle a call to Receive() will successfully retrieve some data.
  bool CanRecieve(const atConnectionHandle &handle, int64_t *pNumBytes = nullptr);

  // Returns a list of all active connection handles.
  atVector<atConnectionHandle> Connections();

  //******************
  // Connection Data
  //******************

  // Return all bytes that have been received from a connection.
  // Clears the receive buffer.
  atVector<uint8_t> GetRecieve(const atConnectionHandle &handle);

protected:
  void ProcessJobs();
  JobStatus QueueJob(ConnectionJob *pJob, const atConnectionHandle &handle);

  static void DoJob(ConnectionJob *pJob);
  static void DeleteJob(ConnectionJob *pJob);

  static int64_t GetNextHandle();

  atHashMap<atConnectionHandle, Connection*> m_connections;
  
  bool m_running;
  std::thread *m_pJobThread;
  std::mutex m_connectionLock;
  std::mutex m_jobLock;
  atVector<JobStatus> m_jobQueue;
};

#endif // atNetwork_h__
