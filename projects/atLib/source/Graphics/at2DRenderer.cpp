
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

#include "atAABB.h"
#include "atFont.h"
#include "atRect.h"
#include "atInput.h"
#include "at2DRenderer.h"
#include "atRenderable.h"
#include "atRenderState.h"

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
  static atFont& Font() { return fonts[activeFont]; }
  static const atVec4I Clip() { return clip.size() ? clip.back() : atVec4I(-INT32_MAX, -INT32_MAX, INT32_MAX, INT32_MAX);; }
  static const atVec4F Colour() { return col.size() ? col.back() : atVec4F::one(); }
  static atTexture* Texture() { return tex.size() ? tex.back() : Font().GetTexture(false); }
  static DrawData& Data() { return drawList.back(); }

  static int64_t activeFont;
  
  static atVector<atFont> fonts;
  static atVector<atTexture*> tex;
  static atVector<atVec4F> col;
  static atVector<atVec4I> clip;

  struct Buffers
  {
    atGPUBuffer *pColour = nullptr;
    atGPUBuffer *pPos = nullptr;
    atGPUBuffer *pUV = nullptr;
    atGPUBuffer *pIndices = nullptr;

    atProgram *pProgram = nullptr;
    atSampler *pSampler = nullptr;
  } static gpu;

  struct DD
  {
    atVector<atVec4F> color;
    atVector<atVec3F> pos;
    atVector<atVec2F> uvs;
    atVector<uint32_t> indices;
  } static drawData;

  static atVector<DrawData> drawList;
  static atRenderable renderer;
};

int64_t DrawContext::activeFont;
atVector<atFont> DrawContext::fonts;
atVector<atTexture*> DrawContext::tex;
atVector<atVec4F> DrawContext::col;
atVector<atVec4I> DrawContext::clip;

DrawContext::Buffers DrawContext::gpu;
DrawContext::DD DrawContext::drawData;

atVector<DrawData> DrawContext::drawList;

atRenderable DrawContext::renderer;

static void _AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs);
static void _AddPoly(const atVector<atVec2F> &points, const atVec2F &uv);
static void _AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl = { 0, 0 });
static DrawData& _TryAddDrawCommand();
static atAABB<int32_t> _TextBounds(const atString &text);

// ------------------------------------------------------
// API

void at2DRenderer::SetFont(const atFilename &font, const int64_t scale, const int64_t resolution)
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

void at2DRenderer::AddText(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atVec4I clip = DrawContext::Clip();
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_min.y, 0) };
  atVec2I tl = atVec2I{ x, y } - pivot * bounds.Dimensions().xy();
  
  if (!atIntersects(atRectI(clip), atRectI(tl, tl + bounds.Dimensions().xy())))
    return;

  PushTexture(DrawContext::Font().GetTexture());
  tl -= offset;
  atVec2I cursor = tl;
  int64_t rowHeight = 0;

  atFont &font = DrawContext::Font();
  _TryAddDrawCommand();

  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    // uint32_t i = (uint32_t)DrawContext::drawData.pos.size();
    atVec4F rect = { 
      (float)cursor.x + g.xOff,  
      (float)cursor.y + g.yOff, 
      (float)cursor.x + g.xOff + g.width,  
      (float)cursor.y + g.height + g.yOff 
    };

    _AdvanceCursor(c, g, &rowHeight, &cursor, tl);
    if (!atIntersects(atRectI(clip), atRectI(atVec4I(rect))))
      continue;

    _AddPoly(
    { atVec2F(rect.x, rect.y), atVec2F(rect.z, rect.y), atVec2F(rect.z, rect.w), atVec2F(rect.x, rect.w) },
    atVector<atVec2F>({atVec2F(g.tl.x, g.br.y), atVec2F(g.br.x, g.br.y), atVec2F(g.br.x, g.tl.y), atVec2F(g.tl.x, g.tl.y) }));
  }
  PopTexture();
}

static void _InitDC()
{
  static bool initialised = false;
  if (initialised)
    return;

  atGraphics *pCtx = atGraphics::GetCurrent();
  if (!pCtx)
    return;

  // Setup the shader
  DrawContext::gpu.pProgram = pCtx->CreateProgram();
  DrawContext::gpu.pProgram->SetStageFile("assets/shaders/text.vs", atPS_Vertex);
  DrawContext::gpu.pProgram->SetStageFile("assets/shaders/text.ps", atPS_Fragment);

  // Create the sampler
  DrawContext::gpu.pSampler = pCtx->CreateSampler();

  // Create buffers
  DrawContext::gpu.pColour = pCtx->CreateBuffer();
  DrawContext::gpu.pUV = pCtx->CreateBuffer();
  DrawContext::gpu.pPos = pCtx->CreateBuffer();
  DrawContext::gpu.pIndices = pCtx->CreateBuffer(atBT_IndexData);

  // Setup renderable
  DrawContext::renderer.SetProgram(DrawContext::gpu.pProgram);
  DrawContext::renderer.SetSampler("samplerType", DrawContext::gpu.pSampler);
  DrawContext::renderer.SetAttribute("COLOR", DrawContext::gpu.pColour);
  DrawContext::renderer.SetAttribute("TEXCOORD", DrawContext::gpu.pUV);
  DrawContext::renderer.SetAttribute("POSITION", DrawContext::gpu.pPos);
  DrawContext::renderer.SetAttribute("idxBuffer", DrawContext::gpu.pIndices);
  initialised = true;
}

