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
    atVec2F tl;       // Top left UV of the character (0-1)
    atVec2F br;       // Bottom right UV of the character (0-1)
    int32_t xOff;       // Horizontal offset to apply when drawing character in Pixels
    int32_t yOff;       // Vertical offset to apply when drawing character in Pixels
    int64_t advance;  // Distance to move cursor horizontally 
    int64_t width;    // Width of the glyph in pixels
    int64_t height;   // Height of the glyph in pixels
  };

  atFont(const atFilename &filename, const int64_t scale = 32);
  atFont(const atFont &copy);
  atFont(atFont &&move);
  ~atFont();

  // Get the glyph associated with a specific codepoint
  // 
  // Dynamic caching is used so a glyph is only loaded 
  // when it is first used, and stored for later
  //
  // Glyphs are not loaded if they are not used
  Glyph GetGlyph(const uint32_t codepoint);

  // Get the font texture
  const atImage &Bitmap() const;

  // Get the font filename
  const atFilename &Filename() const;

  // This value should not be stored as a call to GetTextureID() will 
  // also update the Texture if new glyphs are loaded
  int64_t GetTextureID(const bool updateTexture = true);

  int64_t Height() const;

protected:
  Glyph LoadGlyph(const uint32_t codepoint);
  void ResizeBitmap(const atVec2I &size);

  bool m_stale = false;
  atImage m_bitmap;
  atVector<uint8_t> m_fd;
  atHashMap<uint32_t, Glyph> m_glyphs;

  atFilename m_filename;
  stbtt_fontinfo m_font = { 0 };
  float m_scale = 0;
  int64_t m_height = 0;

  int64_t m_texID = AT_INVALID_ID;
  atVec2I m_nextPos = atVec2I::zero();
  atVec2I m_lastSize = atVec2I::zero();
};

#endif // atFont_h__
