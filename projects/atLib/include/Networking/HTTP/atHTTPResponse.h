#ifndef atHTTPResponse_h__
#define atHTTPResponse_h__

#include "atWebResponse.h"
#include "atHTTPHeader.h"

class atHTTPResponse : atWebResponseInterface<atHTTPResponse>
{
public:
  atHTTPResponse(const atHTTPResponseCode &code, const atHTTPProtocol &protocol = atHTTP_Ver_1_1);
  atHTTPResponse(const atString &response);

  atString ToString() const;

  void SetCode(const atHTTPResponseCode &code);
  void SetProtocol(const atHTTPProtocol &protocol);

  int64_t CodeValue() const;
  atHTTPResponseCode Code() const;
  atHTTPProtocol Protocol() const;

  atString body;
  atHTTPHeader header;
};

atString atToString(const atHTTPResponse &header);

#endif // atHTTPResponse_h__
