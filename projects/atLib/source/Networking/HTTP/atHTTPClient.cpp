#include "atHTTPClient.h"

atHTTPClient::atHTTPClient(const atString &url, const uint16_t &port)
  : m_url(url)
  , m_port(port)
  , m_client(std::string((url + ":" + atString((int64_t)port)).c_str()), false)
{}

bool atHTTPClient::Send(const atHTTPRequest &request, atHTTPResponse *pResponse)
{
  std::string method = std::string(atString(request.Method()).c_str());
  std::string uri = std::string(request.URI().c_str());
  std::string body = std::string(request.body.c_str());
  auto response = m_client.request(method, uri, body);
  printf(atString(response->content.string()).c_str());
  return true;
  // 
  // atHTTPResponse finalResponse = res;
  // 
  // bool handled = false;
  // for (atHTTPResponseHandler *pHandler : m_handlers)
  // {
  //   if (pHandler->Handle(res, this, &finalResponse))
  //   {
  //     handled = true;
  //     break;
  //   }
  //   else
  //   {
  //     res = finalResponse;
  //   }
  // }
  // 
  // if (pResponse)
  //   *pResponse = finalResponse;
  // return handled;
}

atString atHTTPClient::URL() const { return m_url; }

uint16_t atHTTPClient::Port() const { return m_port; }
