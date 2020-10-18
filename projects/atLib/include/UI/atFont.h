
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

#ifndef atFont_h__
#define atFont_h__

#include "atHashMap.h"
#include "atFilename.h"
#include "atImage.h"
#include "atTexture.h"

//---------------------
// THIRD PARTY INCLUDES

#include "../../3rdParty/stb/stb_truetype.h" // https://github.com/nothings/stb | stb single-file public domain libraries for C/C++ https://twitter.com/nothings
#include "atRect.h"
#include "atResourceHandler.h"

// --------------------

class atFont
{
public:
  struct Glyph
  {
    atVec2F tl;       // Top left UV of the character (0-1)
    atVec2F br;       // Bottom right UV of the character (0-1)
    float xOff;       // Horizontal offset to apply when drawing character in Pixels
    float yOff;       // Vertical offset to apply when drawing character in Pixels
    float advance;  // Distance to move cursor horizontally 
    float width;    // Width of the glyph in pixels
    float height;   // Height of the glyph in pixels
  };

  atFont(const atFilename &filename = "", const int64_t scale = 32, const int64_t resolution = -1);
  atFont(const atFont &copy);
  atFont(atFont &&move);
  ~atFont();

  // Get the glyph associated with a specific codepoint
  // 
  // Dynamic caching is used so a glyph is only loaded 
  // when it is first used, and stored for later
  //
  // Glyphs are not loaded if they are not used
  Glyph GetGlyph(const uint32_t &codepoint, const float &scale = 1);

  // Get the font texture
  const atImage& Bitmap() const;

  // Get the font filename
  const atFilename& Filename() const;

  // Get the texture object for the font
  atTexture* GetTexture(const bool updateTexture = true);

  int64_t Height() const;
  int64_t Resolution() const;

  // Returns the UV of a white pixel
  atVec2F FindWhitePixel();

  atRectF CalcTextBounds(const atString &text, const float &scale = 1);

  void AdvanceCursor(const char c, const atFont::Glyph &g, float *pLineHeight, atVec2F *pPos, const atVec2F &tl = { 0,0 }) const;

  bool IsValid() const;

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
  int64_t m_resolution = 0;

  atTexture* m_pTexture = nullptr;

  int32_t m_lastRowHeight = 0;
  atVec2I m_nextPos = atVec2I::zero();
  atVec2I m_lastSize = atVec2I::zero();
  atVec2F m_lastWhiteUV = atVec2F::zero();
};

namespace atResourceHandlers
{
  class FontHandler : public atResourceHandler<atFont>
  {
  public:
    FontHandler() : atResourceHandler("Font") {}

    bool Load(const atObjectDescriptor &request, atFont *pResource) override;
  };
};

#endif // atFont_h__
