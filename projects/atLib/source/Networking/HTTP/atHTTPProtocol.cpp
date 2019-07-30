#include "atHTTPProtocol.h"
#include "atHashMap.h"
#include <mutex>

struct _HTTPProcLookup
{
  _HTTPProcLookup()
  {
    for (int64_t i = 0; i < atHTTPMethod_Count; ++i)
      method.Add(atToString((atHTTPMethod)i).to_lower(), (atHTTPMethod)i);

    for (int64_t i = 0; i < atHTTPProtocol_Count; ++i)
      protocol.Add(atToString((atHTTPProtocol)i).to_lower(), (atHTTPProtocol)i);

    for (int64_t i = 0; i < atHTTPResponseCode_Count; ++i)
      response.Add(atToString(atHTTPResponseCodeValue[i]).to_lower(), (atHTTPResponseCode)i);

    for (int64_t i = 0; i < atHTTPResponseCode_Count; ++i)
      responseMsg.Add(atToString((atHTTPResponseCode)i).to_lower(), (atHTTPResponseCode)i);
  }

  atHashMap<atString, atHTTPMethod> method;
  atHashMap<atString, atHTTPProtocol> protocol;
  atHashMap<atString, atHTTPResponseCode> response;
  atHashMap<atString, atHTTPResponseCode> responseMsg;
};

static std::mutex _lookupLock;
static _HTTPProcLookup *_pHTTPLookup = nullptr;

void MakeLookup()
{
  _lookupLock.lock();
  if (!_pHTTPLookup)
    _pHTTPLookup = atNew<_HTTPProcLookup>();
  _lookupLock.unlock();
}

atString atToString(const atHTTPResponseCode &code)
{
  return code < 0 || code > atHTTPResponseCodeMessage.size() ? "" : atHTTPResponseCodeMessage[code];
}

atString atToString(const atHTTPProtocol &protocol)
{
  return protocol < 0 || protocol > atHTTPProtocolNames.size() ? "" : atHTTPProtocolNames[protocol];
}

atString atToString(const atHTTPMethod &method)
{
  return method < 0 || method > atHTTPMethodNames.size() ? "" : atHTTPMethodNames[method];
}

template<> atHTTPMethod atFromString<atHTTPMethod>(const atString &method)
{
  MakeLookup();
  atString lwr = method.to_lower();
  atHTTPMethod *pMethod = _pHTTPLookup->method.TryGet(lwr);
  return pMethod ? *pMethod : atHTTPMethod_Unknown;
}

template<> atHTTPProtocol atFromString<atHTTPProtocol>(const atString &proc)
{
  MakeLookup();
  atString lwr = proc.to_lower();
  atHTTPProtocol *pProc = _pHTTPLookup->protocol.TryGet(lwr);
  return pProc ? *pProc : atHTTPProtocol_Unknown;
}

template<> atHTTPResponseCode atFromString<atHTTPResponseCode>(const atString &code)
{
  MakeLookup();
  atString lwr = code.to_lower();
  atHTTPResponseCode *pCode = _pHTTPLookup->response.TryGet(lwr);
  pCode = pCode ? pCode : _pHTTPLookup->responseMsg.TryGet(lwr);
  return pCode ? *pCode : atHTTPResponseCode_Unknown;
}
