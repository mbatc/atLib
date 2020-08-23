#ifndef atResourceGUID_h__
#define atResourceGUID_h__

#include "atObjectDescriptor.h"

class atResourceGUID
{
public:
  atResourceGUID(atResourceGUID &&o);
  atResourceGUID(const atResourceGUID &o);
  atResourceGUID(const atString &resourceType = "", const atObjectDescriptor &request = atObjectDescriptor());

  bool operator==(const atResourceGUID &rhs) const;
  bool operator!=(const atResourceGUID &rhs) const;

  friend int64_t atStreamRead(atReadStream *pStream, atResourceGUID *pData, const int64_t count);
  friend int64_t atStreamWrite(atWriteStream *pStream, atResourceGUID *pData, const int64_t count);

  atResourceGUID& operator=(atResourceGUID &&rhs);
  atResourceGUID& operator=(const atResourceGUID &rhs);

protected:
  atString m_resourceType;
  atObjectDescriptor m_request;
};

#endif // atResourceGUID_h__