#include "atFont.h"
#include "atFile.h"
#include "atHardwareTexture.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "3rdParty/stb/stb_truetype.h"

atFont::~atFont() {}
const atImage &atFont::Bitmap() const { return m_bitmap; }
const atFilename &atFont::Filename() const { return m_filename; }

int64_t atFont::GetTextureID(const bool updateTexture)
{
  if (m_texID == AT_INVALID_ID)
    m_texID = atHardwareTexture::UploadTexture(m_bitmap);
  else if (m_stale && updateTexture)
  {
    atHardwareTexture::UpdateTexture(m_texID, m_bitmap);
    m_stale = false;
  }
  return m_texID;
}

int64_t atFont::Height() const { return m_height; }

atFont::atFont(const atFilename &filename, const int64_t scale)
  : m_bitmap(atVec2I{ 512, 512 })
  , m_filename(filename)
  , m_font({ 0 })
{
  atFile reader;
  if (!reader.Open(filename, atFM_Read | atFM_Binary))
    return;
  m_fd.resize(reader.Info().Size());
  reader.Read(m_fd.data(), m_fd.size());
  m_font.userdata = nullptr;
  if (!stbtt_InitFont(&m_font, m_fd.data(), stbtt_GetFontOffsetForIndex(m_fd.data(), 0)))
    return;
  m_height = scale;
  m_scale = stbtt_ScaleForPixelHeight(&m_font, (float)scale);
}

atFont::atFont(const atFont &copy)
{
  m_texID = AT_INVALID_ID;
  m_lastSize = copy.m_lastSize;
  m_nextPos = copy.m_nextPos;
  m_bitmap = copy.m_bitmap;
  m_fd = copy.m_fd;
  m_font = copy.m_font;
  m_font.data = m_fd.data();
  m_glyphs = copy.m_glyphs;
  m_scale = copy.m_scale;
  m_stale = true;
  m_height = copy.m_height;
}

atFont::atFont(atFont &&move)
  : m_fd(std::move(move.m_fd))
  , m_filename(std::move(move.m_filename))
  , m_font(std::move(move.m_font))
  , m_glyphs(std::move(move.m_glyphs))
  , m_stale(move.m_stale)
  , m_texID(move.m_texID)
  , m_bitmap(std::move(move.m_bitmap))
  , m_scale(move.m_scale)
  , m_height(move.m_height)
{
  move.m_texID = AT_INVALID_ID;
  move.m_stale = true;
}

atFont::Glyph atFont::GetGlyph(const uint32_t codepoint)
{
  atAssert(m_font.data != nullptr, "Font is not initialised!");
  if (!m_glyphs.Contains(codepoint))
    m_glyphs.Add(codepoint, LoadGlyph(codepoint));
  Glyph g = m_glyphs.Get(codepoint);
  g.tl /= m_bitmap.Size();
  g.br /= m_bitmap.Size();
  return g;
}

atFont::Glyph atFont::LoadGlyph(const uint32_t codepoint)
{
  Glyph glyph;
  int x0, y0, x1, y1, advance, lsb;
  int g = stbtt_FindGlyphIndex(&m_font, codepoint);
  stbtt_GetGlyphHMetrics(&m_font, g, &advance, &lsb);
  stbtt_GetGlyphBitmapBox(&m_font, g, m_scale, m_scale, &x0, &y0, &x1, &y1);
  int64_t width = x1 - x0;
  int64_t height = y1 - y0;

  if (m_nextPos.x + width >= m_bitmap.Width())
  {
    m_nextPos.y += (int32_t)m_scale;
    m_nextPos.x = m_nextPos.y > m_lastSize.y ? 0 : m_lastSize.x;
  }
  if (m_nextPos.y + height >= m_bitmap.Height())
    ResizeBitmap(Bitmap().Size() * 2);

  atVector<uint8_t> glyphPixels(width * height, 0);
  stbtt_MakeGlyphBitmap(&m_font, (unsigned char*)glyphPixels.data(), x1 - x0, y1 - y0, (int)width, m_scale, m_scale, g);

  for (int64_t y = 0; y < height; ++y)
    for (int64_t x = 0; x < width; ++x)
    {
      uint8_t col = glyphPixels[x + y * width];
      m_bitmap.Pixels()[(m_nextPos.x + x) + (m_nextPos.y + y) * m_bitmap.Width()] = atColor::Pack(255ui8, 255ui8, 255ui8, col);
    }

  glyph.advance = (int64_t)(advance * m_scale);
  glyph.xOff = x0;
  glyph.yOff = y0;
  glyph.width = (int64_t)(x1 - x0);
  glyph.height = (int64_t)(y1 - y0);
  glyph.tl = atVec2I{ m_nextPos.x, m_nextPos.y + (int32_t)glyph.height };
  glyph.br = atVec2I{ m_nextPos.x + (int32_t)glyph.width, m_nextPos.y  };
  m_stale = true;
  m_nextPos.x += (int32_t)glyph.width;
  return glyph;
}

void atFont::ResizeBitmap(const atVec2I &size)
{
  m_lastSize = m_bitmap.Size();
  m_bitmap = m_bitmap.Pad(0, 0, size.y - m_bitmap.Height(), size.x - m_bitmap.Width());
  m_nextPos.x = m_nextPos.x;
  m_nextPos.y = 0;
}
