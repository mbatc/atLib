
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atFont.h"
#include "atFile.h"

//---------------------
// THIRD PARTY INCLUDES
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

//---------------------

atFont::~atFont() {}
const atImage &atFont::Bitmap() const { return m_bitmap; }
const atFilename &atFont::Filename() const { return m_filename; }
int64_t atFont::Height() const { return m_height; }
int64_t atFont::Resolution() const { return m_resolution; }

atTexture* atFont::GetTexture(const bool updateTexture)
{
  if (!m_pTexture)
    m_pTexture = atGraphics::GetCurrent()->CreateTexture();
  if (m_stale && updateTexture)
    m_pTexture->Set(m_bitmap);
  m_stale = false;
  return m_pTexture;
}

atFont::atFont(const atFilename &filename, const int64_t scale, const int64_t resolution)
  : m_bitmap(atVec2I{ 512, 512 })
  , m_filename(filename)
  , m_font({ 0 })
  , m_resolution(resolution <= 0 ? scale : resolution)
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
  m_scale = stbtt_ScaleForPixelHeight(&m_font, (float)m_resolution);

  for (int64_t y = 0; y < 4; ++y)
    for (int64_t x = 0; x < 4; ++x)
      m_bitmap[x + y * m_bitmap.Width()] = 0xFFFFFFFF;

  m_nextPos.x = 4;
  m_lastSize = 4;
  m_lastWhiteUV = 0.f;
}

atFont::atFont(const atFont &copy)
{
  m_pTexture = nullptr;
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
  m_resolution = copy.m_resolution;
  m_nextPos = copy.m_nextPos;
  m_lastWhiteUV = copy.m_lastWhiteUV;
  m_lastSize = copy.m_lastSize;
}

atFont::atFont(atFont &&move)
  : m_fd(std::move(move.m_fd))
  , m_filename(std::move(move.m_filename))
  , m_font(std::move(move.m_font))
  , m_glyphs(std::move(move.m_glyphs))
  , m_stale(move.m_stale)
  , m_pTexture(move.m_pTexture)
  , m_bitmap(std::move(move.m_bitmap))
  , m_scale(move.m_scale)
  , m_height(move.m_height)
  , m_resolution(move.m_resolution)
  , m_nextPos(move.m_nextPos)
  , m_lastWhiteUV(move.m_lastWhiteUV)
  , m_lastSize(move.m_lastSize)
{
  move.m_pTexture = nullptr;
  move.m_stale = true;
}

atVec2F atFont::FindWhitePixel()
{
  if (m_stale)
  {
    bool found = false;
    for (int64_t y = 0; y < m_bitmap.Height() && !found; ++y)
      for (int64_t x = 0; x < m_bitmap.Width() && !found; ++x)
        if (m_bitmap[x + y * m_bitmap.Width()] == atColor::Pack(255, 255, 255, 255))
        {
          m_lastWhiteUV = { (float)(x+1) / (float)m_bitmap.Width(), (float)(y+1) / (float)m_bitmap.Height() };
          found = true;
        }
  }
  return m_lastWhiteUV;
}

atRectF atFont::CalcTextBounds(const atString &text, const float &scale /*= 1*/)
{
  atVec2F pos = 0;
  float lineHeight = 0;
  atRectF bounds;
  for (const char c : text)
  {
    atFont::Glyph g = GetGlyph(c, scale);
    atVec4F rect(float(pos.x), float(pos.y + g.yOff), float(pos.x + g.width), float(pos.y + g.height + g.yOff));
    bounds.GrowToContain(rect.xy());
    bounds.GrowToContain(rect.zw());
    AdvanceCursor(c, g, &lineHeight, &pos);
  }
  return bounds;
}

void atFont::AdvanceCursor(const char c, const Glyph &g, float *pLineHeight, atVec2F *pPos, const atVec2F &tl /*= { 0,0 }*/) const
{
  pPos->x += g.advance;
  *pLineHeight = atMax(*pLineHeight, pPos->y + g.height);
  if (c == '\n')
  {
    pPos->y = *pLineHeight;
    pPos->x = tl.x;
    *pLineHeight = 0;
  }
}

bool atFont::IsValid() const { return m_font.data != nullptr; }

atFont::Glyph atFont::GetGlyph(const uint32_t &codepoint, const float &scale)
{
  atAssert(IsValid(), "Font is not initialised!");
  if (!m_glyphs.Contains(codepoint))
    m_glyphs.Add(codepoint, LoadGlyph(codepoint));
  Glyph g = m_glyphs.Get(codepoint);

  // Calculate UVs
  g.tl /= m_bitmap.Size();
  g.br /= m_bitmap.Size();

  // Scale geometry
  g.advance *= scale;
  g.xOff *= scale;
  g.yOff *= scale;
  g.width *= scale;
  g.height *= scale;
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
    m_nextPos.y += m_lastRowHeight + m_padding;
    m_lastRowHeight = 0;
    m_nextPos.x = m_nextPos.y > m_lastSize.y ? 0 : m_lastSize.x;
  }
  if (m_nextPos.y + height >= m_bitmap.Height())
    ResizeBitmap(Bitmap().Size() * 2);

  atVector<uint8_t> glyphPixels(width * height, 0);
  stbtt_MakeGlyphBitmap(&m_font, (unsigned char*)glyphPixels.data(), (int)width, (int)height, (int)width, m_scale, m_scale, g);

  for (int64_t y = 0; y < height; ++y)
    for (int64_t x = 0; x < width; ++x)
    {
      uint8_t col = glyphPixels[x + y * width];
      m_bitmap.Pixels()[(m_nextPos.x + x) + (m_nextPos.y + y) * m_bitmap.Width()] = atColor::Pack((uint8_t)255, (uint8_t)255, (uint8_t)255, col);
    }

  glyph.tl = atVec2I{ m_nextPos.x, m_nextPos.y + (int32_t)height };
  glyph.br = atVec2I{ m_nextPos.x + (int32_t)width, m_nextPos.y  };

  m_stale = true;
  m_nextPos.x += (int32_t)width + m_padding;
  m_lastRowHeight = atMax(m_lastRowHeight, height);

  float ratio = (float)m_height / (float)m_resolution;
  glyph.xOff = (float)ceil(x0 * ratio);
  glyph.yOff = (float)ceil(y0 * ratio);
  glyph.width = (float)ceil(width * ratio);
  glyph.height = (float)ceil(height * ratio);
  glyph.advance = advance * m_scale * ratio;
  return glyph;
}

void atFont::ResizeBitmap(const atVec2I &size)
{
  m_lastSize = m_bitmap.Size();
  m_bitmap = m_bitmap.Pad(0, 0, size.y - m_bitmap.Height(), size.x - m_bitmap.Width());
  m_nextPos.x = m_lastSize.x;
  m_nextPos.y = 0;
}

bool atResourceHandlers::FontHandler::Load(const atObjectDescriptor &request, atFont *pResource)
{
  atString path = request["url"].AsString();
  int64_t scale = request["scale"].AsInt(24);
  int64_t res = request["resolution"].AsInt(-1);
  atConstruct(pResource, path, scale, res);
  return pResource->IsValid();
}
