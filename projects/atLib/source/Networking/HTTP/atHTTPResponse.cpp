#include "atHTTPResponse.h"

atHTTPResponse::atHTTPResponse(const atHTTPResponseCode &code, const atHTTPProtocol &protocol)
{
  SetCode(code);
  SetProtocol(protocol);
}

atHTTPResponse::atHTTPResponse(const atString &response)
{
  atString res = response.replace("\r\n", "\n");
  int64_t headerEnd = res.find("\n\n") + 2;
  header = atHTTPHeader::Parse(res.substr(0, headerEnd));
  body = res.substr(headerEnd, res.length());
}

atString atHTTPResponse::ToString() const
{
  return atString(header) + body;
}

void atHTTPResponse::SetCode(const atHTTPResponseCode &code)
{
  header.head[1] = atString(atHTTPResponseCodeValue[code]);
  header.head[2] = atString(code);
}

void atHTTPResponse::SetProtocol(const atHTTPProtocol &protocol)
{
  header.head[0] = atString(protocol);
}

atHTTPProtocol atHTTPResponse::Protocol() const
{
  return (atHTTPProtocol)header.head[0];
}

int64_t atHTTPResponse::CodeValue() const
{
  return (int64_t)header.head[1];
}

atHTTPResponseCode atHTTPResponse::Code() const
{
  return (atHTTPResponseCode)header.head[1];
}

atString atToString(const atHTTPResponse &header)
{
  return header.ToString();
}
