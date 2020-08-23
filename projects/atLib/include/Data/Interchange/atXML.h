#ifndef atXML_h__
#define atXML_h__

#include "atString.h"
#include "atSeek.h"
#include "atHashMap.h"

class atXML
{
public:
  atXML(const atString &xml = "");
  atXML(const atXML &copy);
  atXML(atXML &&move);

  atXML& operator=(atXML &&rhs);
  atXML& operator=(const atXML &rhs);

  bool Parse(const atString &xml);

  // Tag
  void SetTag(const atString &tag);
  const atString& GetTag() const;

  // Attributes
  void SetAttribute(const atString &name, const atString &value);
  atString GetAttribute(const atString &name) const;
  void SetAttributes(const atHashMap<atString, atString> &attribs);
  const atHashMap<atString, atString>& GetAttributes() const;

  // Values
  double AsFloat() const;
  int64_t AsInt() const;
  bool AsBool() const;
  const atString& AsString() const;

  void Set(const bool &value);
  void Set(const double &value);
  void Set(const int64_t &value);
  void Set(const atString &value);

  // Children
  int64_t GetChildCount() const;

  atXML* GetChild(const int64_t &index);
  const atXML* GetChild(const int64_t &index) const;

  void AddChild(const atXML &xml, const int64_t &index = INT64_MAX);
  void RemoveChild(const int64_t &index);

  friend atString atToString(const atXML &xml);

protected:
  static int64_t BuildElement(atStringSeeker *pSeeker, atXML *pElem, atVector<atString> *pTagStack, atString *pEndTag);

  atString m_tag;
  atString m_value;
  atHashMap<atString, atString> m_attributes;
  atVector<atXML> m_children;
};

#endif // atXML_h__
