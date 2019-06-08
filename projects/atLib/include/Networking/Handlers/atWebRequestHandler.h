#ifndef atWebRequestHandler_h__
#define atWebRequestHandler_h__

#include "atHTTPRequest.h"
#include "atWebResponse.h"

class atWebRequestHandler
{
public:
  virtual bool HandleRequest(atWebRequest *pRequest, atWebResponse *pResponse);
};

#endif // atWebRequestHandler_h__
