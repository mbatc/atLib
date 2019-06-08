#ifndef atHTTPRequest_h__
#define atHTTPRequest_h__

#include "atWebRequest.h"
#include "atHTTPHeader.h"

class atHTTPRequest : public atWebRequestInterface<atHTTPRequest>
{
public:
  atHTTPRequest(const atString &request);
  atHTTPRequest(const atHTTPMethod &method, const atString &uri = "/", const atHTTPProtocol &protocol = atHTTP_Ver_1_1);
  
  atString ToString() const;

  void SetURI(const atString &uri);
  void SetMethod(const atHTTPMethod &method);
  void SetProtocol(const atHTTPProtocol &protocol);

  const atString& URI() const;
  atHTTPMethod Method() const;
  atHTTPProtocol Protocol() const;

  atHTTPHeader header;
  atString body;
};

atString atToString(const atHTTPRequest &header);

#endif // atHTTPRequest_h__
