
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
#include "atCanvas2D.h"
#include "atRenderable.h"
#include "atRenderState.h"

void atCanvas2D::Initialise(atGraphics *pAPI)
{
  if (pAPI == m_pGfx)
    return;

  m_pGfx = pAPI;
  if (!m_pGfx)
    return;
  gpu.renderer.Clear();
  atGraphics::Release(gpu.pSampler);
  atGraphics::Release(gpu.pProgram);
  atGraphics::Release(gpu.pColour);
  atGraphics::Release(gpu.pUV);
  atGraphics::Release(gpu.pPos);
  atGraphics::Release(gpu.pIndices);

  // Setup the shader
  gpu.pProgram = m_pGfx->CreateProgram();
  if (m_pGfx->API() == atGfxApi_DirectX)
  {
    gpu.pProgram->SetStageFile("assets/shaders/text.vs", atPS_Vertex);
    gpu.pProgram->SetStageFile("assets/shaders/text.ps", atPS_Fragment);
  }
  else
  {
    gpu.pProgram->SetStageFile("assets/shaders/text.vert", atPS_Vertex);
    gpu.pProgram->SetStageFile("assets/shaders/text.frag", atPS_Fragment);
  }

  // Create the sampler
  gpu.pSampler = m_pGfx->CreateSampler();

  // Create buffers
  gpu.pColour = m_pGfx->CreateBuffer();
  gpu.pUV = m_pGfx->CreateBuffer();
  gpu.pPos = m_pGfx->CreateBuffer();
  gpu.pIndices = m_pGfx->CreateBuffer(atBT_IndexData);

  // Setup renderable
  gpu.renderer.SetProgram(gpu.pProgram);
  gpu.renderer.SetSampler("samplerType", gpu.pSampler);
  gpu.renderer.SetAttribute("COLOR", gpu.pColour);
  gpu.renderer.SetAttribute("TEXCOORD", gpu.pUV);
  gpu.renderer.SetAttribute("POSITION", gpu.pPos);
  gpu.renderer.SetAttribute("idxBuffer", gpu.pIndices);
}

void atCanvas2D::Draw(const atVec2I &dimensions)
{
  Initialise(atGraphics::GetCurrent());

  // Make sure the textures are up to date
  // for (atFont &f : m_drawContext.fonts)
  //   f.GetTexture();

  if (atInput::ButtonPressed(atKC_Apostraphe))
    gpu.pProgram->Reload();

  // Update the buffers
  gpu.pIndices->Set(m_drawData.indices);
  gpu.pColour->Set(m_drawData.col);
  gpu.pPos->Set(m_drawData.pos);
  gpu.pUV->Set(m_drawData.uvs);

  // Update the viewport
  gpu.renderer.SetUniform("mvp", atMat4F(atMatrixOrtho((float)dimensions.x, (float)dimensions.y, -1.f, 1.f)));

  // Draw each of command
  atRenderState rs;
  for (DrawCommand &dd : m_cmds)
  {
    rs.SetScissor(dd.clipRect.ToVector4());
    gpu.renderer.SetTexture("tex0", dd.pTexture);
    gpu.renderer.Draw(true, atGFX_PT_TriangleList, dd.elementCount, dd.startElement);
  }

  // Clear the draw list
  Clear();
}

void atCanvas2D::Draw(atWindow &wnd)
{
  atGraphics::SetCurrent(wnd.Graphics());
  Draw(wnd.Size());
}

void atCanvas2D::SetFont(atFont *pFont) { m_pFont = pFont; }

void atCanvas2D::AddText(const atVec2F &pos, const char *str, atCol color, const float &scale, const char *pEnd)
{
  if (!m_pFont)
    return;
  atTexture *pTexture = m_pFont->GetTexture();

  atVec2F cursor = pos;
  atRectF rect = m_pFont->CalcTextBounds(str, scale);
  cursor.x += abs(atMin(rect.m_min.x, 0));
  cursor.y += abs(atMin(rect.m_min.y, 0));

  float lineHeight = 0;
  while (*str != '\0' && (!pEnd || str < pEnd))
  {
    atFont::Glyph g = m_pFont->GetGlyph(*str, scale);
    atVec2F cPos = cursor + atVec2F(g.xOff, g.yOff);
    AddRectTextured(pTexture, cPos, cPos + atVec2F(g.width, g.height), color, g.tl, g.br);
    m_pFont->AdvanceCursor(*str, g, &lineHeight, &cursor, pos);
    ++str;
  }
}

void atCanvas2D::AddLine(const atVec2F &a, const atVec2F &b, atCol color, float thickness)
{
  PathColor(color);
  PathLineTo(a);
  PathLineTo(b);
  AddPath(false, thickness);
}

void atCanvas2D::AddRect(const atVec2F &a, const atVec2F &b, atCol color, float rounding, float thickness)
{
  PathRect(a, b, rounding, color, color, color, color);
  AddPath(true, thickness);
}

