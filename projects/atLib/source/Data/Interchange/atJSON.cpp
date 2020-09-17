#include "atJSON.h"
#include "atScan.h"

static const atString _delimterSet = atString("[]{},:");
static const atString _delimiterAndWhitespaceSet = atString::Whitespace() + _delimterSet;

static atString _ParseString(const char **pJson, int64_t *pLength);
static atJSON _ParseObject(const char **pJson, int64_t *pLength);
static atJSON _ParseArray(const char **pJson, int64_t *pLength);
static atJSON _ParseValue(const char **pJson, int64_t *pLength);

atJSON::atJSON(atJSON &&o) { *this = std::move(o); }
atJSON::atJSON(const atJSON &o) { *this = o; }
atJSON::atJSON(const atString &json) { Parse(json); }
atJSON::~atJSON() { MakeNull(); }

const atJSON& atJSON::operator=(atJSON &&o)
{
  m_isString = o.m_isString;
  m_pValue = o.m_pValue;
  m_pArray = o.m_pArray;
  m_pObject = o.m_pObject;
  o.m_pArray = nullptr;
  o.m_pValue = nullptr;
  o.m_pObject = nullptr;
  return *this;
}

const atJSON& atJSON::operator=(const atJSON &o)
{
  if (o.IsObject())
  {
    MakeObject();
    *m_pObject = *o.m_pObject;
  }

  if (o.IsArray())
  {
    MakeArray();
    *m_pArray = *o.m_pArray;
  }

  if (o.IsValue() || o.IsString())
  {
    MakeValue(o.IsString());
    *m_pValue = *o.m_pValue;
  }

  return *this;
}

atString atJSON::ToString(const bool prettyPrint) const
{
  if (IsNull()) return "null";

  atString ret;
  if (IsObject() || IsArray())
  {
    ret += IsArray() ? "[" : "{";
    bool useNewLines = false;
    atVector<atString> pairs;
    if (IsObject())
    {
      useNewLines = true;
      for (auto &kvp : *m_pObject)
        pairs.push_back("\"" + kvp.m_key + (prettyPrint ? "\": " : "\":") + kvp.m_val.ToString(prettyPrint));
    }

    if (IsArray())
    {
      for (auto &val : *m_pArray)
      {
        useNewLines |= val.IsObject() || val.IsArray();
        pairs.push_back(val.ToString(prettyPrint));
      }
    }
    atString valueString = atString::join(pairs, prettyPrint ? (useNewLines ? ",\n" : ", ") : ",");
    if (prettyPrint && useNewLines)
      valueString = "\n  " + valueString.replace("\n", "\n  ") + "\n";

    ret += valueString;
    ret += IsArray() ? "]" : "}";
  }
  else if (IsString())
  {
    ret = "\"" + *m_pValue + "\"";
  }
  else
  {
    return *m_pValue;
  }

  return ret;
}

void atJSON::MakeNull()
{
  if (m_pValue) atDelete(m_pValue);
  if (m_pObject) atDelete(m_pObject);
  if (m_pArray) atDelete(m_pArray);
  m_pValue = nullptr;
  m_pObject = nullptr;
  m_pArray = nullptr;
  m_isString = false;
}

void atJSON::MakeArray()
{
  if (IsArray())
    return;
  MakeNull();
  m_pArray = atNew<atVector<atJSON>>();
}

void atJSON::MakeObject()
{
  if (IsObject())
    return;
  MakeNull();
  m_pObject = atNew<atHashMap<atString, atJSON>>();
}

void atJSON::MakeValue(const bool &isString)
{
  if (!(IsValue() || IsString()))
  {
    MakeNull();
    m_pValue = atNew<atString>();
  }
  m_isString = isString;
}

int64_t atJSON::Parse(const char *json, const int64_t &length)
{
  if (length == 0)
    return 0;
  int64_t len = length;
  *this = _ParseValue(&json, &len);
  return length - len;
}

void atJSON::SetElement(const int64_t &index, const atJSON &value)
{
  MakeArray();
  GetElement(index) = value;
}

void atJSON::SetMember(const atString &key, const atJSON &value)
{
  MakeObject();
  GetMember(key) = value;
}

int64_t atJSON::ElementCount() const
{
  if (IsNull() || IsValue() || IsString())
    return 0;
  if (IsObject())
    return m_pObject->Size();
  return m_pArray->size();
}

atJSON& atJSON::GetMember(const atString &key)
{
  MakeObject();
  atJSON *pMember = TryGetMember(key);
  if (!pMember)
  {
    m_pObject->Add(key, atJSON());
    return *m_pObject->TryGet(key);
  }
  return *pMember;
}

atJSON& atJSON::GetElement(const int64_t &index)
{
  MakeArray();
  if (m_pArray->size() <= index)
    m_pArray->resize(index + 1, atJSON());
  return m_pArray->at(index);
}

void atJSON::SetBool(const bool &val, const bool verbose) { SetValue(atToString(val, verbose), false); }
void atJSON::SetInt(const int &val) { SetValue(val, false); }
void atJSON::SetDecimal(const float &val) { SetValue(val, false); }
void atJSON::SetDecimal(const double &val) { SetValue(val, false); }

