#ifndef atFont_h__
#define atFont_h__

#include "atHashMap.h"
#include "atFilename.h"
#include "atImage.h"
#include "3rdParty/stb/stb_truetype.h"

class atFont
{
public:
  struct Glyph
  {
    atVec2F tl;
    atVec2F br;
    float xOff;
    float yOff;
    int64_t advance;
    int64_t width;
    int64_t height;
  };

  atFont(const atFilename &filename, const int64_t scale = 32);
  ~atFont();

  Glyph GetGlyph(const uint32_t codepoint);
  const atImage &Bitmap() const;
  const atFilename &Filename() const;

protected:
  Glyph LoadGlyph(const uint32_t codepoint);
  void ResizeBitmap(const atVec2I &size);

  atHashMap<uint32_t, Glyph> m_glyphs;
  atVector<uint8_t> m_fd;
  atImage m_bitmap;

  stbtt_fontinfo m_font;
  atFilename m_filename;
  float m_scale;

  atVec2I m_nextPos;
  atVec2I m_lastSize;
};

#endif // atFont_h__
