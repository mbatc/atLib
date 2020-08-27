
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

// ------------------------------------------------------
// API

void at2DRenderer::SetFont(const atFilename &font, const int64_t scale, const int64_t resolution)
{
  for (const atFont &f : m_drawContext.fonts)
    if (f.Filename() == font && f.Height() == scale && f.Resolution() == resolution)
    {
      m_drawContext.activeFont = &f - m_drawContext.fonts.begin();
      return;
    }

  m_drawContext.fonts.push_back(atFont(font, scale, resolution));
  m_drawContext.activeFont = m_drawContext.fonts.size() - 1;
}

void at2DRenderer::AddText(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atVec4I clip = m_drawContext.Clip();
  atAABB<int32_t> bounds = TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_min.y, 0) };
  atVec2I tl = atVec2I{ x, y } - pivot * bounds.Dimensions().xy();
  
  if (!atIntersects(atRectI(clip), atRectI(tl, tl + bounds.Dimensions().xy())))
    return;

  PushTexture(m_drawContext.Font().GetTexture());
  tl -= offset;
  atVec2I cursor = tl;
  int64_t rowHeight = 0;

  atFont &font = m_drawContext.Font();
  TryAddDrawCommand();

  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    // uint32_t i = (uint32_t)m_drawContext.drawData.pos.size();
    atVec4F rect = { 
      (float)cursor.x + g.xOff,  
      (float)cursor.y + g.yOff, 
      (float)cursor.x + g.xOff + g.width,  
      (float)cursor.y + g.height + g.yOff 
    };

    AdvanceCursor(c, g, &rowHeight, &cursor, tl);
    if (!atIntersects(atRectI(clip), atRectI(atVec4I(rect))))
      continue;

    AddPoly(
    { atVec2F(rect.x, rect.y), atVec2F(rect.z, rect.y), atVec2F(rect.z, rect.w), atVec2F(rect.x, rect.w) },
    atVector<atVec2F>({atVec2F(g.tl.x, g.br.y), atVec2F(g.br.x, g.br.y), atVec2F(g.br.x, g.tl.y), atVec2F(g.tl.x, g.tl.y) }));
  }
  PopTexture();
}

void at2DRenderer::InitDC()
{
  static bool initialised = false;
  if (initialised)
    return;

  atGraphics *pCtx = atGraphics::GetCurrent();
  if (!pCtx)
    return;

  // Setup the shader
  m_drawContext.gpu.pProgram = pCtx->CreateProgram();
  if (pCtx->API() == atGfxApi_OpenGL)
  {
    m_drawContext.gpu.pProgram->SetStageFile("assets/shaders/text.vs", atPS_Vertex);
    m_drawContext.gpu.pProgram->SetStageFile("assets/shaders/text.ps", atPS_Fragment);
  }
  else
  {
    m_drawContext.gpu.pProgram->SetStageFile("assets/shaders/text.vert", atPS_Vertex);
    m_drawContext.gpu.pProgram->SetStageFile("assets/shaders/text.frag", atPS_Fragment);
  }

  // Create the sampler
  m_drawContext.gpu.pSampler = pCtx->CreateSampler();

  // Create buffers
  m_drawContext.gpu.pColour = pCtx->CreateBuffer();
  m_drawContext.gpu.pUV = pCtx->CreateBuffer();
  m_drawContext.gpu.pPos = pCtx->CreateBuffer();
  m_drawContext.gpu.pIndices = pCtx->CreateBuffer(atBT_IndexData);

  // Setup renderable
  m_drawContext.renderer.SetProgram(m_drawContext.gpu.pProgram);
  m_drawContext.renderer.SetSampler("samplerType", m_drawContext.gpu.pSampler);
  m_drawContext.renderer.SetAttribute("COLOR", m_drawContext.gpu.pColour);
  m_drawContext.renderer.SetAttribute("TEXCOORD", m_drawContext.gpu.pUV);
  m_drawContext.renderer.SetAttribute("POSITION", m_drawContext.gpu.pPos);
  m_drawContext.renderer.SetAttribute("idxBuffer", m_drawContext.gpu.pIndices);
  initialised = true;
}

void at2DRenderer::Draw(const atVec2I &dimensions)
{
  atAssert(m_drawContext.clip.size() == 0, "Mismatched Push/Pop Clip Rects");
  atAssert(m_drawContext.tex.size() == 0, "Mismatched Push/Pop Textures");
  atAssert(m_drawContext.col.size() == 0, "Mismatched Push/Pop Colours");

  atUnused(dimensions);

  atRenderState rs;

  // Make sure the textures are up to date
  for (atFont &f : m_drawContext.fonts)
    f.GetTexture();

  // Update the buffers
  m_drawContext.gpu.pIndices->Set(m_drawContext.drawData.indices);
  m_drawContext.gpu.pColour->Set(m_drawContext.drawData.color);
  m_drawContext.gpu.pPos->Set(m_drawContext.drawData.pos);
  m_drawContext.gpu.pUV->Set(m_drawContext.drawData.uvs);

  // Update the viewport
  m_drawContext.renderer.SetUniform("mvp", atMat4F(atMatrixOrtho((float)dimensions.x, (float)dimensions.y, -1.f, 1.f)));

  // Draw each of command
  for (DrawData &dd : m_drawContext.drawList)
  {
    rs.SetScissor(dd.clipRect);
    m_drawContext.renderer.SetTexture("tex0", dd.pTexture);
    m_drawContext.renderer.Draw(true, atGFX_PT_TriangleList, dd.elementCount, dd.startElement);
  }

  // Clear the draw list
  m_drawContext.drawList.clear();
  m_drawContext.drawData.indices.clear();
  m_drawContext.drawData.color.clear();
  m_drawContext.drawData.pos.clear();
  m_drawContext.drawData.uvs.clear();
}

