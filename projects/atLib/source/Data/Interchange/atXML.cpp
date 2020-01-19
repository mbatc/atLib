#include "atXML.h"
#include "atScan.h"
#include "atSeek.h"

#define XML_SEPERATOR " \t\r\n/=<>"

atXML::atXML(const atString &xml)
{
  if (xml.length() > 0)
    Parse(xml);
}

atXML::atXML(const atXML &copy) { *this = copy; }
atXML::atXML(atXML &&move) { *this = std::move(move); }

atXML& atXML::operator=(atXML &&rhs)
{
  m_children = std::move(rhs.m_children);
  m_tag = std::move(rhs.m_tag);
  m_value = std::move(rhs.m_value);
  m_attributes = std::move(rhs.m_attributes);
  return *this;
}

atXML& atXML::operator=(const atXML &rhs)
{
  m_children = rhs.m_children;
  m_tag = rhs.m_tag;
  m_value = rhs.m_value;
  m_attributes = rhs.m_attributes;
  return *this;
}

bool atXML::Parse(const atString &xml)
{
  if (xml.length() == 0)
    return false;

  // Strip XML comments
  atString strippedXml;
  {
    atStringSeeker seeker(xml);
    bool addEnd = true;
    const char *lastPos = seeker.Text();
    while (seeker.SeekTo("<!--"))
    {
      strippedXml += atString(lastPos, seeker.Text());

      if (!seeker.SeekTo("-->", true))
      {
        addEnd = false;
        break;
      }

      lastPos = seeker.Text();
    }

    if (addEnd)
      strippedXml += seeker.GetString();
  }

  if (strippedXml.length() == 0)
    return false;

  { // Build XML elements
    atStringSeeker seeker(strippedXml);
    atXML child;
    atString endTag;
    atVector<atString> tags;
    while (BuildElement(&seeker, &child, &tags, &endTag))
      m_children.push_back(std::move(child));
  }

  return true;
}

void atXML::SetTag(const atString &tag) { m_tag = tag; }
const atString& atXML::GetTag() const { return m_tag; }

void atXML::SetAttributeValue(const atString &name, const atString &value)
{
  if (!m_attributes.TryAdd(name, value))
    m_attributes[name] = value;
}

atString atXML::GetAttributeValue(const atString &name) const
{
  const atString *pValue = m_attributes.TryGet(name);
  return pValue ? *pValue : "";
}

void atXML::SetAttributes(const atHashMap<atString, atString> &attribs) { m_attributes = attribs; }
const atHashMap<atString, atString>& atXML::GetAttributes() const { return m_attributes; }

double atXML::AsFloat() const { return atScan::Float(m_value); }
int64_t atXML::AsInt() const { return atScan::Int(m_value); }
bool atXML::AsBool() const { return m_value == "1" || m_value.compare("true", atSCO_None); }

const atString& atXML::AsString() const { return m_value; }
int64_t atXML::GetChildCount() const { return m_children.size(); }

atXML* atXML::GetChild(const int64_t &index) { return index >= 0 && index < m_children.size() ? &m_children[index] : nullptr; }
const atXML* atXML::GetChild(const int64_t &index) const { return index >= 0 && index < m_children.size() ? &m_children[index] : nullptr; }

void atXML::AddChild(const atXML &xml, const int64_t &index) { m_children.insert(atClamp(index, 0, m_children.size()), xml); }
void atXML::RemoveChild(const int64_t &index)
{
  if (index >= 0 && index < m_children.size())
    m_children.erase(index);
}

static atString _FormatContent(const atString &text)
{
  atString formatted;
  bool first = true;
  bool startsWithWhitespace = false;

  atStringSeeker seeker(text);

  while (seeker.SkipWhitespace())
  {
    startsWithWhitespace |= first && seeker.Text() == seeker.begin();
    const char *start = seeker.Text();
    bool exit = !seeker.SeekToWhitespace();
    formatted += atString(start, seeker.Text());
    if (seeker.Text() != seeker.end())
      formatted += " ";
    if (exit)
      break;
  }

  if (seeker.end() != seeker.Text())
    formatted += (formatted.length() ? " " : "") + seeker.GetString();
  return formatted;
}

