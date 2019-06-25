#ifndef atHTTPClient_h__
#define atHTTPClient_h__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef min
#undef max

#include "server_https.hpp"
#include "client_https.hpp"

#include "atHTTPRequest.h"
#include "atHTTPResponse.h"

class atHTTPResponseHandler;

class atHTTPClient
{
public:
  atHTTPClient(const atString &url, const uint16_t &port = 80);

  bool Send(const atHTTPRequest &request, atHTTPResponse *pResponse);
  atString URL() const;
  uint16_t Port() const;

  atVector<atHTTPResponseHandler*> m_handlers;

protected:
  atString m_url;
  uint16_t m_port;
  SimpleWeb::Client<SimpleWeb::HTTPS> m_client;
};

class atHTTPResponseHandler
{
public:
  virtual bool Handle(const atHTTPResponse &response, atHTTPClient *pClient, atHTTPResponse *pUpdatedResponse) = 0;
};

#endif // atHTTPClient_h__
