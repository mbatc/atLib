
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
  int64_t texID;
  atHashMap<atVec4I64, atVector<atVec4F>> color;
  atHashMap<atVec4I64, atVector<atVec3F>> verts;
  atHashMap<atVec4I64, atVector<atVec2F>> uvs;
  atHashMap<atVec4I64, atVector<uint32_t>> indices;
};

static void _AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs, atVector<uint32_t> *pIdx = nullptr, atVector<atVec3F> *pVert = nullptr, atVector<atVec4F> *pCol = nullptr, atVector<atVec2F> *pUV = nullptr);
static void _AddPoly(const atVector<atVec2F> &points, const atVec2F &uv, atVector<uint32_t> *pIdx = nullptr, atVector<atVec3F> *pVert = nullptr, atVector<atVec4F> *pCol = nullptr, atVector<atVec2F> *pUV = nullptr);
static void _AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl = { 0, 0 });
static void _GetDrawData(atVector<uint32_t> **pIdx, atVector<atVec3F> **pVert, atVector<atVec4F> **pCol, atVector<atVec2F> **pUV);
static DrawData& _GetDrawData(int64_t texID);
static atAABB<int32_t> _TextBounds(const atString &text);
static atVec4I _ActiveClipRect();

static atVector<atFont> s_fonts;
static atVector<atVec4F> s_colStack;
static atVector<atVec4I> s_clipStack;

static int64_t s_activeFont = -1;
static atVector<DrawData> s_drawList;

// ------------------------------------------------------
// API

void atPrimitiveRenderer::SetFont(const atFilename &font)
{
  for (const atFont &f : s_fonts)
    if (f.Filename() == font)
    {
      s_activeFont = &f - s_fonts.begin();
      return;
    }
  s_fonts.push_back(atFont(font, 24));
  s_activeFont = s_fonts.size() - 1;
}

void atPrimitiveRenderer::AddText(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atVec4I clip = _ActiveClipRect();
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_min.y, 0) };
  atVec2I tl = atVec2I{ x, y } - pivot * bounds.Dimensions().xy();
  
  if (!atIntersects(clip, atVec4I(tl, tl + bounds.Dimensions().xy())))
    return;

  tl -= offset;
  atVec2I cursor = tl;
  int64_t rowHeight = 0;

  atFont &font = s_fonts[s_activeFont];
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
}

void atPrimitiveRenderer::Draw(const atWindow &wnd)
{
  atRenderable ro;
  for (atFont &f : s_fonts)
    f.GetTextureID();
  for (DrawData &dd : s_drawList)
  {
    ro.SetShader("assets/shaders/text");
    ro.SetChannel("samplerType", AT_INVALID_ID, atRRT_Sampler);
    ro.SetChannel("font0", dd.texID, atRRT_Texture);
    ro.SetChannel("mvp", atMat4(atMatrixOrtho((float)wnd.Width(), (float)wnd.Height(), -1.f, 1.f)), atRRT_Variable);

    for (const atVec4I64 key : dd.verts.GetKeys())
    {
      atRenderState::SetScissor(key);
      ro.SetChannel("POSITION", dd.verts[key], atRRT_VertexData);
      ro.SetChannel("COLOR", dd.color[key], atRRT_VertexData);
      ro.SetChannel("TEXCOORD", dd.uvs[key], atRRT_VertexData);
      ro.SetChannel("idxBuffer", dd.indices[key], atRRT_Indices);
      ro.Draw();
    }

    dd.verts.Clear();
    dd.color.Clear();
    dd.uvs.Clear();
    dd.indices.Clear();
  }
}

atVec4I atPrimitiveRenderer::TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_max.y, 0) };
  return atVec4I(bounds.m_min.xy() + offset + atVec2I{ x, y }, bounds.m_max.xy() + offset + atVec2I{ x, y });
}

