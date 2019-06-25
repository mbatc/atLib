#include "atHTTPRequest.h"

atHTTPRequest::atHTTPRequest() {}

atHTTPRequest::atHTTPRequest(const atHTTPMethod &method, const atString &uri, const atHTTPProtocol &protocol)
{
  SetMethod(method);
  SetURI(uri);
  SetProtocol(protocol);
}

atHTTPRequest::atHTTPRequest(const atString &request)
{
  atString req = request.replace("\r\n", "\n");
  int64_t headerEnd = req.find("\r\n\r\n") + 4;
  header = atHTTPHeader::Parse(atString(req.substr(0, headerEnd)));
  body = req.substr(headerEnd, req.length());
}

atString atHTTPRequest::ToString() const
{
  return atString(header) + body;
}

void atHTTPRequest::SetURI(const atString &uri)
{
  header.head.resize(3);
  header.head[1] = uri;
}

void atHTTPRequest::SetMethod(const atHTTPMethod &method)
{
  header.head.resize(3);
  header.head[0] = atString(method);
}

void atHTTPRequest::SetProtocol(const atHTTPProtocol &protocol)
{
  header.head.resize(3);
  header.head[2] = atString(protocol);
}

const atString& atHTTPRequest::URI() const
{
  return header.head[1];
}

atHTTPMethod atHTTPRequest::Method() const
{
  return (atHTTPMethod)header.head[0];
}

atHTTPProtocol atHTTPRequest::Protocol() const
{
  return (atHTTPProtocol)header.head[2];
}

atString atToString(const atHTTPRequest &header)
{
  return header.ToString();
}