atVec4I at2DRenderer::TextRect(const int64_t x, const int64_t y, const atString &text, const atVec2F &pivot)
{
  atAABB<int32_t> bounds = TextBounds(text);
  atVec2I offset = { atMin(bounds.m_min.x, 0), atMin(bounds.m_max.y, 0) };
  return atVec4I(bounds.m_min.xy() + offset + atVec2I{ x, y }, bounds.m_max.xy() + offset + atVec2I{ x, y });
}

void at2DRenderer::AddRectangle(const atVec2I &topLeft, const atVec2I &bottomRight)
{
  if (!atIntersects(atRectI(m_drawContext.Clip()), atRectI(topLeft, bottomRight)))
    return;
  atFont &font = m_drawContext.Font();
  AddPoly({ atVec2F(topLeft), atVec2F(bottomRight.x, topLeft.y), atVec2F(bottomRight), atVec2F(topLeft.x, bottomRight.y) }, font.FindWhitePixel());
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
  atFont &font = m_drawContext.Font();
  AddPoly(points, font.FindWhitePixel());
}

atVec2I at2DRenderer::TextSize(const atString &text) { return TextBounds(text).Dimensions().xy(); }
void at2DRenderer::Draw(atWindow &wnd) { Draw(wnd.Size()); }
void at2DRenderer::AddPolygon(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs) { AddPoly(points, uvs); }
void at2DRenderer::PushTexture(atTexture *pTexture) { m_drawContext.tex.push_back(pTexture); }
void at2DRenderer::PopTexture(const int64_t count) { m_drawContext.tex.erase(atMax(0, m_drawContext.tex.size() - count), (atMin(count, m_drawContext.tex.size()))); }
void at2DRenderer::PushColour(const atVec4F &color) { m_drawContext.col.push_back(color); }
void at2DRenderer::PushClipRect(const atVec4I &rect) { m_drawContext.clip.push_back(rect); }
void at2DRenderer::PopColour(const int64_t count) { m_drawContext.col.erase(atMax(0, m_drawContext.col.size() - count), (atMin(count, m_drawContext.col.size()))); }
void at2DRenderer::PopClipRect(const int64_t count) { m_drawContext.clip.erase(atMax(0, m_drawContext.clip.size() - count), (atMin(count, m_drawContext.clip.size()))); }

// ------------------------------------------------------
// Internal Functions

void at2DRenderer::AdvanceCursor(const char c, const atFont::Glyph &g, int64_t *pLineHeight, atVec2I *pPos, const atVec2I &tl)
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

atAABB<int32_t>at2DRenderer::TextBounds(const atString &text)
{
  atVec2I pos = 0;
  int64_t lineHeight = 0;
  atFont &font = m_drawContext.Font();
  atAABB<int32_t> bounds;
  for (const char c : text)
  {
    atFont::Glyph g = font.GetGlyph(c);
    atVec4I rect = { pos.x,  pos.y + g.yOff, pos.x + (int32_t)g.width,  pos.y + (int32_t)g.height + g.yOff };
    bounds.GrowToContain(atAABB<int32_t>({ rect.x, rect.y, 0 }, { rect.z, rect.w, 0 }));
    AdvanceCursor(c, g, &lineHeight, &pos);
  }
  return bounds;
}

void at2DRenderer::AddPoly(const atVector<atVec2F> &points, const atVector<atVec2F> &uvs)
{
  atAssert(points.size() == uvs.size(), "Points and UVS must contain the same number of elements.");
  DrawData &dd = TryAddDrawCommand();
  atVec4F color = m_drawContext.Colour();
  uint32_t start = (uint32_t)m_drawContext.drawData.pos.size();
  for (const atVec2F &v : points)
  {
    m_drawContext.drawData.pos.push_back(atVec3F(v, 0.f));
    m_drawContext.drawData.color.push_back(color);
  }

  m_drawContext.drawData.uvs.push_back(uvs);
  for (uint32_t i = 0; i < points.size() - 2; ++i)
    m_drawContext.drawData.indices.push_back({ start, start + i + 1, start + i + 2 });
  dd.elementCount = m_drawContext.drawData.indices.size() - dd.startElement;
}

void at2DRenderer::AddPoly(const atVector<atVec2F> &points, const atVec2F &uv)
{
  TryAddDrawCommand();
  atVec4F color = m_drawContext.Colour();
  uint32_t start = (uint32_t)m_drawContext.drawData.pos.size();
  for (const atVec2F &v : points)
  {
    m_drawContext.drawData.uvs.push_back(uv);
    m_drawContext.drawData.pos.push_back(atVec3F(v, 0.f));
    m_drawContext.drawData.color.push_back(color);
  }

  for (uint32_t i = 0; i < points.size() - 2; ++i)
    m_drawContext.drawData.indices.push_back({ start, start + i + 1, start + i + 2 });
}

at2DRenderer::DrawData& at2DRenderer::TryAddDrawCommand()
{
  if (m_drawContext.drawList.size() > 0)
  {
    DrawData& dd = m_drawContext.Data();
    dd.startElement = m_drawContext.drawData.indices.size();
    if (m_drawContext.Clip() == dd.clipRect && m_drawContext.Texture() == dd.pTexture)
      return dd;
  }

  DrawData data;
  data.clipRect = m_drawContext.Clip();
  data.pTexture = m_drawContext.Texture();
  m_drawContext.drawList.push_back(data);
  return m_drawContext.Data();
}
