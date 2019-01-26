
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
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

#include "atPrimitiveRenderer.h"
#include "atFont.h"
#include "atRenderable.h"
#include "atInput.h"
#include "atIntersects.h"
#include "atRenderState.h"

struct DrawData
{
  atVec4F colour;
  int64_t texture;
  atVec4I clipRect;

  atVector<atVec4F> color;
  atVector<atVec3F> verts;
  atVector<atVec2F> uvs;
  atVector<uint32_t> indices;
};

struct DrawContext
{
  static atFont& Font() { return fonts[activeFont]; }
  static const atVec4I Clip() { return clip.size() ? clip.back() : atVec4I(-INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX);; }
  static const atVec4F Colour() { return col.size() ? col.back() : atVec4F::one(); }
  static const int64_t Texture() { return tex.size() ? tex.back() : Font().GetTextureID(false); }
  static DrawData& Data() { return drawList.back(); }

  static int64_t activeFont;
  
  static atVector<atFont> fonts;
  static atVector<int64_t> tex;
  static atVector<atVec4F> col;
  static atVector<atVec4I> clip;

  static atVector<DrawData> drawList;
};

int64_t DrawContext::activeFont;
atVector<atFont> DrawContext::fonts;
atVector<int64_t> DrawContext::tex;
atVector<atVec4F> DrawContext::col;
atVector<atVec4I> DrawContext::clip;
atVector<DrawData> DrawContext::drawList;

static void _AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs, atVector<uint32_t> *pIdx = nullptr, atVector<atVec3F> *pVert = nullptr, atVector<atVec4F> *pCol = nullptr, atVector<atVec2F> *pUV = nullptr);
static void _AddPoly(const atVector<atVec2F> &points, const atVec2F &uv, atVector<uint32_t> *pIdx = nullptr, atVector<atVec3F> *pVert = nullptr, atVector<atVec4F> *pCol = nullptr, atVector<atVec2F> *pUV = nullptr);
static void _AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl = { 0, 0 });
static void _GetDrawData(atVector<uint32_t> **pIdx, atVector<atVec3F> **pVert, atVector<atVec4F> **pCol, atVector<atVec2F> **pUV);
static DrawData& _GetDrawData();
static atAABB<int32_t> _TextBounds(const atString &text);

// ------------------------------------------------------
// API

void atPrimitiveRenderer::SetFont(const atFilename &font, const int64_t scale, const int64_t resolution)
{
  for (const atFont &f : DrawContext::fonts)
    if (f.Filename() == font && f.Height() == scale && f.Resolution() == resolution)
    {
      DrawContext::activeFont = &f - DrawContext::fonts.begin();
      return;
    }

  DrawContext::fonts.push_back(atFont(font, scale, resolution));
  DrawContext::activeFont = DrawContext::fonts.size() - 1;
}

void atPrimitiveRenderer::AddText(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atVec4I clip = DrawContext::Clip();
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_min.y, 0) };
  atVec2I tl = atVec2I{ x, y } - pivot * bounds.Dimensions().xy();
  
  if (!atIntersects(clip, atVec4I(tl, tl + bounds.Dimensions().xy())))
    return;

  PushTexture(DrawContext::Font().GetTextureID());
  tl -= offset;
  atVec2I cursor = tl;
  int64_t rowHeight = 0;

  atFont &font = DrawContext::Font();
  atVector<uint32_t> *pIdx;
  atVector<atVec3F> *pVert;
  atVector<atVec4F> *pCol;
  atVector<atVec2F> *pUV;
  _GetDrawData(&pIdx, &pVert, &pCol, &pUV);

  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    uint32_t i = (uint32_t)pVert->size();
    atVec4F rect = { 
      (float)cursor.x + g.xOff,  
      (float)cursor.y + g.yOff, 
      (float)cursor.x + g.xOff + g.width,  
      (float)cursor.y + g.height + g.yOff 
    };

    _AdvanceCursor(c, g, &rowHeight, &cursor, tl);
    if (!atIntersects(clip, atVec4I(rect)))
      continue;

    _AddPoly(
    { atVec2F(rect.x, rect.y), atVec2F(rect.z, rect.y), atVec2F(rect.z, rect.w), atVec2F(rect.x, rect.w) },
    atVector<atVec2F>({atVec2F(g.tl.x, g.br.y), atVec2F(g.br.x, g.br.y), atVec2F(g.br.x, g.tl.y), atVec2F(g.tl.x, g.tl.y) }),
      pIdx, pVert, pCol, pUV);
  }

  PopTexture();
}

