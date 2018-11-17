#include "atFont.h"
#include "atFile.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "3rdParty/stb/stb_truetype.h"

atFont::~atFont() {}
const atImage &atFont::Bitmap() const { return m_bitmap; }
const atFilename &atFont::Filename() const { return m_filename; }

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
  if (!stbtt_InitFont(&m_font, m_fd.data(), 0))
    return;
  m_scale = stbtt_ScaleForPixelHeight(&m_font, (float)scale);
}

atFont::Glyph atFont::GetGlyph(const uint32_t codepoint)
{
  if (!m_glyphs.Contains(codepoint))
    m_glyphs.Add(codepoint, LoadGlyph(codepoint));
  Glyph g = m_glyphs.Get(codepoint);
  g.tl /= m_bitmap.Size();
  g.br /= m_bitmap.Size();
  g.xOff /= m_bitmap.Width();
  g.yOff /= m_bitmap.Height();
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
  stbtt_MakeGlyphBitmap(&m_font, (unsigned char*)m_bitmap.Pixels().data(), x1 - x0, y1 - y0, (int)Bitmap().Width(), m_scale, m_scale, g);
  glyph.advance = (int64_t)(advance * m_scale);
  glyph.xOff = (float)x0;
  glyph.yOff = (float)y0;
  glyph.tl = { (float)x0, (float)y0 };
  glyph.br = { (float)x1, (float)y1 };
  glyph.height = (int64_t)(y1 - y0);
  glyph.width = (int64_t)(x1 - x0);
  return glyph;
}

void atFont::ResizeBitmap(const atVec2I &size)
{
  m_lastSize = m_bitmap.Size();
  m_bitmap = m_bitmap.Pad(0, 0, size.y - m_bitmap.Height(), size.x - m_bitmap.Width());
  m_nextPos.x = m_nextPos.x;
  m_nextPos.y = 0;
}
