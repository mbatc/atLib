#include "atHTTPHeader.h"

atHTTPHeader::atHTTPHeader() {}

atHTTPHeader::atHTTPHeader(const atString &header)
{
  *this = Parse(header); 
}

atHTTPHeader atHTTPHeader::Parse(const atString &header)
{
  atHTTPHeader httpHeader;
  atVector<atString> statements = header.replace("\r\n", "\n").split('\n');
  for (const atString &line : statements)
  {
    int64_t attrStart = line.find_first_of(':');
    if (attrStart > 0)
    {
      atString fieldName = line.substr(0, attrStart);
      atString fieldData = line.substr(line.find_first_not(atString::Whitespace(), attrStart + 1), line.length());
      httpHeader[fieldName] = fieldData;
    }
    else if (httpHeader.head.size() == 0 && line.length() > 0)
    {
      httpHeader.head = line.split(atString::Whitespace(), true, true);
    }
  }
  return httpHeader;
}

atString atHTTPHeader::ToString() const { return ToString(true); }

atString atHTTPHeader::ToString(const bool &hasHead) const
{
  atString header = hasHead ? atString::join(head, ' ') + "\r\n" : "";
  for (auto &kvp : m_fields)
    header += kvp.m_key + ": " + kvp.m_val + "\r\n";
  header += "\r\n";
  return header;
}

atString &atHTTPHeader::HeaderField(const atString &name)
{
  return m_fields.GetOrAdd(name.to_lower());
}

atString atHTTPHeader::HeaderField(const atString &name) const
{
  const atString *pField = m_fields.TryGet(name.to_lower());
  return pField ? *pField : "";
}

void atHTTPHeader::SetField(const atString &name, const atString &data)
{
  m_fields.GetOrAdd(name) = data;
}

bool atHTTPHeader::HasHeaderField(const atString &name) const
{
  return m_fields.Contains(name.to_lower());
}

bool atHTTPHeader::RemoveField(const atString &name)
{
  atString nameLw = name.to_lower();
  if (!m_fields.Contains(nameLw))
    return false;
  m_fields.Remove(nameLw);
  return true;
}

atString& atHTTPHeader::operator[](const atString &field)
{
  return HeaderField(field);
}

atString atHTTPHeader::operator[](const atString &field) const
{
  return HeaderField(field);
}

atString atToString(const atHTTPHeader &header)
{
  return header.ToString();
}