void atCanvas2D::AddRectFilled(const atVec2F &a, const atVec2F &b, atCol color, float rounding)
{
  PathRect(a, b, rounding, color, color, color, color);
  AddPathConcaveFilled();
}

void atCanvas2D::AddRectColoured(const atVec2F &a, const atVec2F &b, atCol tlColor, atCol trColor, atCol blColor, atCol brColor, float rounding, float thickness)
{
  PathRect(a, b, rounding, tlColor, trColor, blColor, brColor);
  AddPath(true, thickness);
}

void atCanvas2D::AddRectColouredFilled(const atVec2F &a, const atVec2F &b, atCol tlColor, atCol trColor, atCol blColor, atCol brColor, float rounding)
{
  PathRect(a, b, rounding, tlColor, trColor, blColor, brColor);
  AddPathConcaveFilled();
}

void atCanvas2D::AddRectTextured(atTexture *pTexture, const atVec2F &a, const atVec2F &b, atCol color, const atVec2F &tlUV, const atVec2F &brUV, float rounding)
{
  PathRect(a, b, rounding, color, color, color, color);
  AddPolylineTextured(pTexture, m_path.data(), m_pathCol.data(), m_path.size(), tlUV, brUV);
  PathClear();
}

void atCanvas2D::PathRect(const atVec2F &a, const atVec2F &b, float rounding, atCol tlCol, atCol trCol, atCol blCol, atCol brCol)
{
  if (a == b)
    return;

  rounding = atMax(0.0f, rounding);
  atVec2F tl(a.x + rounding, a.y + rounding);
  atVec2F tr(b.x - rounding - 1, a.y + rounding);
  atVec2F bl(a.x + rounding, b.y - rounding - 1);
  atVec2F br(b.x - rounding - 1, b.y - rounding - 1);

  if (rounding <= 0)
  {
    PathColor(tlCol);
    PathLineTo(tl);
    PathColor(trCol);
    PathLineTo(tr);
    PathColor(brCol);
    PathLineTo(br);
    PathColor(blCol);
    PathLineTo(bl);
  }
  else
  {
    int64_t segments = atMax(1ll, (int64_t)rounding / 2);
    PathColor(trCol);
    PathArc(tr, rounding, -(float)atPi_2, 0, segments);

    PathColor(brCol);
    PathLineTo(atVec2F(b.x, br.y));
    PathArc(br, rounding, 0, (float)atPi_2, segments);

    PathColor(blCol);
    PathLineTo(atVec2F(bl.x, b.y));
    PathArc(bl, rounding, (float)atPi_2, (float)atPi, segments);

    PathColor(tlCol);
    PathLineTo(atVec2F(a.x, tr.y));
    PathArc(tl, rounding, (float)atPi, (float)atPi_2 + (float)atPi, segments);

    PathColor(trCol);
    PathLineTo(atVec2F(tl.x, a.y));
  }
}

void atCanvas2D::PathArc(const atVec2F &center, float radius, float angleStart, float angleEnd, int64_t nSegments)
{
  float angle = angleStart;
  float angleStep = (angleEnd - angleStart) / nSegments;
  for (int64_t i = 0; i < nSegments + 1; ++i)
  {
    atVec2F pos(cos(angle), sin(angle));
    pos *= radius;
    PathLineTo(center + pos);
    angle += angleStep;
  }
}

void atCanvas2D::AddPolyline(atVec2F *pPoints, atCol *pColors, int64_t nPoints, bool closed, float thickness)
{
  if (nPoints == 0)
    return;

  int64_t primCount = nPoints - (int)(!closed);
  float halfThickness = thickness * 0.5f;
  for (int64_t edge = 0; edge < primCount; ++edge)
  {
    int64_t indexA = edge;
    int64_t indexB = (edge + 1) % nPoints;
    const atVec2F &a = pPoints[indexA];
    const atVec2F &b = pPoints[indexB];
    atVec2F delta = (b - a).Normalize();
    delta = atVec2F(-delta.y, delta.x);

    atCol colors[4] = {
      pColors[indexA],
      pColors[indexB],
      pColors[indexB],
      pColors[indexA]
    };

    atVec2F points[4] = {
      { a - delta * halfThickness },
      { b - delta * halfThickness },
      { b + delta * halfThickness },
      { a + delta * halfThickness }
    };

    AddPolylineConcaveFilled(points, colors, 4);
  }
}

