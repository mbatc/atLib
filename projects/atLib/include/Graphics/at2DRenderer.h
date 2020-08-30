
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

#ifndef at2DRenderer_h__
#define at2DRenderer_h__

#include "atFilename.h"
#include "atTexture.h"
#include "atWindow.h"
#include "atFont.h"
#include "atRenderable.h"

class at2DRenderer
{
public:
  // TODO: Add a font resolution and font scale
  // - Resolution being the quality of each character
  // - Scale being how large the font is when displayed at scale 1.0
  void SetFont(const atFilename &font, const int64_t scale = 24, const int64_t resolution = -1);
  void Draw(const atVec2I &dimensions);
  void Draw(atWindow &wnd);

  atVec2I TextSize(const atString &text);
  atVec4I TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot = { 0, 0 });

  // TODO: Add a scale parameter
  void AddText(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot = { 0, 0 });

  // TODO: Add textured versions of these functions
  void AddPolygon(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs);
  void AddRectangle(const atVec2I &topLeft, const atVec2I &topRight);
  void AddRectangle(const int64_t x, const int64_t y, const atVec2I &dimensions, const atVec2F &pivot = { 0, 0 });
  void AddCircle(const int64_t x, const int64_t y, const double radius, int64_t segments = 16, double phase = 0.0, const atVec2F &pivot = { 0, 0 });
  
  // TODO: Create a global state to contain different rendering properties that may be added
  void PushTexture(atTexture *pTexture);
  void PopTexture(const int64_t count = 1);
  void PushColour(const atVec4F &color);
  void PopColour(const int64_t count = 1);
  void PushClipRect(const atVec4I &rect);
  void PopClipRect(const int64_t count = 1);

protected:
  struct DrawData
  {
    atVec4F colour;
    atTexture *pTexture = nullptr;
    atVec4I clipRect;

    int64_t startElement = 0;
    int64_t elementCount = -1;
  };

  struct DrawContext
  {
    atFont& Font() { return fonts[activeFont]; }
    const atVec4I Clip() { return clip.size() ? clip.back() : atVec4I(-INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX);; }
    const atVec4F Colour() { return col.size() ? col.back() : atVec4F::one(); }
    atTexture* Texture() { return tex.size() ? tex.back() : Font().GetTexture(false); }
    DrawData& Data() { return drawList.back(); }

    int64_t activeFont;

    atVector<atFont> fonts;
    atVector<atTexture*> tex;
    atVector<atVec4F> col;
    atVector<atVec4I> clip;

    struct Buffers
    {
      atGPUBuffer *pColour = nullptr;
      atGPUBuffer *pPos = nullptr;
      atGPUBuffer *pUV = nullptr;
      atGPUBuffer *pIndices = nullptr;

      atProgram *pProgram = nullptr;
      atSampler *pSampler = nullptr;
    } gpu;

    struct DD
    {
      atVector<atVec4F> color;
      atVector<atVec3F> pos;
      atVector<atVec2F> uvs;
      atVector<uint32_t> indices;
    } drawData;

    atVector<DrawData> drawList;
    atRenderable renderer;
  } m_drawContext;

  void InitDC();
  void AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl = { 0, 0 });
  atAABB<int32_t> TextBounds(const atString &text);
  void AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs);
  void AddPoly(const atVector<atVec2F> &points, const atVec2F &uv);
  DrawData& TryAddDrawCommand();
};

#endif // at2DRenderer_h__