void atPrimitiveRenderer::AddRectangle(const atVec2I &topLeft, const atVec2I &bottomRight)
{
  if (!atIntersects(_ActiveClipRect(), atVec4I(topLeft, bottomRight)))
    return;
  atFont &font = s_fonts[s_activeFont];
  _AddPoly({ atVec2F(topLeft), atVec2F(bottomRight.x, topLeft.y), atVec2F(bottomRight), atVec2F(topLeft.x, bottomRight.y) }, font.FindWhitePixel());
}

void atPrimitiveRenderer::AddRectangle(const int64_t x, const int64_t y, const atVec2I &dimensions, const atVec2F &pivot)
{
  atVec2I tl = { x - pivot.x * dimensions.x, y - pivot.y * dimensions.y };
  atVec2I br = tl + dimensions;
  AddRectangle(tl, br);
}

void atPrimitiveRenderer::AddCircle(const int64_t x, const int64_t y, const double radius, int64_t segments, double phase)
{
  atVector<atVec2F> points;
  points.reserve(segments + 1);
  double step = atPi * 2.0 / (double)segments;
  for (int64_t i = 0; i < segments; ++i)
  {
    double angle = phase + step * i;
    points.push_back(atVec2F(atCos(angle), atSin(angle)) * radius + atVec2F(x, y));
  }
  atFont &font = s_fonts[s_activeFont];
  _AddPoly(points, font.FindWhitePixel());
}

atVec2I atPrimitiveRenderer::TextSize(const atString &text) { return _TextBounds(text).Dimensions().xy(); }
void atPrimitiveRenderer::AddPolygon(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs) { _AddPoly(points, uvs); }
void atPrimitiveRenderer::PushColour(const atVec4F &color) { s_colStack.push_back(color); }
void atPrimitiveRenderer::PushClipRect(const atVec4I &rect) { s_clipStack.push_back(rect); }
void atPrimitiveRenderer::PopColour(const int64_t count) { s_colStack.erase(atMax(0, s_colStack.size() - count), (atMin(count, s_colStack.size()))); }
void atPrimitiveRenderer::PopClipRect(const int64_t count) { s_clipStack.erase(atMax(0, s_clipStack.size() - count), (atMin(count, s_clipStack.size()))); }

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
  atFont &font = s_fonts[s_activeFont];
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
  atVec4F color = s_colStack.size() ? s_colStack.back() : atVec4F::one();
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
  atVec4F color = s_colStack.size() ? s_colStack.back() : atVec4F::one();
  uint32_t start = (uint32_t)pVert->size();
  for (const atVec2F &v : points)
  {
    pUV->push_back(uv);
    pVert->push_back(atVec3F(v, 0.f));
    pCol->push_back(color);
  }

  for (uint32_t i = 0; i < points.size() - 2; ++i) pIdx->push_back({ start, start + i + 1, start + i + 2 });
}

static DrawData& _GetDrawData(int64_t texID)
{
  for (DrawData &dd : s_drawList)
    if (dd.texID == texID)
      return dd;
  s_drawList.push_back(DrawData());
  s_drawList.back().texID = texID;
  return s_drawList.back();
}

static void _GetDrawData(atVector<uint32_t> **pIdx, atVector<atVec3F> **pVert, atVector<atVec4F> **pCol, atVector<atVec2F> **pUV)
{
  atFont &font = s_fonts[s_activeFont];
  DrawData &dd = _GetDrawData(font.GetTextureID(false));
  atVec4I clip = _ActiveClipRect();
  dd.indices.TryAdd(clip);
  dd.verts.TryAdd(clip);
  dd.color.TryAdd(clip);
  dd.uvs.TryAdd(clip);
  *pIdx = dd.indices.TryGet(clip);
  *pVert = dd.verts.TryGet(clip);
  *pCol = dd.color.TryGet(clip);
  *pUV = dd.uvs.TryGet(clip);
}

static atVec4I _ActiveClipRect() { return s_clipStack.size() ? s_clipStack.back() : atVec4I64(-INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX); }