int64_t atJSON::ToInt() const { return GetValueAs<int64_t>(); }
float atJSON::ToFloat() const { return GetValueAs<float>(); }
double atJSON::ToDouble() const { return GetValueAs<double>(); }
bool atJSON::ToBool() const { return GetValueAs<bool>(); }

bool atJSON::IsNull() const { return !(IsObject() || IsString() || IsValue() || IsArray()); }
bool atJSON::IsArray() const { return m_pArray != nullptr; }
bool atJSON::IsObject() const { return m_pObject != nullptr; }
bool atJSON::IsString() const { return m_pValue != nullptr && m_isString; }
bool atJSON::IsValue() const { return m_pValue != nullptr && !m_isString; }

atJSON* atJSON::TryGetMember(const atString &key) const { return IsObject() ? m_pObject->TryGet(key) : nullptr; }
atJSON* atJSON::TryGetElement(const int64_t &index) const { return IsArray() ? &m_pArray->at(index) : nullptr; }

atVector<atString> atJSON::GetKeys() const { return IsObject() ? m_pObject->GetKeys() : atVector<atString>{}; }

const atString& atJSON::Value() const { return *m_pValue; }

const atVector<atJSON>& atJSON::Array() const { return *m_pArray; }
const atHashMap<atString, atJSON>& atJSON::Object() const { return *m_pObject; }

atJSON& atJSON::operator[](const atString &key) { return GetMember(key); }
atJSON& atJSON::operator[](const int64_t &index) { return GetElement(index); }

const atJSON& atJSON::operator[](const atString &key) const { return *TryGetMember(key); }
const atJSON& atJSON::operator[](const int64_t &index) const { return *TryGetElement(index); }

bool atJSON::Parse(const atString &json) { return Parse(json.c_str(), json.length()) == json.length(); }

atString atToString(const atJSON &json) { return json.ToString(); }
atJSON atFromString(const atString &json) { return atJSON(json); }

static bool _Seek(const char **pJson, int64_t *pLength, int64_t dist)
{
  if (*pLength == 0)
    return false;
  if (dist < 0) dist = *pLength;
  dist = atMin(*pLength, dist);
  (*pJson) += dist;
  (*pLength) -= dist;
  return *pLength != 0;
}

static bool _SkipDelimiter(const char **pJson, int64_t *pLength) { return _Seek(pJson, pLength, atString::_find_first_not(*pJson, _delimterSet)); }
static bool _SkipToDelimiter(const char **pJson, int64_t *pLength) { return _Seek(pJson, pLength, atString::_find_first_of(*pJson, _delimterSet)); }
static bool _SkipWhitespace(const char **pJson, int64_t *pLength) { return _Seek(pJson, pLength, atString::_find_first_not(*pJson, atString::Whitespace())); }

static atString _ParseString(const char **pJson, int64_t *pLength)
{
  atString ret;
  _SkipWhitespace(pJson, pLength);
  if (**pJson != '"')
    return ret;

  int64_t len = 0;
  ret = atScan::Quote(pJson, &len, *pLength);
  *pLength -= len;
  return ret;
}

static atJSON _ParseObject(const char **pJson, int64_t *pLength)
{
  _SkipWhitespace(pJson, pLength);
  atJSON ret;
  if (**pJson != '{')
    return ret;
  _Seek(pJson, pLength, 1);
  atString key;
  atJSON value;

  while (_SkipWhitespace(pJson, pLength))
  {
    key = _ParseString(pJson, pLength);
    while (**pJson != ':' || *pLength == 0)
      if (!_Seek(pJson, pLength, 1) || !_SkipToDelimiter(pJson, pLength))
        return ret;

    _Seek(pJson, pLength, 1);
    value = _ParseValue(pJson, pLength);
    ret.SetMember(key, value); // TODO: Add error reporting if this fails
    _SkipToDelimiter(pJson, pLength);
    if (**pJson == '}')
      break;
    _Seek(pJson, pLength, 1);
  }
  _Seek(pJson, pLength, 1);
  return ret;
}

static atJSON _ParseArray(const char **pJson, int64_t *pLength)
{
  atJSON ret;
  if (**pJson != '[')
    return ret;
  _Seek(pJson, pLength, 1);
  atJSON value;
  int64_t arrayLen = 0;
  while (_SkipWhitespace(pJson, pLength))
  {
    value = _ParseValue(pJson, pLength);
    ret.SetElement(arrayLen, value);
    arrayLen++;

    _SkipToDelimiter(pJson, pLength);
    if (**pJson == ']')
      break;
    _Seek(pJson, pLength, atString::_find_first(*pJson, ',') + 1);
  }

  return ret;
}

static atJSON _ParseValue(const char **pJson, int64_t *pLength)
{
  _SkipWhitespace(pJson, pLength);
  atJSON ret;
  switch (**pJson)
  {
  case '{': return _ParseObject(pJson, pLength);
  case '[': return _ParseArray(pJson, pLength);
  case '"': ret.SetValue(_ParseString(pJson, pLength)); break;
  default:
    const char *start = *pJson;
    _Seek(pJson, pLength, atString::_find_first_of(*pJson, _delimiterAndWhitespaceSet));
    ret.SetValue(atString(start, *pJson), false);
  }
  return ret;
}