void atPrimitiveRenderer::Draw(const atVec2I &dimensions)
{
  atAssert(DrawContext::clip.size() == 0, "Mismatched Push/Pop Clip Rects");
  atAssert(DrawContext::tex.size() == 0, "Mismatched Push/Pop Textures");
  atAssert(DrawContext::col.size() == 0, "Mismatched Push/Pop Colours");

  atRenderable ro;
  atRenderState rs;
  for (atFont &f : DrawContext::fonts)
    f.GetTextureID();

  ro.SetShader("assets/shaders/text");
  ro.SetSampler("samplerType", AT_INVALID_ID);
  ro.SetUniform("mvp", atMat4F(atMatrixOrtho((float)dimensions.x, (float)dimensions.y, -1.f, 1.f)));
  for (DrawData &dd : DrawContext::drawList)
  {
    rs.SetScissor(dd.clipRect);
    ro.SetTexture("tex0", dd.texture);
    ro.SetAttribute("COLOR", dd.color);
    ro.SetAttribute("TEXCOORD", dd.uvs);
    ro.SetAttribute("POSITION", dd.verts);
    ro.SetIndices("idxBuffer", dd.indices);
    ro.DrawTriangles();
  }
  DrawContext::drawList.clear();
}

atVec4I atPrimitiveRenderer::TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_max.y, 0) };
  return atVec4I(bounds.m_min.xy() + offset + atVec2I{ x, y }, bounds.m_max.xy() + offset + atVec2I{ x, y });
}

void atPrimitiveRenderer::AddRectangle(const atVec2I &topLeft, const atVec2I &bottomRight)
{
  if (!atIntersects(DrawContext::Clip(), atVec4I(topLeft, bottomRight)))
    return;
  atFont &font = DrawContext::Font();
  _AddPoly({ atVec2F(topLeft), atVec2F(bottomRight.x, topLeft.y), atVec2F(bottomRight), atVec2F(topLeft.x, bottomRight.y) }, font.FindWhitePixel());
}

void atPrimitiveRenderer::AddRectangle(const int64_t x, const int64_t y, const atVec2I &dimensions, const atVec2F &pivot)
{
  atVec2I tl = { x - pivot.x * dimensions.x, y - pivot.y * dimensions.y };
  atVec2I br = tl + dimensions;
  AddRectangle(tl, br);
}

void atPrimitiveRenderer::AddCircle(const int64_t x, const int64_t y, const double radius, int64_t segments, double phase, const atVec2F &pivot)
{
  atVector<atVec2F> points;
  points.reserve(segments + 1);
  double step = atPi * 2.0 / (double)segments;
  atVec2F tl = atVec2F(x, y) + atVec2F(radius, radius) - pivot * radius * 2;
  for (int64_t i = 0; i < segments; ++i)
  {
    double angle = phase + step * i;
    points.push_back(atVec2F(atCos(angle), atSin(angle)) * radius + tl);
  }
  atFont &font = DrawContext::Font();
  _AddPoly(points, font.FindWhitePixel());
}

atVec2I atPrimitiveRenderer::TextSize(const atString &text) { return _TextBounds(text).Dimensions().xy(); }
void atPrimitiveRenderer::Draw(atWindow &wnd) { Draw(wnd.Size()); }
void atPrimitiveRenderer::AddPolygon(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs) { _AddPoly(points, uvs); }
void atPrimitiveRenderer::PushTexture(const int64_t id) { DrawContext::tex.push_back(id); }
void atPrimitiveRenderer::PopTexture(const int64_t count) { DrawContext::tex.erase(atMax(0, DrawContext::tex.size() - count), (atMin(count, DrawContext::tex.size()))); }
void atPrimitiveRenderer::PushColour(const atVec4F &color) { DrawContext::col.push_back(color); }
void atPrimitiveRenderer::PushClipRect(const atVec4I &rect) { DrawContext::clip.push_back(rect); }
void atPrimitiveRenderer::PopColour(const int64_t count) { DrawContext::col.erase(atMax(0, DrawContext::col.size() - count), (atMin(count, DrawContext::col.size()))); }
void atPrimitiveRenderer::PopClipRect(const int64_t count) { DrawContext::clip.erase(atMax(0, DrawContext::clip.size() - count), (atMin(count, DrawContext::clip.size()))); }

