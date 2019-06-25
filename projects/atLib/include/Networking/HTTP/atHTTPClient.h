#ifndef atHTTPClient_h__
#define atHTTPClient_h__

#include "atNetwork.h"
#include "atHTTPRequest.h"
#include "atHTTPResponse.h"

class atHTTPResponseHandler;

class atHTTPClient
{
public:
  atHTTPClient(const atString &url, const uint16_t &port = 80);
  ~atHTTPClient();

  bool Send(const atHTTPRequest &request, atHTTPResponse *pResponse);
  atString URL() const;

  atVector<atHTTPResponseHandler*> m_handlers;

protected:
  atString m_url;
  atNetwork m_network;
  atConnectionHandle m_serverHandle;
};

class atHTTPResponseHandler
{
public:
  virtual bool Handle(const atHTTPResponse &response, atHTTPClient *pClient, atHTTPResponse *pUpdatedResponse) = 0;
};

#endif // atHTTPClient_h__
