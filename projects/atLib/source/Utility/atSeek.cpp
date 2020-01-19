#include "atSeek.h"
#include "atString.h"

static inline bool _Seek(const char **ppText, const int64_t &dist)
{
  if (dist != AT_INVALID_INDEX)
  {
    *ppText += dist;
    return true;
  }

  return false;
}

bool atSeek::SeekTo(const char **ppText, const char target, const bool &positionAtEnd) { return _Seek(ppText, atString::_find(*ppText, target, 0) + (positionAtEnd ? 1 : 0)); }
bool atSeek::SeekTo(const char **ppText, const char *target, const bool &positionAtEnd) { return _Seek(ppText, atString::_find(*ppText, target, 0) + (positionAtEnd ? strlen(target) : 0)); }
bool atSeek::SeekToSet(const char **ppText, const char *charList) { return _Seek(ppText, atString::_find_first_of(*ppText, charList)); }
bool atSeek::Skip(const char **ppText, const char c) { return _Seek(ppText, atString::_find_first_not(*ppText, c)); }
bool atSeek::Skip(const char **ppText, const char *charList) { return _Seek(ppText, atString::_find_first_not(*ppText, charList)); }
bool atSeek::SkipWhitespace(const char **ppText) { return _Seek(ppText, atString::_find_first_not(*ppText, atString::Whitespace())); }
bool atSeek::SeekToWhitespace(const char **ppText) { return _Seek(ppText, atString::_find_first_of(*ppText, atString::Whitespace())); }

atStringSeeker::atStringSeeker(const atString &text)
{
  m_text = text;
  m_pText = m_text.c_str();
}

bool atStringSeeker::Seek(int64_t pos, const atSeekOrigin &origin)
{
  switch (origin)
  {
  case atSO_Current:
    break;
  case atSO_Start:
    m_pText = m_text.begin();
    break;
  case atSO_End:
    m_pText = m_text.end();
    pos = -pos;
    break;
  }

  m_pText += pos;

  bool result = m_pText < m_text.begin() || m_pText > m_text.end();
  m_pText = atClamp(m_pText, m_text.begin(), m_text.end());
  return result;
}

int64_t atStringSeeker::Length() const { return m_text.length(); }
const char* atStringSeeker::Text() const { return m_pText; }
const char* atStringSeeker::begin() const { return m_text.begin(); }
const char* atStringSeeker::end() const { return m_text.end(); }

atString atStringSeeker::GetString(const int64_t &endIdx, const int64_t &startIdx)
{
  const char *startPos = startIdx == AT_INVALID_INDEX ? m_pText : (m_pText + startIdx);
  const char *endPos = endIdx == AT_INVALID_INDEX ?  m_text.end() : (m_pText + endIdx);
  return atString(atClamp(startPos, begin(), end()), atClamp(endPos, begin(), end()));
}

char atStringSeeker::Char() const { return *Text(); }

bool atStringSeeker::SeekTo(const char target, const bool &positionAtEnd) { return DoSeek(atSeek::SeekTo(&m_pText, target, positionAtEnd)); }
bool atStringSeeker::SeekTo(const char *target, const bool &positionAtEnd) { return DoSeek(atSeek::SeekTo(&m_pText, target, positionAtEnd)); }
bool atStringSeeker::SeekToSet(const char *charList) { return DoSeek(atSeek::SeekToSet(&m_pText, charList)); }
bool atStringSeeker::Skip(const char c) { return DoSeek(atSeek::Skip(&m_pText, c)); }

bool atStringSeeker::Skip(const char *charList) { return DoSeek(atSeek::Skip(&m_pText, charList)); }
bool atStringSeeker::SkipWhitespace() { return DoSeek(atSeek::SkipWhitespace(&m_pText)); }
bool atStringSeeker::SeekToWhitespace() { return DoSeek(atSeek::SeekToWhitespace(&m_pText)); }

bool atStringSeeker::DoSeek(const bool &success)
{
  if (success)
    return true;

  m_pText = end();
  return false;
}