// ------------------------------------------------------
// Internal Functions

static void _AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl)
{
  pPos->x += (int32_t)g.advance;
  *pLineHeight = atMax(*pLineHeight, pPos->y + g.height);
  if (c == '\n')
  {
    pPos->y = (int32_t)*pLineHeight;
    pPos->x = tl.x;
    *pLineHeight = 0;
  }
}

static atAABB<int32_t> _TextBounds(const atString &text)
{
  atVec2I pos = 0;
  int64_t lineHeight = 0;
  atFont &font = DrawContext::Font();
  atAABB<int32_t> bounds;
  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    atVec4I rect = { pos.x,  pos.y + g.yOff, pos.x + (int32_t)g.width,  pos.y + (int32_t)g.height + g.yOff };
    bounds.GrowToContain(atAABB<int32_t>({ rect.x, rect.y, 0 }, { rect.z, rect.w, 0 }));
    _AdvanceCursor(c, g, &lineHeight, &pos);
  }
  return bounds;
}

static void _AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs, atVector<uint32_t> *pIdx, atVector<atVec3F> *pVert, atVector<atVec4F> *pCol, atVector<atVec2F> *pUV)
{
  atAssert(points.size() == uvs.size(), "Points and UVS must contain the same number of elements.");
  if (!pIdx || !pVert || !pCol || !pUV)
    _GetDrawData(&pIdx, &pVert, &pCol, &pUV);
  atVec4F color = DrawContext::Colour();
  uint32_t start = (uint32_t)pVert->size();
  for (const atVec2F &v : points)
  {
    pVert->push_back(atVec3F(v, 0.f));
    pCol->push_back(color);
  }

  pUV->push_back(uvs);
  for (uint32_t i = 0; i < points.size() - 2; ++i) pIdx->push_back({ start, start + i + 1, start + i + 2 });
}

void _AddPoly(const atVector<atVec2F>& points, const atVec2F &uv, atVector<uint32_t>* pIdx, atVector<atVec3F>* pVert, atVector<atVec4F>* pCol, atVector<atVec2F>* pUV)
{
  if (!pIdx || !pVert || !pCol || !pUV)
    _GetDrawData(&pIdx, &pVert, &pCol, &pUV);
  atVec4F color = DrawContext::Colour();
  uint32_t start = (uint32_t)pVert->size();
  for (const atVec2F &v : points)
  {
    pUV->push_back(uv);
    pVert->push_back(atVec3F(v, 0.f));
    pCol->push_back(color);
  }

  for (uint32_t i = 0; i < points.size() - 2; ++i) pIdx->push_back({ start, start + i + 1, start + i + 2 });
}

static DrawData& _GetDrawData()
{
  if (DrawContext::drawList.size() > 0)
  {
    DrawData& dd = DrawContext::Data();
    if (DrawContext::Clip() == dd.clipRect && DrawContext::Texture() == dd.texture)
      return dd;
  }

  DrawData data;
  data.clipRect = DrawContext::Clip();
  data.texture = DrawContext::Texture();
  DrawContext::drawList.push_back(data);
  return DrawContext::Data();
}

static void _GetDrawData(atVector<uint32_t> **pIdx, atVector<atVec3F> **pVert, atVector<atVec4F> **pCol, atVector<atVec2F> **pUV)
{
  DrawData &dd = _GetDrawData();
  *pIdx = &dd.indices;
  *pVert = &dd.verts;
  *pCol = &dd.color;
  *pUV = &dd.uvs;
}