#include "atHTTPRedirectHandler.h"

bool atHTTPRedirectHandler::Handle(const atHTTPResponse &response, atHTTPClient *pClient, atHTTPResponse *pResponse)
{
  atHTTPResponseCode code = response.Code();
  if (code != atHTTP_301 && code != atHTTP_307 && code != atHTTP_503)
    return false;

  atHTTPRequest request(atHTTP_Get);
  request.SetURI(response.header["location"]);
  request.header["host"] = pClient->URL();
  return pClient->Send(request, pResponse);
}
