#ifndef atSeek_h__
#define atSeek_h__

#include "atStreamSeekable.h"
#include "atString.h"

class atSeek
{
public:
  // Seek to the next instance of the character 'target'
  static bool SeekTo(const char **ppText, const char target, const bool &positionAtEnd = false);

  // Seek to the next instance of the string 'target'
  // If positionAtEnd is true, then ppText will point to the end of the occurrence of 'target'
  static bool SeekTo(const char **ppText, const char *target, const bool &positionAtEnd = false);

  // Seek to the next character that is contained in 'charList'  
  static bool SeekToSet(const char **ppText, const char *charList);

  // Seek to the next character that does not equal 'c'
  static bool Skip(const char **ppText, const char c);

  // Seek to the next character that is not contained in 'charList'
  static bool Skip(const char **ppText, const char *charList);

  // Seek to the next non-whitespace character
  static bool SkipWhitespace(const char **ppText);

  // Seek to the next whitespace character
  static bool SeekToWhitespace(const char **ppText);
};

class atStringSeeker
{
public:
  // Create a seek-able string
  atStringSeeker(const atString &text);

  // Manually seek 
  bool Seek(int64_t pos = 0, const atSeekOrigin &origin = atSO_Current);

  // Get the length of the text
  int64_t Length() const;

  // Get the current character
  char Char() const;

  // Get the current text pointed to
  const char* Text() const;
  const char* begin() const;
  const char* end() const;

  atString GetString(const int64_t &endIdx = AT_INVALID_INDEX, const int64_t &start = AT_INVALID_INDEX);

  // Seek to the next instance of the character 'target'
  bool SeekTo(const char target, const bool &positionAtEnd = false);

  // Seek to the next instance of the string 'target'
  // If positionAtEnd is true, then ppText will point to the end of the occurrence of 'target'
  bool SeekTo(const char *target, const bool &positionAtEnd = false);

  // Seek to the next character that is contained in 'charList'  
  bool SeekToSet(const char *charList);

  // Seek to the next character that does not equal 'c'
  bool Skip(const char c);

  // Seek to the next character that is not contained in 'charList'
  bool Skip(const char *charList);

  // Seek to the next non-whitespace character
  bool SkipWhitespace();

  // Seek to the next whitespace character
  bool SeekToWhitespace();

protected:
  bool DoSeek(const bool &success);
  atString m_text;
  const char *m_pText;
};

#endif // atSeek_h__
