#ifndef atEUIStyle_h__
#define atEUIStyle_h__

#include "atColor.h"
#include "atFilename.h"
#include "atSerialize.h"
#include "atFont.h"

class at2DRenderer;

class atEUIStyle
{
public:
  atEUIStyle();
  
  enum Color
  {
    Color_None = -1,
    Color_Background,
    Color_Text,
    Color_TextAccent,
    Color_Border,
    Color_Highlight,
    Color_Count
  };

  enum TextStyle
  {
    TS_Regular,
    TS_Bold,
    TS_Italics,
    TS_Count,
  };

  static atEUIStyle* GlobalStyle();

  static Color GetColourID(const atString &name);
  static atString GetColourName(const Color &colID);

  void SetColour(const Color &colID, const atCol &color);
  void SetColour(const Color &colID, const atVec4D &color);

  atCol GetColour(const Color &colID) const;
  atVec4D GetColour4(const Color &colID) const;

  void SetFont(const TextStyle &style, const atFilename &path, const int64_t &scale = 24, const int64_t &resolution = -1);
  atResourceHandleT<atFont> GetFont(const TextStyle &style = TS_Regular);

  friend bool atSerialize(atObjectDescriptor *pSerialized, const atEUIStyle &src);
  friend bool atDeserialize(const atObjectDescriptor &serialized, const atEUIStyle *pDst);

protected:
  struct FontInfo
  {
    atFilename path = "";
    int64_t scale = 24;
    int64_t resolution = -1;
    atResourceHandleT<atFont> font;
  };

  FontInfo m_fonts[TS_Count];
  atCol m_colours[Color_Count];
};

#endif // atEUIStyle_h__
