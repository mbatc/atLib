#include "atHTTPClient.h"

atHTTPClient::atHTTPClient(const atString &url, const uint16_t &port) : m_url(url)
{
  atNetwork::JobStatus connectJob = m_network.Connect(url, port);
  while (!connectJob.Done());
  m_serverHandle = connectJob.Handle();
}

atHTTPClient::~atHTTPClient() { m_network.Disconnect(m_serverHandle); }

bool atHTTPClient::Send(const atHTTPRequest &request, atHTTPResponse *pResponse)
{
  if (!m_network.IsConnected(m_serverHandle))
    return false;
  
  {
    atNetwork::JobStatus sendJob = m_network.Send(m_serverHandle, atVector<uint8_t>(request.ToString()));
    while (!sendJob.Done());
    if (sendJob.Failed())
      return false;
  }

  {
    atNetwork::JobStatus recvJob = m_network.Receive(m_serverHandle);
    while (!recvJob.Done());
    if (recvJob.Failed())
      return false;

    atHTTPResponse res(m_network.GetRecieve(m_serverHandle));
    atHTTPResponse finalResponse = res;

    bool handled = false;
    for (atHTTPResponseHandler *pHandler : m_handlers)
    {
      if (pHandler->Handle(res, this, &finalResponse))
      {
        handled = true;
        break;
      }
      else
      {
        res = finalResponse;
      }
    }

    if (pResponse)
      *pResponse = finalResponse;
    return handled;
  }
}

atString atHTTPClient::URL() const { return m_url; }