bool atXML::BuildElement(atStringSeeker *pSeeker, atXML *pElem, atVector<atString> *pTagStack, atString *pEndTag)
{
  *pElem = atXML();

  pSeeker->SeekTo('<');
  if (pSeeker->Text()[1] == '/')
  {
    if (pEndTag->length() == 0)
    {
      const char *start = pSeeker->Text();
      pSeeker->SeekTo('>');
      *pEndTag = atString(start + 2, pSeeker->Text());
      pSeeker->Seek(start - pSeeker->begin(), atSO_Start);
    }

    return false;
  }

  // Find opening tag
  int64_t closePos = atString::_find_first_of(pSeeker->Text(), '>');
  if (closePos < 0)
    return false;

  bool hasBody = (pSeeker->Text())[closePos - 1] != '/';
  const char *end = pSeeker->Text() + closePos - !hasBody;
  pSeeker->Seek(1);
  int64_t tagEnd = atString::_find_first_of(pSeeker->Text(), XML_SEPERATOR);
  if (tagEnd > closePos || tagEnd <= 0)
    return false;
  pElem->m_tag = atString(pSeeker->Text(), pSeeker->Text() + tagEnd).to_lower();

  if (!hasBody)
    return true;

  // Find attributes
  atVector<atString> attributes;
  atVector<atString> attrValues;
  while (pSeeker->SeekToSet(XML_SEPERATOR))
  {
    pSeeker->SkipWhitespace();
    if (pSeeker->Char() == '=')
    {
      pSeeker->Seek(1); // Skip '=' character
      pSeeker->SkipWhitespace();
      char quoteType = pSeeker->Char();
      bool isQuote = atString::_find_first_of("'\"", quoteType) != AT_INVALID_INDEX;
      pSeeker->Seek(isQuote);
      int64_t end = isQuote ? atString::_find(pSeeker->Text(), quoteType) : atString::_find_first_of(pSeeker->Text(), XML_SEPERATOR);

      if (attrValues.size() >= attributes.size())
        attrValues.at(attributes.size() - 1) = pSeeker->GetString(end);
      pSeeker->Seek(end);
    }
    else
    {
      const char *start = pSeeker->Text();
      if (!pSeeker->SeekToSet(XML_SEPERATOR) || pSeeker->Text() >= end || start == pSeeker->Text())
        break;
      attributes.push_back(atString(start, pSeeker->Text()));
      attrValues.push_back(atString());
    }
  }

  for (int64_t attrIdx = 0; attrIdx < (int64_t)attributes.size(); ++attrIdx)
    pElem->SetAttributeValue(attributes[attrIdx], attrValues[attrIdx]);

  pSeeker->Seek(end - pSeeker->begin() + 1 + !hasBody, atSO_Start);

  // Find any content before any child elements
  int64_t nextTagPos = atString::_find_first_of(pSeeker->Text(), '<');
  pElem->m_value = _FormatContent(pSeeker->GetString(nextTagPos));
  pSeeker->Seek(nextTagPos);

  pTagStack->push_back(pElem->m_tag);

  int64_t tagID = pTagStack->size();

  atXML child;
  while (BuildElement(pSeeker, &child, pTagStack, pEndTag))
  {
    pElem->m_children.push_back(std::move(child));

    // Find any content after this child
    int64_t nextTagPos = atString::_find_first_of(pSeeker->Text(), '<');

    atString extra = _FormatContent(pSeeker->GetString(nextTagPos));
    if (extra.length())
      pElem->m_value += (pElem->m_value.length() ? "\n": "") + extra;

    pSeeker->Seek(nextTagPos);
  }

  pTagStack->pop_back();
  if (pEndTag->length() && pElem->m_tag.compare(*pEndTag, atSCO_None))
  {
    *pEndTag = "";
    pSeeker->SeekTo('>', true);
  }

  return true;
  // *pElem = atXML();
  // 
  // atVector<atString> attributes;
  // atVector<atString> attrValues;
  // atString body;
  // 
  // pElem->m_tag = _ParseTag(pSeeker, &attributes, &attrValues, &body);
  // if (pElem->m_tag.length() == 0)
  //   return false;
  // 
  // for (int64_t attrIdx = 0; attrIdx < (int64_t)attributes.size(); ++attrIdx)
  //   pElem->SetAttributeValue(attributes[attrIdx], attrValues[attrIdx]);
  // 
  // if (pElem->GetAttributeValue("id") == "logo")
  //   int i = 0;
  // 
  // atStringSeeker bodySeeker(body);
  // 
  // // Get Content before nested tag or before the end of tag
  // int64_t nextTag = atString::_find(bodySeeker.Text(), '<');
  // 
  // if (nextTag < 0)
  //   nextTag = body.length();
  // 
  // if (nextTag > 0)
  // {
  //   pElem->m_value = _FormatContent(bodySeeker.GetString(nextTag));
  //   bodySeeker.Seek(nextTag);
  // }
  // 
  // atXML child;
  // while (BuildElement(&bodySeeker, &child))
  // {
  //   pElem->m_children.push_back(std::move(child));
  //   nextTag = atString::_find(bodySeeker.Text(), '<');
  // 
  //   // Get content after nested tag
  //   if (nextTag < 0)
  //     nextTag = bodySeeker.Length();
  // 
  //   if (nextTag > 0)
  //   {
  //     atString newValue = _FormatContent(bodySeeker.GetString(nextTag));
  //     bodySeeker.Seek(nextTag);
  //     if (pElem->m_value.length() && *pElem->m_value.end() != '\n')
  //       pElem->m_value += "\n";
  //     pElem->m_value += newValue;
  //   }
  // }
  // 
  // return true;
}

atString atToString(const atXML &xml)
{
  atString xmlTag = "<" + xml.GetTag();
  for (const atKeyValue<atString, atString> &kvp : xml.GetAttributes())
    xmlTag += " " + kvp.m_key + "=\"" + kvp.m_val + "\"";
  
  atString xmlContent = xml.AsString();
  for (int64_t childIdx = 0; childIdx < xml.GetChildCount(); ++childIdx)
    xmlContent += "\n" + atToString(*xml.GetChild(childIdx));
  if (xmlContent.length())
  {
    xmlTag += ">\n  " + xmlContent.replace("\n", "\n  ") + "\n</" + xml.GetTag() + ">";
  }
  else
  {
    xmlTag + "/>";
  }

  return atString();
}
