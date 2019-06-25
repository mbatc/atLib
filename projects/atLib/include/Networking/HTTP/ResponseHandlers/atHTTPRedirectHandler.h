#ifndef atHTTPRedirectHandler_h__
#define atHTTPRedirectHandler_h__

#include "atHTTPClient.h"

class atHTTPRedirectHandler : public atHTTPResponseHandler
{
public:
  bool Handle(const atHTTPResponse &response, atHTTPClient *pClient, atHTTPResponse *pResponse);
};

#endif // atHTTPRedirectHandler_h__
