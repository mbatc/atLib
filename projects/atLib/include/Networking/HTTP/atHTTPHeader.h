#ifndef atHTTPHeader_h__
#define atHTTPHeader_h__

#include "atHTTPProtocol.h"
#include "atHashMap.h"

class atHTTPHeader
{
public:
  atHTTPHeader();
  atHTTPHeader(const atString &header);

  static atHTTPHeader Parse(const atString &header);

  atString ToString() const;
  atString ToString(const bool &hasHead) const;

  atString& HeaderField(const atString &name);
  atString HeaderField(const atString &name) const;
  
  void SetField(const atString &name, const atString &data);

  bool HasHeaderField(const atString &name) const;
  bool RemoveField(const atString &name);

  atVector<atString> head;

  atString& operator[](const atString &field);
  atString operator[](const atString &field) const;

protected:
  atHashMap<atString, atString> m_fields;
};

atString atToString(const atHTTPHeader &header);

#endif // atHTTPHeader