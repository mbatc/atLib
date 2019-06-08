#ifndef atWebClient_h__
#define atWebClient_h__

#include "atVector.h"
#include "atWebRequest.h"
#include "atWebResponse.h"

class atWebResponseHandler;
class atWebRequestHandler;

class atWebClient
{
public:
  bool SendRequest(const atWebRequest *pRequest);
  template<typename T> bool SendRequest(const T &request);

  atVector<atWebResponseHandler*> m_handlers;
};

template<typename T>
inline bool atWebClient::SendRequest(const T &request)
{
  static_assert(std::is_base_of<atWebRequest, T>::value, "T must be derived from atWebRequest");
  return SendRequest((atWebRequest*)&request);
}

#endif // atWebClient_h__
