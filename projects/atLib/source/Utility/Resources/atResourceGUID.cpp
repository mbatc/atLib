#include "atResourceGUID.h"

atResourceGUID::atResourceGUID(atResourceGUID &&o) { *this = std::move(o); }
atResourceGUID::atResourceGUID(const atResourceGUID &o) { *this = o; }

atResourceGUID::atResourceGUID(const atString &resourceType, const atObjectDescriptor &request)
  : m_resourceType(resourceType)
{
  m_request.Set(request);
}

bool atResourceGUID::operator==(const atResourceGUID &rhs) const { return m_resourceType == rhs.m_resourceType && m_request == rhs.m_request; }
bool atResourceGUID::operator!=(const atResourceGUID &rhs) const { return !(*this == rhs); }

atResourceGUID& atResourceGUID::operator=(atResourceGUID &&rhs)
{
  m_resourceType = std::move(rhs.m_resourceType);
  m_request = std::move(rhs.m_request);
  return *this;
}

atResourceGUID& atResourceGUID::operator=(const atResourceGUID &rhs)
{
  m_resourceType = rhs.m_resourceType;
  m_request.Set(rhs.m_request);
  return *this;
}

int64_t atStreamRead(atReadStream *pStream, atResourceGUID *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    size += atStreamRead(pStream, &pData[i].m_resourceType, 1);
    size += atStreamRead(pStream, &pData[i].m_request, 1);
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, atResourceGUID *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    size += atStreamWrite(pStream, &pData[i].m_resourceType, 1);
    size += atStreamWrite(pStream, &pData[i].m_request, 1);
  }
  return size;
}