void atCanvas2D::AddPolylineTextured(atTexture *pTexture, atVec2F *pPoints, atCol *pColors, int64_t nPoints, const atVec2F &tlTexCoord, const atVec2F &brTexCoord)
{
  if (nPoints == 0)
    return;

  bool calcUV = tlTexCoord != brTexCoord;

  DrawCommand& cmd = GetDrawCommand(clipRect, pTexture);
  int64_t vtxOffset = m_drawData.pos.size();
  int64_t idxOffset = m_drawData.indices.size();

  atVec2F uvDir(0);
  atVec2F tlVert(FLT_MAX);
  atVec2F brVert(-FLT_MAX);

  if (calcUV)
  {
    for (int64_t vtx = 0; vtx < nPoints; ++vtx)
    {
      tlVert.x = atMin(pPoints[vtx].x, tlVert.x);
      tlVert.y = atMin(pPoints[vtx].y, tlVert.y);
      brVert.x = atMax(pPoints[vtx].x, brVert.x);
      brVert.y = atMax(pPoints[vtx].y, brVert.y);
    }
    uvDir = (brTexCoord - tlTexCoord) / (brVert - tlVert);
  }

  // Copy vertex data
  m_drawData.pos.resize(vtxOffset + nPoints);
  m_drawData.uvs.resize(vtxOffset + nPoints);
  m_drawData.col.resize(vtxOffset + nPoints);
  for (int64_t vtx = 0; vtx < nPoints; ++vtx)
  {
    int64_t i = vtxOffset + vtx;
    m_drawData.col[i] = atColor::UnPack<float>(pColors[vtx]);
    m_drawData.pos[i] = pPoints[vtx];
    if (calcUV) // Calculate the new UV using the tex coords tangent
    {
      atVec2F uvOffset = uvDir * (pPoints[vtx] - tlVert);
      m_drawData.uvs[i] = atVec2F(tlTexCoord.x, brTexCoord.y) + atVec2F(uvOffset.x, -uvOffset.y);
    }
    else // min/max UV are the same so just use min
    {
      m_drawData.uvs[i] = tlTexCoord;
    }
  }

  // Generate indices
  int64_t triCount = nPoints - 2;
  m_drawData.indices.resize(idxOffset + triCount * 3);

  for (int64_t idx = 0; idx < triCount; ++idx)
  {
    m_drawData.indices[idxOffset + idx * 3] = (uint32_t)vtxOffset;
    m_drawData.indices[idxOffset + idx * 3 + 1] = (uint32_t)(vtxOffset + idx + 1);
    m_drawData.indices[idxOffset + idx * 3 + 2] = (uint32_t)(vtxOffset + idx + 2);
  }

  cmd.elementCount = m_drawData.indices.size() - cmd.startElement;
}

void atCanvas2D::AddPolylineConcaveFilled(atVec2F *pPoints, atCol *pColors, int64_t nPoints)
{
  if (nPoints < 3)
    return;
  atVec2F uv = m_pFont->FindWhitePixel();
  AddPolylineTextured(m_pFont->GetTexture(), pPoints, pColors, nPoints, uv, uv);
}

atCanvas2D::DrawCommand& atCanvas2D::GetDrawCommand(const atRectF &clipRect, atTexture *pTexture)
{
  bool addNewCommand = true;
  if (m_cmds.size() > 0)
  {
    const DrawCommand &curCmd = m_cmds[m_cmds.size() - 1];
    addNewCommand = curCmd.clipRect != clipRect || curCmd.pTexture != pTexture;
  }

  if (addNewCommand)
  {
    DrawCommand cmd;
    cmd.clipRect = clipRect;
    cmd.pTexture = pTexture;
    cmd.startElement = m_drawData.indices.size();
    cmd.elementCount = 0;
    m_cmds.push_back(cmd);
  }

  return m_cmds[m_cmds.size() - 1];
}

void atCanvas2D::Clear()
{
  PathClear();
  m_cmds.clear();
  m_drawData.col.clear();
  m_drawData.pos.clear();
  m_drawData.uvs.clear();
  m_drawData.indices.clear();
}

void atCanvas2D::PathClear()
{
  m_path.clear();
  m_pathCol.clear();
  m_pathColVal = 0xFFFFFFFF;
}

void atCanvas2D::PathLineTo(const atVec2F &a)
{
  m_path.push_back(a);
  m_pathCol.push_back(m_pathColVal);
}

void atCanvas2D::AddPath(bool closed, float thickness)
{
  AddPolyline(m_path.data(), m_pathCol.data(), m_path.size(), closed, thickness);
  PathClear();
}

void atCanvas2D::AddPathConcaveFilled()
{
  AddPolylineConcaveFilled(m_path.data(), m_pathCol.data(), m_path.size());
  PathClear();
}

const atCanvas2D::DrawCommand& atCanvas2D::Command(int64_t index) const { return m_cmds[index]; }
const atVector<atCanvas2D::DrawCommand>& atCanvas2D::Commands() const { return m_cmds; }
int64_t atCanvas2D::CmdCount() const { return m_cmds.size(); }
int64_t atCanvas2D::VertexCount() const { return m_drawData.pos.size(); }
int64_t atCanvas2D::IndexCount() const { return m_drawData.indices.size(); }
void atCanvas2D::PathColor(const atCol &color) { m_pathColVal = color; }