void at2DRenderer::Draw(const atVec2I &dimensions)
{
  atAssert(DrawContext::clip.size() == 0, "Mismatched Push/Pop Clip Rects");
  atAssert(DrawContext::tex.size() == 0, "Mismatched Push/Pop Textures");
  atAssert(DrawContext::col.size() == 0, "Mismatched Push/Pop Colours");

  atUnused(dimensions);

  atRenderState rs;

  // Make sure the textures are up to date
  for (atFont &f : DrawContext::fonts)
    f.GetTexture();

  // Update the buffers
  DrawContext::gpu.pIndices->Set(DrawContext::drawData.indices);
  DrawContext::gpu.pColour->Set(DrawContext::drawData.color);
  DrawContext::gpu.pPos->Set(DrawContext::drawData.pos);
  DrawContext::gpu.pUV->Set(DrawContext::drawData.uvs);

  // Update the viewport
  DrawContext::renderer.SetUniform("mvp", atMat4F(atMatrixOrtho((float)dimensions.x, (float)dimensions.y, -1.f, 1.f)));

  // Draw each of command
  for (DrawData &dd : DrawContext::drawList)
  {
    rs.SetScissor(dd.clipRect);
    DrawContext::renderer.SetTexture("tex0", dd.pTexture);
    DrawContext::renderer.Draw(true, atGFX_PT_TriangleList, dd.elementCount, dd.startElement);
  }

  // Clear the draw list
  DrawContext::drawList.clear();
  DrawContext::drawData.indices.clear();
  DrawContext::drawData.color.clear();
  DrawContext::drawData.pos.clear();
  DrawContext::drawData.uvs.clear();
}

atVec4I at2DRenderer::TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atAABB<int32_t> bounds = _TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_max.y, 0) };
  return atVec4I(bounds.m_min.xy() + offset + atVec2I{ x, y }, bounds.m_max.xy() + offset + atVec2I{ x, y });
}

void at2DRenderer::AddRectangle(const atVec2I &topLeft, const atVec2I &bottomRight)
{
  if (!atIntersects(atRectI(DrawContext::Clip()), atRectI(topLeft, bottomRight)))
    return;
  atFont &font = DrawContext::Font();
  _AddPoly({ atVec2F(topLeft), atVec2F(bottomRight.x, topLeft.y), atVec2F(bottomRight), atVec2F(topLeft.x, bottomRight.y) }, font.FindWhitePixel());
}

void at2DRenderer::AddRectangle(const int64_t x, const int64_t y, const atVec2I &dimensions, const atVec2F &pivot)
{
  atVec2I tl = { x - pivot.x * dimensions.x, y - pivot.y * dimensions.y };
  atVec2I br = tl + dimensions;
  AddRectangle(tl, br);
}

void at2DRenderer::AddCircle(const int64_t x, const int64_t y, const double radius, int64_t segments, double phase, const atVec2F &pivot)
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

atVec2I at2DRenderer::TextSize(const atString &text) { return _TextBounds(text).Dimensions().xy(); }
void at2DRenderer::Draw(atWindow &wnd) { Draw(wnd.Size()); }
void at2DRenderer::AddPolygon(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs) { _AddPoly(points, uvs); }
void at2DRenderer::PushTexture(atTexture *pTexture) { DrawContext::tex.push_back(pTexture); }
void at2DRenderer::PopTexture(const int64_t count) { DrawContext::tex.erase(atMax(0, DrawContext::tex.size() - count), (atMin(count, DrawContext::tex.size()))); }
void at2DRenderer::PushColour(const atVec4F &color) { DrawContext::col.push_back(color); }
void at2DRenderer::PushClipRect(const atVec4I &rect) { DrawContext::clip.push_back(rect); }
void at2DRenderer::PopColour(const int64_t count) { DrawContext::col.erase(atMax(0, DrawContext::col.size() - count), (atMin(count, DrawContext::col.size()))); }
void at2DRenderer::PopClipRect(const int64_t count) { DrawContext::clip.erase(atMax(0, DrawContext::clip.size() - count), (atMin(count, DrawContext::clip.size()))); }

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

static void _AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs)
{
  atAssert(points.size() == uvs.size(), "Points and UVS must contain the same number of elements.");
  DrawData &dd = _TryAddDrawCommand();
  atVec4F color = DrawContext::Colour();
  uint32_t start = (uint32_t)DrawContext::drawData.pos.size();
  for (const atVec2F &v : points)
  {
    DrawContext::drawData.pos.push_back(atVec3F(v, 0.f));
    DrawContext::drawData.color.push_back(color);
  }

  DrawContext::drawData.uvs.push_back(uvs);
  for (uint32_t i = 0; i < points.size() - 2; ++i)
    DrawContext::drawData.indices.push_back({ start, start + i + 1, start + i + 2 });
  dd.elementCount = DrawContext::drawData.indices.size() - dd.startElement;
}

void _AddPoly(const atVector<atVec2F> &points, const atVec2F &uv)
{
  _TryAddDrawCommand();
  atVec4F color = DrawContext::Colour();
  uint32_t start = (uint32_t)DrawContext::drawData.pos.size();
  for (const atVec2F &v : points)
  {
    DrawContext::drawData.uvs.push_back(uv);
    DrawContext::drawData.pos.push_back(atVec3F(v, 0.f));
    DrawContext::drawData.color.push_back(color);
  }

  for (uint32_t i = 0; i < points.size() - 2; ++i)
    DrawContext::drawData.indices.push_back({ start, start + i + 1, start + i + 2 });
}

static DrawData& _TryAddDrawCommand()
{
  if (DrawContext::drawList.size() > 0)
  {
    DrawData& dd = DrawContext::Data();
    dd.startElement = DrawContext::drawData.indices.size();
    if (DrawContext::Clip() == dd.clipRect && DrawContext::Texture() == dd.pTexture)
      return dd;
  }

  DrawData data;
  data.clipRect = DrawContext::Clip();
  data.pTexture = DrawContext::Texture();
  DrawContext::drawList.push_back(data);
  return DrawContext::Data();
}
